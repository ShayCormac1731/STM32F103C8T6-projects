#include "stm32f10x.h"                  // Device header
#include "gp_drive.h"

void delay(int);
void systick_init(void);
void DelayMillis(void);
void DelayMs(unsigned long t);
int signal = 0;


int main(void){
	
	void systick_init();
	init_GP(PA,0,IN,I_PP);
	init_GP(PC,13,OUT50,O_GP_PP);
	init_GP(PA,12,OUT50,O_GP_PP);
	
	__disable_irq();
	AFIO->EXTICR[0] = 0x00;
	EXTI->IMR |= 1;
	EXTI->RTSR |= 1;
	NVIC_EnableIRQ(EXTI0_IRQn);
	__enable_irq();
	
	while(1){
	
				W_GP(PA,12,signal);		
			if(signal)
			{
				DelayMs(1000);
				toogle_GP(PC,13);
				DelayMs(1000);
			}
			else
			{
				W_GP(PA,13,1);
			}
	}
	
	//return 0;
}

void delay(int rep){
	int i;
	for(;rep>0;rep--)
		for(i=0;i<100000;i++){}
	
}

void EXTI0_IRQHandler()
{
	EXTI->PR |= 1;
	if(signal)
	{
		signal = 0;
	}
	else
	{
		signal = 1;
	}
}

void systick_init(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->VAL = 0;
	
	SysTick->CTRL = 5;
	
}

void DelayMillis(void)
{
	SysTick->LOAD = 72000-1;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
}

void DelayMs(unsigned long t)
{
	for(;t>0;t--)
		DelayMillis();
}
