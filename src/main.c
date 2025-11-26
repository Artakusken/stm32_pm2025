#include <stm32f10x.h>

#include "ssd1306.h"

int main(void) {
    RCC->CFGR = 0x00000000;
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    SSD1306_Init();

    SSD1306_DrawPattern();

    SSD1306_UpdateScreen();

    while(1) {

    }
}