#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"
#include "stdbool.h"

volatile int TIMER = 0;
volatile int BaseFreq = 3000;
volatile int BaseFreqShow = 3000;

volatile int select_flag = 0;

int Duty_Cycle = 50;
int step = 10;

void TIM3_IRQHandler(void)
{
	if (TIM3->SR & TIM_SR_UIF)
	{
		TIM3->SR &= ~TIM_SR_UIF;
		TIMER++;
	}
}

void show_clock(void)
{
	char timerStr[20]; // Buffer to hold the resulting string
	snprintf(timerStr, sizeof(timerStr), "time: %d", TIMER);

	// Print on the first line
	LCD_Puts(0, 0, timerStr);
}

void Configure_TIM2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 clock
	TIM2->PSC = 1600 - 1;				// Prescaler
	TIM2->ARR = BaseFreq - 1;			// Auto-reload value
	TIM2->CNT = 0;						// Reset counter
	TIM2->DIER |= TIM_DIER_UIE;			// Enable update interrupt
	TIM2->CR1 |= TIM_CR1_CEN;			// Enable timer
	NVIC_EnableIRQ(TIM2_IRQn);			// Enable TIM2 interrupt in NVIC
}

void TIM2_IRQHandler(void)
{
	if (TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF; // Clear update interrupt flag
	}
}

void Configure_ADC(void)
{
	// Initializing ADC
	RCC->APB2ENR |= (1 << 8); // Enable ADC Clock
	GPIOA->MODER |= 3 << 0;	  // PA1: Analog mode
	ADC1->SQR3 = 0;			  // Conversion sequence starts at CH1
	ADC1->SQR1 = 0;			  // Conversion sequence length: 1
	ADC1->CR2 |= (1 << 0);	  // Enable ADC
}

void Update_Freq(bool up)
{
	if (up)
	{
		BaseFreq -= 100;
		BaseFreqShow += 100;
	}
	else if (BaseFreq > 100)
	{
		BaseFreq += 100;
		BaseFreqShow -= 100;
	}
	TIM2->ARR = BaseFreq - 1; // Auto-reload value
}

void show_freq(void)
{
	char str[20]; // Buffer to hold the resulting string
	snprintf(str, sizeof(str), "f:%d", BaseFreqShow);

	LCD_Puts(0, 1, "     "); // Clear the LCD line before writing
	LCD_Puts(0, 1, str);	 // Print on the first line
}

void ADC_CONTROLLER(void)
{
	ADC1->CR2 |= (1 << 30); // SWSTART: start a conversion
	while ((ADC1->SR & (1 << 1)) == 0)
	{
	} // wait for completing conversion
	unsigned int ADC_Result = ADC1->DR & 0x0FFF;

	// char str[20];
	// sprintf(str, "%d", ADC_Result);
	// LCD_Clear();
	// LCD_Puts(0, 0, " ");
	// LCD_Puts(0, 0, str);
	// ;
	// for (int i = 0; i < 100000; i++)

	if ((ADC_Result > 0 && ADC_Result < 200) && (select_flag))
	{
		// increase dc (right)
		Duty_Cycle += step;
		if (Duty_Cycle >= 100)
		{
			Duty_Cycle = 100;
		}
		TIM2->CCR1 = (BaseFreq * Duty_Cycle) / 100;

		for (int i = 0; i < 100000; i++)
			;
	}
	else if ((ADC_Result > 2500 && ADC_Result < 3100) && (select_flag))
	{
		// decrease dc (left)
		Duty_Cycle -= step;
		if (Duty_Cycle <= 0)
		{
			Duty_Cycle = 0;
		}
		TIM2->CCR1 = (BaseFreq * Duty_Cycle) / 100;
		for (int i = 0; i < 100000; i++)
			;
	}
 	else if ((ADC_Result > 500 && ADC_Result < 900) && (!select_flag))
	{
		// increase period (up)
		Update_Freq(true);
		show_freq();
		for (int i = 0; i < 100000; i++)
			;
	}
	else if ((ADC_Result > 1500 && ADC_Result < 2100) && (!select_flag))
	{
		// decrease period (down)
		Update_Freq(false);
		show_freq();
		for (int i = 0; i < 100000; i++)
			;
	}
	else if (ADC_Result > 4090 && ADC_Result < 4100)
	{
		// (select)
		select_flag = !select_flag;
	}
	char s[20];
	sprintf(s, "duty: %d", Duty_Cycle);
	LCD_Puts(8, 1, "        ");
	LCD_Puts(8, 1, s);
}

void Configure_PWM(void)
{
	// GPIO Configuration for TIM2 Channel 1 on PA5
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 clock
	RCC->AHB1ENR |= (1 << 0);			// Enable GPIOA clock
	GPIOA->MODER |= (2 << 10);			// PA5: Alternate function mode
	GPIOA->AFR[0] |= (1 << 20);			// PA5: AF1 (TIM2_CH1)

	TIM2->PSC = 1600 - 1;	  // Prescaler
	TIM2->ARR = BaseFreq - 1; // Auto-reload value
	TIM2->CCMR1 &= ~(3 << 0); // CC1S: CC1 channel output
	TIM2->CCMR1 &= ~(7 << 4); // Clear OC1M field
	TIM2->CCMR1 |= (6 << 4);  // OC1M: PWM mode 1
	TIM2->CCR1 = 0;			  // Set compare value
	TIM2->CCER |= (1 << 0);	  // CC1E: Enable Compare 1 output
	TIM2->CR1 |= TIM_CR1_CEN; // Enable counter (CEN)

	TIM2->CCR1 = (BaseFreq * Duty_Cycle) / 100;
}

int main(void)
{
	SystemCoreClockUpdate();
	HAL_Init();

	// Enable GPIO clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	// LCD Initialization
	LCD_Init();

	// Configure GPIOA PA5 as alternate function (PWM)
	RCC->AHB1ENR |= (1 << 0);
	GPIOA->MODER &= ~(3 << 10); // clear PA5 pin mode
	GPIOA->MODER |= (2 << 10);	// set PA5 to alternate function

	// Configure TIM3 for timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 1600 - 1;
	TIM3->ARR = 10 - 1;
	TIM3->CNT = 0;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM3_IRQn);

	// Configure TIM2 for PWM
	Configure_TIM2();

	// Configure ADC
	Configure_ADC();

	// Configure PWM on TIM2
	Configure_PWM();

	show_freq();
	char s[20];
	sprintf(s, "%d", Duty_Cycle);
	LCD_Puts(8, 1, " ");
	LCD_Puts(8, 1, s);

	while (1)
	{
		show_clock();
		ADC_CONTROLLER();
	}
	return 0;
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}