#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32l4xx_hal.h"
#include "delay.h"

void ADC_init();
void ADC_triggerNewVal();
void ADC1_2_IRQHandler();

#endif
