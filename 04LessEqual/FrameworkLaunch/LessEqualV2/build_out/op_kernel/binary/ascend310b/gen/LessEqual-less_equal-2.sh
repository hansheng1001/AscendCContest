#!/bin/bash
echo "[Ascend310B1] Generating LessEqual_bedb359b1d7590ad484035bea230ab78 ..."
opc $1 --main_func=less_equal --input_param=/home/wanghs/example/samples/operator/LessEqual/FrameworkLaunch/LessEqualV2/build_out/op_kernel/binary/ascend310b/gen/LessEqual_bedb359b1d7590ad484035bea230ab78_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LessEqual_bedb359b1d7590ad484035bea230ab78.json ; then
  echo "$2/LessEqual_bedb359b1d7590ad484035bea230ab78.json not generated!"
  exit 1
fi

if ! test -f $2/LessEqual_bedb359b1d7590ad484035bea230ab78.o ; then
  echo "$2/LessEqual_bedb359b1d7590ad484035bea230ab78.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LessEqual_bedb359b1d7590ad484035bea230ab78 Done"
