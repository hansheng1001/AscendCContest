#!/bin/bash
echo "[Ascend310B1] Generating ScatterSub_136ebf418250c2a7f523fa2112b3ba43 ..."
opc $1 --main_func=scatter_sub --input_param=/home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b/gen/ScatterSub_136ebf418250c2a7f523fa2112b3ba43_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ScatterSub_136ebf418250c2a7f523fa2112b3ba43.json ; then
  echo "$2/ScatterSub_136ebf418250c2a7f523fa2112b3ba43.json not generated!"
  exit 1
fi

if ! test -f $2/ScatterSub_136ebf418250c2a7f523fa2112b3ba43.o ; then
  echo "$2/ScatterSub_136ebf418250c2a7f523fa2112b3ba43.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ScatterSub_136ebf418250c2a7f523fa2112b3ba43 Done"
