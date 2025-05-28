# Smart-seat-cushion
Intelligent cushion acquisition system based on neural network and MCU

这是一个阵列式压力传感器的采集装置软件部分代码，以及硬件电路原理图，主要包括四个部分：

1. PressureSensorArray 这是单片机的代码，负责采集压力传感器阵列的数据，并通过WIFI模块使用UDP将采集信息发送至上位机。
2. seat_pad_py 这是PC上位机的UDP接收代码，负责接收单片机发送的压力矩阵信息，并将该矩阵信息送入神经网络中进行聚类，最后连同聚类结果和压力矩阵数据输出热力图
3. Seat_Classification 这是神经网络聚类的代码
4. pdf文件是硬件原理图，引脚连接仅供参考

This is the software code of the acquisition device of an array pressure sensor, as well as the hardware circuit schematic diagram, which mainly includes four parts:

1. PressureSensorArray This is the code of the microcontroller, which is responsible for collecting the data of the pressure sensor array and sending the collected information to the host computer using UDP through the WIFI module.
2. seat_pad_py This is the UDP receiving code of the PC upper computer, which is responsible for receiving the pressure matrix information sent by the microcontroller, and sending the matrix information into the neural network for clustering, and finally outputting the heat map with the clustering results and the pressure matrix data
3. Seat_Classification Here is the code for neural network clustering
4. The pdf file is a hardware schematic and the pin connection is for reference only

如果有任何交流，欢迎联系我！
If you have any communication, please contact me!