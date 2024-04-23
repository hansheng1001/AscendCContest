#!/bin/bash
echo "[Ascend310B1] Generating ClipByValue_f8e3faca955f5d8208ed69ab17bcd785 ..."
opc $1 --main_func=clip_by_value --input_param=/home/wanghs/example/samples/operator/ClipByValue/FrameworkLaunch/ClipByValue/build_out/op_kernel/binary/ascend310b/gen/ClipByValue_f8e3faca955f5d8208ed69ab17bcd785_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ClipByValue_f8e3faca955f5d8208ed69ab17bcd785.json ; then
  echo "$2/ClipByValue_f8e3faca955f5d8208ed69ab17bcd785.json not generated!"
  exit 1
fi

if ! test -f $2/ClipByValue_f8e3faca955f5d8208ed69ab17bcd785.o ; then
  echo "$2/ClipByValue_f8e3faca955f5d8208ed69ab17bcd785.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ClipByValue_f8e3faca955f5d8208ed69ab17bcd785 Done"
