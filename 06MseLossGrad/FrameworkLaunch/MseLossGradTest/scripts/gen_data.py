#!/usr/bin/python3
# -*- coding:utf-8 -*-
# Copyright 2022-2023 Huawei Technologies Co., Ltd
import numpy as np
import os
import torch
import torch.nn as nn

def gen_golden_data_simple():
    input_predict = np.random.uniform(1, 10, [1024]).astype(np.float16)
    input_label  = np.random.uniform(1, 10, [1024]).astype(np.float16)
    input_dout = np.random.uniform(1, 10, [1024]).astype(np.float16)
    reduction = "mean"
    if 'mean' == reduction:
        reduce_elts = 1.0
        for i in input_predict.shape:
            reduce_elts *= i
        cof = (reduce_elts**(-1)) * 2.0
    else:
        cof = 2.0

    sub_res = input_predict - input_label
    norm_grad = sub_res * cof
    golden = norm_grad * input_dout
    
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    input_predict.tofile("./input/input_predict.bin")
    input_label.tofile("./input/input_label.bin")
    input_dout.tofile("./input/input_dout.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
