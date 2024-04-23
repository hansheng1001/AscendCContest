#!/bin/bash
echo "[Ascend310B1] Generating LessEqual_618d866b56d6a7b84ac354a8bd4c80e9 ..."
opc $1 --main_func=less_equal --input_param=/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/gen/LessEqual_618d866b56d6a7b84ac354a8bd4c80e9_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LessEqual_618d866b56d6a7b84ac354a8bd4c80e9.json ; then
  echo "$2/LessEqual_618d866b56d6a7b84ac354a8bd4c80e9.json not generated!"
  exit 1
fi

if ! test -f $2/LessEqual_618d866b56d6a7b84ac354a8bd4c80e9.o ; then
  echo "$2/LessEqual_618d866b56d6a7b84ac354a8bd4c80e9.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LessEqual_618d866b56d6a7b84ac354a8bd4c80e9 Done"
