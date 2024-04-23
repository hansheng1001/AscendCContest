#!/bin/bash
echo "[Ascend310B1] Generating Spence_1b553df3d3306b59b4cdf031bb857cf8 ..."
opc $1 --main_func=spence --input_param=/home/wanghs/example/samples/operator/Spence/FrameworkLaunch/Spence/build_out/op_kernel/binary/ascend310b/gen/Spence_1b553df3d3306b59b4cdf031bb857cf8_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/Spence_1b553df3d3306b59b4cdf031bb857cf8.json ; then
  echo "$2/Spence_1b553df3d3306b59b4cdf031bb857cf8.json not generated!"
  exit 1
fi

if ! test -f $2/Spence_1b553df3d3306b59b4cdf031bb857cf8.o ; then
  echo "$2/Spence_1b553df3d3306b59b4cdf031bb857cf8.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating Spence_1b553df3d3306b59b4cdf031bb857cf8 Done"
