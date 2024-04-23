#include "kernel_operator.h"

#if 0 
extern "C" __global__ __aicore__ void scatter_sub(GM_ADDR x, GM_ADDR index, GM_ADDR update, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
    GET_TILING_DATA(tiling_data, tiling);
    // TODO: user kernel impl
}
#endif

using namespace AscendC;
// constexpr int32_t BUFFER_NUM = 2;
constexpr int32_t BUFFER_NUM = 1;

class KernelScatterSub {
public:
  __aicore__ inline KernelScatterSub() {}
  __aicore__ inline void
  Init(GM_ADDR x, GM_ADDR index, GM_ADDR update, uint32_t dataType,
       int32_t indexLength, uint32_t updateType, uint32_t blockLength,
       uint32_t tileNum, uint32_t tileLength, uint32_t lasttileLength,
       uint32_t formerNum, uint32_t formerLength, uint32_t formertileNum,
       uint32_t formertileLength, uint32_t formerlasttileLength,
       uint32_t tailNum, uint32_t tailLength, uint32_t tailtileNum,
       uint32_t tailtileLength, uint32_t taillasttileLength,
       uint32_t tilingKey) {
    ASSERT(GetBlockNum() != 0 && "block dim can not be zero!");

    this->dataType = dataType;

    this->indexLength = indexLength;
    this->updateType = updateType;

    //获取update中的值
    // if (0 == this->updateType) {
    //   uGm.SetGlobalBuffer((__gm__ DTYPE_X *)update, this->indexLength);
    // } else {
    //   uGm.SetGlobalBuffer((__gm__ DTYPE_X *)update, 1);
    // }
    // uGm.SetGlobalBuffer((__gm__ DTYPE_X *)update, this->indexLength );

    // 获取index中的值
    // iGm.SetGlobalBuffer((__gm__ int32_t *)index, this->indexLength);

    // this->value = valueGm.GetValue(0);
    // this->negAttr = -1.702; //-1.702
    // this->halfAttr = 0.851; // 1.702/2=0.851
    // this->one = 1.0;

    if (tilingKey == 1) {
      this->blockLength = blockLength;
      this->tileNum =
          tileNum ASSERT(tileNum != 0 && "tile num can not be zero!");
      this->tileLength = tileLength / BUFFER_NUM;
      this->lasttileLength = lasttileLength;
      //   this->lasttileLength = lasttileLength / BUFFER_NUM;

      xGm.SetGlobalBuffer((__gm__ DTYPE_X *)x +
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

        xGm.SetGlobalBuffer((__gm__ DTYPE_X *)x +
                                this->formerLength * GetBlockIdx(),
                            this->formerLength);

      } else { //分到小块核的处理，需要处理的数据量比大核少alignNum个
        this->tileLength = this->tailtileLength / BUFFER_NUM;
        // this->lasttileLength = this->taillasttileLength;
        this->lasttileLength = this->taillasttileLength / BUFFER_NUM;
        this->tileNum = this->tailtileNum * BUFFER_NUM;

        xGm.SetGlobalBuffer(
            (__gm__ DTYPE_X *)x + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);
      }
    }

    // 有多填充的嫌疑,应该问题不大
    //获取update中的值
    uGm.SetGlobalBuffer((__gm__ DTYPE_X *)update, this->tileLength);
    // 获取index中的值
    iGm.SetGlobalBuffer((__gm__ DTYPE_INDEX *)index, this->tileLength);

    if (1 == this->updateType) {
      this->attrScaler = (float)((half)(uGm.GetValue(0)));
    }

    pipe.InitBuffer(inQueueIN, BUFFER_NUM,
                    this->tileLength * 2 * sizeof(DTYPE_X));
    pipe.InitBuffer(outQueueOUT, BUFFER_NUM,
                    this->tileLength * sizeof(DTYPE_X));

    pipe.InitBuffer(iQue, BUFFER_NUM, this->tileLength * sizeof(int32_t));
    // pipe.InitBuffer(uBuf, 1, this->tileLength * sizeof(DTYPE_X));
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
    LocalTensor<DTYPE_X> inLocal = inQueueIN.AllocTensor<DTYPE_X>();

    LocalTensor<int32_t> inLocal2 = iQue.AllocTensor<int32_t>();

    if (BUFFER_NUM == 1) {
      if (progress == this->tileNum - 1) {

        if (progress == 0) {
          //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
          DataCopy(inLocal[0], xGm[progress * this->tileLength],
                   this->tileLength);

        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength

          DataCopy(
              inLocal[0],
              xGm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);
        }

      } else {

        DataCopy(inLocal[0], xGm[progress * this->tileLength],
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

          DataCopy(inLocal[0], xGm[progress * this->tileLength],
                   this->tileLength);

        } else {

          //分块大小变为tileLength的一半
          //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动

          DataCopy(
              inLocal[0],
              xGm[(progress - 2) * (this->tileLength) + this->lasttileLength],
              this->tileLength);
        }
      } else {
        DataCopy(inLocal[0], xGm[progress * this->tileLength],
                 this->tileLength);
      }
    }

    DataCopy(inLocal2[0], iGm[0], this->tileLength);

    if (0 == this->updateType) {
      DataCopy(inLocal[this->tileLength], uGm[0], this->tileLength);
    }

    iQue.EnQue(inLocal2);
    inQueueIN.EnQue(inLocal);
  }

  __aicore__ inline void Compute(int32_t progress) {
    LocalTensor<DTYPE_X> inLocal = inQueueIN.DeQue<DTYPE_X>();

    LocalTensor<DTYPE_X> xLocal = inLocal;

    LocalTensor<DTYPE_X> uLocal = inLocal[this->tileLength];

    LocalTensor<DTYPE_X> yLocal = outQueueOUT.AllocTensor<DTYPE_X>();

    LocalTensor<int32_t> inLocal2 = iQue.DeQue<int32_t>();

    LocalTensor<int32_t> iLocal = inLocal2;

    // LocalTensor<int32_t> iLocal = iBuf.Get<int32_t>();
    // DataCopy(iLocal[0], iGm[0], this->tileLength);

    // LocalTensor<DTYPE_X> uLocal = uBuf.Get<DTYPE_X>();
    // DataCopy(uLocal[0], uGm[0], this->tileLength);

#if 0
    if (0 == this->updateType) {

      // PRINTF("come 1.\n");

      for (int32_t i = 0; i < this->indexLength; i++) {
        int32_t index = iLocal.GetValue(i);
        // int32_t index = iGm.GetValue(i);

        if ((progress * this->tileLength <= index) &&
            (index < (progress + 1) * this->tileLength)) {
          int32_t rIndex = index - progress * this->tileLength;

          DTYPE_X newValue;

          // fp16
          //   if (0 == this->dataType) {
          //     float x1 = xLocal.GetValue(rIndex);
          //     float u1 = uLocal.GetValue(i);
          //     newValue = x1 - u1;
          //   } else {
          //     newValue = xLocal.GetValue(rIndex) - uLocal.GetValue(i);
          //   }

          float x1 = (float)((half)xLocal.GetValue(rIndex));
          float u1 = (float)(half)(uLocal.GetValue(i));

          // float u1 = (float)(half)(uGm.GetValue(i));
          newValue = (DTYPE_X)(x1 - u1);

          xLocal.SetValue(rIndex, newValue);
        }
      }
      DataCopy(yLocal, xLocal, this->tileLength);
    } else {
      for (int32_t i = 0; i < this->indexLength; i++) {
        int32_t index = iLocal.GetValue(i);

        // int32_t index = iGm.GetValue(i);

        if ((progress * this->tileLength <= index) &&
            (index < (progress + 1) * this->tileLength)) {
          int32_t rIndex = index - progress * this->tileLength;

          DTYPE_X newValue;

          // fp16
          //   if (0 == this->dataType) {
          //     float x1 = xLocal.GetValue(rIndex);
          //     float u1 = uLocal.GetValue(i);
          //     newValue = x1 - u1;
          //   } else {
          //     newValue = xLocal.GetValue(rIndex) - uLocal.GetValue(i);
          //   }

          float x1 = (float)((half)xLocal.GetValue(rIndex));
          // float u1 = (float)(half)(uLocal.GetValue(i));
          newValue = (DTYPE_X)(x1 - this->attrScaler);

          xLocal.SetValue(rIndex, newValue);
        }
      }

      DataCopy(yLocal, xLocal, this->tileLength);
    }
#endif

    Mul(xLocal, xLocal, xLocal, this->tileLength);
    // for (int32_t i = 0; i < this->indexLength; i++) {

    //   float t = 1.5;

    //   xLocal.SetValue(i, t);
    // }

    // float t = 1.5;

    // xLocal.SetValue(0, (DTYPE_X)t);

    // outQueueOUT.EnQue<DTYPE_X>(yLocal);
    outQueueOUT.EnQue<DTYPE_X>(xLocal);

    inQueueIN.FreeTensor(inLocal);

    iQue.FreeTensor(inLocal2);
  }

  __aicore__ inline void CopyOut(int32_t progress) {
    LocalTensor<DTYPE_X> yLocal = outQueueOUT.DeQue<DTYPE_X>();

    if (BUFFER_NUM == 1) {
      if (progress == this->tileNum - 1) {

#if 1
        if (progress == 0) {
          //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
          DataCopy(xGm[0], yLocal, this->tileLength);
        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(
              xGm[(progress - 1) * this->tileLength + this->lasttileLength],
              yLocal, this->tileLength);
        }
#endif

      } else {
        DataCopy(xGm[progress * this->tileLength], yLocal, this->tileLength);
      }
    }

    if (BUFFER_NUM == 2) {
      //开启double
      // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
      // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
      if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
          (progress == (this->tileNum * BUFFER_NUM - 1))) {

        if (progress < 2) {
          DataCopy(xGm[progress * (this->tileLength)], yLocal,
                   (this->tileLength));
        } else {
          //分块大小变为tileLength的一半
          //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
          DataCopy(
              xGm[(progress - 2) * (this->tileLength) + this->lasttileLength],
              yLocal, (this->tileLength));
        }
      } else {
        DataCopy(xGm[progress * (this->tileLength)], yLocal,
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

  // TBuf<QuePosition::VECCALC> comBuf;

  // TBuf<QuePosition::VECCALC> iBuf, uBuf;
  TQue<QuePosition::VECIN, BUFFER_NUM> iQue;

  // GlobalTensor<float> xGm;
  // GlobalTensor<float> yGm;
  // GlobalTensor<float> outGm;

  //   GlobalTensor<DTYPE_VALUE> valueGm;
  //   GlobalTensor<DTYPE_INPUT_DATA> input_dataGm;
  //   GlobalTensor<DTYPE_X2> x2Gm;

  GlobalTensor<DTYPE_X> xGm;
  GlobalTensor<DTYPE_INDEX> iGm;
  GlobalTensor<DTYPE_X> uGm;

  // GlobalTensor<DTYPE_Y> yGm;

  DTYPE_X negAttr;  //-1.702
  DTYPE_X halfAttr; // 1.702/2=0.851
  DTYPE_X one;

  float attrScaler;
  // float sum; //用于存放sum
  // float sum; //用于存放sum
  // uint32_t dataTotalLength;

  uint32_t dataType;

  int32_t indexLength;
  uint32_t updateType;

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

extern "C" __global__ __aicore__ void scatter_sub(GM_ADDR x, GM_ADDR index,
                                                  GM_ADDR update,
                                                  GM_ADDR workspace,
                                                  GM_ADDR tiling) {
  GET_TILING_DATA(tiling_data, tiling);
  // TODO: user kernel impl
  KernelScatterSub op;

  uint32_t tilingKey = 1;
  if (TILING_KEY_IS(1)) {
    tilingKey = 1;
  } else if (TILING_KEY_IS(2)) {
    tilingKey = 2;
  } else {
    tilingKey = 1;
  }

  op.Init(
      x, index, update, tiling_data.dataType, tiling_data.indexLength,
      tiling_data.updateType, tiling_data.blockLength, tiling_data.tileNum,
      tiling_data.tileLength, tiling_data.lasttileLength, tiling_data.formerNum,
      tiling_data.formerLength, tiling_data.formertileNum,
      tiling_data.formertileLength, tiling_data.formerlasttileLength,
      tiling_data.tailNum, tiling_data.tailLength, tiling_data.tailtileNum,
      tiling_data.tailtileLength, tiling_data.taillasttileLength, tilingKey);
  op.Process();
}

#ifndef __CCE_KT_TEST__
// call of kernel function
void scatter_sub_do(uint32_t blockDim, void *l2ctrl, void *stream, GM_ADDR x,
                    GM_ADDR index, GM_ADDR update, GM_ADDR workspace,
                    GM_ADDR tiling) {
  scatter_sub<<<blockDim, l2ctrl, stream>>>(x, index, update, workspace,
                                            tiling);

  aclrtSynchronizeStream(stream);

  // 这个问题需要解决呢？
  // GET_TILING_DATA(tiling_data, tiling);

  //   std::cout << "reduction" << tiling_data.reduction
  //             << ", BlockNum=" << GetBlockNum() << "TILING_KEY_IS(1)"
  //             << TILING_KEY_IS(1) << std::endl;
}
#endif