//
// Created by 86135 on 2024/9/16.
//
#include "adc_stm32.h"
#include "stdio.h"
#include "esp8266.h"

extern ADC_HandleTypeDef hadc1;
extern AdcSwitchGpio adc_channel;
extern AdcSwitchGpio row_channel;

static uint8_t adc_flag = 0;
static uint8_t adc_channel_number = 0;
static uint8_t row_channel_number = 0;
static uint16_t value[SIZE] = {0};
static uint16_t array_value[SIZE][SIZE] = {0};

void AdcInitUser(AdcStm32* adc_stm32)
{
    adc_stm32->adc_handle = hadc1;
    adc_stm32->adc_value = 0;
    adc_stm32->adc_state = kIdle;
}

void AdcSwitchGpioInit(AdcSwitchGpio* adc_switch_gpio,uint8_t adc_or_row)
{
    if(adc_or_row == ADCSWITCHNUMBER)
    {
        adc_switch_gpio->En_A.GpioPort = GPIOB;
        adc_switch_gpio->En_A.GpioPin = GPIO_PIN_10;
        adc_switch_gpio->En_B.GpioPort = GPIOB;
        adc_switch_gpio->En_B.GpioPin = GPIO_PIN_1;
        adc_switch_gpio->S0_A.GpioPort = GPIOA;
        adc_switch_gpio->S0_A.GpioPin = GPIO_PIN_4;
        adc_switch_gpio->S1_A.GpioPort = GPIOA;
        adc_switch_gpio->S1_A.GpioPin = GPIO_PIN_5;
        adc_switch_gpio->S2_A.GpioPort = GPIOA;
        adc_switch_gpio->S2_A.GpioPin = GPIO_PIN_6;
        adc_switch_gpio->S3_A.GpioPort = GPIOA;
        adc_switch_gpio->S3_A.GpioPin = GPIO_PIN_7;
        adc_switch_gpio->S0_B.GpioPort = GPIOC;
        adc_switch_gpio->S0_B.GpioPin = GPIO_PIN_14;
        adc_switch_gpio->S1_B.GpioPort = GPIOC;
        adc_switch_gpio->S1_B.GpioPin = GPIO_PIN_15;
    }
    else if(adc_or_row == ROWSWITCHNUMBER)
    {
        adc_switch_gpio->En_A.GpioPort = GPIOB;
        adc_switch_gpio->En_A.GpioPin = GPIO_PIN_15;
        adc_switch_gpio->En_B.GpioPort = GPIOB;
        adc_switch_gpio->En_B.GpioPin = GPIO_PIN_14;
        adc_switch_gpio->S0_A.GpioPort = GPIOB;
        adc_switch_gpio->S0_A.GpioPin = GPIO_PIN_6;
        adc_switch_gpio->S1_A.GpioPort = GPIOB;
        adc_switch_gpio->S1_A.GpioPin = GPIO_PIN_5;
        adc_switch_gpio->S2_A.GpioPort = GPIOB;
        adc_switch_gpio->S2_A.GpioPin = GPIO_PIN_4;
        adc_switch_gpio->S3_A.GpioPort = GPIOB;
        adc_switch_gpio->S3_A.GpioPin = GPIO_PIN_3;
        adc_switch_gpio->S0_B.GpioPort = GPIOB;
        adc_switch_gpio->S0_B.GpioPin = GPIO_PIN_12;
        adc_switch_gpio->S1_B.GpioPort = GPIOB;
        adc_switch_gpio->S1_B.GpioPin = GPIO_PIN_13;
    }
    else
    {
        printf("error number!\n");
    }
}

void AdcSpinOnce(AdcStm32* adc_stm32){
    switch(adc_stm32->adc_state){
        case kIdle:
            adc_stm32->adc_state = kStart;
            SwitchChannel(&adc_channel, adc_channel_number);
            if(adc_channel_number == 0){
                SwitchChannel(&row_channel, row_channel_number);
            }
            //HAL_Delay(1);
            break;
        case kStart:
            adc_flag = 0;
            adc_stm32->adc_state = kWait;
            HAL_ADC_Start_IT(&hadc1);
            break;
        case kWait:
            if(adc_flag == 1)
            {
                adc_stm32->adc_state = kDone;
            }
            break;
        case kDone:
            adc_stm32->adc_state = kIdle;
            adc_stm32->adc_value = HAL_ADC_GetValue(&hadc1);
            AdcGetValue(adc_stm32);
            break;
        default:
            break;
    }
}

void AdcGetValue(AdcStm32* adc_stm32)
{
    array_value[adc_channel_number][row_channel_number] = adc_stm32->adc_value;
//    if(adc_stm32->adc_value<=500 ){
//        array_value[adc_channel_number][row_channel_number] = 0;
//    }
//    else{
//        array_value[adc_channel_number][row_channel_number] = adc_stm32->adc_value - 500;
//    }
    SwitchAdcRowNumber();
}

void SwitchAdcRowNumber()
{
    if(adc_channel_number == SIZE-1 )
    {
        adc_channel_number = 0;
        //row switch
        if(row_channel_number == SIZE-1){
            row_channel_number = 0;
            HAL_Delay(10);
            PrintDataArray();
        }
        else if(row_channel_number < SIZE-1){
            row_channel_number++;
        }
        else{
            printf("row switch error!%d\n", row_channel_number);
        }

    }
    else if(adc_channel_number < SIZE-1 )
    {
        adc_channel_number++;
    }
    else
    {
        printf("adc switch error!%d\n", adc_channel_number);
    }
}


void PrintDataArray(){
    if(row_channel_number == 0)
    {
        printf("START\r\n");
        HAL_Delay(5);
        for(int row = 0; row < SIZE; row++)
        {
            for(int adc = 0; adc < SIZE; adc++)
            {
                printf("%d ",array_value[row][adc]);
            }
            HAL_Delay(1);
            printf("\r\n");
            HAL_Delay(5);
        }
        Esp32SendArray(array_value);
    }
}

void SwitchChannel(AdcSwitchGpio* adc_switch_gpio, uint8_t channel)
{
    if(channel < 16)
    {
        HAL_GPIO_WritePin(adc_switch_gpio->En_A.GpioPort,adc_switch_gpio->En_A.GpioPin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(adc_switch_gpio->En_B.GpioPort,adc_switch_gpio->En_B.GpioPin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin | adc_switch_gpio->S2_A.GpioPin |
            adc_switch_gpio->S1_A.GpioPin | adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_RESET);
        switch (channel)
        {
            case 0:
                // S3 = 0, S2 = 0, S1 = 0, S0 = 0
                break;
            case 1:
                // S3 = 0, S2 = 0, S1 = 0, S0 = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_SET);
                break;
            case 2:
                // S3 = 0, S2 = 0, S1 = 1, S0 = 0
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S1_A.GpioPin, GPIO_PIN_SET);
                break;
            case 3:
                // S3 = 0, S2 = 0, S1 = 1, S0 = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S1_A.GpioPin |
                    adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_SET);
                break;
            case 4:
                // S3 = 0, S2 = 1, S1 = 0, S0 = 0
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S2_A.GpioPin, GPIO_PIN_SET);
                break;
            case 5:
                // S3 = 0, S2 = 1, S1 = 0, S0 = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S2_A.GpioPin |
                    adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_SET);
                break;
            case 6:
                // S3 = 0, S2 = 1, S1 = 1, S0 = 0
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S2_A.GpioPin |
                    adc_switch_gpio->S1_A.GpioPin, GPIO_PIN_SET);
                break;
            case 7:
                // S3 = 0, S2 = 1, S1 = 1, S0 = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S2_A.GpioPin |
                    adc_switch_gpio->S1_A.GpioPin | adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_SET);
                break;
            case 8:
                // S3 = 1, S2 = 0, S1 = 0, S0 = 0
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin, GPIO_PIN_SET);
                break;
            case 9:
                // S3 = 1, S2 = 0, S1 = 0, S0 = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin |
                    adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_SET);
                break;
            case 10:
                // S3 = 1, S2 = 0, S1 = 1, S0 = 0
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin |
                    adc_switch_gpio->S1_A.GpioPin, GPIO_PIN_SET);
                break;
            case 11:
                // S3 = 1, S2 = 0, S1 = 1, S0 = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin |
                    adc_switch_gpio->S1_A.GpioPin | adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_SET);
                break;
            case 12:
                // S3 = 1, S2 = 1, S1 = 0, S0 = 0
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin |
                    adc_switch_gpio->S2_A.GpioPin, GPIO_PIN_SET);
                break;
            case 13:
                // S3 = 1, S2 = 1, S1 = 0, S0 = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin |
                    adc_switch_gpio->S2_A.GpioPin | adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_SET);
                break;
            case 14:
                // S3 = 1, S2 = 1, S1 = 1, S0 = 0
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin |
                    adc_switch_gpio->S2_A.GpioPin | adc_switch_gpio->S1_A.GpioPin, GPIO_PIN_SET);
                break;
            case 15:
                // S3 = 1, S2 = 1, S1 = 1, S0 = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_A.GpioPort, adc_switch_gpio->S3_A.GpioPin |
                    adc_switch_gpio->S2_A.GpioPin | adc_switch_gpio->S1_A.GpioPin | adc_switch_gpio->S0_A.GpioPin, GPIO_PIN_SET);
                break;
            default:
                break;
        }
    }
    else if(channel > 15 && channel < 20)
    {
        HAL_GPIO_WritePin(adc_switch_gpio->En_A.GpioPort,adc_switch_gpio->En_A.GpioPin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(adc_switch_gpio->En_B.GpioPort,adc_switch_gpio->En_B.GpioPin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(adc_switch_gpio->S0_B.GpioPort, adc_switch_gpio->S0_B.GpioPin | adc_switch_gpio->S1_B.GpioPin, GPIO_PIN_RESET);
        switch (channel - 16)
        {
            case 0:
                // S1_B = 0, S0_B = 0
                break;
            case 1:
                // S1_B = 0, S0_B = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_B.GpioPort, adc_switch_gpio->S0_B.GpioPin, GPIO_PIN_SET);
                break;
            case 2:
                // S1_B = 1, S0_B = 0
                HAL_GPIO_WritePin(adc_switch_gpio->S0_B.GpioPort, adc_switch_gpio->S1_B.GpioPin, GPIO_PIN_SET);
                break;
            case 3:
                // S1_B = 1, S0_B = 1
                HAL_GPIO_WritePin(adc_switch_gpio->S0_B.GpioPort, adc_switch_gpio->S1_B.GpioPin |
                        adc_switch_gpio->S0_B.GpioPin, GPIO_PIN_SET);
                break;
            default:
                break;
        }
    }
    else
    {
        printf("error number for choose! %d\n",channel);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    adc_flag = 1;
}
