#include "kernel_operator.h"

#if 0
extern "C" __global__ __aicore__ void less_equal(GM_ADDR x1, GM_ADDR x2, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
    GET_TILING_DATA(tiling_data, tiling);
    // TODO: user kernel impl
}
#endif

using namespace AscendC;
// constexpr int32_t BUFFER_NUM = 2;
constexpr int32_t BUFFER_NUM = 1;

class KernelLessEqual {
public:
  __aicore__ inline KernelLessEqual() {}
  __aicore__ inline void
  Init(GM_ADDR x1, GM_ADDR x2, GM_ADDR y, uint32_t dataType,
       uint32_t blockLength, uint32_t tileNum, uint32_t tileLength,
       uint32_t lasttileLength, uint32_t formerNum, uint32_t formerLength,
       uint32_t formertileNum, uint32_t formertileLength,
       uint32_t formerlasttileLength, uint32_t tailNum, uint32_t tailLength,
       uint32_t tailtileNum, uint32_t tailtileLength,
       uint32_t taillasttileLength, uint32_t tilingKey) {
    ASSERT(GetBlockNum() != 0 && "block dim can not be zero!");

    // this->value = valueGm.GetValue(0);
    // this->posAttr = 1.702;  //-1.702
    // this->negAttr = -1.702; //-1.702
    // this->halfAttr = 0.851; // 1.702/2=0.851
    // this->one = 1.0;

    this->datatype_ = dataType;

    this->attrOne = (DTYPE_Y)1;
    this->attrZero = (DTYPE_Y)0;

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

      yGm.SetGlobalBuffer((__gm__ DTYPE_Y *)y +
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

        yGm.SetGlobalBuffer((__gm__ DTYPE_Y *)y +
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
            (__gm__ DTYPE_Y *)y + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);
      }
    }

    pipe.InitBuffer(inQueueIN, BUFFER_NUM,
                    this->tileLength * 2 * sizeof(DTYPE_X1));
    pipe.InitBuffer(outQueueOUT, BUFFER_NUM,
                    this->tileLength * 3 * sizeof(DTYPE_Y));

    pipe.InitBuffer(outQueueSelect, BUFFER_NUM,
                    this->tileLength * sizeof(uint8_t));

    // pipe.InitBuffer(half1, this->tileLength * 2 * sizeof(half));
    pipe.InitBuffer(half1, this->tileLength * sizeof(half));
    pipe.InitBuffer(half2, this->tileLength * sizeof(half));

    // pipe.InitBuffer(floatp1, this->tileLength * 2 * sizeof(float));
    pipe.InitBuffer(floatp1, this->tileLength * sizeof(float));
    pipe.InitBuffer(floatp2, this->tileLength * sizeof(float));
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
          DataCopy(inLocal[0], x1Gm[progress * this->tileLength],
                   this->tileLength);

          DataCopy(inLocal[this->tileLength], x2Gm[progress * this->tileLength],
                   this->tileLength);

        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength

          DataCopy(
              inLocal[0],
              x1Gm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);

          DataCopy(
              inLocal[this->tileLength],
              x2Gm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);
        }

      } else {

        DataCopy(inLocal[0], x1Gm[progress * this->tileLength],
                 this->tileLength);

        DataCopy(inLocal[this->tileLength], x2Gm[progress * this->tileLength],
                 this->tileLength);
      }
    }

    if (BUFFER_NUM == 2) {
      //开启double
      // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
      // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理

      if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
          (progress == (this->tileNum * BUFFER_NUM - 1))) {

        // 只有一个分块
        if (progress < 2) {

          DataCopy(inLocal[0], x1Gm[progress * this->tileLength],
                   this->tileLength);

          DataCopy(inLocal[this->tileLength], x2Gm[progress * this->tileLength],
                   this->tileLength);

        } else {

          //分块大小变为tileLength的一半
          //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动

          DataCopy(
              inLocal[0],
              x1Gm[(progress - 2) * (this->tileLength) + this->lasttileLength],
              this->tileLength);

          DataCopy(
              inLocal[this->tileLength],
              x2Gm[(progress - 2) * (this->tileLength) + this->lasttileLength],
              this->tileLength);
        }
      } else {
        DataCopy(inLocal[0], x1Gm[progress * this->tileLength],
                 this->tileLength);

        DataCopy(inLocal[this->tileLength], x2Gm[progress * this->tileLength],
                 this->tileLength);
      }
    }

    inQueueIN.EnQue(inLocal);
  }

  __aicore__ inline void Compute(int32_t progress) {
    LocalTensor<DTYPE_X1> inLocal = inQueueIN.DeQue<DTYPE_X1>();

    LocalTensor<DTYPE_X1> x1Local = inLocal;
    LocalTensor<DTYPE_X1> x2Local = inLocal[this->tileLength];

    // LocalTensor<DTYPE_X> divDownLocal = inLocal[2 * this->tileLength];
    // LocalTensor<DTYPE_X> divUpLocal = inLocal[3 * this->tileLength];

    // LocalTensor<half> comHalfLocal = half1.Get<half>();
    // LocalTensor<half> hf1 = comLocal;
    // LocalTensor<half> hf2 = comLocal[this->tileLength];

    LocalTensor<half> hf1 = half1.Get<half>();
    LocalTensor<half> hf2 = half2.Get<half>();

    LocalTensor<float> fp1 = floatp1.Get<float>();
    LocalTensor<float> fp2 = floatp2.Get<float>();

    // LocalTensor<half> comFloatLocal = half1.Get<half>();
    // LocalTensor<float> fp1 = comFloatLocal;
    // LocalTensor<float> fp2 = [this->tileLength];

    if (this->datatype_ == 2) { // *int8
      Cast(hf1, x1Local, RoundMode::CAST_NONE, this->tileLength);
      Cast(hf2, x2Local, RoundMode::CAST_NONE, this->tileLength);
    }

    if (this->datatype_ == 3) {
      Cast(fp1, x1Local, RoundMode::CAST_NONE, this->tileLength);
      Cast(fp2, x2Local, RoundMode::CAST_NONE, this->tileLength);
    }

    LocalTensor<DTYPE_Y> outLocal = outQueueOUT.AllocTensor<DTYPE_Y>();
    LocalTensor<DTYPE_Y> yLocal = outLocal;
    LocalTensor<DTYPE_Y> y1Local = outLocal[this->tileLength];
    LocalTensor<DTYPE_Y> y2Local = outLocal[2 * this->tileLength];

    LocalTensor<uint8_t> selMaskLocal = outQueueSelect.AllocTensor<uint8_t>();

    Duplicate(y1Local, this->attrOne, this->tileLength);
    Duplicate(y2Local, this->attrZero, this->tileLength);

    // Compare(selMaskLocal, x1Local, x2Local, CMPMODE::LE, this->tileLength);
    if (this->datatype_ == 2) {
      Compare(selMaskLocal, hf1, hf2, CMPMODE::LE, this->tileLength);
    }
    if (this->datatype_ == 3) {
      Compare(selMaskLocal, fp1, fp2, CMPMODE::LE, this->tileLength);
    } else {
      Compare(selMaskLocal, x1Local, x2Local, CMPMODE::LE, this->tileLength);
    }

    Select(yLocal, selMaskLocal, y1Local, y2Local,
           SELMODE::VSEL_TENSOR_TENSOR_MODE, this->tileLength);

    // Compare(yLocal, x1Local, x2Local, CMPMODE::LT, this->tileLength);

    outQueueOUT.EnQue<DTYPE_Y>(outLocal);

    inQueueIN.FreeTensor(inLocal);

    outQueueSelect.FreeTensor(selMaskLocal);
  }

  __aicore__ inline void CopyOut(int32_t progress) {
    LocalTensor<DTYPE_Y> yLocal = outQueueOUT.DeQue<DTYPE_Y>();

    if (BUFFER_NUM == 1) {
      if (progress == this->tileNum - 1) {

#if 1
        if (progress == 0) {
          //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
          DataCopy(yGm[0], yLocal, this->tileLength);
        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(
              yGm[(progress - 1) * this->tileLength + this->lasttileLength],
              yLocal, this->tileLength);
        }
#endif

      } else {
        DataCopy(yGm[progress * this->tileLength], yLocal, this->tileLength);
      }
    }

    if (BUFFER_NUM == 2) {
      //开启double
      // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
      // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
      if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
          (progress == (this->tileNum * BUFFER_NUM - 1))) {

        if (progress < 2) {
          DataCopy(yGm[progress * (this->tileLength)], yLocal,
                   (this->tileLength));
        } else {
          //分块大小变为tileLength的一半
          //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
          DataCopy(
              yGm[(progress - 2) * (this->tileLength) + this->lasttileLength],
              yLocal, (this->tileLength));
        }
      } else {
        DataCopy(yGm[progress * (this->tileLength)], yLocal,
                 (this->tileLength));
      }
    }

    outQueueOUT.FreeTensor(yLocal);
  }

private:
  TPipe pipe;
  // TQue<QuePosition::VECIN, BUFFER_NUM> inQueueX, inQueueY, inQueueZ;
  TQue<QuePosition::VECIN, BUFFER_NUM> inQueueIN;
  TQue<QuePosition::VECOUT, BUFFER_NUM> outQueueOUT;
  TQue<QuePosition::VECOUT, BUFFER_NUM> outQueueSelect;
  TBuf<TPosition::VECCALC> half1, half2, floatp1, floatp2;
  // TBuf<TPosition::VECCALC> floatp1, floatp2;
  // GlobalTensor<float> xGm;
  // GlobalTensor<float> yGm;
  // GlobalTensor<float> outGm;

  //   GlobalTensor<DTYPE_VALUE> valueGm;
  //   GlobalTensor<DTYPE_INPUT_DATA> input_dataGm;
  //   GlobalTensor<DTYPE_X2> x2Gm;

  GlobalTensor<DTYPE_X1> x1Gm;
  GlobalTensor<DTYPE_X2> x2Gm;
  GlobalTensor<DTYPE_Y> yGm;

  DTYPE_Y attrOne;
  DTYPE_Y attrZero;

  uint32_t datatype_;

  //   DTYPE_X posAttr;  // 1.702
  //   DTYPE_X negAttr;  //-1.702
  //   DTYPE_X halfAttr; // 1.702/2=0.851
  //   DTYPE_X one;
  // float sum; //用于存放sum
  // float sum; //用于存放sum
  // uint32_t dataTotalLength;

  uint32_t reduction;
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
                                                 GM_ADDR y, GM_ADDR workspace,
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

  op.Init(x1, x2, y, tiling_data.dataType, tiling_data.blockLength,
          tiling_data.tileNum, tiling_data.tileLength,
          tiling_data.lasttileLength, tiling_data.formerNum,
          tiling_data.formerLength, tiling_data.formertileNum,
          tiling_data.formertileLength, tiling_data.formerlasttileLength,
          tiling_data.tailNum, tiling_data.tailLength, tiling_data.tailtileNum,
          tiling_data.tailtileLength, tiling_data.taillasttileLength,
          tilingKey);
  op.Process();
}

#ifndef __CCE_KT_TEST__
// call of kernel function
void less_equal_do(uint32_t blockDim, void *l2ctrl, void *stream, GM_ADDR x1,
                   GM_ADDR x2, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
  less_equal<<<blockDim, l2ctrl, stream>>>(x1, x2, y, workspace, tiling);

  aclrtSynchronizeStream(stream);

  // 这个问题需要解决呢？
  // GET_TILING_DATA(tiling_data, tiling);

  //   std::cout << "reduction" << tiling_data.reduction
  //             << ", BlockNum=" << GetBlockNum() << "TILING_KEY_IS(1)"
  //             << TILING_KEY_IS(1) << std::endl;
}
#endif