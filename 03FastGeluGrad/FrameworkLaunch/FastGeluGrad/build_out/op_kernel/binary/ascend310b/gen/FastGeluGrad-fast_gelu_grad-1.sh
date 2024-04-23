#!/bin/bash
echo "[Ascend310B1] Generating FastGeluGrad_1faee25121f0603c567cd8b30eaab4a2 ..."
opc $1 --main_func=fast_gelu_grad --input_param=/home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/op_kernel/binary/ascend310b/gen/FastGeluGrad_1faee25121f0603c567cd8b30eaab4a2_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/FastGeluGrad_1faee25121f0603c567cd8b30eaab4a2.json ; then
  echo "$2/FastGeluGrad_1faee25121f0603c567cd8b30eaab4a2.json not generated!"
  exit 1
fi

if ! test -f $2/FastGeluGrad_1faee25121f0603c567cd8b30eaab4a2.o ; then
  echo "$2/FastGeluGrad_1faee25121f0603c567cd8b30eaab4a2.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating FastGeluGrad_1faee25121f0603c567cd8b30eaab4a2 Done"
