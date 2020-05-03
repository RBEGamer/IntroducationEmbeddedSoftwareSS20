/*
 * stm32f407xx_gpio_driver.c
 * Vorlage fürs Praktikum 
 */


#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}else if (pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
	}
	else
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
		else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
		else if (pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}
	}

}




// Implementieren Sie die weiteren Schritte. Denken Sie daran die Einträge eines Registers zurückzusetzen, bevor Sie einzelne Bits setzen
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	 //Anschalten der Clock
	 GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);


	 //Konfigurieren des OUTPUT-Modus
	 if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	 {
		 //RESET MODE REG =  INPUT
		 pGPIOHandle->pGPIOx->MODE_REG &= ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2)) & ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2) + 1);

		 if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_OUT)
			 {
			 pGPIOHandle->pGPIOx->MODE_REG |= (1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber*2));
			 }


	 }else{ // Interrupt Modus kommt im zweiten Teil
	    // IRQ-Modus
		// 1. Konfiguration für Trigger auf fallenende, steigende oder beide Flanken
		

		//2. Konfiguration des entsprechenden GPIO-Ports in SYSCFG_EXTICR
		//3  Aktivieren des EXTI Interrupts handling in IMR-Register
		 
	 }


	 //Konfigurieren des Output-Speeds
	 //RESET SPEED REGISTER TO DEFAULT
	 pGPIOHandle->pGPIOx->OUTPUT_SPEED &= ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2)) & ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2)+1);

	 if (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed & 0b01){
		 pGPIOHandle->pGPIOx->OUTPUT_SPEED |= (1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2)); //SPEED LOW BIT
	 }
	 if (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed & 0b10)
	 {
		 pGPIOHandle->pGPIOx->OUTPUT_SPEED |= (1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2)+1); //SPEED HIGH BIT
	 }

	 //Konfigurieren des Pull-up/down Settings
	 //RESET PULL_UP_DOWN
	 pGPIOHandle->pGPIOx->PULL_UP_DOWN &= ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2)) & ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2) + 1);

	 if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl & 0b01)
	 {
		 pGPIOHandle->pGPIOx->PULL_UP_DOWN |= (1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2)); //PUD LOW BIT
	 }
	 if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl & 0b10)
	 {
		 pGPIOHandle->pGPIOx->PULL_UP_DOWN |= (1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2) + 1); //PUD HIGH BIT
	 }


	 
}


void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}else if (pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}else if (pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}else if (pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}else if (pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}else if (pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}else if (pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}

}

// Implementieren Sie die Funktionalität
uint8_t GPIO_ReadFromInputPin(GPIO_Handle_t *pGPIO_Handle){
	return pGPIO_Handle->pGPIOx->INPUT_DATA_REG & (1<<pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
}

// Implementieren Sie die Funktionalität
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	return (uint16_t)(pGPIOx->INPUT_DATA_REG & 0xFFFF); //RETURN ONLY THE LOWER 16BIT
}

// Implementieren Sie die Funktionalität
void GPIO_WriteToOutputPin(GPIO_Handle_t *pGPIO_Handle, uint8_t Value)
{
	if (Value){
		pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG |= (1 << (pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber));
	}else{
		pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG &= ~(1 << (pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber));
	}
}

// Implementieren Sie die Funktionalität
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->OUTPUT_DATA_REG =Value;
}

// Implementieren Sie die Funktionalität
void GPIO_ToggleOutputPin(GPIO_Handle_t *pGPIO_Handle)
{
	if(GPIO_ReadFromInputPin(pGPIO_Handle)){
		GPIO_WriteToOutputPin(pGPIO_Handle, GPIO_PIN_RESET);
	}else{
		GPIO_WriteToOutputPin(pGPIO_Handle, GPIO_PIN_SET);
	}
}

// ####################################### Teil IRQ ############################################################
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	// Hinweis: Bedenken Sie welche ISER und ICER Register des Prozessors in dem verwendeten Mikrocontroller benutzt werden können.
	// Nicht alle Interrupts sind im Mikrocontroller aktiv. Überprüfen sie dazu das Handbuch (Reference Manual) des Mikrocontrollers.
	if(EnorDi == ENABLE)
	{
		//ToDo: Programmieren der entsprechenden ISERx register
	}else
	{
		//ToDo: Programmieren der entsprechenden ICERx register
	}

}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	// Abfrage und zurücksetzen des EXTI-PR bits
	if(EXTI->PR & ( 1 << PinNumber))
	{
		//clear
		EXTI->PR |= ( 1 << PinNumber);
	}

}

