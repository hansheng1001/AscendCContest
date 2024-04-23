#include "kernel_operator.h"
using DTYPEY = bool;
using namespace AscendC;
// constexpr int32_t BUFFER_NUM = 2;
constexpr int32_t BUFFER_NUM = 1;
constexpr int32_t el_offset = 256;
class KernelLessEqual {
public:
  __aicore__ inline KernelLessEqual() {}
  __aicore__ inline void
  Init(GM_ADDR x1, GM_ADDR x2, GM_ADDR y,uint32_t dataType,uint32_t blockLength, uint32_t tileNum,
       uint32_t tileLength, uint32_t lasttileLength, uint32_t formerNum,
       uint32_t formerLength, uint32_t formertileNum, uint32_t formertileLength,
       uint32_t formerlasttileLength, uint32_t tailNum, uint32_t tailLength,
       uint32_t tailtileNum, uint32_t tailtileLength,
       uint32_t taillasttileLength, uint32_t tilingKey) {
    ASSERT(GetBlockNum() != 0 && "block dim can not be zero!");
    
    // this->value = valueGm.GetValue(0);
    // this->posAttr = 1.702;  //-1.702
    // this->negAttr = -1.702; //-1.702
    // this->halfAttr = 0.851; // 1.702/2=0.851
    // this->one = 1.0;
    this->dataType = dataType;
    if (tilingKey == 1) {
      this->blockLength = blockLength;
      this->tileNum =
          tileNum ASSERT(tileNum != 0 && "tile num can not be zero!");
      this->tileLength = tileLength / BUFFER_NUM;
      this->lasttileLength = lasttileLength;
      //   this->lasttileLength = lasttileLength / BUFFER_NUM;

      x1Gm.SetGlobalBuffer((__gm__ DTYPE_X1 *)x1 +
                               this->blockLength * GetBlockIdx(),
                           this->blockLength);

      x2Gm.SetGlobalBuffer((__gm__ DTYPE_X2 *)x2 +
                              this->blockLength * GetBlockIdx(),
                          this->blockLength);

      yGm.SetGlobalBuffer((__gm__ DTYPEY *)y +
                              this->blockLength * GetBlockIdx(),
                          this->blockLength);
    }

    if (tilingKey == 2) {
      this->formerNum = formerNum;
      this->formerLength = formerLength;
      this->formertileNum = formertileNum;
      this->formertileLength = formertileLength;
      this->formerlasttileLength = formerlasttileLength;

      this->tailNum = tailNum;
      this->tailLength = tailLength;
      this->tailtileNum = tailtileNum;
      this->tailtileLength = tailtileLength;
      this->taillasttileLength = taillasttileLength;

      if (GetBlockIdx() < this->formerNum) { //分到大块核的处理
        this->tileLength = this->formertileLength / BUFFER_NUM;
        this->lasttileLength = this->formerlasttileLength;
        // this->lasttileLength = this->formerlasttileLength / BUFFER_NUM;
        this->tileNum = this->formertileNum * BUFFER_NUM;

        x1Gm.SetGlobalBuffer((__gm__ DTYPE_X1 *)x1 +
                                 this->formerLength * GetBlockIdx(),
                             this->formerLength);

        x2Gm.SetGlobalBuffer((__gm__ DTYPE_X2 *)x2 +
                                this->formerLength * GetBlockIdx(),
                            this->formerLength);

        yGm.SetGlobalBuffer((__gm__ DTYPEY *)y +
                                this->formerLength * GetBlockIdx(),
                            this->formerLength);

      } else { //分到小块核的处理，需要处理的数据量比大核少alignNum个
        this->tileLength = this->tailtileLength / BUFFER_NUM;
        // this->lasttileLength = this->taillasttileLength;
        this->lasttileLength = this->taillasttileLength / BUFFER_NUM;
        this->tileNum = this->tailtileNum * BUFFER_NUM;

        x1Gm.SetGlobalBuffer(
            (__gm__ DTYPE_X1 *)x1 + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);

        x2Gm.SetGlobalBuffer(
            (__gm__ DTYPE_X2 *)x2 + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);

        yGm.SetGlobalBuffer(
            (__gm__ DTYPEY *)y + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);
      }
    }
    this->tileLengthtemp = this->tileLength + el_offset;
    pipe.InitBuffer(inQueueIN, BUFFER_NUM, this->tileLengthtemp * 2 * sizeof(DTYPE_X1));
    pipe.InitBuffer(outQueueOUT, BUFFER_NUM, this->tileLengthtemp * 1 * sizeof(DTYPEY));
    // pipe.InitBuffer(outQueueSelect, 1, this->tileLengthtemp * sizeof(uint8_t));
    // pipe.InitBuffer(this->tempBufferfp16, 1, this->tileLengthtemp * 2 * sizeof(half));
    // pipe.InitBuffer(this->tempBufferfp32, 1, this->tileLengthtemp * 2 * sizeof(float));
    // pipe.InitBuffer(this->tempyLocalfp16, 1, this->tileLengthtemp * 3 * sizeof(half));
  }
  __aicore__ inline void Process() {
    int32_t loopCount = this->tileNum * BUFFER_NUM;
    for (int32_t i = 0; i < loopCount; i++) {
      CopyIn(i);
      Compute(i);
      CopyOut(i);
    }
  }

private:
  __aicore__ inline void CopyIn(int32_t progress) {
    LocalTensor<DTYPE_X1> inLocal = inQueueIN.AllocTensor<DTYPE_X1>();

    if (BUFFER_NUM == 1) {
      if (progress == this->tileNum - 1) {

        if (progress == 0) {
          //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
          DataCopy(inLocal[el_offset], x1Gm[progress * this->tileLength],
                   this->tileLength);

          DataCopy(inLocal[this->tileLengthtemp+el_offset], x2Gm[progress * this->tileLength],
                   this->tileLength);

        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength

          DataCopy(
              inLocal[el_offset],
              x1Gm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);

          DataCopy(
              inLocal[this->tileLengthtemp+el_offset],
              x2Gm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);
        }

      } else {

        DataCopy(inLocal[el_offset], x1Gm[progress * this->tileLength],
                 this->tileLength);

        DataCopy(inLocal[this->tileLengthtemp+el_offset], x2Gm[progress * this->tileLength],
                 this->tileLength);
      }
    }

    // if (BUFFER_NUM == 2) {
    //   //开启double
    //   // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
    //   // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理

    //   if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
    //       (progress == (this->tileNum * BUFFER_NUM - 1))) {

    //     // 只有一个分块
    //     if (progress < 2) {

    //       DataCopy(inLocal[0], x1Gm[progress * this->tileLength],
    //                this->tileLength);

    //       DataCopy(inLocal[this->tileLength], x2Gm[progress * this->tileLength],
    //                this->tileLength);

    //     } else {

    //       //分块大小变为tileLength的一半
    //       //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动

    //       DataCopy(
    //           inLocal[0],
    //           x1Gm[(progress - 2) * (this->tileLength) + this->lasttileLength],
    //           this->tileLength);

    //       DataCopy(
    //           inLocal[this->tileLength],
    //           x2Gm[(progress - 2) * (this->tileLength) + this->lasttileLength],
    //           this->tileLength);
    //     }
    //   } else {
    //     DataCopy(inLocal[0], x1Gm[progress * this->tileLength],
    //              this->tileLength);

    //     DataCopy(inLocal[this->tileLength], x2Gm[progress * this->tileLength],
    //              this->tileLength);
    //   }
    // }
    inQueueIN.EnQue(inLocal);
  }

  __aicore__ inline void Compute(int32_t progress)
    {
        LocalTensor<DTYPE_X1> inLocal = inQueueIN.DeQue<DTYPE_X1>(); // *他们三个放在了同一个inQueue里面，所以取的时候加偏移
        LocalTensor<DTYPE_X1> x1Local = inLocal[0];
        LocalTensor<DTYPE_X1> x2Local = inLocal[this->tileLengthtemp];

        LocalTensor<DTYPEY> yLocal = outQueueOUT.AllocTensor<DTYPEY>();
        // LocalTensor<half> outLocal = tempyLocalfp16.Get<half>();
        // // LocalTensor<int16_t> outLocaltemp = tempyLocalint32.Get<int16_t>();

        // LocalTensor<half> y1Local = outLocal;
        // LocalTensor<half> y2Local = outLocal[this->tileLength];
        // LocalTensor<half> y3Local = outLocal[2 * this->tileLength];

        // LocalTensor<uint8_t> selMaskLocal = outQueueSelect.Get<uint8_t>();
        // 不同数据类型返回的selMaskLocal不一样
        if (this->dataType == 2)
        { // int8
            // LocalTensor<half> x1Localfp16 = tempBufferfp16.Get<half>();
            // LocalTensor<half> x2Localfp16 = tempBufferfp16.Get<half>();
            // LocalTensor<int8_t> interpre_tensor1 = x1Local.ReinterpretCast<int8_t>();
            // LocalTensor<int8_t> interpre_tensor2 = x2Local.ReinterpretCast<int8_t>();
            // // Duplicate<int8_t>(interpre_tensor1,1,this->tileLength);//默认this->tileLength是256的整数倍，如果报错后面进行填充
            // // Duplicate<int8_t>(interpre_tensor2,1,this->tileLength);
            // AscendC::Cast(x1Localfp16, interpre_tensor1, RoundMode::CAST_NONE, this->tileLengthtemp);
            // AscendC::Cast(x2Localfp16, interpre_tensor2, RoundMode::CAST_NONE, this->tileLengthtemp);
            // Compare(yLocal, x1Localfp16, x2Localfp16, CMPMODE::LE, this->tileLength);
            for (uint32_t i = 0; i < this->tileLengthtemp; ++i) {
              float element1 = x1Local.GetValue(i);
              float element2 = x2Local.GetValue(i);
              if (element1 > element2)
                yLocal.SetValue(i,false);
              else
                yLocal.SetValue(i,true);
            }
            // tempBufferfp16.FreeTensor(x1Localfp16);
            // tempBufferfp16.FreeTensor(x2Localfp16);
        }
        else if (this->dataType == 3)
        { // int32
            // LocalTensor<float> x1Localfp32 = tempBufferfp32.Get<float>();
            // LocalTensor<float> x2Localfp32 = tempBufferfp32.Get<float>();
            // LocalTensor<int32_t> interpre_tensor1 = x1Local.ReinterpretCast<int32_t>();
            // LocalTensor<int32_t> interpre_tensor2 = x2Local.ReinterpretCast<int32_t>();
            // AscendC::Cast(x1Localfp32, interpre_tensor1, RoundMode::CAST_NONE, this->tileLengthtemp);
            // AscendC::Cast(x2Localfp32, interpre_tensor2, RoundMode::CAST_NONE, this->tileLengthtemp);
            // Compare(yLocal, x1Localfp32, x2Localfp32, CMPMODE::LE, this->tileLength);
            for (uint32_t i = 0; i < this->tileLengthtemp; ++i) {
              float element1 = x1Local.GetValue(i);
              float element2 = x2Local.GetValue(i);
              if (element1 > element2)
                yLocal.SetValue(i,false);
              else
                yLocal.SetValue(i,true);
            }
            // tempBufferfp32.FreeTensor(x1Localfp32);
            // tempBufferfp32.FreeTensor(x2Localfp32);
        }
        else if (this->dataType == 0)
        { // float32
            // 不知道是否需要显示的指明类型？？？需要则额外创建处理
          // LocalTensor<float> x1Localfp32 = x1Local.ReinterpretCast<float>();
          // LocalTensor<float> x2Localfp32 = x2Local.ReinterpretCast<float>();
          // Compare(yLocal, x1Localfp32, x2Localfp32, CMPMODE::LE, this->tileLength);
          for (int32_t i = 0; i < this->tileLengthtemp; ++i) {
              float element1 = x1Local.GetValue(i);
              float element2 = x2Local.GetValue(i);
              if ( element1 > element2) 
                yLocal.SetValue(i,false);
              else
                yLocal.SetValue(i,true);
          }
        }
        else
        {
          // LocalTensor<half> x1Localfp16 = x1Local.ReinterpretCast<half>();
          // LocalTensor<half> x2Localfp16 = x2Local.ReinterpretCast<half>();
          for (uint32_t i = 0; i < this->tileLengthtemp; ++i) {
              float element1 = x1Local.GetValue(i);
              float element2 = x2Local.GetValue(i);
              if (element1 > element2)
                yLocal.SetValue(i,false);
              else
                yLocal.SetValue(i,true);
          }
          // Compare(yLocal, x1Localfp16, x2Localfp16, CMPMODE::LE, this->tileLength);
        }

        // Duplicate<half>(y1Local, (half)10, this->tileLength);
        // Duplicate<half>(y2Local, (half)0, this->tileLength);
        // // // AscendC::Cast(y1Localtemp, y1Local, RoundMode::CAST_NONE, this->tileLength);
        // // // AscendC::Cast(y2Localtemp, y2Local, RoundMode::CAST_NONE, this->tileLength);
        // Select(y3Local, selMaskLocal, y1Local, y2Local,
        //        SELMODE::VSEL_TENSOR_TENSOR_MODE, this->tileLength);
        // 实现后入队，资源清理
        // AscendC::Cast(outLocaltemp, y3Local, RoundMode::CAST_NONE, this->tileLength);
        // int16_t limit = 1;
        // for (int32_t ix = 0; ix < this->tileLength; ++ix)
        // {
        //     int16_t element = outLocaltemp.GetValue(ix);
        //     if (element < limit)
        //         yLocal.SetValue(ix, 0);
        //     else
        //         yLocal.SetValue(ix, 1);
        // }
        // LocalTensor<uint8_t> yLocaltemp = selMaskLocal.ReinterpretCast<uint8_t>();
        // AscendC::Cast(yLocal, y3Local, RoundMode::CAST_ROUND, this->tileLength);
        // LocalTensor<bool> yLocaltemp = yLocal.ReinterpretCast<bool>();

        inQueueIN.FreeTensor(inLocal);
        outQueueOUT.EnQue<DTYPEY>(yLocal);
        // tempyLocalint32.FreeTensor(outLocaltemp);
        // outQueueSelect.FreeTensor(selMaskLocal);
        // tempyLocalfp16.FreeTensor(outLocal);
    }

  __aicore__ inline void CopyOut(int32_t progress) {
    LocalTensor<DTYPEY> zLocal = outQueueOUT.DeQue<DTYPEY>();

    if (BUFFER_NUM == 1) {
      if (progress == this->tileNum - 1) {

#if 1
        if (progress == 0) {
          //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
          DataCopy(yGm[0], zLocal[el_offset], this->tileLength);
        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(
              yGm[(progress - 1) * this->tileLength + this->lasttileLength],
              zLocal[el_offset], this->tileLength);
        }
#endif

      } else {
        DataCopy(yGm[progress * this->tileLength], zLocal[el_offset], this->tileLength);
      }
    }

    // if (BUFFER_NUM == 2) {
    //   //开启double
    //   // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
    //   // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
    //   if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
    //       (progress == (this->tileNum * BUFFER_NUM - 1))) {

    //     if (progress < 2) {
    //       DataCopy(yGm[progress * (this->tileLength)], zLocal,
    //                (this->tileLength));
    //     } else {
    //       //分块大小变为tileLength的一半
    //       //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
    //       DataCopy(
    //           yGm[(progress - 2) * (this->tileLength) + this->lasttileLength],
    //           zLocal, (this->tileLength));
    //     }
    //   } else {
    //     DataCopy(yGm[progress * (this->tileLength)], zLocal,
    //              (this->tileLength));
    //   }
    // }

    outQueueOUT.FreeTensor(zLocal);
  }

private:
  TPipe pipe;
  // TQue<QuePosition::VECIN, BUFFER_NUM> inQueueX, inQueueY, inQueueZ;
  TQue<QuePosition::VECIN, BUFFER_NUM> inQueueIN;
  TQue<QuePosition::VECOUT, BUFFER_NUM> outQueueOUT;
  // TBuf<QuePosition::VECCALC> tempBufferfp16, tempBufferfp32;//, outQueueSelect, tempyLocalfp16; //,tempBufferfp321,tempBufferfp32
  // GlobalTensor<float> xGm;
  // GlobalTensor<float> yGm;
  // GlobalTensor<float> outGm;

  //   GlobalTensor<DTYPE_VALUE> valueGm;
  //   GlobalTensor<DTYPE_INPUT_DATA> input_dataGm;
  //   GlobalTensor<DTYPE_X2> x2Gm;

  GlobalTensor<DTYPE_X1> x1Gm;
  GlobalTensor<DTYPE_X2> x2Gm;
  GlobalTensor<DTYPEY> yGm;

  uint32_t tileLengthtemp;
  uint32_t dataType;
  uint32_t blockLength;
  uint32_t tileNum;
  uint32_t tileLength;
  uint32_t lasttileLength;

  uint32_t formerNum;
  uint32_t formerLength;
  uint32_t formertileNum;
  uint32_t formertileLength;
  uint32_t formerlasttileLength;

  uint32_t tailNum;
  uint32_t tailLength;
  uint32_t tailtileNum;
  uint32_t tailtileLength;
  uint32_t taillasttileLength;
};

extern "C" __global__ __aicore__ void less_equal(GM_ADDR x1, GM_ADDR x2,
                                                     GM_ADDR y,
                                                     GM_ADDR workspace,
                                                     GM_ADDR tiling) {
  GET_TILING_DATA(tiling_data, tiling);
  // TODO: user kernel impl
  KernelLessEqual op;

  uint32_t tilingKey = 1;
  if (TILING_KEY_IS(1)) {
    tilingKey = 1;
  } else if (TILING_KEY_IS(2)) {
    tilingKey = 2;
  } else {
    tilingKey = 1;
  }

  op.Init(
      x1, x2, y,tiling_data.dataType,tiling_data.blockLength, tiling_data.tileNum,
      tiling_data.tileLength, tiling_data.lasttileLength, tiling_data.formerNum,
      tiling_data.formerLength, tiling_data.formertileNum,
      tiling_data.formertileLength, tiling_data.formerlasttileLength,
      tiling_data.tailNum, tiling_data.tailLength, tiling_data.tailtileNum,
      tiling_data.tailtileLength, tiling_data.taillasttileLength, tilingKey);
  op.Process();
}

#ifndef __CCE_KT_TEST__
// call of kernel function
void less_equal_do(uint32_t blockDim, void *l2ctrl, void *stream,
                       GM_ADDR x, GM_ADDR y, GM_ADDR z, GM_ADDR workspace,
                       GM_ADDR tiling) {

  less_equal<<<blockDim, l2ctrl, stream>>>(x, y, z, workspace, tiling);
  // aclrtSynchronizeStream(stream);

  // 这个问题需要解决呢？
  // GET_TILING_DATA(tiling_data, tiling);

  //   std::cout << "reduction" << tiling_data.reduction
  //             << ", BlockNum=" << GetBlockNum() << "TILING_KEY_IS(1)"
  //             << TILING_KEY_IS(1) << std::endl;
}
#endif
