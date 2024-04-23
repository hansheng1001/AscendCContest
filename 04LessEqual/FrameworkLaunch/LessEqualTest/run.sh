#!/bin/bash
export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

CURRENT_DIR=$(
    cd $(dirname ${BASH_SOURCE:-$0})
    pwd
)
cd $CURRENT_DIR

# 导出环境变量
SHORT=v:,
LONG=dtype:,
OPTS=$(getopt -a --options $SHORT --longoptions $LONG -- "$@")
eval set -- "$OPTS"
while :
do
    case "$1" in
        # float16, float, int32
        (-v | --dtype)
            DTYPE="$2"
            shift 2;;
        (--)
            shift;
            break;;
        (*)
            echo "[ERROR] Unexpected option: $1";
            break;;
    esac
done

if [ ! $ASCEND_HOME_DIR ]; then
    if [ -d "$HOME/Ascend/ascend-toolkit/latest" ]; then
        export ASCEND_HOME_DIR=$HOME/Ascend/ascend-toolkit/latest
    else
        export ASCEND_HOME_DIR=/usr/local/Ascend/ascend-toolkit/latest
    fi
fi
source $ASCEND_HOME_DIR/bin/setenv.bash

export DDK_PATH=$ASCEND_HOME_DIR
arch=$(uname -m)
export NPU_HOST_LIB=$ASCEND_HOME_DIR/${arch}-linux/lib64

function main {
    # 1. 清除遗留生成文件和日志文件
    rm -rf $HOME/ascend/log/*
    rm ./input/*.bin
    rm ./output/*.bin

    # 2. 生成输入数据和真值数据
    cd $CURRENT_DIR
    python3 scripts/gen_data.py
    if [ $? -ne 0 ]; then
        echo "ERROR: generate input data failed!"
        return 1
    fi
    echo "INFO: generate input data success!"

    # 3. 编译acl可执行文件
    cd $CURRENT_DIR; rm -rf build; mkdir -p build; cd build
    cmake ../src
    if [ $? -ne 0 ]; then
        echo "ERROR: cmake failed!"
        return 1
    fi
    echo "INFO: cmake success!"
    make
    if [ $? -ne 0 ]; then
        echo "ERROR: make failed!"
        return 1
    fi
    echo "INFO: make success!"

    # 4. 运行可执行文件
    cd $CURRENT_DIR/output
    echo "INFO: execute op!"
    ./execute_op

    if [ $? -ne 0 ]; then
        echo "ERROR: acl executable run failed! please check your project!"
        return 1
    fi
    echo "INFO: acl executable run success!"

    # 5. 比较真值文件
    cd $CURRENT_DIR
    ret=`python3 scripts/verify_result.py output/output_z.bin output/golden.bin`
    echo $ret
    if [ "x$ret" == "xtest pass" ]; then
        echo ""
        echo "#####################################"
        echo "INFO: you have passed the Precision!"
        echo "#####################################"
        echo ""
    fi
}

main
