#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"

#include "stdbool.h"

#define LCD_LENGTH 16

// char message[] = "Hello!";
char message[] = "vay vay vay vay vay vay vay vay vay vay!";
char messageMHz[] = " MHz"; // Adding a space for readability

volatile int BaseClock = 16;

void Set_Clock(bool increase);
void show_clock(void);
void show_freq_clock(void);
void Configure_PA1(void);
void Configure_PA0(void);
void PrintSystemClock();

void Configure_PC13(void);

volatile unsigned char Counter = 1;

void delay(int cycle)
{
	for (volatile int i = 0; i < cycle; i++)
		; // Simple delay
}

int main(void)
{
	SystemCoreClockUpdate();
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	LCD_Init();
	show_freq_clock();

	Configure_PA0();
	Configure_PA1();

	size_t length = sizeof(message);

	if (length <= LCD_LENGTH)
	{
		// Print message on the second line
		LCD_Puts(0, 1, message);
	}
	else
	{
		int first_index = 0;
		char temp[LCD_LENGTH];
		while (1)
		{
			while (first_index + LCD_LENGTH - 2 != length)
			{
				memcpy(temp, &message[first_index], LCD_LENGTH - 1);
				temp[LCD_LENGTH - 1] = '\0';
				LCD_Puts(0, 1, temp);
				HAL_Delay(250);
				++first_index;
				show_freq_clock();
			}
			first_index = 0;
		}
	}

	return 0;
}

void Configure_PA0(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	GPIOA->MODER &= ~(3 << 0);

	// SYSCFG->EXTICR[0] &= (0x0 << 0); // Clear EXTI0 field (bits 0-3)
	SYSCFG->EXTICR[0] &= SYSCFG_EXTICR1_EXTI0_PA;
	// Connect

	EXTI->FTSR |= (1 << 0); // Falling trigger
	EXTI->IMR |= (1 << 0);	// Clear mask
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void Configure_PA1(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	GPIOA->MODER &= ~(3 << 2);

	// SYSCFG->EXTICR[0] &= (0x0 << 4); // Clear EXTI1 field (bits 4-7)

	SYSCFG->EXTICR[0] &= SYSCFG_EXTICR1_EXTI1_PA;

	EXTI->FTSR |= (1 << 1); // Falling trigger
	EXTI->IMR |= (1 << 1);	// Clear mask

	NVIC_EnableIRQ(EXTI1_IRQn);
}

/* Set interrupt handlers */
/* Handle PA0 interrupt */
void EXTI0_IRQHandler(void)
{

	Set_Clock(true);
	EXTI->PR |= (1 << 0);			  // Clear pending interrupt
	NVIC_ClearPendingIRQ(EXTI0_IRQn); // Not necessary if using debouncing circuitry
}

/* Set interrupt handlers */
/* Handle PA1 interrupt */
void EXTI1_IRQHandler(void)
{

	Set_Clock(false);
	EXTI->PR |= (1 << 1);			  // Clear pending interrupt
	NVIC_ClearPendingIRQ(EXTI1_IRQn); // Not necessary if using debouncing circuitry
}

void Set_Clock(bool increase)
{

	BaseClock += increase ? 1 : -1;
	RCC->PLLCFGR = (16 << RCC_PLLCFGR_PLLM_Pos) | (BaseClock * 4 << RCC_PLLCFGR_PLLN_Pos) | (1 << RCC_PLLCFGR_PLLP_Pos);

	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0)
		;

	RCC->CFGR |= (RCC_CFGR_SW_PLL | (4 << RCC_CFGR_PPRE1_Pos));

	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
		;

	SystemCoreClockUpdate();
}

void show_clock(void)
{
	char systemClockFreqStr[20]; // Buffer to hold the resulting string

	uint32_t systemClockFreq = HAL_RCC_GetHCLKFreq();
	systemClockFreq /= 1000000; // Convert to MHz

	// Format the system clock frequency and concatenate with " MHz"
	snprintf(systemClockFreqStr, sizeof(systemClockFreqStr), "%lu", systemClockFreq);
	strncat(systemClockFreqStr, messageMHz, sizeof(systemClockFreqStr) - strlen(systemClockFreqStr) - 1);

	// Clear the LCD before writing
	// LCD_Clear();

	// Print frequency on the first line
	LCD_Puts(0, 1, systemClockFreqStr);
}

void Configure_PC13(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	GPIOA->MODER |= 1 << 10;	   // PA5: output (LED)
	GPIOC->MODER &= ~(3 << 26);	   // PC13: input (Push button)
	SYSCFG->EXTICR[3] |= (2 << 4); // EXTI13: PC13
	EXTI->FTSR |= (1 << 13);	   // Falling trigger
	EXTI->IMR |= (1 << 13);		   // Clear mask
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler()
{
	Counter = Counter + 1;
	EXTI->PR |= (1 << 13);				  // Clear pending interrupt
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn); // Not necessary if using debouncing circuitry
}

void show_freq_clock(void)
{
	size_t length = sizeof(message);

	char systemClockFreqStr[20]; // Buffer to hold the resulting string

	uint32_t systemClockFreq = HAL_RCC_GetHCLKFreq();
	systemClockFreq /= 1000000; // Convert to MHz

	// Format the system clock frequency and concatenate with " MHz"
	snprintf(systemClockFreqStr, sizeof(systemClockFreqStr), "%lu", systemClockFreq);
	strncat(systemClockFreqStr, messageMHz, sizeof(systemClockFreqStr) - strlen(systemClockFreqStr) - 1);

	// Clear the LCD before writing
	LCD_Clear();

	// Print frequency on the first line
	LCD_Puts(0, 0, systemClockFreqStr);

	delay(100);
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}