/*
 *
 *
 * Function :
 *
 */
#include "kernel_operator.h"
using namespace AscendC;

// constexpr int32_t TOTAL_LENGTH_IN = 8 * 2048;// 8 * 2048 // total length of
// data
constexpr int32_t TOTAL_LENGTH = 8 * 2048;
constexpr int32_t USE_CORE_NUM = 8; // num of core used
constexpr int32_t BLOCK_LENGTH =
    TOTAL_LENGTH / USE_CORE_NUM;  // length computed of each core
constexpr int32_t TILE_NUM = 16;  // split data into 8 tiles for each core
constexpr int32_t BUFFER_NUM = 1; // tensor num for each queue
constexpr int32_t TILE_LENGTH =
    BLOCK_LENGTH / TILE_NUM /
    BUFFER_NUM; // seperate to 2 parts, due to double buffer

class KernelMseLoss {
public:
  __aicore__ inline KernelMseLoss() {}
  __aicore__ inline void Init(GM_ADDR x, GM_ADDR y, GM_ADDR out) {
    this->blockLength = BLOCK_LENGTH;
    this->tileNum = TILE_NUM;
    ASSERT(tileNum != 0 && "tile num can not be zero!");
    this->tileLength = TILE_LENGTH;
    this->reduction = 1; // 与gen_data.py内value保存一致

    xGm.SetGlobalBuffer((__gm__ float *)x + this->blockLength * GetBlockIdx(),
                        this->blockLength);
    yGm.SetGlobalBuffer((__gm__ float *)y + this->blockLength * GetBlockIdx(),
                        this->blockLength);
    outGm.SetGlobalBuffer((__gm__ float *)out +
                              this->blockLength * GetBlockIdx(),
                          this->blockLength);

    pipe.InitBuffer(inQueueIN, BUFFER_NUM,
                    this->tileLength * 2 * sizeof(float));
    pipe.InitBuffer(outQueueOUT, BUFFER_NUM, this->tileLength * sizeof(float));
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
    LocalTensor<float> inLocal = inQueueIN.AllocTensor<float>();
    DataCopy(inLocal[0], xGm[progress * (this->tileLength)],
             (this->tileLength));
    DataCopy(inLocal[this->tileLength], yGm[progress * this->tileLength],
             this->tileLength);

    inQueueIN.EnQue(inLocal);
  }
  __aicore__ inline void Compute(int32_t progress) {
    LocalTensor<float> inLocal = inQueueIN.DeQue<float>();
    LocalTensor<float> xLocal = inLocal;
    LocalTensor<float> yLocal = inLocal[this->tileLength];

    LocalTensor<float> outLocal = outQueueOUT.AllocTensor<float>();

    Sub<float>(outLocal, xLocal, yLocal, this->tileLength);
    Mul<float>(outLocal, outLocal, outLocal, this->tileLength);

    // 需要考虑最后tiling的长度
    if ((1 == this->reduction) || (2 == this->reduction)) {
      ReduceSum<float>(outLocal, outLocal, xLocal, this->tileLength);
    }

    outQueueOUT.EnQue<float>(outLocal);

    inQueueIN.FreeTensor(inLocal);
  }
  __aicore__ inline void CopyOut(int32_t progress) {
    LocalTensor<float> outLocal = outQueueOUT.DeQue<float>();
    DataCopy(outGm[progress * this->tileLength], outLocal, this->tileLength);
    if ((1 == this->reduction) || (2 == this->reduction)) {
      DataCopy(outGm[progress], outLocal, 32);
    }

    outQueueOUT.FreeTensor(outLocal);
  }

private:
  TPipe pipe;
  TQue<QuePosition::VECIN, BUFFER_NUM> inQueueIN;
  TQue<QuePosition::VECOUT, BUFFER_NUM> outQueueOUT;

  GlobalTensor<float> xGm;
  GlobalTensor<float> yGm;
  GlobalTensor<float> outGm;
  uint32_t reduction;
  uint32_t blockLength;
  uint32_t tileNum;
  uint32_t tileLength;
};

extern "C" __global__ __aicore__ void mse_loss(GM_ADDR x, GM_ADDR y,
                                               GM_ADDR out) {
  KernelMseLoss op;
  op.Init(x, y, out);
  op.Process();
}

#ifndef __CCE_KT_TEST__
// call of kernel function
void mse_loss_do(uint32_t blockDim, void *l2ctrl, void *stream, uint8_t *x,
                 uint8_t *y, uint8_t *out) {
  mse_loss<<<blockDim, l2ctrl, stream>>>(x, y, out);

  aclrtSynchronizeStream(stream);

  float sum;
  float t;

  uint32_t offset = 0;
  int sumCount = 0;

  for (int i = 0; i < USE_CORE_NUM; i++) {

    sumCount = TILE_NUM * BUFFER_NUM;

    for (int j = 0; j < sumCount; j++) {
      DataCopy(&t, out + offset + j * sizeof(float), sizeof(float));
      sum += t;
    }

    offset += BLOCK_LENGTH * sizeof(float);
  }

  DataCopy(out, &sum, sizeof(float));
}
#endif