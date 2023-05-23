#include "keypad.h"

//run init to setup col/row ports
void keypad_init(){
	RCC->AHB2ENR |= (ROW_CLK_PORT | COL_CLK_PORT);
	COL_PORT->OTYPER &= ~((1<<COL_1_PIN) | (1<<COL_2_PIN) | (1<<COL_3_PIN) | (1<<COL_4_PIN));
	//cols/rows are set to pulldown to prevent floating
	COL_PORT->PUPDR &= ~((3<<COL_1_PIN*2) | (3<<COL_2_PIN*2) | (3<<COL_3_PIN*2) | (3<<COL_4_PIN*2));
	COL_PORT->PUPDR |= ((2<<COL_1_PIN*2) | (2<<COL_2_PIN*2) | (2<<COL_3_PIN*2) | (2<<COL_4_PIN*2));
	COL_PORT->OSPEEDR &= ~((3<<COL_1_PIN*2) | (3<<COL_2_PIN*2) | (3<<COL_3_PIN*2) | (3<<COL_4_PIN*2));
	ROW_PORT->OTYPER &= ~((1<<ROW_1_PIN) | (1<<ROW_2_PIN) | (1<<ROW_3_PIN) | (1<<ROW_4_PIN));
	ROW_PORT->PUPDR &= ~((3<<ROW_1_PIN*2) | (3<<ROW_2_PIN*2) | (3<<ROW_3_PIN*2) | (3<<ROW_4_PIN*2));
	ROW_PORT->PUPDR |= ((2<<ROW_1_PIN*2) | (2<<ROW_2_PIN*2) | (2<<ROW_3_PIN*2) | (2<<ROW_4_PIN*2));
	ROW_PORT->OSPEEDR &= ~((3<<ROW_1_PIN*2) | (3<<ROW_2_PIN*2) | (3<<ROW_3_PIN*2) | (3<<ROW_4_PIN*2));
	ROW_PORT->BRR &= ~((1<<ROW_1_PIN) | (1<<ROW_2_PIN) | (1<<ROW_3_PIN) | (1<<ROW_4_PIN));
	COL_PORT->BRR &= ~((1<<COL_1_PIN) | (1<<COL_2_PIN) | (1<<COL_3_PIN) | (1<<COL_4_PIN));
}

//returns given pressed row
unsigned int keypad_rowDetect(){
	//set col pins all HIGH then read row pins using IDR
	ROW_PORT->MODER &= ~((3<<ROW_1_PIN*2) | (3<<ROW_2_PIN*2) | (3<<ROW_3_PIN*2) | (3<<ROW_4_PIN*2));
	COL_PORT->MODER &= ~((3<<COL_1_PIN*2) | (3<<COL_2_PIN*2) | (3<<COL_3_PIN*2) | (3<<COL_4_PIN*2));
	COL_PORT->MODER |= ((1<<COL_1_PIN*2) | (1<<COL_2_PIN*2) | (1<<COL_3_PIN*2) | (1<<COL_4_PIN*2));
	COL_PORT->BSRR |= ((1<<COL_1_PIN) | (1<<COL_2_PIN) | (1<<COL_3_PIN) | (1<<COL_4_PIN));
	if(ROW_PORT->IDR & (1<<ROW_1_PIN)) return 1;
	else if(ROW_PORT->IDR & (1<<ROW_2_PIN)) return 2;
	else if(ROW_PORT->IDR & (1<<ROW_3_PIN)) return 3;
	else if(ROW_PORT->IDR & (1<<ROW_4_PIN)) return 4;
	return 0; //0=no detect
}

//returns given pressed col
unsigned int keypad_colDetect(){
	//set row pins all HIGH then read col pins using IDR
	COL_PORT->MODER &= ~((3<<COL_1_PIN*2) | (3<<COL_2_PIN*2) | (3<<COL_3_PIN*2) | (3<<COL_4_PIN*2));
	ROW_PORT->MODER &= ~((3<<ROW_1_PIN*2) | (3<<ROW_2_PIN*2) | (3<<ROW_3_PIN*2) | (3<<ROW_4_PIN*2));
	ROW_PORT->MODER |= ((1<<ROW_1_PIN*2) | (1<<ROW_2_PIN*2) | (1<<ROW_3_PIN*2) | (1<<ROW_4_PIN*2));
	ROW_PORT->BSRR |= ((1<<ROW_1_PIN) | (1<<ROW_2_PIN) | (1<<ROW_3_PIN) | (1<<ROW_4_PIN));
	if(COL_PORT->IDR & (1<<COL_1_PIN)) return 1;
	else if(COL_PORT->IDR & (1<<COL_2_PIN)) return 2;
	else if(COL_PORT->IDR & (1<<COL_3_PIN)) return 3;
	else if(COL_PORT->IDR & (1<<COL_4_PIN)) return 4;
	return 0; //0=no detect
}

//returns pressed button ID 1-16
int keypad_getPressedKey(){
	unsigned int row = keypad_rowDetect();
	unsigned int col = keypad_colDetect();
	if(!row||!col) return -1; //if either results in no detect, return 0
	else for(int i=0;i<25000;i++); //debounce delay
	if(col==4) return 9+row; //return A-D ID (as hex)
	if(row==4&&col==1) return 14; //return * ID
	if(row==4&&col==2) return 0; //return 0 for 0
	if(row==4&&col==3) return 15; //return # ID
	return (row-1)*3+col; //return button ID 1 to 9
}






