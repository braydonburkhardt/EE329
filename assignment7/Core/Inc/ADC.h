#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32l4xx_hal.h"
#include "delay.h"
#include "uart.h" //temp

//define ADC port/pin
#define ADC_PORT_RCC RCC_AHB2ENR_GPIOCEN
#define ADC_PORT GPIOC
#define ADC_PIN 0


void ADC_init();
void ADC2_IRQHandler();

#endif
