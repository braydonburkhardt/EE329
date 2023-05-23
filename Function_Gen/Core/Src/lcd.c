#include "lcd.h"

void LCD_init() {
	//set all pins to output, push-pull, no pupd, and lowspeed then clear outputs
	RCC->AHB2ENR |= LCD_PORT_RCC;
	LCD_PORT->MODER &= ~((3<<LCD_PIN_RS*2) | (3<<LCD_PIN_RW*2) | (3<<LCD_PIN_EN*2) | (3<<LCD_PIN_D7*2) | (3<<LCD_PIN_D6*2) | (3<<LCD_PIN_D5*2) | (3<<LCD_PIN_D4*2));
	LCD_PORT->MODER |= ((1<<LCD_PIN_RS*2) | (1<<LCD_PIN_RW*2) | (1<<LCD_PIN_EN*2) | (1<<LCD_PIN_D7*2) | (1<<LCD_PIN_D6*2) | (1<<LCD_PIN_D5*2) | (1<<LCD_PIN_D4*2));
	LCD_PORT->OTYPER &= ~((1<<LCD_PIN_RS) | (1<<LCD_PIN_RW) | (1<<LCD_PIN_EN) | (1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	LCD_PORT->PUPDR &= ~((3<<LCD_PIN_RS*2) | (3<<LCD_PIN_RW*2) | (3<<LCD_PIN_EN*2) | (3<<LCD_PIN_D7*2) | (3<<LCD_PIN_D6*2) | (3<<LCD_PIN_D5*2) | (3<<LCD_PIN_D4*2));
	LCD_PORT->OSPEEDR &= ~((3<<LCD_PIN_RS*2) | (3<<LCD_PIN_RW*2) | (3<<LCD_PIN_EN*2) | (3<<LCD_PIN_D7*2) | (3<<LCD_PIN_D6*2) | (3<<LCD_PIN_D5*2) | (3<<LCD_PIN_D4*2));
	LCD_PORT->ODR &= ~((1<<LCD_PIN_RS) | (1<<LCD_PIN_RW) | (1<<LCD_PIN_EN) | (1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));

	//run init sequence
	delay_us(100000);
	LCD_PORT->ODR &= ~((1<<LCD_PIN_RS) | (1<<LCD_PIN_RW) | (1<<LCD_PIN_EN) | (1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	LCD_PORT->ODR |= (0x30); //wake up
	delay_us(5000);
	LCD_pulse();
	delay_us(5000);
	LCD_pulse();
	delay_us(5000);
	LCD_pulse();
	delay_us(2000);
	LCD_PORT->ODR &= ~((1<<LCD_PIN_RS) | (1<<LCD_PIN_RW) | (1<<LCD_PIN_EN) | (1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	LCD_PORT->ODR |= ((0x30<<16) | 0x20); //4b interface mode
	LCD_pulse();
	LCD_command(0x28);

	//turn the display on and set the entry mode to increment
	LCD_command(LCD_COMMAND_DISPLAY_ON);
	LCD_command(LCD_COMMAND_ENTRY_INC);
}

void LCD_pulse() {
	//quick pulse of the enable pin to push data
	LCD_PORT->ODR |= (1<<LCD_PIN_EN);
	delay_us(10);
	LCD_PORT->ODR &= ~(1<<LCD_PIN_EN);
}

void LCD_command(uint8_t data) {
	LCD_PORT->ODR &= ~((1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	LCD_PORT->ODR |= (0xF0 & data) | ((0xF0 & (~data)) << 16); //set upper 4 bit data
	LCD_pulse();
	LCD_PORT->ODR &= ~((1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	LCD_PORT->ODR |= (0xF0 & (data << 4)) | ((0xF0 & (~data << 4)) << 16); //set lower 4 bit data
	LCD_pulse();
	LCD_PORT->ODR &= ~((1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	delay_us(1000);
}

void LCD_write(uint8_t data) {
	LCD_PORT->ODR &= ~((1<<LCD_PIN_RS) | (1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	LCD_PORT->ODR |= ((1<<LCD_PIN_RS) | (0xF0 & data) | ((0xF0 & (~data)) << 16)); //set upper 4 bit data
	LCD_pulse();
	LCD_PORT->ODR &= ~((1<<LCD_PIN_RS) | (1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	LCD_PORT->ODR |= ((1<<LCD_PIN_RS) | (0xF0 & (data << 4)) | ((0xF0 & (~data << 4)) << 16)); //set lower 4 bit data
	LCD_pulse();
	LCD_PORT->ODR &= ~((1<<LCD_PIN_RS) | (1<<LCD_PIN_D7) | (1<<LCD_PIN_D6) | (1<<LCD_PIN_D5) | (1<<LCD_PIN_D4));
	delay_us(1000);
}

void LCD_write_string(char input[2][16]) {
	LCD_command(LCD_COMMAND_WRITE);
	//loop through character array and print each char individually
	for (int i=0; i<2; i++) {
		for (int k=0; k<16; k++) {
			if (input[i][k]=='\0') break; //done!
			LCD_write(input[i][k]);
		}
		LCD_command(LCD_COMMAND_WRITE | ((i+1)<<6));
	}
}






