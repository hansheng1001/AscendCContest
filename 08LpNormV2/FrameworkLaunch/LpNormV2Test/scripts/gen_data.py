#!/usr/bin/python3
# -*- coding:utf-8 -*-
# Copyright 2022-2023 Huawei Technologies Co., Ltd
import numpy as np
import torch
import os


def gen_golden_data_simple():
    input_x = np.random.uniform(1, 2, [2, 1, 1024]).astype(np.float16)
    # p = float('inf')
    p = 3.0
    axes = []
    keepdim = False
    epsilon = 1e-12
    input_x = input_x.astype(np.float32)
    result = torch.norm(torch.tensor(input_x), p=p, dim=None, keepdim=keepdim)
    result = result.numpy()
    golden = result.astype(np.float16)
    input_x = input_x.astype(np.float16)
    os.system("mkdir -p input")
    os.system("mkdir -p output")

    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
