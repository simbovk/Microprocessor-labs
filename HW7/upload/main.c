#include <stdint.h>
#include <stdio.h>

#include "stm32f4xx.h"

int seg[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
void delay(void)
{
    for (int i = 0; i < 2000000; i++)
        ;
}

void delay_inside(void)
{
    for (int i = 0; i < 500000; i++)
        ;
}

int prime(int number)
{
    int flag = 0;
    if (number == 0 || number == 1)
        flag = 1;

    for (int i = 2; i <= number / 2; ++i)
    {
        if (number % i == 0)
        {
            flag = 1;
            break;
        }
    }

    if (flag == 0)
        return 1;
    else
        return 0;
}

int palindrome(int number)
{
    int r, sum = 0, temp;
    temp = number;
    while (number > 0)
    {
        r = number % 10;
        sum = (sum * 10) + r;
        number = number / 10;
    }
    if (temp == sum)
        return 1;
    else
        return 0;
}

void show_number(int number)
{

    while (number > 0)
    {
        int num = number % 10;
        number /= 10;

        if (num == 0)
        {
            GPIOC->ODR = ~seg[0];
        }
        else if (num == 1)
        {
            GPIOC->ODR = ~seg[1];
        }
        else if (num == 2)
        {
            GPIOC->ODR = ~seg[2];
        }
        else if (num == 3)
        {
            GPIOC->ODR = ~seg[3];
        }
        else if (num == 4)
        {
            GPIOC->ODR = ~seg[4];
        }
        else if (num == 5)
        {
            GPIOC->ODR = ~seg[5];
        }
        else if (num == 6)
        {
            GPIOC->ODR = ~seg[6];
        }
        else if (num == 7)
        {
            GPIOC->ODR = ~seg[7];
        }
        else if (num == 8)
        {
            GPIOC->ODR = ~seg[8];
        }
        else if (num == 9)
        {
            GPIOC->ODR = ~seg[9];
        }

        delay_inside();
        GPIOC->ODR = ~0;
    }
}

int main(void)
{

    int prime_pali[20] = {};
    RCC->AHB1ENR = ((1 << 0) | (1 << 2));
    GPIOC->MODER |= ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10) | (1 << 12));
    GPIOA->MODER &= ((1 << 10) & (1 << 12));

    while (1)
    {
        int i = 0;
        int num = 1;

        for (int i = 0; i < 20; i++)
        {
            while (1)
            {
                num++;
                if (prime(num) == 1 && palindrome(num) == 1)
                {
                    prime_pali[i] = num;
                    break;
                }
            }
        }
        while (1)
        {
            int number = prime_pali[i];
            show_number(number);
            delay();
            if ((GPIOA->IDR & (1 << 5)) == 0)
            {
                if(i > 0){
                    number = prime_pali[--i];
                    show_number(number);
                    delay();
                }
            }
            else if ((GPIOA->IDR & (1 << 6)) == 0)
            {
                if(i < 100){
                    number = prime_pali[++i];
                    show_number(number);
                    delay();
                }
            }
        }
        for (;;)
            ;
    }
}