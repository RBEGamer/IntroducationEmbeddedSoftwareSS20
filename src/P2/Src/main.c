
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif



#include <stddef.h>
#include <stdint.h>


#include "stm32f407xx_gpio_driver.h"



GPIO_Handle_t led_1;
GPIO_Handle_t led_2;
GPIO_Handle_t led_3;
GPIO_Handle_t user_button;
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
	GPIO_WriteToOutputPin(&led_1, GPIO_PIN_SET);
	GPIO_WriteToOutputPin(&led_2, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(&led_3, GPIO_PIN_RESET);
	delay_s(7);
	GPIO_IRQHandling(GPIO_PIN_NO_0); //CLEAR PA FOR USER_BUTTON PA0
}


int main(void)
{


	
	GPIO_PinConfig_t led_1_pin_config;
	led_1_pin_config.GPIO_PinMode = GPIO_MODE_OUT;
	led_1_pin_config.GPIO_PinNumber = GPIO_PIN_NO_5;
	led_1_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	led_1_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_1_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;
	led_1.GPIO_PinConfig = led_1_pin_config;
	led_1.pGPIOx = GPIOB_BASEADDR;
	GPIO_Init(&led_1);

	
	GPIO_PinConfig_t led_2_pin_config;
	led_2_pin_config.GPIO_PinMode = GPIO_MODE_OUT;
	led_2_pin_config.GPIO_PinNumber = GPIO_PIN_NO_7;
	led_2_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	led_2_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_2_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;
	led_2.GPIO_PinConfig = led_2_pin_config;
	led_2.pGPIOx = GPIOB_BASEADDR;
	GPIO_Init(&led_2);

	
	GPIO_PinConfig_t led_3_pin_config;
	led_3_pin_config.GPIO_PinMode = GPIO_MODE_OUT;
	led_3_pin_config.GPIO_PinNumber = GPIO_PIN_NO_9;
	led_3_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	led_3_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_3_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;
	led_3.GPIO_PinConfig = led_3_pin_config;
	led_3.pGPIOx = GPIOB_BASEADDR;
	GPIO_Init(&led_3);


	
	GPIO_PinConfig_t user_button_pin_config;
	user_button_pin_config.GPIO_PinMode = GPIO_MODE_IT_FT;
	user_button_pin_config.GPIO_PinNumber = GPIO_PIN_NO_0;
	user_button_pin_config.GPIO_PinOPType = GPIO_OP_TYPE_PP; //?
	user_button_pin_config.GPIO_PinPuPdControl = GPIO_PIN_PU;
	user_button_pin_config.GPIO_PinSpeed = GPIO_SPEED_LOW;
	user_button.GPIO_PinConfig = user_button_pin_config;
	user_button.pGPIOx = GPIOA_BASEADDR;

	GPIO_Init(&user_button);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0,ENABLE);
	
	 for (;;){
		GPIO_WriteToOutputPin(&led_1, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(&led_2, GPIO_PIN_RESET);
		GPIO_WriteToOutputPin(&led_3, GPIO_PIN_RESET);
		delay_s(5);
		GPIO_WriteToOutputPin(&led_1, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(&led_2, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(&led_3, GPIO_PIN_RESET);
		delay_s(5);
		GPIO_WriteToOutputPin(&led_1, GPIO_PIN_RESET);
		GPIO_WriteToOutputPin(&led_2, GPIO_PIN_RESET);
		GPIO_WriteToOutputPin(&led_3, GPIO_PIN_SET);
		delay_s(5);
		GPIO_WriteToOutputPin(&led_1, GPIO_PIN_RESET);
		GPIO_WriteToOutputPin(&led_2, GPIO_PIN_RESET);
		GPIO_WriteToOutputPin(&led_3, GPIO_PIN_RESET);
		delay_s(5);
	}
}
