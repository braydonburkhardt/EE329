#include "DAC.h"

void DAC_init(){
	RCC->AHB2ENR |= DAC_PORT_RCC;
	DAC_PORT->MODER &= ~((3<<DAC_CS*2)|(3<<DAC_SCK*2)|(3<<DAC_COPI*2));
	DAC_PORT->MODER = ((2<<DAC_CS*2)|(2<<DAC_SCK*2)|(2<<DAC_COPI*2));
	DAC_PORT->OTYPER &= ~((1<<DAC_CS)|(1<<DAC_SCK)|(1<<DAC_COPI));
	DAC_PORT->PUPDR &= ~((3<<DAC_CS*2)|(3<<DAC_SCK*2)|(3<<DAC_COPI*2));
	DAC_PORT->OSPEEDR |= ((3<<DAC_CS*2)|(3<<DAC_SCK*2)|(3<<DAC_COPI*2));

	//set the AF pins to FN5
	GPIOA->AFR[0] |= ((5<<DAC_CS*4)|(5<<DAC_SCK*4)|(5<<DAC_COPI*4));

	//control regs
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);
	SPI1->CR1 = 0; //clear any prexisting controls
	SPI1->CR1 |= (SPI_CR1_MSTR); //set as controller (master)
	SPI1->CR2 |= (SPI_CR2_NSSP |SPI_CR2_DS_Msk); //enable 16b data transfer and set NSS to pulse
	SPI1->CR1 |= (SPI_CR1_SPE); //enable SPI
}

unsigned int DAC_volt_conv(unsigned int voltage){

	voltage = voltage * DAC_cal_multiplier - DAC_cal_offset; //calibration remapping

	if(voltage>max_voltage) voltage = max_voltage; //set upper-bounds

	return ((voltage*0x1000)/max_voltage)-1; //format 16b val to 12b res
}

void DAC_write(unsigned int spiData){
	spiData += 0x3000; //add control data at msb end 0b0011<<16

	//wait until transmit buffer empty before sending data
	if(!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = spiData;
}
