# SitClassification

## 简介

`SitClassification` 是一个用于坐姿分类的 Python 包。它提供了一个坐姿分类接口SitPostureClassify，对输入的坐姿数据进行分类。

## 目录结构

SitClassification/
│
├── SitClassification/
│   ├── init.py
│   ├── model.py
│   ├── utils.py
│   ├── SitClassificationInterfaces.py
│   ├── setup.py
│   └── checkpoints/
│       └── model_weights.pth  # 模型权重文件
│
└── README.txt

## 相关依赖

1. pytorch >= 1.9.0
2. scipy >= 1.7.3

## 安装

可以通过以下方式安装此包：

1. 将SitClassification.zip解压到项目根目录：

MyProject
│
├── SitClassification
└── test.py

2. 进入目录：

    cd SitClassification

3. 安装SitClassification包：

    pip install .

## 使用方法

以下是如何使用 `SitClassification` 包的示例：

根目录下创建test.py文件:

from SitClassification import SitPostureClassify
import numpy as np
import torch

sample_input = np.random.rand(50, 50).astype(np.float32)  # 创建一个随机 numpy 数组
# 将其转换为张量
sample_input = torch.tensor(sample_input)

output = SitPostureClassify(sample_input)
print(output)


