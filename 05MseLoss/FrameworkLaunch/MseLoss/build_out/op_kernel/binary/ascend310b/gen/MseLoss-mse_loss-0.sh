#!/bin/bash
echo "[Ascend310B1] Generating MseLoss_e5b38527ad0231aea944dc0da4441869 ..."
opc $1 --main_func=mse_loss --input_param=/home/wanghs/example/samples/operator/MseLoss/FrameworkLaunch/MseLoss_v2/build_out/op_kernel/binary/ascend310b/gen/MseLoss_e5b38527ad0231aea944dc0da4441869_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/MseLoss_e5b38527ad0231aea944dc0da4441869.json ; then
  echo "$2/MseLoss_e5b38527ad0231aea944dc0da4441869.json not generated!"
  exit 1
fi

if ! test -f $2/MseLoss_e5b38527ad0231aea944dc0da4441869.o ; then
  echo "$2/MseLoss_e5b38527ad0231aea944dc0da4441869.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating MseLoss_e5b38527ad0231aea944dc0da4441869 Done"
