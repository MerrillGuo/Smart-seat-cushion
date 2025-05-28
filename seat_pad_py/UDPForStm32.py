import socket
import math
import signal
import matplotlib.pyplot as plt
import numpy as np

# 设置监听的IP地址和端口号
host = '192.168.43.233'  # 监听所有可用的接口
port = 8266     # 端口号

# 热力图的标签序号
size = 20

# 创建UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((host, port))

print(f"UDP server listening on {host}:{port}")

# 定义一个信号处理函数
def signal_handler(signum, frame):
    print("Server is shutting down.")
    sock.close()


try:
    full_matrix = []  # 初始化用于存储完整20x20矩阵的列表
    part_count = 0  # 用于跟踪接收到的矩阵部分数量
    parts_needed = 2  # 需要接收的矩阵部分数量
    while True:
        part_count = 0
        full_matrix = []
        while part_count < parts_needed:
            # 接收数据和客户端地址
            data, addr = sock.recvfrom(2048)  # buffer size is 2048 bytes
            received_data = data.decode()  # 解码数据

            # 将字符串数据分割成行
            lines = received_data.strip().split('\r\n')

            # 遍历每一行，将每个数据转换为整数并存储在临时列表中
            temp_matrix = []
            for line in lines:
                # row = [int(num) for num in line.split()]
                row = [float(math.log10(num)) for num in line.split()]
                temp_matrix.append(row)

            # print("test")
            # for row in temp_matrix:
            #     print(row)

            # 将接收到的矩阵部分添加到累加器中
            full_matrix.append(temp_matrix)
            part_count += 1  # 增加接收到的部分数量

        # 创建一个新的20x20矩阵，初始化为0
        new_matrix = [[0 for _ in range(20)] for _ in range(20)]

        # 将数据复制到新的20x20矩阵中
        for i in range(10):  # 遍历前10行
            for j in range(20):  # 遍历所有列
                new_matrix[i][j] = full_matrix[0][i][j]
                new_matrix[i + 10][j] = full_matrix[1][i][j]

        # 打印完整的20x20矩阵
        # print("Complete 20x20 matrix:")
        # for row in full_matrix[0]:  # 第一部分
        #     print(row)
        # for row in full_matrix[1]:  # 第二部分
        #     print(row)
        print("New 20x20 matrix:")
        for row in new_matrix:
            print(row)

        # # 绘制热力图
        # plt.imshow(new_matrix)
        # plt.colorbar()
        # plt.tight_layout()
        # plt.show()
except KeyboardInterrupt:
    print("Server is shutting down.")
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    sock.close()  # 关闭套接字
