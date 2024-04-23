import os
import sys
import numpy as np


def verify_result(real_result, golden):
    real_result = np.fromfile(real_result, dtype=np.bool_) # 从bin文件读取实际运算结果
    golden = np.fromfile(golden, dtype=np.bool_) # 从bin文件读取预期运算结果
    for i in range(len(real_result)):
        if real_result[i] != golden[i]:
            print("[ERROR] result error out {} expect {} but {}".format(i,golden[i],real_result[i]))
            return False
    print("test pass")
    return True

if __name__ == '__main__':
    verify_result(sys.argv[1],sys.argv[2])
