#!/bin/bash
echo "[Ascend310B1] Generating LpNormV2_21726fe1ea06b195906a669d751e728c ..."
opc $1 --main_func=lp_norm_v2 --input_param=/home/wanghs/example/samples/operator/LpNormV2/FrameworkLaunch/LpNormV2/build_out/op_kernel/binary/ascend310b/gen/LpNormV2_21726fe1ea06b195906a669d751e728c_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LpNormV2_21726fe1ea06b195906a669d751e728c.json ; then
  echo "$2/LpNormV2_21726fe1ea06b195906a669d751e728c.json not generated!"
  exit 1
fi

if ! test -f $2/LpNormV2_21726fe1ea06b195906a669d751e728c.o ; then
  echo "$2/LpNormV2_21726fe1ea06b195906a669d751e728c.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LpNormV2_21726fe1ea06b195906a669d751e728c Done"
