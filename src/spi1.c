#include <stm32f10x.h>

#include "spi1.h"

void SPI1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA |
                 SPI_CR1_MSTR | SPI_CR1_BR |
                 SPI_CR1_SSM | SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data) {
    while(!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
}

uint8_t SPI1_Read(void) {
    SPI1->DR = 0;
    while(!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}