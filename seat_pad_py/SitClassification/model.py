# -*- coding: utf-8 -*-
"""
Created on Tue Oct 15 23:37:42 2024

@author: 叶小明
"""

import torch
import torch.nn as nn
import torch.nn.functional as F 
import warnings
warnings.filterwarnings("ignore", category=UserWarning, module='torch')

class SittingPostureClassificationModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(1, 1, 2)
        self.pool = nn.MaxPool2d(2, 1)
        self.conv2 = nn.Conv2d(1, 1, 3)
        self.fc1 = nn.Linear(1 * 35 * 35, 120) # 225, 120
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 5)

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = torch.flatten(x, 1) # flatten all dimensions except batch
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x