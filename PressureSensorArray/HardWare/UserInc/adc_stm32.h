//
// Created by 86135 on 2024/9/16.
//

#ifndef ADC_STM32_H
#define ADC_STM32_H

#include "stm32f1xx_hal.h"

#define ADCSWITCHNUMBER 1
#define ROWSWITCHNUMBER 2
#define SIZE 20


typedef enum
{
    kIdle = 0U,
    kStart,
    kWait,
    kDone,
} AdcState;

typedef struct
{
    ADC_HandleTypeDef adc_handle;
    uint16_t adc_value;
    AdcState adc_state;
} AdcStm32;

typedef struct
{
    GPIO_TypeDef *GpioPort;
    uint16_t GpioPin;
} GpioDefine;

typedef struct
{
    GpioDefine En_A;
    GpioDefine En_B;
    GpioDefine S0_A;
    GpioDefine S1_A;
    GpioDefine S2_A;
    GpioDefine S3_A;
    GpioDefine S0_B;
    GpioDefine S1_B;
} AdcSwitchGpio;

void AdcInitUser(AdcStm32* adc_stm32);
void AdcSwitchGpioInit(AdcSwitchGpio* adc_switch_gpio,uint8_t adc_or_row);
void AdcSpinOnce(AdcStm32* adc_stm32);
void AdcGetValue(AdcStm32* adc_stm32);
void SwitchChannel(AdcSwitchGpio* adc_switch_gpio, uint8_t channel);
void SwitchAdcRowNumber();
void PrintDataArray();

#endif //ADC_STM32_H
