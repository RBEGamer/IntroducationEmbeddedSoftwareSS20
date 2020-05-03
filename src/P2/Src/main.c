
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif



#include <stddef.h>
#include <stdint.h>


#include "stm32f407xx_gpio_driver.h"

#define AHB1ENR 0x40023800U
#define GPIOBEN 0x30U



void delay_s(uint32_t _sec){

	for (uint32_t i = 0; i < (1000000*_sec); ++i) {
		asm("nop");
	}

}

void delay(){
	for (uint32_t i = 0; i < 1000000; ++i) {
			asm("nop");
		}

}

//HANDLING IRQ FOR USER BUTTON
void EXTI0_IRQHandler(void){
	GPIO_IRQHandling(GPIO_PIN_NO_0); //CLEAR PA FOR USER_BUTTON PA0
}



int main(void)
{


	GPIO_Handle_t led_1;
	GPIO_PinConfig_t led_1_pin_config;
	led_1_pin_config.GPIO_PinMode = GPIO_MODE_OUT;
	led_1_pin_config.GPIO_PinNumber = GPIO_PIN_NO_5;
	led_1_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	led_1_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_1_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;
	led_1.GPIO_PinConfig = led_1_pin_config;
	led_1.pGPIOx = GPIOB_BASEADDR;
	GPIO_Init(&led_1);

	GPIO_Handle_t led_2;
	GPIO_PinConfig_t led_2_pin_config;
	led_2_pin_config.GPIO_PinMode = GPIO_MODE_OUT;
	led_2_pin_config.GPIO_PinNumber = GPIO_PIN_NO_7;
	led_2_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	led_2_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_2_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;
	led_2.GPIO_PinConfig = led_2_pin_config;
	led_2.pGPIOx = GPIOB_BASEADDR;
	GPIO_Init(&led_2);

	GPIO_Handle_t led_3;
	GPIO_PinConfig_t led_3_pin_config;
	led_3_pin_config.GPIO_PinMode = GPIO_MODE_OUT;
	led_3_pin_config.GPIO_PinNumber = GPIO_PIN_NO_9;
	led_3_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	led_3_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_3_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;
	led_3.GPIO_PinConfig = led_2_pin_config;
	led_3.pGPIOx = GPIOB_BASEADDR;
	GPIO_Init(&led_3);


	 for (;;){
		GPIO_WriteToOutputPin(&led_1, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(&led_2, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(&led_3, GPIO_PIN_SET);
		delay();
		GPIO_WriteToOutputPin(&led_1, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(&led_2, GPIO_PIN_RESET);
		GPIO_WriteToOutputPin(&led_3, GPIO_PIN_SET);
		delay();
	}
}
