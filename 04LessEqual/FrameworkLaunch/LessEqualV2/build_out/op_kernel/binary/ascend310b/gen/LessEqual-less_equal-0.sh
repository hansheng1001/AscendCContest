#!/bin/bash
echo "[Ascend310B1] Generating LessEqual_cbe28bab2130dcd0f50c746873f5cfdf ..."
opc $1 --main_func=less_equal --input_param=/home/wanghs/example/samples/operator/LessEqual/FrameworkLaunch/LessEqualV2/build_out/op_kernel/binary/ascend310b/gen/LessEqual_cbe28bab2130dcd0f50c746873f5cfdf_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LessEqual_cbe28bab2130dcd0f50c746873f5cfdf.json ; then
  echo "$2/LessEqual_cbe28bab2130dcd0f50c746873f5cfdf.json not generated!"
  exit 1
fi

if ! test -f $2/LessEqual_cbe28bab2130dcd0f50c746873f5cfdf.o ; then
  echo "$2/LessEqual_cbe28bab2130dcd0f50c746873f5cfdf.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LessEqual_cbe28bab2130dcd0f50c746873f5cfdf Done"
