#!/bin/bash
echo "[Ascend310B1] Generating MseLossGrad_d39922bbcdd33a94de9015ce72213c34 ..."
opc $1 --main_func=mse_loss_grad --input_param=/home/wanghs/example/samples/operator/MseLossGrad/FrameworkLaunch/MseLossGrad/build_out/op_kernel/binary/ascend310b/gen/MseLossGrad_d39922bbcdd33a94de9015ce72213c34_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/MseLossGrad_d39922bbcdd33a94de9015ce72213c34.json ; then
  echo "$2/MseLossGrad_d39922bbcdd33a94de9015ce72213c34.json not generated!"
  exit 1
fi

if ! test -f $2/MseLossGrad_d39922bbcdd33a94de9015ce72213c34.o ; then
  echo "$2/MseLossGrad_d39922bbcdd33a94de9015ce72213c34.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating MseLossGrad_d39922bbcdd33a94de9015ce72213c34 Done"
