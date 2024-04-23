#!/bin/bash
echo "[Ascend310B1] Generating ScatterSub_7564ceb25b09520d7248a455c437355f ..."
opc $1 --main_func=scatter_sub --input_param=/home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b/gen/ScatterSub_7564ceb25b09520d7248a455c437355f_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ScatterSub_7564ceb25b09520d7248a455c437355f.json ; then
  echo "$2/ScatterSub_7564ceb25b09520d7248a455c437355f.json not generated!"
  exit 1
fi

if ! test -f $2/ScatterSub_7564ceb25b09520d7248a455c437355f.o ; then
  echo "$2/ScatterSub_7564ceb25b09520d7248a455c437355f.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ScatterSub_7564ceb25b09520d7248a455c437355f Done"
