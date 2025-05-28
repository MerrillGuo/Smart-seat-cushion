import socket

# 设置服务器的IP地址和端口号
server_ip = '192.168.43.233'
server_port = 8080

# 创建TCP/IP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# 绑定socket到指定的IP地址和端口号
server_socket.bind((server_ip, server_port))

# 使socket处于监听状态
server_socket.listen()

print(f"Listening on {server_ip}:{server_port}...")

try:
    while True:
        # 接受一个新连接
        client_socket, addr = server_socket.accept()
        print(f"Accepted connection from {addr}")

        try:
            while True:
                # 接收客户端发送的数据
                data = client_socket.recv(1024)
                if not data:
                    break  # 如果没有数据，跳出循环
                print(f"Received data: {data.decode('utf-8')}")
                # 可以在这里处理接收到的数据
        finally:
            # 关闭客户端连接
            client_socket.close()
            print(f"Connection with {addr} closed.")
except KeyboardInterrupt:
    print("Server is shutting down.")
finally:
    # 关闭服务器socket
    server_socket.close()
    print("Server socket closed.")