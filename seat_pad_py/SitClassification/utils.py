# -*- coding: utf-8 -*-
"""
Created on Thu Oct 17 20:02:21 2024

@author: 叶小明
"""

import numpy as np
import torch
from scipy.ndimage import zoom

def read_matrix_from_file(file_path):
    # 使用numpy加载文件内容，数据由空格分隔
    matrix = np.loadtxt(file_path)
    return matrix

def normalize_array(array):
    # 使用NumPy的最大最小归一化公式
    min_val = np.min(array)
    max_val = np.max(array)
    normalized_array = (array - min_val) / (max_val - min_val)
    return normalized_array

def data_processing(matrix):
    matrix = zoom(matrix, (40/20, 40/20))
    matrix = torch.tensor(matrix, dtype=torch.float32)
    return matrix
