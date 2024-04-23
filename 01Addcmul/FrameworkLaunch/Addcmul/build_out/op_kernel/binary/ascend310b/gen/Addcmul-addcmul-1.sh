#!/bin/bash
echo "[Ascend310B1] Generating Addcmul_db66f24816f2050898b51af965b29955 ..."
opc $1 --main_func=addcmul --input_param=/home/wanghs/example/samples/operator/Addcmul/FrameworkLaunch/Addcmul/build_out/op_kernel/binary/ascend310b/gen/Addcmul_db66f24816f2050898b51af965b29955_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/Addcmul_db66f24816f2050898b51af965b29955.json ; then
  echo "$2/Addcmul_db66f24816f2050898b51af965b29955.json not generated!"
  exit 1
fi

if ! test -f $2/Addcmul_db66f24816f2050898b51af965b29955.o ; then
  echo "$2/Addcmul_db66f24816f2050898b51af965b29955.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating Addcmul_db66f24816f2050898b51af965b29955 Done"
