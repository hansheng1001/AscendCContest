#!/bin/bash
echo "[Ascend310B1] Generating LessEqual_b616c20e75477e47d2645f96eb8a14cb ..."
opc $1 --main_func=less_equal --input_param=/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/gen/LessEqual_b616c20e75477e47d2645f96eb8a14cb_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LessEqual_b616c20e75477e47d2645f96eb8a14cb.json ; then
  echo "$2/LessEqual_b616c20e75477e47d2645f96eb8a14cb.json not generated!"
  exit 1
fi

if ! test -f $2/LessEqual_b616c20e75477e47d2645f96eb8a14cb.o ; then
  echo "$2/LessEqual_b616c20e75477e47d2645f96eb8a14cb.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LessEqual_b616c20e75477e47d2645f96eb8a14cb Done"
