#!/usr/bin/python3
# -*- coding:utf-8 -*-
# Copyright 2022-2023 Huawei Technologies Co., Ltd
import tensorflow._api.v2.compat.v1 as tf
import numpy as np
tf.disable_v2_behavior()
import os


def gen_golden_data_simple():

    # input_var = np.random.uniform(1, 10, [8, 1024]).astype(np.float16)
    input_var = np.random.uniform(1, 10, [1024]).astype(np.float16)
    input_indices = np.random.uniform(0, 7, [8]).astype(np.int32)

    # input_updates = np.random.uniform(-10, 10, [8, 1024]).astype(np.float16)
    input_updates = np.random.uniform(-10, 10, [8]).astype(np.float16)
    use_locking = False

    ref = tf.Variable(input_var)
    indices = tf.constant(input_indices)
    updates = tf.constant(input_updates)
    scatter_max_op = tf.scatter_max(ref=ref, indices=indices, updates=updates)
    with tf.Session() as sess:
        sess.run(tf.compat.v1.global_variables_initializer())
        golden = sess.run(scatter_max_op)


    os.system("mkdir -p input")
    os.system("mkdir -p output")
    input_var.tofile("./input/input_var.bin")
    input_indices.tofile("./input/input_indices.bin")
    input_updates.tofile("./input/input_updates.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
