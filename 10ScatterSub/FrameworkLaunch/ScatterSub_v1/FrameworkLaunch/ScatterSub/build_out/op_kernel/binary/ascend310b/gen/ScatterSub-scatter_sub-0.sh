#!/bin/bash
echo "[Ascend310B1] Generating ScatterSub_a7322a0f4454573b5c5f493ab1132b53 ..."
opc $1 --main_func=scatter_sub --input_param=/home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b/gen/ScatterSub_a7322a0f4454573b5c5f493ab1132b53_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ScatterSub_a7322a0f4454573b5c5f493ab1132b53.json ; then
  echo "$2/ScatterSub_a7322a0f4454573b5c5f493ab1132b53.json not generated!"
  exit 1
fi

if ! test -f $2/ScatterSub_a7322a0f4454573b5c5f493ab1132b53.o ; then
  echo "$2/ScatterSub_a7322a0f4454573b5c5f493ab1132b53.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ScatterSub_a7322a0f4454573b5c5f493ab1132b53 Done"
