#!/usr/bin/python3
# -*- coding:utf-8 -*-
# Copyright 2022-2023 Huawei Technologies Co., Ltd
import numpy as np
import os
import tensorflow as tf

def gen_golden_data_simple():
    input_x = np.random.uniform(1, 10, [1024]).astype(np.float16)
    
    golden = tf.math.special.spence(input_x.astype(np.float32)).numpy().astype(np.float16)

    os.system("mkdir -p input")
    os.system("mkdir -p output")
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
