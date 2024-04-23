#!/bin/bash
echo "[Ascend310B1] Generating LessEqual_b2c27848ebd50085ba542bf08cb4829b ..."
opc $1 --main_func=less_equal --input_param=/home/wanghs/example/samples/operator/LessEqual/FrameworkLaunch/LessEqualV2/build_out/op_kernel/binary/ascend310b/gen/LessEqual_b2c27848ebd50085ba542bf08cb4829b_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LessEqual_b2c27848ebd50085ba542bf08cb4829b.json ; then
  echo "$2/LessEqual_b2c27848ebd50085ba542bf08cb4829b.json not generated!"
  exit 1
fi

if ! test -f $2/LessEqual_b2c27848ebd50085ba542bf08cb4829b.o ; then
  echo "$2/LessEqual_b2c27848ebd50085ba542bf08cb4829b.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LessEqual_b2c27848ebd50085ba542bf08cb4829b Done"
