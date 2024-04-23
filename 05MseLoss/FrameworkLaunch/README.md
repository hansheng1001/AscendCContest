## 目录结构
| 目录                  | 描述                   |
|---------------------|----------------------|
| AddcdivCustom.json       | AddcdivCustom算子的原型定义json文件 |
| [AddcdivCustom](./AddcdivCustom)       | AddcdivCustom算子工程 |
| [AclNNInvocation](./AclNNInvocation) | 通过aclnn调用的方式调用AddcdivCustom算子工程。 |

## 样例支持的产品型号为：
- Atlas 训练系列产品
- Atlas 推理系列产品（Ascend 310P处理器）
- Atlas A2训练系列产品

## 编译算子工程部署算子包

### 1.获取源码包
    
 可以使用以下两种方式下载，请选择其中一种进行源码准备。

 - 命令行方式下载（下载时间较长，但步骤简单）。

   ```    
   # 开发环境，非root用户命令行中执行以下命令下载源码仓。    
   cd ${HOME}     
   git clone https://gitee.com/ascend/samples.git
   ```
   **注：如果需要切换到其它tag版本，以v0.5.0为例，可执行以下命令。**
   ```
   git checkout v0.5.0
   ```   
 - 压缩包方式下载（下载时间较短，但步骤稍微复杂）。   
   **注：如果需要下载其它版本代码，请先请根据前置条件说明进行samples仓分支切换。**   
   ``` 
   # 1. samples仓右上角选择 【克隆/下载】 下拉框并选择 【下载ZIP】。    
   # 2. 将ZIP包上传到开发环境中的普通用户家目录中，【例如：${HOME}/ascend-samples-master.zip】。     
   # 3. 开发环境中，执行以下命令，解压zip包。     
   cd ${HOME}    
   unzip ascend-samples-master.zip
   ```

### 2.编译算子工程

  编译自定义算子工程，构建生成自定义算子包

  - 执行如下命令，切换到算子工程AddcdivCustom目录

    ```
    cd $HOME/samples/operator/AddcdivCustomSample/FrameworkLaunch/AddcdivCustom
    ```

  - 修改CMakePresets.json中ASCEND_CANN_PACKAGE_PATH为CANN软件包安装后的实际路径。

        
    ```    
    {
        ……
        "configurePresets": [
            {
                    ……
                    "ASCEND_CANN_PACKAGE_PATH": {
                        "type": "PATH",
                        "value": "~/Ascend/ascend-toolkit/latest"        //请替换为CANN软件包安装后的实际路径。eg:/home/HwHiAiUser/Ascend/ascend-toolkit/latest
                    },
                    ……
            }
        ]
    }
    ```
  - 在算子工程AddcdivCustom目录下执行如下命令，进行算子工程编译。

    ```
    ./build.sh
    ```
    编译成功后，会在当前目录下创建build_out目录，并在build_out目录下生成自定义算子安装包custom_opp_<target os>_<target architecture>.run，例如“custom_opp_ubuntu_x86_64.run”。
  - 备注：如果要使用dump调试功能，需要移除op_host内和CMakeLists.txt内的Atlas 训练系列产品的配置

### 3.部署算子包

  - 执行如下命令，在自定义算子安装包所在路径下，安装自定义算子包。

    ```
    cd build_out
    ./custom_opp_<target os>_<target architecture>.run
    ```
    
    命令执行成功后，自定义算子包中的相关文件将部署至当前环境的OPP算子库的vendors/customize目录中。

## 配置环境变量

  这里的\$HOME需要替换为CANN包的安装路径。
  ```
  export ASCEND_HOME_DIR=$HOME/Ascend/ascend-toolkit/latest
  ```

## 通过aclnn调用的方式调用AddcdivCustom算子工程

### 样例运行

  - 进入到样例目录
        
    ```    
    cd $HOME/samples/operator/AddcdivCustomSample/FrameworkLaunch/AclNNInvocation
    ```

  - 样例执行          

    样例执行过程中会自动生成测试数据，然后编译与运行aclnn样例，最后检验运行结果。具体过程可参见run.sh脚本。
    ```
    bash run.sh
    ```

## 更新说明
  | 时间 | 更新事项 |
|----|------|
| 2024/02/22 | 新增AclNNInvocation样例|
| 2024/02/22 | 新增AddcdivCustom算子工程|