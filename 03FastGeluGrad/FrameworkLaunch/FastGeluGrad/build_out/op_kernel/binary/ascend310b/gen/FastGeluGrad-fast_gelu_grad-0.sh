#!/bin/bash
echo "[Ascend310B1] Generating FastGeluGrad_1be32c52415bb5be058a7feb0505a8ed ..."
opc $1 --main_func=fast_gelu_grad --input_param=/home/wanghs/example/samples/operator/FastGeluGrad/FrameworkLaunch/FastGeluGrad/build_out/op_kernel/binary/ascend310b/gen/FastGeluGrad_1be32c52415bb5be058a7feb0505a8ed_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/FastGeluGrad_1be32c52415bb5be058a7feb0505a8ed.json ; then
  echo "$2/FastGeluGrad_1be32c52415bb5be058a7feb0505a8ed.json not generated!"
  exit 1
fi

if ! test -f $2/FastGeluGrad_1be32c52415bb5be058a7feb0505a8ed.o ; then
  echo "$2/FastGeluGrad_1be32c52415bb5be058a7feb0505a8ed.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating FastGeluGrad_1be32c52415bb5be058a7feb0505a8ed Done"
