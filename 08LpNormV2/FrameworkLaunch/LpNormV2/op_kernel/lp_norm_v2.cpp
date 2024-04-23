#include "kernel_operator.h"
#include <cmath>

#if 0
extern "C" __global__ __aicore__ void lp_norm_v2(GM_ADDR x, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
    GET_TILING_DATA(tiling_data, tiling);
    // TODO: user kernel impl
}
#endif

using namespace AscendC;
// constexpr int32_t BUFFER_NUM = 2;
constexpr int32_t BUFFER_NUM = 1;

class KernelLpNormV2 {
public:
  __aicore__ inline KernelLpNormV2() {}
  __aicore__ inline void
  Init(GM_ADDR x, GM_ADDR y, float p, uint32_t axes, bool keepdim,
       float epsilon, uint32_t dataType, uint32_t blockLength, uint32_t tileNum,
       uint32_t tileLength, uint32_t lasttileLength, uint32_t formerNum,
       uint32_t formerLength, uint32_t formertileNum, uint32_t formertileLength,
       uint32_t formerlasttileLength, uint32_t tailNum, uint32_t tailLength,
       uint32_t tailtileNum, uint32_t tailtileLength,
       uint32_t taillasttileLength, uint32_t tilingKey) {
    ASSERT(GetBlockNum() != 0 && "block dim can not be zero!");

    this->p = p;
    this->axes = axes;
    this->keepdim = keepdim;
    this->epsilon = epsilon;

    this->dataType = dataType;

    this->sum = 0.0f;
    this->attrSqrt = 0.0;

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

        xGm.SetGlobalBuffer((__gm__ DTYPE_X *)x +
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

        xGm.SetGlobalBuffer(
            (__gm__ DTYPE_X *)x + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);

        yGm.SetGlobalBuffer(
            (__gm__ DTYPE_Y *)y + this->formerLength * this->formerNum +
                this->tailLength * (GetBlockIdx() - this->formerNum),
            this->tailLength);
      }
    }

    pipe.InitBuffer(inQueueIN, BUFFER_NUM,
                    this->tileLength * 2 * sizeof(DTYPE_X));

    pipe.InitBuffer(outQueueOUT, BUFFER_NUM, this->tileLength * sizeof(float));

    pipe.InitBuffer(fp32Queue, BUFFER_NUM,
                    this->tileLength * sizeof(float)); // *从这个fp32Queue拿空间

    pipe.InitBuffer(attrOne, 1, this->tileLength * sizeof(float));
    pipe.InitBuffer(attrZero, 1, this->tileLength * sizeof(float));

    pipe.InitBuffer(selMask, 1, this->tileLength * sizeof(uint8_t));
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
          //将最后一个分块的起始地址向前移动tileLength-lasttileLength
          DataCopy(inLocal[0], xGm[progress * this->tileLength],
                   this->tileLength);
        } else {
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

  // template <typename T>
  // __aicore__ inline void
  // Quickpower(const LocalTensor<T> &dstTensor, const LocalTensor<T>
  // &srcTensor,
  //            const float scalarValue, const uint32_t calCount) {
  //   Ln(srcTensor, srcTensor, calCount);
  //   Muls(srcTensor, srcTensor, static_cast<T>(scalarValue), calCount);
  //   Exp(dstTensor, srcTensor, calCount);
  // }

  // template <typename T>
  __aicore__ inline void Quickpower(const LocalTensor<float> &dstTensor,
                                    const LocalTensor<float> &srcTensor,
                                    const float scalarValue,
                                    const uint32_t calCount) {
    Ln(srcTensor, srcTensor, calCount);
    Muls(srcTensor, srcTensor, scalarValue, calCount);
    Exp(dstTensor, srcTensor, calCount);
  }

  __aicore__ inline void Compute(int32_t progress) {
    LocalTensor<DTYPE_X> inLocal = inQueueIN.DeQue<DTYPE_X>();
    LocalTensor<DTYPE_X> xLocal = inLocal;

    // LocalTensor<DTYPE_X> workLocal = inLocal[this->tileLength];

    LocalTensor<float> fpxLocal = fp32Queue.AllocTensor<float>();
    LocalTensor<float> fp32xLocal =
        fpxLocal;              // *这个用来代替之前的xLocal进行运算
    if (this->dataType == 1) { // *fp16->float
      LocalTensor<half> interxLocal = xLocal.ReinterpretCast<half>();
      AscendC::Cast(fp32xLocal, interxLocal, RoundMode::CAST_NONE,
                    this->tileLength);
    } else { // *fp32->float
      LocalTensor<float> interxLocal = xLocal.ReinterpretCast<float>();
      AscendC::Cast(fp32xLocal, interxLocal, RoundMode::CAST_NONE,
                    this->tileLength);
    }

    LocalTensor<float> yLocal = outQueueOUT.AllocTensor<float>();

    LocalTensor<float> oneLocal = attrOne.Get<float>();
    LocalTensor<float> zeroLocal = attrZero.Get<float>();

    LocalTensor<uint8_t> selMaskLocal = selMask.Get<uint8_t>();

    if (1 == BUFFER_NUM) {
      if (progress == this->tileNum - 1) {

        if (((p - 2.0f) < 0.01f) && (2.0f - p < 0.01f)) {

          // Abs(xLocal, xLocal, this->lasttileLength);
          Mul(fp32xLocal, fp32xLocal, fp32xLocal, this->lasttileLength);

          ReduceSum(fp32xLocal, fp32xLocal, yLocal, this->lasttileLength);
          this->sum += fp32xLocal.GetValue(0);

        } else if ((p - 0.0f < 0.01f) && (0.0f - p < 0.01f)) {

          // 还有些问题,需要求出所有非0元素的个数
          Duplicate(oneLocal, (float)1.0, this->lasttileLength);
          Duplicate(zeroLocal, (float)0.0, this->lasttileLength);

          Abs(fp32xLocal, fp32xLocal, this->lasttileLength);

          Compare(selMaskLocal, fp32xLocal, zeroLocal, CMPMODE::GT,
                  this->lasttileLength);
          Select(fp32xLocal, selMaskLocal, oneLocal, zeroLocal,
                 SELMODE::VSEL_TENSOR_TENSOR_MODE, this->lasttileLength);

          ReduceSum(fp32xLocal, fp32xLocal, yLocal, this->lasttileLength);
          this->sum += fp32xLocal.GetValue(0);

        } else if ((p - 1.0f < 0.01f) && (1.0f - p < 0.01f)) {

          Abs(fp32xLocal, fp32xLocal, this->lasttileLength);
          ReduceSum(fp32xLocal, fp32xLocal, yLocal, this->lasttileLength);

          this->sum += fp32xLocal.GetValue(0);

        } else if ((p - 9999.f < 0.01f) && (9999.f - p < 0.01f)) {
          Abs(fp32xLocal, fp32xLocal, this->lasttileLength);
          ReduceMax(fp32xLocal, fp32xLocal, yLocal, this->lasttileLength);

          float t = fp32xLocal.GetValue(0);

          this->sum = t > this->sum ? t : this->sum;

        } else if ((p - (-9999.f) < 0.01f) && ((-9999.f) - p < 0.01f)) {
          // 这个有问题,这儿取出来都是0,需要先把0剔除出去
          Abs(fp32xLocal, fp32xLocal, this->lasttileLength);
          ReduceMin(fp32xLocal, fp32xLocal, yLocal, this->lasttileLength);

          float t = fp32xLocal.GetValue(0);

          this->sum = t < this->sum ? t : this->sum;

        } else {
          Abs(fp32xLocal, fp32xLocal, this->lasttileLength);
          Quickpower(fp32xLocal, fp32xLocal, p, this->lasttileLength);
          ReduceSum(fp32xLocal, fp32xLocal, yLocal, this->lasttileLength);

          this->sum += fp32xLocal.GetValue(0);
        }

      } else {

        if (((p - 2.0f) < 0.01f) && (2.0f - p < 0.01f)) {

          Abs(fp32xLocal, fp32xLocal, this->tileLength);
          Mul(fp32xLocal, fp32xLocal, fp32xLocal, this->tileLength);

          ReduceSum(fp32xLocal, fp32xLocal, yLocal, this->tileLength);
          this->sum += fp32xLocal.GetValue(0);

        } else if ((p - 0.0f < 0.01f) && (0.0f - p < 0.01f)) {

          // 还有些问题,需要求出所有非0元素的个数
          Duplicate(oneLocal, (float)1.0, this->tileLength);
          Duplicate(zeroLocal, (float)0.0, this->tileLength);

          Abs(fp32xLocal, fp32xLocal, this->tileLength);

          Compare(selMaskLocal, fp32xLocal, zeroLocal, CMPMODE::GT,
                  this->tileLength);
          Select(fp32xLocal, selMaskLocal, oneLocal, zeroLocal,
                 SELMODE::VSEL_TENSOR_TENSOR_MODE, this->tileLength);

          ReduceSum(fp32xLocal, fp32xLocal, yLocal, this->tileLength);
          this->sum += fp32xLocal.GetValue(0);

        } else if ((p - 1.0f) < 0.01f && (1.0f - p < 0.01f)) {

          Abs(fp32xLocal, fp32xLocal, this->tileLength);
          ReduceSum(fp32xLocal, fp32xLocal, yLocal, this->tileLength);

          this->sum += fp32xLocal.GetValue(0);

        } else if ((p - 9999.f < 0.01f) && (9999.f - p < 0.01f)) {
          Abs(fp32xLocal, fp32xLocal, this->tileLength);
          ReduceMax(fp32xLocal, fp32xLocal, yLocal, this->tileLength);

          float t = fp32xLocal.GetValue(0);

          this->sum = t > this->sum ? t : this->sum;

        } else if ((p - (-9999.f) < 0.01f) && ((-9999.f) - p < 0.01f)) {
          // 这个有问题,这儿取出来都是0,需要先把0剔除出去
          Abs(fp32xLocal, fp32xLocal, this->tileLength);
          ReduceMin(fp32xLocal, fp32xLocal, yLocal, this->tileLength);

          float t = fp32xLocal.GetValue(0);

          this->sum = t < this->sum ? t : this->sum;

        } else {
          Abs(fp32xLocal, fp32xLocal, this->tileLength);
          Quickpower(fp32xLocal, fp32xLocal, p, this->tileLength);
          ReduceSum(fp32xLocal, fp32xLocal, yLocal, this->tileLength);

          this->sum += fp32xLocal.GetValue(0);
        }
      }
    }

    outQueueOUT.EnQue<float>(yLocal);

    inQueueIN.FreeTensor(inLocal);
  }

  __aicore__ inline void CopyOut(int32_t progress) {
    LocalTensor<float> yLocal = outQueueOUT.DeQue<float>();

    // if (BUFFER_NUM == 1) {
    //   if (progress == this->tileNum - 1) {
    //     if (progress == 0) {
    //       //如果只有一包，则搬运的起始地址为0，tileLength为实际分块的数据量
    //       DataCopy(yGm[0], yLocal, this->tileLength);
    //     } else {
    //       //将最后一个分块的起始地址向前移动tileLength-lasttileLength
    //       DataCopy(
    //           yGm[(progress - 1) * this->tileLength + this->lasttileLength],
    //           yLocal, this->tileLength);
    //     }
    //   } else {
    //     DataCopy(yGm[progress * this->tileLength], yLocal, this->tileLength);
    //   }
    // }

    // if (BUFFER_NUM == 2) {
    //   //开启double
    //   // buffer时，由于将输入数据分成了相等的2部分，分块大小为不开启double
    //   // buffer的一半， 所以需要对最后两个分块数据的起始地址做处理
    //   if ((progress == (this->tileNum * BUFFER_NUM - 2)) ||
    //       (progress == (this->tileNum * BUFFER_NUM - 1))) {

    //     if (progress < 2) {
    //       DataCopy(yGm[progress * (this->tileLength)], yLocal,
    //                (this->tileLength));
    //     } else {
    //       //分块大小变为tileLength的一半
    //       //倒数第2个分块数据的起始地址向前移动（tileLength-lasttileLength)，最后一个分块的起始地址以此为基础进行移动
    //       DataCopy(
    //           yGm[(progress - 2) * (this->tileLength) +
    //           this->lasttileLength], yLocal, (this->tileLength));
    //     }
    //   } else {
    //     DataCopy(yGm[progress * (this->tileLength)], yLocal,
    //              (this->tileLength));
    //   }
    // }

    if (progress == (this->tileNum * BUFFER_NUM - 1)) {

#if 0
      if (2.0f == p) {
        this->attrSqrt = sqrt(this->sum);
        yGm.SetValue(0, this->attrSqrt);
        yGm.SetValue(1, this->sum);
      } else if (0.0f == p) {
        yGm.SetValue(0, this->sum);
      } else if (1.0f == p) {

        yGm.SetValue(0, this->sum);

      } else if (p > 9999.0f) {
        yGm.SetValue(0, this->sum);

      } else if (p < (-9999.f)) {
        yGm.SetValue(0, this->sum);
      } else {

        yLocal.SetValue(0, this->sum);
        Quickpower(yLocal, yLocal, ((float)1.0) / p, 1);

        yGm.SetValue(0, yLocal.GetValue(0));
      }
#endif

      if (((p - 2.0f) < 0.01f) && (2.0f - p < 0.01f)) {
        this->attrSqrt = sqrt(this->sum);
        yGm.SetValue(0, this->attrSqrt);
        yGm.SetValue(1, this->sum);

      } else if ((p - 0.0f < 0.01f) && (0.0f - p < 0.01f)) {

        this->sum =
            this->blockLength > this->sum ? this->sum : this->blockLength;
        yGm.SetValue(0, this->sum);

      } else if ((p - 1.0f < 0.01f) && (1.0f - p < 0.01f)) {
        yGm.SetValue(0, this->sum);
      } else if ((p - 9999.f < 0.01f) && (9999.f - p < 0.01f)) {
        yGm.SetValue(0, this->sum);
      } else if ((p - (-9999.f) < 0.01f) && ((-9999.f) - p < 0.01f)) {
        yGm.SetValue(0, this->sum);
      } else {

        yLocal.SetValue(0, this->sum);
        Quickpower(yLocal, yLocal, ((float)1.0) / p, 1);

        yGm.SetValue(0, yLocal.GetValue(0));
      }
      // outGm.SetValue(1, this->sum);
    }

    outQueueOUT.FreeTensor(yLocal);
  }

private:
  TPipe pipe;
  // TQue<QuePosition::VECIN, BUFFER_NUM> inQueueX, inQueueY, inQueueZ;
  TQue<QuePosition::VECIN, BUFFER_NUM> inQueueIN, fp32Queue;
  TQue<QuePosition::VECOUT, BUFFER_NUM> outQueueOUT;
  TBuf<QuePosition::VECCALC> attrOne, attrZero;

  TBuf<QuePosition::VECCALC> selMask;
  // GlobalTensor<float> xGm;
  // GlobalTensor<float> yGm;
  // GlobalTensor<float> outGm;

  //   GlobalTensor<DTYPE_VALUE> valueGm;
  //   GlobalTensor<DTYPE_INPUT_DATA> input_dataGm;
  //   GlobalTensor<DTYPE_X2> x2Gm;

  GlobalTensor<DTYPE_X> xGm;
  GlobalTensor<DTYPE_Y> yGm;

  float p;
  uint32_t axes;
  bool keepdim;
  float epsilon;

  float sum;
  float attrSqrt;

  // DTYPE_X negAttr;  //-1.702
  // DTYPE_X halfAttr; // 1.702/2=0.851
  // DTYPE_X one;
  // float sum; //用于存放sum
  // float sum; //用于存放sum
  uint32_t dataType;

  // uint32_t reduction;
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
lp_norm_v2(GM_ADDR x, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
  GET_TILING_DATA(tiling_data, tiling);
  // TODO: user kernel impl
  KernelLpNormV2 op;

  uint32_t tilingKey = 1;
  if (TILING_KEY_IS(1)) {
    tilingKey = 1;
  } else if (TILING_KEY_IS(2)) {
    tilingKey = 2;
  } else {
    tilingKey = 1;
  }

  op.Init(x, y, tiling_data.p, tiling_data.axes, tiling_data.keepdim,
          tiling_data.epsilon, tiling_data.dataType, tiling_data.blockLength,
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
void lp_norm_v2_do(uint32_t blockDim, void *l2ctrl, void *stream, GM_ADDR x,
                   GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
  lp_norm_v2<<<blockDim, l2ctrl, stream>>>(x, y, workspace, tiling);

  aclrtSynchronizeStream(stream);

  // 这个问题需要解决呢？
  // GET_TILING_DATA(tiling_data, tiling);

  //   std::cout << "reduction" << tiling_data.reduction
  //             << ", BlockNum=" << GetBlockNum() << "TILING_KEY_IS(1)"
  //             << TILING_KEY_IS(1) << std::endl;
}
#endif