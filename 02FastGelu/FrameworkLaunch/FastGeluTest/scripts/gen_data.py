#!/usr/bin/python3
# -*- coding:utf-8 -*-
# Copyright 2022-2023 Huawei Technologies Co., Ltd
import numpy as np
import os
import torch
import torch.nn.functional as F

def gen_golden_data_simple():
    input_x = np.random.uniform(1, 10, [1024]).astype(np.float16)
    attr = 1.702
    attr_opp = 0 - attr
    attr_half = attr / 2
    abs_x = input_x
    mul_abs_x = abs_x * attr_opp
    exp_abs_x = np.exp(mul_abs_x)
    div_down = exp_abs_x + 1.0

    pn_x = input_x - abs_x
    mul_pn_x = pn_x * attr_half
    exp_pn_x = np.exp(mul_pn_x)
    div_up = input_x * exp_pn_x
    golden = div_up / div_down

    os.system("mkdir -p input")
    os.system("mkdir -p output")
    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
