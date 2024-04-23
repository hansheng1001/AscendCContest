#!/bin/bash
echo "[Ascend310B1] Generating ScatterMax_e3bce227480555e6e9f5aebacd1234c6 ..."
opc $1 --main_func=scatter_max --input_param=/home/wanghs/example/samples/operator/ScatterMax/FrameworkLaunch/ScatterMax/build_out/op_kernel/binary/ascend310b/gen/ScatterMax_e3bce227480555e6e9f5aebacd1234c6_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ScatterMax_e3bce227480555e6e9f5aebacd1234c6.json ; then
  echo "$2/ScatterMax_e3bce227480555e6e9f5aebacd1234c6.json not generated!"
  exit 1
fi

if ! test -f $2/ScatterMax_e3bce227480555e6e9f5aebacd1234c6.o ; then
  echo "$2/ScatterMax_e3bce227480555e6e9f5aebacd1234c6.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ScatterMax_e3bce227480555e6e9f5aebacd1234c6 Done"
