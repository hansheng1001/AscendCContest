#!/bin/bash
echo "[Ascend310B1] Generating LessEqual_92b9c095f157421a5cb0380b027e984e ..."
opc $1 --main_func=less_equal --input_param=/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/gen/LessEqual_92b9c095f157421a5cb0380b027e984e_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LessEqual_92b9c095f157421a5cb0380b027e984e.json ; then
  echo "$2/LessEqual_92b9c095f157421a5cb0380b027e984e.json not generated!"
  exit 1
fi

if ! test -f $2/LessEqual_92b9c095f157421a5cb0380b027e984e.o ; then
  echo "$2/LessEqual_92b9c095f157421a5cb0380b027e984e.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LessEqual_92b9c095f157421a5cb0380b027e984e Done"
