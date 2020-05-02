
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif



#include <stddef.h>
#include <stdint.h>


#include "stm32f407xx_gpio_driver.h"



void delay(){

	for (uint32_t i = 0; i < 500000; ++i) {
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
	led_1_pin_config.GPIO_PinNumber = GPIO_PIN_NO_0;
	led_1_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	led_1_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_1_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;

	led_1.GPIO_PinConfig = led_1_pin_config;
	led_1.pGPIOx = GPIOH_BASEADDR;

	GPIO_Init(&led_1);

	//USER BUTTON PA0
	GPIO_Handle_t user_button;
	GPIO_PinConfig_t user_button_pin_config;

	user_button_pin_config.GPIO_PinMode = GPIO_MODE_IT_FT;
	user_button_pin_config.GPIO_PinNumber = GPIO_PIN_NO_0;
	user_button_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	user_button_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	user_button_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;

	user_button.GPIO_PinConfig = user_button_pin_config;
	user_button.pGPIOx = GPIOA_BASEADDR;
	GPIO_Init(&user_button); //INIT GPIO	
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0,ENABLE); //INIT IRQ


	 for (;;){
		GPIO_WriteToOutputPin(&led_1, GPIO_PIN_RESET);
		delay();
		GPIO_WriteToOutputPin(&led_1, GPIO_PIN_SET);
		delay();
	}
}
