#!/bin/bash
echo "[Ascend310B1] Generating ScatterSub_1efd72332c5ee6cbcba6fbc23a6ffe57 ..."
opc $1 --main_func=scatter_sub --input_param=/home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b/gen/ScatterSub_1efd72332c5ee6cbcba6fbc23a6ffe57_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ScatterSub_1efd72332c5ee6cbcba6fbc23a6ffe57.json ; then
  echo "$2/ScatterSub_1efd72332c5ee6cbcba6fbc23a6ffe57.json not generated!"
  exit 1
fi

if ! test -f $2/ScatterSub_1efd72332c5ee6cbcba6fbc23a6ffe57.o ; then
  echo "$2/ScatterSub_1efd72332c5ee6cbcba6fbc23a6ffe57.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ScatterSub_1efd72332c5ee6cbcba6fbc23a6ffe57 Done"
