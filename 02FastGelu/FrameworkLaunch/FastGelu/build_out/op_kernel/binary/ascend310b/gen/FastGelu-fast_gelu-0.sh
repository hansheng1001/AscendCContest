#!/bin/bash
echo "[Ascend310B1] Generating FastGelu_5df0e856755d845700f1868cb808b5ba ..."
opc $1 --main_func=fast_gelu --input_param=/home/wanghs/example/samples/operator/FastGelu/FrameworkLaunch/FastGelu/build_out/op_kernel/binary/ascend310b/gen/FastGelu_5df0e856755d845700f1868cb808b5ba_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/FastGelu_5df0e856755d845700f1868cb808b5ba.json ; then
  echo "$2/FastGelu_5df0e856755d845700f1868cb808b5ba.json not generated!"
  exit 1
fi

if ! test -f $2/FastGelu_5df0e856755d845700f1868cb808b5ba.o ; then
  echo "$2/FastGelu_5df0e856755d845700f1868cb808b5ba.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating FastGelu_5df0e856755d845700f1868cb808b5ba Done"
