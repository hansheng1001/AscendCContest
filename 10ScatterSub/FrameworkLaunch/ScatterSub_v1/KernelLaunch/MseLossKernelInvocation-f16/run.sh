#!/bin/bash
export PRINT_TIK_MEM_ACCESS=FALSE

CURRENT_DIR=$(
    cd $(dirname ${BASH_SOURCE:-$0})
    pwd
); cd $CURRENT_DIR

declare -A VersionMap
VersionMap["Ascend910A"]="Ascend910A"
VersionMap["Ascend910B"]="Ascend910A"
VersionMap["Ascend910ProA"]="Ascend910A"
VersionMap["Ascend910ProB"]="Ascend910A"
VersionMap["Ascend910PremiumA"]="Ascend910A"
VersionMap["Ascend310B1"]="Ascend310B1"
VersionMap["Ascend310B2"]="Ascend310B1"
VersionMap["Ascend310B3"]="Ascend310B1"
VersionMap["Ascend310B4"]="Ascend310B1"
VersionMap["Ascend310P1"]="Ascend310P1"
VersionMap["Ascend310P3"]="Ascend310P1"
VersionMap["Ascend910B1"]="Ascend910B1"
VersionMap["Ascend910B2"]="Ascend910B1"
VersionMap["Ascend910B3"]="Ascend910B1"
VersionMap["Ascend910B4"]="Ascend910B1"
# legacy
VersionMap["ascend910"]="Ascend910A"
VersionMap["ascend310p"]="Ascend310P1"
VersionMap["ascend310B1"]="Ascend310B1"
VersionMap["ascend910B1"]="Ascend910B1"

FILE_NAME="addcdiv_custom"

SHORT=r:,v:,i:,
LONG=run-mode:,soc-version:,install-path:,
OPTS=$(getopt -a --options $SHORT --longoptions $LONG -- "$@")
eval set -- "$OPTS"

while :
do
    case "$1" in
        (-r | --run-mode )
            RUN_MODE="$2"
            shift 2;;
        (-v | --soc-version )
            SOC_VERSION="$2"
            shift 2;;
        (-i | --install-path )
            ASCEND_INSTALL_PATH="$2"
            shift 2;;
        (--)
            shift;
            break;;
        (*)
            echo "[ERROR] Unexpected option: $1";
            break;;
    esac
done


if [ -n "$ASCEND_INSTALL_PATH" ]; then
    _ASCEND_INSTALL_PATH=$ASCEND_INSTALL_PATH
elif [ -n "$ASCEND_HOME_PATH" ]; then
    _ASCEND_INSTALL_PATH=$ASCEND_HOME_PATH
else
    if [ -d "$HOME/Ascend/ascend-toolkit/latest" ]; then
        _ASCEND_INSTALL_PATH=$HOME/Ascend/ascend-toolkit/latest
    else
        _ASCEND_INSTALL_PATH=/usr/local/Ascend/ascend-toolkit/latest
    fi
fi
# in case of running op in simulator, use stub so instead
if [ "${RUN_MODE}" = "sim" ]; then
    export LD_LIBRARY_PATH=$_ASCEND_INSTALL_PATH/runtime/lib64/stub:$LD_LIBRARY_PATH
    if [ ! $CAMODEL_LOG_PATH ]; then
        export CAMODEL_LOG_PATH=./sim_log
    fi
    rm -rf $CAMODEL_LOG_PATH
    mkdir -p $CAMODEL_LOG_PATH
fi
source $_ASCEND_INSTALL_PATH/bin/setenv.bash

if [[ " ${!VersionMap[*]} " != *" $SOC_VERSION "* ]]; then
    echo "ERROR: SOC_VERSION should be in [${!VersionMap[*]}]"
    exit -1
fi
_SOC_VERSION=${VersionMap[$SOC_VERSION]}

if [ $_SOC_VERSION"x" = "Ascend910Ax" ] || [ $_SOC_VERSION"x" = "Ascend310P1x" ] || [ $_SOC_VERSION"x" = "Ascend310B1x" ]; then
    CORE_TYPE="AiCore"
elif [ $_SOC_VERSION"x" = "Ascend910B1x" ]; then
    CORE_TYPE="VectorCore"
fi

RUN_MODE_LIST="cpu sim npu"
if [[ " $RUN_MODE_LIST " != *" $RUN_MODE "* ]]; then
    echo "ERROR: RUN_MODE error, This sample only support specify cpu, sim or npu!"
    exit -1
fi

set -e
rm -rf build *_cpu *_sim *_npu cceprint npuchk *log *.vcd

mkdir build
cmake -B build                            \
    -Dsmoke_testcase=${FILE_NAME}         \
    -DASCEND_PRODUCT_TYPE=${_SOC_VERSION} \
    -DASCEND_CORE_TYPE=${CORE_TYPE}       \
    -DASCEND_RUN_MODE=${RUN_MODE}         \
    -DASCEND_INSTALL_PATH=${_ASCEND_INSTALL_PATH}
cmake --build build --target ${FILE_NAME}_${RUN_MODE}
if [ $? -ne 0 ]; then
    echo "ERROR: compile op on failed!"
    exit -1
fi
echo "INFO: compile op on ${RUN_MODE} succeed!"

rm -rf input/*.bin output/*.bin
python3 scripts/gen_data.py
(export LD_LIBRARY_PATH=${_ASCEND_INSTALL_PATH}/tools/simulator/${_SOC_VERSION}/lib:$LD_LIBRARY_PATH && ./${FILE_NAME}_${RUN_MODE})
if [ $? -ne 0 ]; then
    echo "ERROR: execute op on ${RUN_MODE} failed!"
    exit -1
fi
echo "INFO: execute op on ${RUN_MODE} succeed!"
python3 scripts/verify_result.py output/output_out.bin output/golden.bin

rm -rf *log *.vcd
