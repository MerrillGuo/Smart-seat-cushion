import socket
import numpy as np
import cv2

# 设置UDP端口和IP
UDP_IP = "192.168.43.233"
UDP_PORT = 8266

# 创建并绑定socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
print("UDP server up and running on {}:{}".format(UDP_IP, UDP_PORT))

# 初始化数据
data_array = np.zeros((10, 10))

# 创建OpenCV窗口
cv2.namedWindow('UDP Data Visualization', cv2.WINDOW_NORMAL)


def receive_and_visualize(sock):
    while True:
        try:
            # 接收数据
            raw_data, addr = sock.recvfrom(1024)
            array_str = raw_data.decode('utf-8').strip()
            array_values = [int(x) for x in array_str.split()]

            # 将接收到的数据重构为10x10的二维数组
            data_array = np.array(array_values).reshape((10, 10))

            # 将数据规范化到0-255范围以显示
            data_array_normalized = cv2.normalize(data_array, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX)

            # 转换为整数并转换为无符号8位
            data_image = np.uint8(data_array_normalized)

            # 显示图像
            cv2.imshow('UDP Data Visualization', data_image)

            # 检查按键，如果按下'q'，则退出
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        except socket.error as e:
            print(f"Socket error: {e}")

    # 释放窗口
    cv2.destroyAllWindows()


# 开始接收和可视化数据
receive_and_visualize(sock)