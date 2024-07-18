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

void print_persian_style(void);

void Configure_PC13(void);

volatile unsigned char Counter = 1;

void delay(int cycle)
{
	for (volatile int i = 0; i < cycle; i++)
		; // Simple delay
}

// persian words

// ک میانی
static uint8_t ke_middle[] = {0x01, 0x02, 0x04, 0x02, 0x1F, 0x00, 0x00, 0x00};
// ی میانی
static uint8_t ye_middle[] = {
	0x00,
	0x00,
	0x01,
	0x01,
	0x1F,
	0x00,
	0x0A,
	0x00}

// ر
static uint8_t re[] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08};
// م پایانی
static uint8_t mim_end[] = {0x00, 0x00, 0x07, 0x05, 0x0F, 0x08, 0x08, 0x08};
// ت میانی
static uint8_t te_middle[] = {0x00, 0x0A, 0x00, 0x01, 0x1F, 0x00, 0x00, 0x00};
// و
static uint8_t vav[] = {0x00, 0x00, 0x07, 0x05, 0x07, 0x02, 0x04, 0x08};
// ز
static uint8_t ze[] = {0x00, 0x00, 0x02, 0x00, 0x01, 0x02, 0x04, 0x08};
// آ
static uint8_t alef_hat[] = {0x01, 0x1F, 0x10, 0x04, 0x04, 0x04, 0x00, 0x00};
// ا
static uint8_t alef[] = {0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00};
// م میانی
static uint8_t mim_middle[] = {0x00, 0x00, 0x07, 0x05, 0x1F, 0x00, 0x00, 0x00};
// ی پایانی
static uint8_t ye_end[] = {0x00, 0x00, 0x07, 0x04, 0x17, 0x11, 0x1F, 0x00};
// ش پایانی
static uint8_t she_end[] = {0x02, 0x05, 0x00, 0x05, 0x17, 0x14, 0x14, 0x1C};

int main(void)
{
	SystemCoreClockUpdate();
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	LCD_Init();
	show_freq_clock();
	print_persian_style();
	delay(250);

	return 0;
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

void print_persian_style(void)
{
	int sc = 0;
	LCD_CreateChar(0, alef_hat);
	LCD_CreateChar(1, ze);
	LCD_CreateChar(2, re);
	LCD_CreateChar(3, ye_middle_2);

	LCD_PutCustom(1, 1, 1);
	LCD_PutCustom(2, 1, 3);
	LCD_PutCustom(3, 1, 2);
	LCD_PutCustom(4, 1, 1);
	LCD_PutCustom(5, 1, 0);

	// int l = 6;
	// while (true)
	// {
	// 	LCD_PutCustom((1 + sc) % l, 1, 1);
	// 	LCD_PutCustom((2 + sc) % l, 1, 3);
	// 	LCD_PutCustom((3 + sc) % l, 1, 2);
	// 	LCD_PutCustom((4 + sc) % l, 1, 1);
	// 	LCD_PutCustom((5 + sc) % l, 1, 0);
	// 	HAL_Delay(250);
	// 	show_freq_clock();
	// 	sc = (sc + 1) % l;
	// }
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}