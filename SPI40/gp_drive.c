#include "gp_drive.h"

void init_GP(unsigned short port, unsigned short pin,
						 unsigned short dir, unsigned short opt)
{
	volatile unsigned long * CR;
	unsigned short tPin = pin;
	unsigned short offset = 0x00;
	
	if(pin>7)
		{
			tPin -=8;
			offset =0x01;		//Moving to the next memory location - See Register Maps (GPIO and AFIO Register Maps)
		}
	
	if(port == PA)
		{
			RCC_APB2ENR |= (1<<2);				//Enabling PORT A
			CR = (volatile unsigned long *) (&GPIO_A + offset);
		}
	else if(port == PB)
		{
			RCC_APB2ENR |= (1<<3);				//Enabling PORT B
			CR = (volatile unsigned long *) (&GPIO_B + offset);
		}	
	else if(port == PC)
		{
			RCC_APB2ENR |= (1<<4);				//Enabling PORT C
			CR = (volatile unsigned long *) (&GPIO_C + offset);
		}	
		
		*CR &= ~( 0xF << (tPin)*4);		//Reset the target pin
		*CR |= ((dir << (tPin*4)) | (opt << (tPin*4+2))); 	//Set up the direction and option of the pin
	
}

int R_GP(unsigned short port, unsigned short pin)
{
	volatile unsigned long * IDR;
	unsigned long offset = 0x02;			//Moving to the next memory location - See Register Maps (GPIO and AFIO Register Maps)
	int state;
	
	if(port == PA)
	{
		IDR = (volatile unsigned long *) (&GPIO_A + offset);
	}
	else if(port == PB)
	{
		IDR = (volatile unsigned long *) (&GPIO_B + offset);
	}
	else if(port == PC)
	{
		IDR = (volatile unsigned long *) (&GPIO_C + offset);
	}
	
	state = ((*IDR& (1 << pin)) >> pin);
	
	return state;
}

void W_GP(unsigned short port, unsigned short pin, unsigned short state)
{
	volatile unsigned long * ODR;
	unsigned long offset = 0x03;			//Moving to the next memory location - See Register Maps (GPIO and AFIO Register Maps)
	
	if(port == PA)
	{
		ODR = (volatile unsigned long *) (&GPIO_A + offset);
	}
	else if(port == PB)
	{
		ODR = (volatile unsigned long *) (&GPIO_B + offset);
	}
	else if(port == PC)
	{
		ODR = (volatile unsigned long *) (&GPIO_C + offset);
	}
	
	state ? (*ODR |= (state << pin)) : (*ODR &= ~(1 << pin)); 
	
}

void toogle_GP(unsigned short port, unsigned short pin)
{
	if(R_GP(port,pin))
		{	
			W_GP(port,pin,0);
		}
	else
		{
			W_GP(port,pin,1);
		}
}

