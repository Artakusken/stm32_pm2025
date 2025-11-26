#ifndef STM32_TEMPLATE_SPI1_H
#define STM32_TEMPLATE_SPI1_H

#include <stdint.h>

void SPI1_Init(void);
void SPI1_Write(uint8_t data);
uint8_t SPI1_Read(void);

#endif //STM32_TEMPLATE_SPI1_H
