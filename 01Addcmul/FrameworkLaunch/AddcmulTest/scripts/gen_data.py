#!/usr/bin/python3
# -*- coding:utf-8 -*-
# Copyright 2022-2023 Huawei Technologies Co., Ltd
import numpy as np
import os
def gen_golden_data_simple():
    input_x = np.random.uniform(1, 10, [8, 2048]).astype(np.float16)
    input_y = np.random.uniform(1, 10, [8, 2048]).astype(np.float16)
    input_z = np.random.uniform(1, 10, [8, 2048]).astype(np.float16)
    input_value = np.random.uniform(1, 10, [1]).astype(np.float16)

    golden = (input_x + input_y*input_z*input_value).astype(np.float16)
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    input_z.tofile("./input/input_z.bin")
    input_value.tofile("./input/input_value.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
