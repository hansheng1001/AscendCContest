#!/bin/bash
echo "[Ascend310B1] Generating MseLossGrad_78c72dcc185ea1bc91b2ae1453325324 ..."
opc $1 --main_func=mse_loss_grad --input_param=/home/wanghs/example/samples/operator/MseLossGrad/FrameworkLaunch/MseLossGrad/build_out/op_kernel/binary/ascend310b/gen/MseLossGrad_78c72dcc185ea1bc91b2ae1453325324_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/MseLossGrad_78c72dcc185ea1bc91b2ae1453325324.json ; then
  echo "$2/MseLossGrad_78c72dcc185ea1bc91b2ae1453325324.json not generated!"
  exit 1
fi

if ! test -f $2/MseLossGrad_78c72dcc185ea1bc91b2ae1453325324.o ; then
  echo "$2/MseLossGrad_78c72dcc185ea1bc91b2ae1453325324.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating MseLossGrad_78c72dcc185ea1bc91b2ae1453325324 Done"
