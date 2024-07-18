#include <stdint.h>
#include <stdio.h>

#include "stm32f4xx.h"
int main(void){

    RCC->AHB1ENR = ((1 << 0) | (1 << 2));
    GPIOC->MODER |= ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10) | (1 << 12));
    GPIOA->MODER &= ((1 << 10) & (1 << 12));

    while (1)
        {
            if ((GPIOA->IDR & (1 << 5)) == 0){
                GPIOC->ODR = 0xC0;
            }

        }
}