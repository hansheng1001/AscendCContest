#!/bin/bash
echo "[Ascend310B1] Generating ScatterMax_604faeb8c553d7941c7b9a2659004714 ..."
opc $1 --main_func=scatter_max --input_param=/home/wanghs/example/samples/operator/ScatterMax/FrameworkLaunch/ScatterMax/build_out/op_kernel/binary/ascend310b/gen/ScatterMax_604faeb8c553d7941c7b9a2659004714_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ScatterMax_604faeb8c553d7941c7b9a2659004714.json ; then
  echo "$2/ScatterMax_604faeb8c553d7941c7b9a2659004714.json not generated!"
  exit 1
fi

if ! test -f $2/ScatterMax_604faeb8c553d7941c7b9a2659004714.o ; then
  echo "$2/ScatterMax_604faeb8c553d7941c7b9a2659004714.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ScatterMax_604faeb8c553d7941c7b9a2659004714 Done"
