
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif



#include <stddef.h>
#include <stdint.h>


#define AHB1ENR 0x40023800U
#define GPIOBEN 0x30U

#define GPIOB 0x40020400U
#define GPIOB_MODER 0x00U

#define GPIOB_ODR 0x14U

void delay(){

	for (uint32_t i = 0; i < 500000; ++i) {
		asm("nop");
	}

}
int main(void)
{
	//ENABLE CLOCK
	*((volatile uint32_t*)(AHB1ENR+GPIOBEN)) |=(1 << 1);
	//CLEAR MODE
	*((volatile uint32_t*)(GPIOB+GPIOB_MODER)) |= (0b00000000000000000100000000000000);


	for(;;){

		delay();
		*((volatile uint32_t*)(GPIOB+GPIOB_ODR)) |=(1 << 7);
		delay();
		*((volatile uint32_t*)(GPIOB+GPIOB_ODR)) &= ~(1 << 7);

	}
}
