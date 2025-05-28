//
// Created by 86135 on 2024/9/23.
//

#ifndef PRESSURESENSORARRAY_ESP8266_H
#define PRESSURESENSORARRAY_ESP8266_H

#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    char ssid[32];
    char password[32];
} WifiInformation;

typedef enum {
    kEsp8266OK = 1,
    kEsp8266ERROR,
    kEsp8266IP,
} Esp8266Reply;

void Esp8266Init();//初始化函数
void Esp8266InitWifiInformation(WifiInformation* wifi_information, const char* ssid, const char* password);
void Esp8266SetUdp();
int JudgeEsp8266Reply(uint16_t Size);
void Esp32SendArray(uint16_t seat_array[20][20]);


#endif //PRESSURESENSORARRAY_ESP8266_H
