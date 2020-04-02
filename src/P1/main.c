#include <stddef.h>
#include <stdint.h>


#define RCC_AHB1ENR_BASE 0x40023800
#define RCC_AHB1ENR_OFFSET 0x30



#define GPIO_B_BASE 0x40020400


#define GPIO_B_MODE_OFFSET 0x00
#define GPIO_B_OFFSET_ODR 0x14


#define RCC_AHB1ENR_REGISTER (RCC_AHB1ENR_BASE + RCC_AHB1ENR_OFFSET)
#define GPIO_B_MODE_REGISTER (GPIO_B_BASE + GPIO_B_MODE_OFFSET)
#define GPIO_B_ODR_REGISTER (GPIO_B_BASE + GPIO_B_OFFSET_ODR)


#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7
#define BIT15 15



void delay(){
	for(volatile uint32_t i =0; i <5000000;i++){
		asm("NOP");
	}
}

void set_led(int _state){

	if(_state){
		*((volatile uint32_t*)(GPIO_B_ODR_REGISTER))|= (1 << BIT7);
	}else{
		*((volatile uint32_t*)(GPIO_B_ODR_REGISTER)) &= ~(1 << BIT7);
	}

}



int main(void)
{
	//EN CLOCK FOR GPIOB
	*((volatile uint32_t*)(RCC_AHB1ENR_REGISTER)) |= (1 << BIT1);
	//SET PORT TO OUTPUT
	*((volatile uint32_t*)(GPIO_B_MODE_REGISTER)) |= (1 << BIT15);


	for(;;){
		set_led(1);
		delay();
		set_led(0);
		delay();
	}
}
