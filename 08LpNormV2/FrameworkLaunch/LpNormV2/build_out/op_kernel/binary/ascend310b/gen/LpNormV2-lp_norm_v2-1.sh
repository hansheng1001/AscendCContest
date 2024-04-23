#!/bin/bash
echo "[Ascend310B1] Generating LpNormV2_fdba2d5a48f6c44eb7221a3a4826372e ..."
opc $1 --main_func=lp_norm_v2 --input_param=/home/wanghs/example/samples/operator/LpNormV2/FrameworkLaunch/LpNormV2/build_out/op_kernel/binary/ascend310b/gen/LpNormV2_fdba2d5a48f6c44eb7221a3a4826372e_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LpNormV2_fdba2d5a48f6c44eb7221a3a4826372e.json ; then
  echo "$2/LpNormV2_fdba2d5a48f6c44eb7221a3a4826372e.json not generated!"
  exit 1
fi

if ! test -f $2/LpNormV2_fdba2d5a48f6c44eb7221a3a4826372e.o ; then
  echo "$2/LpNormV2_fdba2d5a48f6c44eb7221a3a4826372e.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LpNormV2_fdba2d5a48f6c44eb7221a3a4826372e Done"
