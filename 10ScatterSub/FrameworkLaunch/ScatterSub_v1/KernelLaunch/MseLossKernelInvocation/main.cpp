/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * This file constains code of cpu debug and npu code.We read data from bin file
 * and write result to file.
 */
#include "data_utils.h"
// #include <Half.h>

#include "kernel_operator.h"

constexpr int32_t TOTAL_LENGTH = 8 * 2048;
constexpr int32_t USE_CORE_NUM = 8; // num of core used
constexpr int32_t BLOCK_LENGTH =
    TOTAL_LENGTH / USE_CORE_NUM;  // length computed of each core
constexpr int32_t TILE_NUM = 16;  // split data into 8 tiles for each core
constexpr int32_t BUFFER_NUM = 1; // tensor num for each queue

#ifndef __CCE_KT_TEST__
#include "acl/acl.h"
extern void mse_loss_do(uint32_t blockDim, void *l2ctrl, void *stream,
                        uint8_t *x, uint8_t *y, uint8_t *out)
#else
#include "tikicpulib.h"
extern "C" __global__ __aicore__ void mse_loss(GM_ADDR x, GM_ADDR y,
                                               GM_ADDR out);
#endif

    int32_t main(int32_t argc, char *argv[]) {
  uint32_t blockDim = 8;
  size_t inputByteSize = 8 * 2048 * sizeof(float);
  size_t outputByteSize = 8 * 2048 * sizeof(float);

#ifdef __CCE_KT_TEST__
  uint8_t *x = (uint8_t *)AscendC::GmAlloc(inputByteSize);
  uint8_t *y = (uint8_t *)AscendC::GmAlloc(inputByteSize);
  uint8_t *out = (uint8_t *)AscendC::GmAlloc(outputByteSize);
  ReadFile("./input/input_x.bin", inputByteSize, x, inputByteSize);
  ReadFile("./input/input_y.bin", inputByteSize, y, inputByteSize);

  AscendC::SetKernelMode(KernelMode::AIV_MODE);
  ICPU_RUN_KF(mse_loss, blockDim, x, y,
              out); // use this macro for cpu debug

  void *v_x = reinterpret_cast<void *>(x);
  void *v_y = reinterpret_cast<void *>(y);
  void *v_out = reinterpret_cast<void *>(out);

  float *f16_x = reinterpret_cast<float *>(v_x);
  float *f16_y = reinterpret_cast<float *>(v_y);
  float *f16_out = reinterpret_cast<float *>(v_out);

  float sum = 0.0;
  float t;

  uint32_t offset = 0;
  int sumCount = 0;

  for (int i = 0; i < USE_CORE_NUM; i++) {

    sumCount = TILE_NUM * BUFFER_NUM;

    for (int j = 0; j < sumCount; j++) {
      memcpy(&t, out + offset + j * sizeof(float), sizeof(float));
      sum += t;
    }

    offset += BLOCK_LENGTH * sizeof(float);
  }

  memcpy(out, &sum, sizeof(float));

  // WriteFile("./output/output_out.bin", out, outputByteSize);
  WriteFile("./output/output_out.bin", out, 4);

  AscendC::GmFree((void *)x);
  AscendC::GmFree((void *)y);
  AscendC::GmFree((void *)out);
#else
  CHECK_ACL(aclInit("./scripts/acl.json"));
  aclrtContext context;
  int32_t deviceId = 0;
  CHECK_ACL(aclrtSetDevice(deviceId));
  CHECK_ACL(aclrtCreateContext(&context, deviceId));
  aclrtStream stream = nullptr;
  CHECK_ACL(aclrtCreateStream(&stream));

  uint8_t *xHost, *yHost, *outHost;
  uint8_t *xDevice, *yDevice, *outDevice;

  CHECK_ACL(aclrtMallocHost((void **)(&xHost), inputByteSize));
  CHECK_ACL(aclrtMallocHost((void **)(&yHost), inputByteSize));
  CHECK_ACL(aclrtMallocHost((void **)(&outHost), outputByteSize));
  CHECK_ACL(
      aclrtMalloc((void **)&xDevice, inputByteSize, ACL_MEM_MALLOC_HUGE_FIRST));
  CHECK_ACL(
      aclrtMalloc((void **)&yDevice, inputByteSize, ACL_MEM_MALLOC_HUGE_FIRST));

  CHECK_ACL(aclrtMalloc((void **)&outDevice, outputByteSize,
                        ACL_MEM_MALLOC_HUGE_FIRST));

  ReadFile("./input/input_x.bin", inputByteSize, xHost, inputByteSize);
  ReadFile("./input/input_y.bin", inputByteSize, yHost, inputByteSize);

  CHECK_ACL(aclrtMemcpy(xDevice, inputByteSize, xHost, inputByteSize,
                        ACL_MEMCPY_HOST_TO_DEVICE));
  CHECK_ACL(aclrtMemcpy(yDevice, inputByteSize, yHost, inputByteSize,
                        ACL_MEMCPY_HOST_TO_DEVICE));

  mse_loss_do(blockDim, nullptr, stream, xDevice, yDevice, outDevice);
  CHECK_ACL(aclrtSynchronizeStream(stream));

  CHECK_ACL(aclrtMemcpy(outHost, outputByteSize, outDevice, outputByteSize,
                        ACL_MEMCPY_DEVICE_TO_HOST));
  WriteFile("./output/output_out.bin", outHost, outputByteSize);

  CHECK_ACL(aclrtFree(xDevice));
  CHECK_ACL(aclrtFree(yDevice));
  CHECK_ACL(aclrtFree(outDevice));
  CHECK_ACL(aclrtFreeHost(xHost));
  CHECK_ACL(aclrtFreeHost(yHost));
  CHECK_ACL(aclrtFreeHost(outHost));

  CHECK_ACL(aclrtDestroyStream(stream));
  CHECK_ACL(aclrtDestroyContext(context));
  CHECK_ACL(aclrtResetDevice(deviceId));
  CHECK_ACL(aclFinalize());
#endif
  return 0;
}
