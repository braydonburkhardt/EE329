#include "ADC.h"

int ADC_lastVal = -1;
int ADC_newValFlag = 0;

void ADC_init(){

	//pin PC0
	RCC->AHB2ENR |= ADC_PORT_RCC;
	ADC_PORT->MODER |= (3<<ADC_PIN*2); //analog mode
	ADC_PORT->OTYPER &= ~(1<<ADC_PIN*1); //push-pull
	ADC_PORT->PUPDR &= ~(3<<ADC_PIN*2); //no pupd
	ADC_PORT->OSPEEDR |= (3<<ADC_PIN*2); //very high speed

	SysTick_Init(); //delay init

	LPUART_print("adc: port init");

	RCC->AHB2ENR |= ADC_PORT_RCC;
	ADC1->CR &= ~ADC_CR_DEEPPWD; //exit deep-pwr down mode
	ADC1->CR |= ADC_CR_ADVREGEN; //turn on vreg
	delay_us(20); //wait 20us
	ADC1->CR &= ~ADC_CR_ADCALDIF; //set single-ended inputs

	LPUART_print(", ADC init setup");

	//set cal bit and wait until cal is done
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR && ADC_CR_ADCAL));

	LPUART_print(", cal done");

	ADC1->ISR |= (ADC_ISR_ADRDY); // set the ready bit to 1
	ADC1->CR |= ADC_CR_ADEN; //enable the ADC
	ADC1->IER |= ADC_IER_ADRDYIE;
	while(!(ADC1->ISR && ADC_ISR_ADRDY)); //wait for ready flag to be high
	ADC1->ISR |= (ADC_ISR_ADRDY); //set ready bit to 1

	LPUART_print(", ready enabled");

	NVIC->ISER[0] |= (1 << (ADC1_IRQn & 0x1F)); //set NVICint
	__enable_irq();

	LPUART_print(", irq init");
}

void ADC1_IRQHandler(){
	if ((ADC1->ISR && ADC_ISR_EOC) != 0) {
		ADC_lastVal = ADC1->DR;
		ADC_newValFlag = 1;
	}
}
