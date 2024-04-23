#!/bin/bash
echo "[Ascend310B1] Generating Addcmul_7157af633bbaad2480411be98374ca25 ..."
opc $1 --main_func=addcmul --input_param=/home/wanghs/example/samples/operator/Addcmul/FrameworkLaunch/Addcmul/build_out/op_kernel/binary/ascend310b/gen/Addcmul_7157af633bbaad2480411be98374ca25_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/Addcmul_7157af633bbaad2480411be98374ca25.json ; then
  echo "$2/Addcmul_7157af633bbaad2480411be98374ca25.json not generated!"
  exit 1
fi

if ! test -f $2/Addcmul_7157af633bbaad2480411be98374ca25.o ; then
  echo "$2/Addcmul_7157af633bbaad2480411be98374ca25.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating Addcmul_7157af633bbaad2480411be98374ca25 Done"
