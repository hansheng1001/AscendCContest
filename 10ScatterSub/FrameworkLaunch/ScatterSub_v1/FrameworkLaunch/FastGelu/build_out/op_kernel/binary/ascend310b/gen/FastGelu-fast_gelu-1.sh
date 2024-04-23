#!/bin/bash
echo "[Ascend310B1] Generating FastGelu_4e343616fbcf642771189f043cfbd000 ..."
opc $1 --main_func=fast_gelu --input_param=/home/wanghs/example/samples/operator/FastGelu/FrameworkLaunch/FastGelu/build_out/op_kernel/binary/ascend310b/gen/FastGelu_4e343616fbcf642771189f043cfbd000_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/FastGelu_4e343616fbcf642771189f043cfbd000.json ; then
  echo "$2/FastGelu_4e343616fbcf642771189f043cfbd000.json not generated!"
  exit 1
fi

if ! test -f $2/FastGelu_4e343616fbcf642771189f043cfbd000.o ; then
  echo "$2/FastGelu_4e343616fbcf642771189f043cfbd000.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating FastGelu_4e343616fbcf642771189f043cfbd000 Done"
