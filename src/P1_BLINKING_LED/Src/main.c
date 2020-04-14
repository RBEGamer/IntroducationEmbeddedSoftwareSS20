
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif



#include <stddef.h>
#include <stdint.h>

#include "bits.h"

#define AHB1ENR 0x40023800U
#define GPIOBEN 0x30U


#define GPIOB 0x40020c00U
#define GPIOB_MODER 0x00U

#define GPIOB_ODR 0x14U



void delay(){

	for (uint32_t i = 0; i < 80000; ++i) {
		asm("nop");
	}

}
int main(void)
{
	//ENABLE CLOCK für FPIOB
	*((volatile uint32_t*)(AHB1ENR+GPIOBEN)) |=(1 << BIT3);

	//CLEAR MODE -> default state ist input 01-> ist output normal
	*((volatile uint32_t*)(GPIOB+GPIOB_MODER)) |= (0b01010000000000000000000000000000); //PB7 und PB0 sind die led angeschlossen somit BIT15 = 1 BIT 16 = 0 für PB7 und BIT1 = 0 und BIT0 = 1 für PB0


	for(;;){

		delay();
		*((volatile uint32_t*)(GPIOB+GPIOB_ODR)) &= ~(1 << BIT15) & ~(1 << BIT14);
		delay();
		*((volatile uint32_t*)(GPIOB+GPIOB_ODR)) |= (1 << BIT15) | (1 << BIT14);
	}
}
