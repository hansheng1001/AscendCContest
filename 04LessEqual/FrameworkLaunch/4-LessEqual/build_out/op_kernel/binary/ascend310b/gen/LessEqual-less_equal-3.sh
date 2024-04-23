#!/bin/bash
echo "[Ascend310B1] Generating LessEqual_d41dcd5cb223120be228900a62c71e30 ..."
opc $1 --main_func=less_equal --input_param=/root/samples/operator/LessEqual/FrameworkLaunch/chenhLessEqual/build_out/op_kernel/binary/ascend310b/gen/LessEqual_d41dcd5cb223120be228900a62c71e30_param.json --soc_version=Ascend310B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/LessEqual_d41dcd5cb223120be228900a62c71e30.json ; then
  echo "$2/LessEqual_d41dcd5cb223120be228900a62c71e30.json not generated!"
  exit 1
fi

if ! test -f $2/LessEqual_d41dcd5cb223120be228900a62c71e30.o ; then
  echo "$2/LessEqual_d41dcd5cb223120be228900a62c71e30.o not generated!"
  exit 1
fi
echo "[Ascend310B1] Generating LessEqual_d41dcd5cb223120be228900a62c71e30 Done"
