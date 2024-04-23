#include "kernel_operator.h"

using namespace AscendC;
constexpr int32_t BUFFER_NUM = 2;

class KernelMseLoss {
public:
  __aicore__ inline KernelMseLoss() {}
  __aicore__ inline void
  Init(GM_ADDR x, GM_ADDR y, GM_ADDR out, uint32_t reduction,
       uint32_t blockLength, uint32_t tileNum, uint32_t tileLength,
       uint32_t lasttileLength, uint32_t formerNum, uint32_t formerLength,
       uint32_t formertileNum, uint32_t formertileLength,
       uint32_t formerlasttileLength, uint32_t tailNum, uint32_t tailLength,
       uint32_t tailtileNum, uint32_t tailtileLength,
       uint32_t taillasttileLength, uint32_t tilingKey) {
    ASSERT(GetBlockNum() != 0 && "block dim can not be zero!");

    // this->value = static_cast<float>(value);
    this->reduction = reduction;
    this->dataTotalLength = dataTotalLength;

    this->sum = 0.0;

    if (tilingKey == 1) {
      this->blockLength = blockLength;
      this->tileNum =
          tileNum ASSERT(tileNum != 0 && "tile num can not be zero!");
      this->tileLength = tileLength / BUFFER_NUM;
      //   this->lasttileLength = lasttileLength;
      this->lasttileLength = lasttileLength / BUFFER_NUM;

      xGm.SetGlobalBuffer((__gm__ DTYPE_X *)x +
                              this->blockLength * GetBlockIdx(),
                          this->blockLength);
      yGm.SetGlobalBuffer((__gm__ DTYPE_Y *)y +
                              this->blockLength * GetBlockIdx(),
                          this->blockLength);
      outGm.SetGlobalBuffer((__gm__ DTYPE_OUT *)out +
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
        // this->lasttileLength = this->formerlasttileLength;
        this->lasttileLength = this->formerlasttileLength / BUFFER_NUM;
        this->tileNum = this->formertileNum * BUFFER_NUM;
        xGm.SetGlobalBuffer((__gm__ DTYPE_X *)x +
                                this->formerLength * GetBlockIdx(),
                            this->formerLength);
        yGm.SetGlobalBuffer((__gm__ DTYPE_Y *)y +
                                this->formerLength * GetBlockIdx(),
                            this->formerLength);
        outGm.SetGlobalBuffer((__gm__ DTYPE_OUT *)out +
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
        yGm.SetGlobalBuffer(
            (__gm__ DTYPE_Y *)y + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);
        outGm.SetGlobalBuffer(
            (__gm__ DTYPE_OUT *)out + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);
      }
    }

    pipe.InitBuffer(inQueueIN, BUFFER_NUM,
                    this->tileLength * 2 * sizeof(DTYPE_X));
    pipe.InitBuffer(outQueueOUT, BUFFER_NUM,
                    this->tileLength * sizeof(DTYPE_OUT));
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
#if 0
        if (progress == 0) {
          //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
          DataCopy(inLocal[0], xGm[0], this->tileLength);
          DataCopy(inLocal[this->tileLength], yGm[0], this->tileLength);
        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(
              inLocal[0],
              xGm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);
          DataCopy(
              inLocal[this->tileLength],
              yGm[(progress - 1) * this->tileLength + this->lasttileLength],
              this->tileLength);
        }
#endif
        //将最后一个分块的起始地址向前移动tileLength-lasttileLength
        DataCopy(inLocal[0], xGm[progress * this->tileLength],
                 this->lasttileLength);
        DataCopy(inLocal[this->tileLength], yGm[progress * this->tileLength],
                 this->lasttileLength);

      } else {
        DataCopy(inLocal[0], xGm[progress * this->tileLength],
                 this->tileLength);
        DataCopy(inLocal[this->tileLength], yGm[progress * this->tileLength],
                 this->tileLength);
      }
    }

    if (BUFFER_NUM == 2) {
      //开启double
      // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
      // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
#if 0
      if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
          (progress == (this->tileNum * BUFFER_NUM - 1))) {
        //分块大小变为tileLength的一半
        //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
        DataCopy(
            inLocal[0],
            xGm[(progress - 2) * (this->tileLength) + this->lasttileLength],
            (this->tileLength));
        DataCopy(
            inLocal[this->tileLength],
            yGm[(progress - 2) * (this->tileLength) + this->lasttileLength],
            (this->tileLength));
      }
#endif
      if (progress == (this->tileNum * BUFFER_NUM - 2)) {
        DataCopy(inLocal[0], xGm[progress * (this->tileLength)],
                 this->lasttileLength);
        DataCopy(inLocal[this->tileLength], yGm[progress * (this->tileLength)],
                 this->lasttileLength);
      } else if (progress == (this->tileNum * BUFFER_NUM - 1)) {
        DataCopy(
            inLocal[0],
            xGm[(progress - 1) * (this->tileLength) + this->lasttileLength],
            this->lasttileLength);
        DataCopy(
            inLocal[this->tileLength],
            yGm[(progress - 1) * (this->tileLength) + this->lasttileLength],
            this->lasttileLength);
      } else {
        DataCopy(inLocal[0], xGm[progress * (this->tileLength)],
                 (this->tileLength));
        DataCopy(inLocal[this->tileLength], yGm[progress * this->tileLength],
                 this->tileLength);
      }
    }

    inQueueIN.EnQue(inLocal);
  }

  __aicore__ inline void Compute(int32_t progress) {
    LocalTensor<DTYPE_X> inLocal = inQueueIN.DeQue<DTYPE_X>();
    LocalTensor<DTYPE_X> xLocal = inLocal;
    LocalTensor<DTYPE_Y> yLocal = inLocal[this->tileLength];

    LocalTensor<DTYPE_OUT> outLocal = outQueueOUT.AllocTensor<DTYPE_OUT>();

    //采用div + Muls + Add实现
    // 似乎没有考虑溢出的问题
    // Div(outLocal, yLocal, zLocal, this->tileLength);
    // Muls(outLocal, outLocal, this->value, this->tileLength);
    // Add(outLocal, xLocal, outLocal, this->tileLength);

    if (1 == BUFFER_NUM) {
      if (progress == this->tileNum - 1) {

        // 需要考虑最后tiling的长度
        // 未考虑溢出的问题
        if ((1 == this->reduction) || (2 == this->reduction)) {
          Sub(xLocal, xLocal, yLocal, this->lasttileLength);
          Mul(xLocal, xLocal, xLocal, this->lasttileLength);

          ReduceSum(xLocal, xLocal, yLocal, this->lasttileLength);
          this->sum += (float)xLocal.GetValue(0);
          // ReduceSum<float>(outLocal, xLocal, xLocal, 1);
        } else {
          // 未考虑溢出的问题
          Sub(outLocal, xLocal, yLocal, this->lasttileLength);
          Mul(outLocal, outLocal, outLocal, this->lasttileLength);
        }
      } else {

        // 需要考虑最后tiling的长度
        if ((1 == this->reduction) || (2 == this->reduction)) {
          Sub(xLocal, xLocal, yLocal, this->tileLength);
          Mul(xLocal, xLocal, xLocal, this->tileLength);

          ReduceSum(xLocal, xLocal, yLocal, this->tileLength);
          this->sum += (float)xLocal.GetValue(0);

          // ReduceSum<float>(outLocal, xLocal, xLocal, 1);
        } else {
          // 未考虑溢出的问题
          Sub(outLocal, xLocal, yLocal, this->tileLength);
          Mul(outLocal, outLocal, outLocal, this->tileLength);
        }
      }
    } else if (2 == BUFFER_NUM) {
      if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
          (progress == (this->tileNum * BUFFER_NUM - 1))) {

        if ((1 == this->reduction) || (2 == this->reduction)) {
          Sub(xLocal, xLocal, yLocal, this->lasttileLength);
          Mul(xLocal, xLocal, xLocal, this->lasttileLength);

          ReduceSum(xLocal, xLocal, yLocal, this->lasttileLength);
          this->sum += (float)xLocal.GetValue(0);
          // ReduceSum<float>(outLocal, xLocal, xLocal, 1););
        } else {
          // 未考虑溢出的问题
          Sub(outLocal, xLocal, yLocal, this->lasttileLength);
          Mul(outLocal, outLocal, outLocal, this->lasttileLength);
        }
      } else {

        // 需要考虑最后tiling的长度
        if ((1 == this->reduction) || (2 == this->reduction)) {
          Sub(xLocal, xLocal, yLocal, this->tileLength);
          Mul(xLocal, xLocal, xLocal, this->tileLength);

          ReduceSum(xLocal, xLocal, yLocal, this->tileLength);
          this->sum += (float)xLocal.GetValue(0);
          // ReduceSum<float>(outLocal, xLocal, xLocal, 1);

          // this->sum += (float)outLocal.GetValue(0);
          // // this->sum += outLocal.GetValue(0);
        } else {
          // 未考虑溢出的问题
          Sub(outLocal, xLocal, yLocal, this->tileLength);
          Mul(outLocal, outLocal, outLocal, this->tileLength);
        }
      }
    }

    // MSELoss中的reduction：
    // 默认为mean:1; none:0;sum:2

    // // 需要进行求和, 还需要求平均
    // if ((1 == this->reduction) || (2 == this->reduction)) {
    //   this->sum += outLocal.GetValue(0);
    // }

    outQueueOUT.EnQue<DTYPE_OUT>(outLocal);

    inQueueIN.FreeTensor(inLocal);
  }
  __aicore__ inline void CopyOut(int32_t progress) {
    LocalTensor<DTYPE_OUT> outLocal = outQueueOUT.DeQue<DTYPE_OUT>();

    if (0 == this->reduction) {

      if (BUFFER_NUM == 1) {
        if (progress == this->tileNum - 1) {

#if 0        
        if (progress == 0) {
          //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
          DataCopy(outGm[0], outLocal, this->tileLength);
        } else {
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(
              outGm[(progress - 1) * this->tileLength + this->lasttileLength],
              outLocal, this->tileLength);
        }
#endif

          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(outGm[progress * this->tileLength], outLocal,
                   this->lasttileLength);

          // 在最后一个流水线,把最后的求和结果存放到outGm[0]中
          // 还是未考虑溢出的问题
          // if ((1 == this->reduction) || (2 == this->reduction)) {
          //   DataCopy(outGm[0], &(this->sum), 2);
          // }

        } else {
          DataCopy(outGm[progress * this->tileLength], outLocal,
                   this->tileLength);
        }
      }

      if (BUFFER_NUM == 2) {
        //开启double
        // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
        // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
#if 0
      if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
          (progress == (this->tileNum * BUFFER_NUM - 1))) {
        //分块大小变为tileLength的一半
        //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
        DataCopy(
            outGm[(progress - 2) * (this->tileLength) + this->lasttileLength],
            outLocal, (this->tileLength));
      }
#endif
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
      }

      // // 对于需要进行求sum类型,将和放入到
      // if ((1 == this->reduction) || (2 == this->reduction)) {
      //   DataCopy(outGm[progress], outLocal, 32);

      //   // DataCopy(outLocal, outGm[0], ((progress + 1 + 31) / 32) * 32);
      //   // ReduceSum<float>(outLocal, outLocal, outLocal, progress + 1);
      //   // DataCopy(outGm[0], outLocal, ((progress + 1 + 31) / 32) * 32);

      //   // 如果是最后一个tiling,还需要进行最后的求和

      //   // 首先这种方式是否正确
      //   // 另外当数据长度超过this->tileLength
      //   if (progress == this->tileNum * BUFFER_NUM - 1) {
      //     DataCopy(outLocal, outGm[0], ((progress + 1 + 31) / 32) * 32);
      //     ReduceSum<float>(outLocal, outLocal, outLocal, progress + 1);
      //     DataCopy(outGm[0], outLocal, ((progress + 1 + 31) / 32) * 32);
      //   }
      // }
    } else if (1 == this->reduction) {
      if (progress == (this->tileNum * BUFFER_NUM - 1)) {

        outGm.SetValue(0, (this->sum / this->blockLength));
        outGm.SetValue(1, this->sum);
      }
    } else if (2 == this->reduction) {
      if (progress == (this->tileNum * BUFFER_NUM - 1)) {
        outGm.SetValue(0, this->sum);
      }
    }

    outQueueOUT.FreeTensor(outLocal);
  }

private:
  TPipe pipe;
  // TQue<QuePosition::VECIN, BUFFER_NUM> inQueueX, inQueueY, inQueueZ;
  TQue<QuePosition::VECIN, BUFFER_NUM> inQueueIN;
  TQue<QuePosition::VECOUT, BUFFER_NUM> outQueueOUT;
  // GlobalTensor<float> xGm;
  // GlobalTensor<float> yGm;
  // GlobalTensor<float> outGm;
  GlobalTensor<DTYPE_X> xGm;
  GlobalTensor<DTYPE_Y> yGm;
  GlobalTensor<DTYPE_OUT> outGm;

  // float value;
  // float sum; //用于存放sum
  float sum; //用于存放sum
  uint32_t dataTotalLength;

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

extern "C" __global__ __aicore__ void
mse_loss(GM_ADDR x, GM_ADDR y, GM_ADDR out, GM_ADDR workspace, GM_ADDR tiling) {
  GET_TILING_DATA(tiling_data, tiling);
  // TODO: user kernel impl
  KernelMseLoss op;

  uint32_t tilingKey = 1;
  if (TILING_KEY_IS(1)) {
    tilingKey = 1;
  } else if (TILING_KEY_IS(2)) {
    tilingKey = 2;
  } else {
    tilingKey = 1;
  }

  op.Init(x, y, out, tiling_data.reduction, tiling_data.blockLength,
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
void mse_loss_do(uint32_t blockDim, void *l2ctrl, void *stream, uint8_t *x,
                 uint8_t *y, uint8_t *out, uint8_t *workspace,
                 uint8_t *tiling) {
  mse_loss<<<blockDim, l2ctrl, stream>>>(x, y, out, workspace, tiling);

  aclrtSynchronizeStream(stream);

  // 这个问题需要解决呢？
  // GET_TILING_DATA(tiling_data, tiling);

  std::cout << "reduction" << tiling_data.reduction
            << ", BlockNum=" << GetBlockNum() << "TILING_KEY_IS(1)"
            << TILING_KEY_IS(1) << std::endl;

  // if ((1 == tiling_data.reduction) || (2 == tiling_data.reduction)) {
  //   float sum = 0.0;
  //   float t = 0.0;

  //   uint32_t offset = 0;
  //   int sumCount = 0;

  //   if (TILING_KEY_IS(1)) {
  //     for (int i = 0; i < GetBlockNum(); i++) {

  //       sumCount = tiling_data.tileNum * BUFFER_NUM;

  //       for (int j = 0; j < sumCount; j++) {
  //         DataCopy(&t, out + offset + j * sizeof(float), sizeof(float));
  //         sum += t;
  //       }

  //       offset += tiling_data.blockLength * sizeof(float);
  //     }

  //   } else if (TILING_KEY_IS(2)) {

  //     for (int i = 0; i < GetBlockNum(); i++) {

  //       if (i < tiling_data.formerNum) {
  //         sumCount = tiling_data.formertileNum * BUFFER_NUM;
  //       } else {
  //         sumCount = tiling_data.tailtileNum * BUFFER_NUM;
  //       }

  //       for (int j = 0; j < sumCount; j++) {
  //         DataCopy(&t, out + offset + j * sizeof(float), sizeof(float));
  //         sum += t;
  //       }

  //       if (i < tiling_data.formerNum) {
  //         offset += tiling_data.formerLength * sizeof(float);
  //       } else {
  //         offset += tiling_data.tailLength * sizeof(float);
  //       }
  //     }
  //   } else {
  //     for (int i = 0; i < GetBlockNum(); i++) {

  //       sumCount = tiling_data.tileNum * BUFFER_NUM;

  //       for (int j = 0; j < sumCount; j++) {
  //         DataCopy(&t, out + offset + j * sizeof(float), sizeof(float));
  //         sum += t;
  //       }

  //       offset += tiling_data.blockLength * sizeof(float);
  //     }
  //   }
  //   DataCopy(out, &sum, sizeof(float));
  // }
}

#endif