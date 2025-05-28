import socket
import math
import matplotlib.pyplot as plt
from SitClassification import SitPostureClassify
import numpy as np
import torch

# 设置服务器的IP地址和端口号
server_ip = '192.168.43.233'
server_port = 8266

# 创建UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# 绑定到指定的IP地址和端口号
sock.bind((server_ip, server_port))
print(f"UDP server up and listening on {server_ip}:{server_port}")


# def parse_matrix(data):
#     # 按行分割
#     lines = data.strip().split('\r\n')
#
#     matrix = []
#     for line in lines:
#         # 将每行的数据分割并转换为整数
#         row = list(int(num) for num in line.split())
#         matrix.append(row)
#
#     return matrix
#
#
# try:
#     while True:
#         # 接收数据，最大缓冲区大小4096字节
#         data, addr = sock.recvfrom(4096)
#         print(f"Received message from {addr}:")
#
#         try:
#             # 解析矩阵
#             matrix = parse_matrix(data.decode())
#             print("Matrix received and parsed successfully:")
#             for row in matrix:
#                 print(row)
#         except Exception as e:
#             print(f"Error parsing matrix: {e}")
# finally:
#     # 关闭socket
#     sock.close()
def parse_matrix(data):
    matrix = []
    lines = data.strip().split('\r\n')
    for line in lines:
        row = list(map(int, line.split()))
        matrix.append(row)
    return matrix

# 初始化一个缓冲区来存储接收到的数据
buffer = ""

try:
    while True:
        # 接收数据
        data, addr = sock.recvfrom(4096)
        # 将收到的数据添加到缓冲区
        buffer += data.decode('utf-8')

        # 检查是否收到了完整的数据（以"!"结尾）
        if '!' in buffer:
            # 找到最后一个感叹号的位置
            end_index = buffer.rindex('!')
            # 提取完整的数据（包括感叹号）
            complete_data = buffer[:end_index]
            # 剩余的数据（如果有）
            buffer = buffer[end_index:]
            # 解析矩阵
            matrix = parse_matrix(complete_data)
            print("Matrix received and parsed successfully:")
            for row in matrix:
                print(row)
            # log_matrix = []
            # for row in matrix:
            #     # 将每个整数转换为以10为底的对数的浮点数形式，并添加到新的行中
            #     log_row = [float(math.log10(num)) for num in row]
            #     log_matrix.append(log_row)

            sample_input = torch.tensor(matrix)
            output = SitPostureClassify(sample_input)

            # 绘制热力图
            plt.imshow(matrix)
            plt.colorbar()
            # plt.tight_layout()
            plt.xlabel("column", size=15)
            plt.ylabel("row", size=15)
            plt.xlim(0, 20)
            plt.ylim(0, 20)
            plt.title(output, loc="center")
            plt.show(block=False)
            plt.pause(2)
            plt.close()
            buffer = ""

except KeyboardInterrupt:
    print("Server shutdown.")

finally:
    # 关闭socket
    sock.close()