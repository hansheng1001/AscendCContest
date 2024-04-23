#include "kernel_operator.h"

#if 0
extern "C" __global__ __aicore__ void clip_by_value(GM_ADDR x, GM_ADDR minv, GM_ADDR maxv, GM_ADDR z, GM_ADDR workspace, GM_ADDR tiling) {
    GET_TILING_DATA(tiling_data, tiling);
    // TODO: user kernel impl
}
#endif

using namespace AscendC;
// constexpr int32_t BUFFER_NUM = 2;
constexpr int32_t BUFFER_NUM = 1;

class KernelClipByValue {
public:
  __aicore__ inline KernelClipByValue() {}
  __aicore__ inline void Init(GM_ADDR x, GM_ADDR minv, GM_ADDR maxv, GM_ADDR z,
                              uint32_t blockLength, uint32_t tileNum,
                              uint32_t tileLength, uint32_t lasttileLength,
                              uint32_t formerNum, uint32_t formerLength,
                              uint32_t formertileNum, uint32_t formertileLength,
                              uint32_t formerlasttileLength, uint32_t tailNum,
                              uint32_t tailLength, uint32_t tailtileNum,
                              uint32_t tailtileLength,
                              uint32_t taillasttileLength, uint32_t tilingKey) {
    ASSERT(GetBlockNum() != 0 && "block dim can not be zero!");

    minvGm.SetGlobalBuffer((__gm__ DTYPE_X *)minv, 1);
    this->attrMin = minvGm.GetValue(0);

    maxvGm.SetGlobalBuffer((__gm__ DTYPE_X *)maxv, 1);
    this->attrMax = maxvGm.GetValue(0);

    // this->attrOppMax = 0 - this->attrMax;

    this->attrNegOne = -1;

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

      zGm.SetGlobalBuffer((__gm__ DTYPE_Z *)z +
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

        zGm.SetGlobalBuffer((__gm__ DTYPE_Z *)z +
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

        zGm.SetGlobalBuffer(
            (__gm__ DTYPE_Z *)z + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);
      }
    }

    pipe.InitBuffer(inQueueIN, BUFFER_NUM, this->tileLength * sizeof(DTYPE_X));
    pipe.InitBuffer(outQueueOUT, BUFFER_NUM,
                    this->tileLength * sizeof(DTYPE_Z));
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

    inQueueIN.EnQue(inLocal);
  }

  __aicore__ inline void Compute(int32_t progress) {
    LocalTensor<DTYPE_X> inLocal = inQueueIN.DeQue<DTYPE_X>();

    LocalTensor<DTYPE_X> xLocal = inLocal;

    LocalTensor<DTYPE_Z> zLocal = outQueueOUT.AllocTensor<DTYPE_Z>();

#if 0
    // 1.把比min小的元素修改为min
    // 注意没有Subs这个API
    Subs(xLocal, xLocal, this->attrMin, this->tileLength);
    Relu(xLocal, xLocal, this->tileLength);
    Adds(xLocal, xLocal, this->attrMin, this->tileLength);

    // 2.把比max大的元素修改为max
    // 2.1 求-x
    Muls(xLocal, xLocal, this->attrNegOne, this->tileLength);
    // 2.2 求(-x)-(-max)=-x+max
    Adds(xLocal, xLocal, this->attrMax, this->tileLength);
    Relu(xLocal, xLocal, this->tileLength);
    // 2.3 求Relu(-x)+(-max) = Relu(-x)-max
    Subs(xLocal, xLocal, this->attrMax, this->tileLength);

    // 3.求x
    Muls(zLocal, xLocal, this->attrNegOne, this->tileLength);
#endif

    // 把比max大的元素修改为max
    Mins(xLocal, xLocal, this->attrMax, this->tileLength);

    //把比min小的元素修改为min
    Maxs(zLocal, xLocal, this->attrMin, this->tileLength);

    outQueueOUT.EnQue<DTYPE_Z>(zLocal);

    inQueueIN.FreeTensor(inLocal);
  }

  __aicore__ inline void CopyOut(int32_t progress) {
    LocalTensor<DTYPE_Z> zLocal = outQueueOUT.DeQue<DTYPE_Z>();

    if (BUFFER_NUM == 1) {
      if (progress == this->tileNum - 1) {

#if 1
        if (progress == 0) {
          //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
          DataCopy(zGm[0], zLocal, this->tileLength);
        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(
              zGm[(progress - 1) * this->tileLength + this->lasttileLength],
              zLocal, this->tileLength);
        }
#endif

      } else {
        DataCopy(zGm[progress * this->tileLength], zLocal, this->tileLength);
      }
    }

    if (BUFFER_NUM == 2) {
      //开启double
      // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
      // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
      if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
          (progress == (this->tileNum * BUFFER_NUM - 1))) {

        if (progress < 2) {
          DataCopy(zGm[progress * (this->tileLength)], zLocal,
                   (this->tileLength));
        } else {
          //分块大小变为tileLength的一半
          //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
          DataCopy(
              zGm[(progress - 2) * (this->tileLength) + this->lasttileLength],
              zLocal, (this->tileLength));
        }
      } else {
        DataCopy(zGm[progress * (this->tileLength)], zLocal,
                 (this->tileLength));
      }
    }

    outQueueOUT.FreeTensor(zLocal);
  }

private:
  TPipe pipe;
  // TQue<QuePosition::VECIN, BUFFER_NUM> inQueueX, inQueueY, inQueueZ;
  TQue<QuePosition::VECIN, BUFFER_NUM> inQueueIN;
  TQue<QuePosition::VECOUT, BUFFER_NUM> outQueueOUT;
  // GlobalTensor<float> xGm;
  // GlobalTensor<float> yGm;
  // GlobalTensor<float> outGm;

  //   GlobalTensor<DTYPE_VALUE> valueGm;
  //   GlobalTensor<DTYPE_INPUT_DATA> input_dataGm;
  //   GlobalTensor<DTYPE_X2> x2Gm;

  GlobalTensor<DTYPE_X> xGm;
  GlobalTensor<DTYPE_X> minvGm;
  GlobalTensor<DTYPE_X> maxvGm;

  GlobalTensor<DTYPE_Z> zGm;

  DTYPE_X attrMin;
  DTYPE_X attrMax;

  DTYPE_X attrNegOne;

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

extern "C" __global__ __aicore__ void clip_by_value(GM_ADDR x, GM_ADDR minv,
                                                    GM_ADDR maxv, GM_ADDR z,
                                                    GM_ADDR workspace,
                                                    GM_ADDR tiling) {
  GET_TILING_DATA(tiling_data, tiling);
  // TODO: user kernel impl
  KernelClipByValue op;

  uint32_t tilingKey = 1;
  if (TILING_KEY_IS(1)) {
    tilingKey = 1;
  } else if (TILING_KEY_IS(2)) {
    tilingKey = 2;
  } else {
    tilingKey = 1;
  }

  op.Init(
      x, minv, maxv, z, tiling_data.blockLength, tiling_data.tileNum,
      tiling_data.tileLength, tiling_data.lasttileLength, tiling_data.formerNum,
      tiling_data.formerLength, tiling_data.formertileNum,
      tiling_data.formertileLength, tiling_data.formerlasttileLength,
      tiling_data.tailNum, tiling_data.tailLength, tiling_data.tailtileNum,
      tiling_data.tailtileLength, tiling_data.taillasttileLength, tilingKey);
  op.Process();
}

#ifndef __CCE_KT_TEST__
// call of kernel function
void clip_by_value_do(uint32_t blockDim, void *l2ctrl, void *stream, GM_ADDR x,
                      GM_ADDR minv, GM_ADDR maxv, GM_ADDR z, GM_ADDR workspace,
                      GM_ADDR tiling) {
  clip_by_value<<<blockDim, l2ctrl, stream>>>(dy, x, z, workspace, tiling);

  aclrtSynchronizeStream(stream);

  // 这个问题需要解决呢？
  // GET_TILING_DATA(tiling_data, tiling);

  //   std::cout << "reduction" << tiling_data.reduction
  //             << ", BlockNum=" << GetBlockNum() << "TILING_KEY_IS(1)"
  //             << TILING_KEY_IS(1) << std::endl;
}
#endif
