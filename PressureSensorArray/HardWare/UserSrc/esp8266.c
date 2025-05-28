//
// Created by 86135 on 2024/9/23.
//
#include "esp8266.h"
#define SIZE 20
#define BUFFER_SIZE (SIZE * SIZE * 5 + 2 * SIZE)

extern UART_HandleTypeDef huart2;
static char data[2048] = "";
static char data_half1[1024] = "";
static char data_half2[1024] = "";
static uint8_t esp8266_receive_buffer[1024];//存放8266的消息

/*8266初始化*/
void Esp8266Init(){
    HAL_Delay(5000);
    printf("send AT\r\n");
    HAL_Delay(50);
    if(HAL_UART_Transmit(&huart2,"AT\r\n",4,0xff)==HAL_OK){
        printf("send AT OK!\r\n");
        HAL_Delay(50);
    }
    HAL_Delay(3000);
    printf("send AT+CWJAP=\"友晒のP60\",\"12345678900\"\r\n");
    HAL_Delay(50);
    if(HAL_UART_Transmit(&huart2,"AT+CWJAP=\"友晒のP60\",\"12345678900\"\r\n",33,0xff)==HAL_OK){
        printf("send AT+CWJAP=\"友晒のP60\",\"12345678900\" OK!\r\n");
        HAL_Delay(50);
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2,esp8266_receive_buffer,sizeof(esp8266_receive_buffer));//开启接收8266的消息
    HAL_Delay(2000);
//    while(JudgeEsp8266Reply(10)!=kEsp8266IP){
//        printf("1");
//    }


//
//    printf("send AT+CWMODE=1\r\n");
//    HAL_Delay(50);
//    if(HAL_UART_Transmit(&huart2,"AT+CWMODE=1\r\n",13,0xff)==HAL_OK){
//        printf("send AT+CWMODE=1 OK!\r\n");
//        HAL_Delay(50);
//    }
//    HAL_Delay(1000);
//
//    printf("send AT+RST\r\n");
//    HAL_Delay(50);
//    if(HAL_UART_Transmit(&huart2,"AT+RST\r\n",8,0xff)==HAL_OK){
//        printf("send AT+RST OK!\r\n");
//        HAL_Delay(50);
//    }
//    HAL_Delay(3000);
//
//    printf("send AT+CWJAP=\"友晒のP60\",\"12345678900\"\r\n");
//    HAL_Delay(50);
//    if(HAL_UART_Transmit(&huart2,"AT+CWJAP=\"友晒のP60\",\"12345678900\"\r\n",33,0xff)==HAL_OK){
//        printf("send AT+CWJAP=\"友晒のP60\",\"12345678900\" OK!\r\n");
//        HAL_Delay(50);
//    }
//    HAL_Delay(5000);

    printf("send AT+CIPMUX=0\r\n");
    HAL_Delay(50);
    if(HAL_UART_Transmit(&huart2,"AT+CIPMUX=0\r\n",13,0xff)==HAL_OK){
        printf("send AT+CIPMUX=0 OK\r\n");
        HAL_Delay(50);
    }
    HAL_Delay(3000);
    Esp8266SetUdp();
}

void Esp8266InitWifiInformation(WifiInformation* wifi_information, const char* ssid, const char* password) {
    strncpy(wifi_information->ssid, ssid, sizeof(wifi_information->ssid) - 1);
    wifi_information->ssid[sizeof(wifi_information->ssid) - 1] = '\0'; // 确保字符串以空字符结尾
    strncpy(wifi_information->password, password, sizeof(wifi_information->password) - 1);
    wifi_information->password[sizeof(wifi_information->password) - 1] = '\0'; // 确保字符串以空字符结尾
}


void Esp8266SetUdp(){
    printf("Set UDP to PC,IP:192.168.43.233,Port:8266\r\n");
    HAL_Delay(50);
    if(HAL_UART_Transmit(&huart2, "AT+CIPSTART=\"UDP\",\"192.168.43.233\",8266\r\n", 41, 0xff) == HAL_OK){
        printf("send UDP command successful!\r\n");
        HAL_Delay(50);
    }
    HAL_Delay(3000);
}


int JudgeEsp8266Reply(uint16_t Size){
    HAL_UART_DMAStop(&huart2);//先暂停接收
    int i=0;
    do{  //判断消息里有没有OK或者ERROR，8266回复的消息里都会带有OK或者ERROR
        if(esp8266_receive_buffer[i] == 'O' && esp8266_receive_buffer[i + 1] == 'K'){
            memset(esp8266_receive_buffer, 0, sizeof(esp8266_receive_buffer));
            HAL_Delay(1000);//收到OK，延时，等后续的消息发送完成
            printf("8266 say OK\r\n");
            HAL_UARTEx_ReceiveToIdle_DMA(&huart2, esp8266_receive_buffer, sizeof(esp8266_receive_buffer));//开启下次接收
            return kEsp8266OK;
        }
        else if(esp8266_receive_buffer[i] == 'E' && esp8266_receive_buffer[i + 1] == 'R' && esp8266_receive_buffer[i + 2] == 'R' && esp8266_receive_buffer[i + 3] == 'O' && esp8266_receive_buffer[i + 4] == 'R'){
            HAL_UART_Transmit(&huart2,"AT\r\n",4,0xff);//如果返回是error，那么下一条还是发送失败，所以用个AT指令来抵掉这次发送失败
            HAL_Delay(1000);//延时等抵消成功
            printf("\n8266 say ERROR\r\n");
            memset(esp8266_receive_buffer, 0, sizeof(esp8266_receive_buffer));
            HAL_UARTEx_ReceiveToIdle_DMA(&huart2, esp8266_receive_buffer, sizeof(esp8266_receive_buffer));
            return kEsp8266ERROR;
        }
        else if(esp8266_receive_buffer[i] == 'W' && esp8266_receive_buffer[i + 1] == 'I' && esp8266_receive_buffer[i + 2] == 'F' && esp8266_receive_buffer[i + 3] == 'I'){
            memset(esp8266_receive_buffer, 0, sizeof(esp8266_receive_buffer));
            HAL_Delay(1000);//收到OK，延时，等后续的消息发送完成
            printf("8266 say Got IP\r\n");
            HAL_UARTEx_ReceiveToIdle_DMA(&huart2, esp8266_receive_buffer, sizeof(esp8266_receive_buffer));//开启下次接收
            return kEsp8266IP;
        }
    }while(++i<Size);
    /*8266回复的消息有些是分包发回的，即OK或者ERROR可能在后续的包里面*/
    printf("no reply\r\n");
    HAL_Delay(50);
    memset(esp8266_receive_buffer, 0, sizeof(esp8266_receive_buffer));//清空接收下一数据包
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, esp8266_receive_buffer, sizeof(esp8266_receive_buffer));//开始接收下一数据包
    return 0;
}

static char *sprintf_append(const char *format, int value) {
    static char buffer[BUFFER_SIZE];
    sprintf(buffer, format, value);
    return buffer;
}

static int classifyNumber(int number) {
    int digits = 0;
    int temp = number;

    if (temp == 0) {
        digits = 1; // 0是一位数
    } else {
        while (temp > 0) {
            digits++;
            temp /= 10;
        }
    }
    return digits;
}

void Esp32SendArray(uint16_t seat_array[20][20]){
    memset(data, 0, sizeof(data));
    HAL_UART_Transmit(&huart2, "AT+CIPSEND=2041\r\n", 17, 100);
    HAL_Delay(10);
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            switch (classifyNumber(seat_array[i][j])) {
                case 1:
                    strcat(data, sprintf_append("000%d ", seat_array[i][j]));
                    break;
                case 2:
                    strcat(data, sprintf_append("00%d ", seat_array[i][j]));
                    break;
                case 3:
                    strcat(data, sprintf_append("0%d ", seat_array[i][j]));
                    break;
                case 4:
                    strcat(data, sprintf_append("%d ", seat_array[i][j]));
                    break;
                default:
                    break;
            }
        }
        strcat(data,"\r\n");
    }
    strcat(data,"!");
    HAL_UART_Transmit(&huart2,data, strlen(data),0xff);
    HAL_Delay(2000);
//    memset(data_half1, 0, sizeof(data_half1));
//    for(int i = 0; i < 10; i++) {
//        for(int j = 0; j < SIZE; j++) {
//            switch (classifyNumber(seat_array[i][j])) {
//                case 1:
//                    strcat(data_half1, sprintf_append("000%d ", seat_array[i][j]));
//                    break;
//                case 2:
//                    strcat(data_half1, sprintf_append("00%d ", seat_array[i][j]));
//                    break;
//                case 3:
//                    strcat(data_half1, sprintf_append("0%d ", seat_array[i][j]));
//                    break;
//                case 4:
//                    strcat(data_half1, sprintf_append("%d ", seat_array[i][j]));
//                    break;
//                default:
//                    break;
//            }
//        }
//        strcat(data_half1,"\r\n");
//    }
//    HAL_UART_Transmit(&huart2, "AT+CIPSEND=1020\r\n", 17, 100);
//    HAL_Delay(50);
//    HAL_UART_Transmit(&huart2,data_half1, strlen(data_half1),0xff);
//
//    memset(data_half2, 0, sizeof(data_half2));
//    for(int i = 10; i < SIZE; i++) {
//        for(int j = 0; j < SIZE; j++) {
//            switch (classifyNumber(seat_array[i][j])) {
//                case 1:
//                    strcat(data_half2, sprintf_append("000%d ", seat_array[i][j]));
//                    break;
//                case 2:
//                    strcat(data_half2, sprintf_append("00%d ", seat_array[i][j]));
//                    break;
//                case 3:
//                    strcat(data_half2, sprintf_append("0%d ", seat_array[i][j]));
//                    break;
//                case 4:
//                    strcat(data_half2, sprintf_append("%d ", seat_array[i][j]));
//                    break;
//                default:
//                    break;
//            }
//        }
//        strcat(data_half2,"\r\n");
//    }
//    HAL_UART_Transmit(&huart2, "AT+CIPSEND=1020\r\n", 17, 100);
//    HAL_Delay(50);
//    HAL_UART_Transmit(&huart2,data_half2, strlen(data_half2),0xff);
}


