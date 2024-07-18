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

int input()
{
    if ((GPIOC->IDR & (1 << 0)) == 0)
    {
        if ((GPIOC->IDR & (1 << 3)) == 0)
        {
            return 1;
        }
        else if ((GPIOC->IDR & (1 << 4)) == 0)
        {
            return 4;
        }
        else if ((GPIOC->IDR & (1 << 5)) == 0)
        {
            return 7;
        }
        else if ((GPIOC->IDR & (1 << 6)) == 0)
        {
            return 10; // star
        }
    }
    else if ((GPIOC->IDR & (1 << 1)) == 0)
    {
        if ((GPIOC->IDR & (1 << 3)) == 0)
        {
            return 2;
        }
        else if ((GPIOC->IDR & (1 << 4)) == 0)
        {
            return 5;
        }
        else if ((GPIOC->IDR & (1 << 5)) == 0)
        {
            return 8;
        }
        else if ((GPIOC->IDR & (1 << 6)) == 0)
        {
            return 0;
        }
    }
    else if ((GPIOC->IDR & (1 << 2)) == 0)
    {
        if ((GPIOC->IDR & (1 << 3)) == 0)
        {
            return 3;
        }
        else if ((GPIOC->IDR & (1 << 4)) == 0)
        {
            return 6;
        }
        else if ((GPIOC->IDR & (1 << 5)) == 0)
        {
            return 9;
        }
        else if ((GPIOC->IDR & (1 << 6)) == 0)
        {
            return 11; // hashtag
        }
    }
}


void show_number(int number)
{
    // int sum = 0;
    // while (number > 0)
    // {
    //     int r = number % 10;
    //     sum = (sum * 10) + r;
    //     number = number / 10;
    // }


        if (number == 0)
        {
            GPIOB->ODR = ~seg[0];
        }
        else if (number == 1)
        {
            GPIOB->ODR = ~seg[1];
        }
        else if (number == 2)
        {
            GPIOB->ODR = ~seg[2];
        }
        else if (number == 3)
        {
            GPIOB->ODR = ~seg[3];
        }
        else if (number == 4)
        {
            GPIOB->ODR = ~seg[4];
        }
        else if (number == 5)
        {
            GPIOB->ODR = ~seg[5];
        }
        else if (number == 6)
        {
            GPIOB->ODR = ~seg[6];
        }
        else if (number == 7)
        {
            GPIOB->ODR = ~seg[7];
        }
        else if (number == 8)
        {
            GPIOB->ODR = ~seg[8];
        }
        else if (number == 9)
        {
            GPIOB->ODR = ~seg[9];
        }

        delay_inside();
        GPIOB->ODR = ~0;
}


void put_number(){
        show_number(input());
        delay_inside();
        GPIOB->ODR = ~0;
}


int main(void)
{

    int prime_pali[20] = {};
    RCC->AHB1ENR = ((1 << 0) | (1 << 2));
    GPIOB->MODER |= ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10) | (1 << 12));
    GPIOC->MODER |= ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10) | (1 << 12));



    while (1)
    {
        
        while (1)
        {
        show_number(input());
        delay_inside();
        GPIOB->ODR = ~0;

        }
        for (;;)
            ;
    }
}