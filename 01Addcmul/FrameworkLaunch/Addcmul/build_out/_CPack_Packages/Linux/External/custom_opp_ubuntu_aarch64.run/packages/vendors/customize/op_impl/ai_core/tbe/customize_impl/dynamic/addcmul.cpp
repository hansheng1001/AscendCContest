#include "kernel_operator.h"

#if 0
extern "C" __global__ __aicore__ void addcmul(GM_ADDR value, GM_ADDR input_data, GM_ADDR x1, GM_ADDR x2, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
    GET_TILING_DATA(tiling_data, tiling);
    // TODO: user kernel impl
}
#endif

using namespace AscendC;
// constexpr int32_t BUFFER_NUM = 2;
constexpr int32_t BUFFER_NUM = 1;

class KernelAddcmul {
public:
  __aicore__ inline KernelAddcmul() {}
  __aicore__ inline void
  Init(GM_ADDR input_data, GM_ADDR x1, GM_ADDR x2, GM_ADDR pValue, GM_ADDR y,
       uint32_t blockLength, uint32_t tileNum, uint32_t tileLength,
       uint32_t lasttileLength, uint32_t formerNum, uint32_t formerLength,
       uint32_t formertileNum, uint32_t formertileLength,
       uint32_t formerlasttileLength, uint32_t tailNum, uint32_t tailLength,
       uint32_t tailtileNum, uint32_t tailtileLength,
       uint32_t taillasttileLength, uint32_t tilingKey) {
    ASSERT(GetBlockNum() != 0 && "block dim can not be zero!");

    valueGm.SetGlobalBuffer((__gm__ DTYPE_VALUE *)pValue, 1);

    this->value = valueGm.GetValue(0);

    if (tilingKey == 1) {
      this->blockLength = blockLength;
      this->tileNum =
          tileNum ASSERT(tileNum != 0 && "tile num can not be zero!");
      this->tileLength = tileLength / BUFFER_NUM;
      this->lasttileLength = lasttileLength;
      //   this->lasttileLength = lasttileLength / BUFFER_NUM;

      input_dataGm.SetGlobalBuffer((__gm__ DTYPE_INPUT_DATA *)input_data +
                                       this->blockLength * GetBlockIdx(),
                                   this->blockLength);
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

        input_dataGm.SetGlobalBuffer((__gm__ DTYPE_INPUT_DATA *)input_data +
                                         this->formerLength * GetBlockIdx(),
                                     this->formerLength);

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

        input_dataGm.SetGlobalBuffer((__gm__ DTYPE_INPUT_DATA *)input_data +
                                         this->formerLength * this->formerNum +
                                         this->tailLength *
                                             (GetBlockIdx() - this->formerNum),
                                     this->tailLength);

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
                    this->tileLength * 3 * sizeof(DTYPE_X1));
    pipe.InitBuffer(outQueueOUT, BUFFER_NUM,
                    this->tileLength * sizeof(DTYPE_Y));
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
          DataCopy(inLocal[0], input_dataGm[progress * this->tileLength],
                   this->tileLength);
          DataCopy(inLocal[this->tileLength], x1Gm[progress * this->tileLength],
                   this->tileLength);
          DataCopy(inLocal[2 * this->tileLength],
                   x2Gm[progress * this->tileLength], this->tileLength);
        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(inLocal[0],
                   input_dataGm[(progress - 1) * this->tileLength +
                                this->lasttileLength],
                   this->tileLength);
          DataCopy(
              inLocal[this->tileLength],
              x1Gm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);
          DataCopy(
              inLocal[2 * this->tileLength],
              x2Gm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);
        }

      } else {

        DataCopy(inLocal[0], input_dataGm[progress * this->tileLength],
                 this->tileLength);
        DataCopy(inLocal[this->tileLength], x1Gm[progress * this->tileLength],
                 this->tileLength);
        DataCopy(inLocal[2 * this->tileLength],
                 x2Gm[progress * this->tileLength], this->tileLength);
      }
    }

    if (BUFFER_NUM == 2) {
      //开启double
      // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
      // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
#if 1
      if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
          (progress == (this->tileNum * BUFFER_NUM - 1))) {

        // 只有一个分块
        if (progress < 2) {

          DataCopy(inLocal[0], input_dataGm[progress * this->tileLength],
                   this->tileLength);
          DataCopy(inLocal[this->tileLength], x1Gm[progress * this->tileLength],
                   this->tileLength);
          DataCopy(inLocal[2 * this->tileLength],
                   x2Gm[progress * this->tileLength], this->tileLength);

        } else {

          //分块大小变为tileLength的一半
          //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动

          DataCopy(inLocal[0],
                   input_dataGm[(progress - 2) * (this->tileLength) +
                                this->lasttileLength],
                   this->tileLength);
          DataCopy(
              inLocal[this->tileLength],
              x1Gm[(progress - 2) * (this->tileLength) + this->lasttileLength],
              this->tileLength);
          DataCopy(
              inLocal[2 * this->tileLength],
              x2Gm[(progress - 2) * (this->tileLength) + this->lasttileLength],
              this->lasttileLength);
        }
      } else {
        DataCopy(inLocal[0], input_dataGm[progress * this->tileLength],
                 this->tileLength);
        DataCopy(inLocal[this->tileLength], x1Gm[progress * this->tileLength],
                 this->tileLength);
        DataCopy(inLocal[2 * this->tileLength],
                 x2Gm[progress * this->tileLength], this->tileLength);
      }
#endif

#if 0
      if (progress == (this->tileNum * BUFFER_NUM - 2)) {

        DataCopy(inLocal[0], input_dataGm[progress * this->tileLength],
                 this->lasttileLength);
        DataCopy(inLocal[this->tileLength], x1Gm[progress * this->tileLength],
                 this->lasttileLength);
        DataCopy(inLocal[2 * this->tileLength],
                 x2Gm[progress * this->tileLength], this->lasttileLength);

      } else if (progress == (this->tileNum * BUFFER_NUM - 1)) {
        DataCopy(inLocal[0],
                 input_dataGm[(progress - 1) * (this->tileLength) +
                              this->lasttileLength],
                 this->lasttileLength);

        DataCopy(
            inLocal[this->tileLength],
            x1Gm[(progress - 1) * (this->tileLength) + this->lasttileLength],
            this->lasttileLength);

        DataCopy(
            inLocal[2 * this->tileLength],
            x2Gm[(progress - 1) * (this->tileLength) + this->lasttileLength],
            this->lasttileLength);
      } else {
        DataCopy(inLocal[0], input_dataGm[progress * this->tileLength],
                 this->tileLength);
        DataCopy(inLocal[this->tileLength], x1Gm[progress * this->tileLength],
                 this->tileLength);
        DataCopy(inLocal[2 * this->tileLength],
                 x2Gm[progress * this->tileLength], this->tileLength);
      }
#endif
    }

    inQueueIN.EnQue(inLocal);
  }

  __aicore__ inline void Compute(int32_t progress) {
    LocalTensor<DTYPE_X1> inLocal = inQueueIN.DeQue<DTYPE_X1>();

    LocalTensor<DTYPE_INPUT_DATA> input_dataLocal = inLocal;
    LocalTensor<DTYPE_X1> x1Local = inLocal[this->tileLength];
    LocalTensor<DTYPE_X2> x2Local = inLocal[2 * this->tileLength];

    LocalTensor<DTYPE_Y> yLocal = outQueueOUT.AllocTensor<DTYPE_Y>();

    // Div(outLocal, yLocal, zLocal, this->tileLength);
    // Muls(outLocal, outLocal, this->value, this->tileLength);
    // Add(outLocal, xLocal, outLocal, this->tileLength);

    // Mul(x1Local, x1Local, x2Local, this->tileLength);
    // Muls(x1Local, x1Local, this->value, this->tileLength);
    // Add(yLocal, x1Local, input_dataLocal, this->tileLength);

    Mul(yLocal, x1Local, x2Local, this->tileLength);
    Muls(yLocal, yLocal, this->value, this->tileLength);
    Add(yLocal, input_dataLocal, yLocal, this->tileLength);

    outQueueOUT.EnQue<DTYPE_Y>(yLocal);

    inQueueIN.FreeTensor(inLocal);
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

#if 0
        DataCopy(outGm[progress * this->tileLength], outLocal,
                 this->lasttileLength);
#endif

        // 在最后一个流水线,把最后的求和结果存放到outGm[0]中
        // 还是未考虑溢出的问题
        // if ((1 == this->reduction) || (2 == this->reduction)) {
        //   DataCopy(outGm[0], &(this->sum), 2);
        // }

      } else {
        DataCopy(yGm[progress * this->tileLength], yLocal, this->tileLength);
      }
    }

    if (BUFFER_NUM == 2) {
      //开启double
      // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
      // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
#if 1
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
#endif

#if 0
      if (progress == (this->tileNum * BUFFER_NUM - 2)) {
        //分块大小变为tileLength的一半
        //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
        DataCopy(outGm[progress * (this->tileLength)], outLocal,
                 this->blockLength);

        // // 拷贝最后的结果到Global Memory中去
        // if ((1 == this->reduction) || (2 == this->reduction)) {
        //   DataCopy(outGm[0], this->sum, 1);
        // }
      } else if (progress == (this->tileNum * BUFFER_NUM - 1)) {
        //分块大小变为tileLength的一半
        //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
        DataCopy(
            outGm[(progress - 1) * (this->tileLength) + this->lasttileLength],
            outLocal, this->lasttileLength);

        // if ((1 == this->reduction) || (2 == this->reduction)) {
        //   DataCopy(outGm[0], this->sum, 1);
        // }
      } else {
        DataCopy(outGm[progress * (this->tileLength)], outLocal,
                 (this->tileLength));
      }
#endif
    }

    outQueueOUT.FreeTensor(yLocal);
  }

private:
  TPipe pipe;
  // TQue<QuePosition::VECIN, BUFFER_NUM> inQueueX, inQueueY, inQueueZ;
  TQue<QuePosition::VECIN, BUFFER_NUM> inQueueIN;
  TQue<QuePosition::VECOUT, BUFFER_NUM> outQueueOUT;
  // GlobalTensor<float> xGm;
  // GlobalTensor<float> yGm;
  // GlobalTensor<float> outGm;
  GlobalTensor<DTYPE_VALUE> valueGm;

  GlobalTensor<DTYPE_INPUT_DATA> input_dataGm;
  GlobalTensor<DTYPE_X1> x1Gm;
  GlobalTensor<DTYPE_X2> x2Gm;

  GlobalTensor<DTYPE_Y> yGm;

  DTYPE_VALUE value;
  // float value;
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

extern "C" __global__ __aicore__ void addcmul(GM_ADDR input_data, GM_ADDR x1,
                                              GM_ADDR x2, GM_ADDR value,
                                              GM_ADDR y, GM_ADDR workspace,
                                              GM_ADDR tiling) {
  GET_TILING_DATA(tiling_data, tiling);
  // TODO: user kernel impl
  KernelAddcmul op;

  uint32_t tilingKey = 1;
  if (TILING_KEY_IS(1)) {
    tilingKey = 1;
  } else if (TILING_KEY_IS(2)) {
    tilingKey = 2;
  } else {
    tilingKey = 1;
  }

  op.Init(input_data, x1, x2, value, y, tiling_data.blockLength,
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
void addcmul_do(uint32_t blockDim, void *l2ctrl, void *stream,
                GM_ADDR input_data, GM_ADDR x1, GM_ADDR x2, GM_ADDR value,
                GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
  addcmul<<<blockDim, l2ctrl, stream>>>(input_data, x1, x2, value, y, workspace,
                                        tiling);

  aclrtSynchronizeStream(stream);

  // 这个问题需要解决呢？
  // GET_TILING_DATA(tiling_data, tiling);

  //   std::cout << "reduction" << tiling_data.reduction
  //             << ", BlockNum=" << GetBlockNum() << "TILING_KEY_IS(1)"
  //             << TILING_KEY_IS(1) << std::endl;
}
#endif