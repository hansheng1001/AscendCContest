#!/bin/bash
echo "[Ascend310B1] Generating ClipByValue_729872a2a3cbd9f8202d2e9568550287 ..."
opc $1 --main_func=clip_by_value --input_param=/home/wanghs/example/samples/operator/ClipByValue/FrameworkLaunch/ClipByValue/build_out/op_kernel/binary/ascend310b/gen/ClipByValue_729872a2a3cbd9f8202d2e9568550287_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ClipByValue_729872a2a3cbd9f8202d2e9568550287.json ; then
  echo "$2/ClipByValue_729872a2a3cbd9f8202d2e9568550287.json not generated!"
  exit 1
fi

if ! test -f $2/ClipByValue_729872a2a3cbd9f8202d2e9568550287.o ; then
  echo "$2/ClipByValue_729872a2a3cbd9f8202d2e9568550287.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ClipByValue_729872a2a3cbd9f8202d2e9568550287 Done"
