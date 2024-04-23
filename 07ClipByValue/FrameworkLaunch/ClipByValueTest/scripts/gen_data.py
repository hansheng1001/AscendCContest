#!/usr/bin/python3
# -*- coding:utf-8 -*-
# Copyright 2022-2023 Huawei Technologies Co., Ltd
import numpy as np
import os
import tensorflow as tf
def gen_golden_data_simple():
    input_x = np.random.uniform(1, 10, [8, 2048]).astype(np.float16)
    clip_value_min = np.random.uniform(1, 3, [1]).astype(np.float16)
    clip_value_max = np.random.uniform(4, 10, [1]).astype(np.float16)
    y = tf.clip_by_value(input_x, clip_value_min, clip_value_max)

    golden =y.numpy()
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    input_x.tofile("./input/input_x.bin")
    clip_value_min.tofile("./input/input_clip_value_min.bin")
    clip_value_max.tofile("./input/input_clip_value_max.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
