#!/usr/bin/python3
# -*- coding:utf-8 -*-
# Copyright 2022-2023 Huawei Technologies Co., Ltd
import numpy as np
import os
import torch
import torch.nn as nn

def gen_golden_data_simple():
    input_x = np.random.uniform(0, 1, [1, 1*2048]).astype(np.float16)  #[32, 16384];[32, 1365*16];32, (1320*3)*16;368;[1, (31*64+48)]
    input_y = np.random.uniform(0, 1, [1, 1*2048]).astype(np.float16)
    # input_z = np.random.uniform(1, 100, [1, 96*2048]).astype(np.float16)
    # value  = 1.0
    # golden = (input_x + (input_y / input_z) * value).astype(np.float16)

    loss = nn.MSELoss(reduction="none")
    # loss = nn.MSELoss(reduction="sum")
    # loss = nn.MSELoss(reduction="mean")
    golden = loss(torch.from_numpy(input_x), torch.from_numpy(input_y))
    golden = golden.numpy()

    
    os.system("mkdir -p input")
    os.system("mkdir -p output")
    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")

if __name__ == "__main__":
    gen_golden_data_simple()
    
