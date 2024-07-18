#include "stm32f4xx.h"
void delay(int cycle)
{
	for (volatile int i = 0; i < cycle; i++)
		; // Simple delay
}
/* Function to send the command to LCD */
void Lcd_CmdWrite(char cmd)
{
	uint16_t holder = cmd;
	holder &= ~0x0f00;
	holder |= 0x0100;
	// GPIOA->ODR=cmd;    // Send the command to LCD
	// GPIOA->ODR&=~0x0f00;          // Select the Command Register by pulling RS LOW // Select the Write Operation  by pulling RW LOW
	GPIOA->ODR = holder; // Send a High-to-Low Pusle at Enable Pin
	delay(100);
	holder &= ~0x0100;
	GPIOA->ODR = holder; // Send a High-to-Low Pusle at Enable Pin
	delay(1000);
}

/* Function to send the Data to LCD */
void LCD_DataWrite(char dat)
{
	uint16_t holder = dat;
	holder &= ~0x0f00;
	holder |= 0x0500;
	// GPIOA->ODR =dat;	  // Send the data to LCD
	// GPIOA->ODR &=~0x0f00;           // Select the Write Operation  by pulling RW LOW
	// GPIOA->ODR |=0x0500;            // Select the Command Register by pulling RS high   // Send a High-to-Low Pusle at Enable Pin
	GPIOA->ODR = holder;
	delay(100);
	holder &= ~0x0100;
	GPIOA->ODR = holder; // Send a High-to-Low Pusle at Enable Pin
	delay(100);
}

void GPIO_Init(void)
{
	// Enable clock for GPIOA and GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

	// Configure GPIOA pins
	GPIOA->MODER &= ~0xFFFFFFFF;   // Clear mode settings for pins 0-7
	GPIOA->MODER |= 0x55555555;	   // Set pins 0-7 as output
	GPIOA->OTYPER &= ~0xFFFF;	   // Set output type to push-pull for pins 0-7
	GPIOA->OSPEEDR &= ~0xFFFFFFFF; // Clear speed settings for pins 0-7
	GPIOA->OSPEEDR |= 0x55555555;  // Set low speed for pins 0-7

	// Configure GPIOC pins
	GPIOC->MODER &= ~0xFFFFFFFF;   // Clear mode settings for pins 0-7
	GPIOC->MODER |= 0x00000015;	   // Set pins 0, 1, and 2 as output
	GPIOC->OTYPER &= ~0x0007;	   // Set output type to push-pull for pins 0-2
	GPIOC->OSPEEDR &= ~0x0000003F; // Clear speed settings for pins 0-2
	GPIOC->OSPEEDR |= 0x00000015;  // Set low speed for pins 0-2
	GPIOC->PUPDR &= ~0XFFFFFC00;
	GPIOC->PUPDR |= 0XAAAAAA80;
}

uint16_t ReadUpperBitGPIOC(void)
{
	uint16_t inputStates;
	// Read the upper half of GPIOA (pins 8 to 15)
	// Shift right by 8 to align with the least significant bits
	inputStates = (GPIOC->IDR >> 3) & 0x000F;

	return inputStates;
}
int prime(int num)
{
	if (num <= 1)
		return 0; // 0 and 1 are not prime numbers
	if (num <= 3)
		return 1; // 2 and 3 are prime numbers
	if (num % 2 == 0 || num % 3 == 0)
		return 0; // eliminate multiples of 2 and 3 quickly
	int i = 5;
	while (i * i <= num)
	{
		if (num % i == 0 || num % (i + 2) == 0)
			return 0;
		i += 6;
	}
	return 1;
}

int pali(int num)
{
	int holder = 0;
	int numHolder = num;
	while (num > 0)
	{
		holder = holder * 10;
		holder = holder + (num % 10);
		num = num / 10;
	}
	if (numHolder == holder)
		return 0;
	return 1;
}

void wait_delay()
{
	while (ReadUpperBitGPIOC() != 0)
	{
	}
}

uint16_t readData()
{
	uint16_t holder = 0;
	uint16_t arr[3][2] = {{1, 1}, {2, 2}, {4, 3}};
	int mode = 0;
	while (1)
	{
		GPIOC->ODR = arr[mode][0];
		delay(10);
		holder = arr[mode][1];
		uint16_t inputStates;
		inputStates = ReadUpperBitGPIOC();
		if (inputStates == 1)
		{
			holder += 0;
			break;
		}
		else if (inputStates == 2)
		{
			holder += 3;
			break;
		}
		else if (inputStates == 4)
		{
			holder += 6;
			break;
		}
		else if (inputStates == 8)
		{
			if (holder == 2)
				holder = 0;
			else
			{
				holder = 10;
			}
			break;
		}

		mode = (mode + 1) % 3;
	}
	wait_delay();
	return holder;
}

void screanInit()
{
	Lcd_CmdWrite(0x38); // enable 5x7 mode for chars
	Lcd_CmdWrite(0x0E); // Display OFF, Cursor ON
	Lcd_CmdWrite(0x01); // Clear Display
	Lcd_CmdWrite(0x80); // Move the cursor to beginning of first line
}
void writeChars(char *arr, int i)
{
	for (int j = 0; j < i; j++)
	{
		LCD_DataWrite(arr[j]);
		delay(1000);
	}
}

int main(void)
{
	GPIO_Init();
	char isPrime[6] = {"prime&"};
	char NotPrime[7] = {"Nprime&"};
	char isPali[4] = {"pali"};
	char NotPali[5] = {"Npali"};
	while (1)
	{
		screanInit();
		char a[3] = {"no:"};
		writeChars(a, 3);
		uint16_t input = readData();
		int holder = 0;
		while (input != 10)
		{
			char testChar = '0';
			testChar += input;
			LCD_DataWrite(testChar);
			holder = (holder * 10) + input;
			input = readData();
		}
		Lcd_CmdWrite(0XC0);
		if (prime(holder))
		{
			writeChars(isPrime, 6);
		}
		else
		{
			writeChars(NotPrime, 7);
		}
		if (!pali(holder))
		{
			writeChars(isPali, 4);
		}
		else
		{
			writeChars(NotPali, 5);
		}
		input = 0;
		while (input != 10)
		{
			input = readData();
		}
	}
}
