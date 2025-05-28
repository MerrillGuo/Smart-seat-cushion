# -*- coding: utf-8 -*-
"""
Created on Thu Oct 17 20:02:21 2024

@author: 叶小明
"""

import numpy as np
import torch
from scipy.ndimage import zoom
from .model import SittingPostureClassificationModel
from .utils import read_matrix_from_file, normalize_array, data_processing

# 类名
class_names = ['back_posture', 'correct_posture', 'emtpy', 'left_posture', 'right_posture']

# 模型加载为全局变量
model = SittingPostureClassificationModel()
model.load_state_dict(torch.load('./SitClassification/checkpoint/model_weights.pth'))

'''
@Params 
    matrix: array with shape: (20 * 20)
'''
def SitPostureClassify(matrix):
    # 设备
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    
    # 静态变量，获取模型
    global model
    model.eval()
    model = model.to(device)
    
    # 数据处理
    matrix = data_processing(matrix)
    matrix = matrix.to(device).unsqueeze(0).unsqueeze(0)
    
    # 模型推理
    with torch.no_grad():
        output = model(matrix)
    _, preds = torch.max(output, 1)
    
    return class_names[preds]
    
