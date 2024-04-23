#!/bin/bash
echo "[Ascend310B1] Generating ScatterSub_c9d1fd18100cf2381af3ee63bb2e777a ..."
opc $1 --main_func=scatter_sub --input_param=/home/wanghs/example/samples/operator/ScatterSub/FrameworkLaunch/ScatterSub/build_out/op_kernel/binary/ascend310b/gen/ScatterSub_c9d1fd18100cf2381af3ee63bb2e777a_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/ScatterSub_c9d1fd18100cf2381af3ee63bb2e777a.json ; then
  echo "$2/ScatterSub_c9d1fd18100cf2381af3ee63bb2e777a.json not generated!"
  exit 1
fi

if ! test -f $2/ScatterSub_c9d1fd18100cf2381af3ee63bb2e777a.o ; then
  echo "$2/ScatterSub_c9d1fd18100cf2381af3ee63bb2e777a.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating ScatterSub_c9d1fd18100cf2381af3ee63bb2e777a Done"
