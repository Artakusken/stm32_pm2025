#include "ssd1306.h"


uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8] = {0};


static void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 7200; i++) {
        __asm__("nop");
    }
}

static void WriteCommand(uint8_t cmd) {
    SSD1306_PORT->BSRR = GPIO_BSRR_BR0;
    SSD1306_PORT->BSRR = GPIO_BSRR_BR1;

    SPI1_Write(cmd);

    while (SPI1->SR & SPI_SR_BSY);

    SSD1306_PORT->BSRR = GPIO_BSRR_BS0;
    SSD1306_PORT->BSRR = GPIO_BSRR_BS1;
}

static void WriteData(uint8_t data) {
    SSD1306_PORT->BSRR = GPIO_BSRR_BR0;
    SSD1306_PORT->BSRR = GPIO_BSRR_BS1;

    SPI1_Write(data);

    while (SPI1->SR & SPI_SR_BSY);

    SSD1306_PORT->BSRR = GPIO_BSRR_BS0;
}

static void Reset(void) {
    SSD1306_PORT->BSRR = GPIO_BSRR_BR2;
    delay_ms(10);  // TODO try 100000
    SSD1306_PORT->BSRR = GPIO_BSRR_BS2;
    delay_ms(10);  // TODO remove this line
}

void SSD1306_Init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PB0 (DC)
    // PB1 (RST)
    SSD1306_PORT->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0 |
                           GPIO_CRL_CNF1 | GPIO_CRL_MODE1 |
                           GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
    SSD1306_PORT->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE1_1 | GPIO_CRL_MODE2_1);

    // PA5 (SCK)
    // PA7 (MOSI)
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5 | GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5_1 | GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7_1);

    SPI1_Init();

    SSD1306_PORT->BSRR = SSD1306_CS_PIN | SSD1306_DC_PIN | SSD1306_RES_PIN;

    Reset();

    WriteCommand(SSD1306_DISPLAYOFF);
    WriteCommand(SSD1306_SETDISPLAYCLOCKDIV);
    WriteCommand(0x80);
    WriteCommand(SSD1306_SETMULTIPLEX);
    WriteCommand(0x3F);
    WriteCommand(SSD1306_SETDISPLAYOFFSET);
    WriteCommand(0x00);
    WriteCommand(SSD1306_SETSTARTLINE | 0x00);
    WriteCommand(SSD1306_CHARGEPUMP);
    WriteCommand(0x14);
    WriteCommand(SSD1306_MEMORYMODE);
    WriteCommand(0x00);
    WriteCommand(SSD1306_SEGREMAP | 0x01);
    WriteCommand(SSD1306_COMSCANDEC);
    WriteCommand(SSD1306_SETCOMPINS);
    WriteCommand(0x12);
    WriteCommand(SSD1306_SETCONTRAST);
    WriteCommand(0xCF);
    WriteCommand(SSD1306_SETPRECHARGE);
    WriteCommand(0xF1);
    WriteCommand(SSD1306_SETVCOMDETECT);
    WriteCommand(0x40);
    WriteCommand(SSD1306_DISPLAYALLON_RESUME);
    WriteCommand(SSD1306_NORMALDISPLAY);
    WriteCommand(SSD1306_DISPLAYON);

    SSD1306_Clear();
}

void SSD1306_UpdateScreen(void) {
    WriteCommand(SSD1306_COLUMNADDR);
    WriteCommand(0);
    WriteCommand(63);

    WriteCommand(SSD1306_PAGEADDR);
    WriteCommand(0);
    WriteCommand(7);

    for(uint16_t i = 0; i < sizeof(SSD1306_Buffer); i++) {
        WriteData(SSD1306_Buffer[i]);
    }
}

void SSD1306_Clear(void) {
    for(uint16_t i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = 0x00;
    }
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;

    if (color) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}


void SSD1306_DrawPattern(void) {
    SSD1306_Clear();

    uint8_t center_x = SSD1306_WIDTH / 2;
    uint8_t center_y = SSD1306_HEIGHT / 2;

    // Рисуем квадраты
    for (uint8_t size = 3; size <= 30; size += 3) {
        // Рисуем квадрат
        for (int8_t i = -size; i <= size; i++) {
            SSD1306_DrawPixel(center_x + i, center_y - size, 1);
            SSD1306_DrawPixel(center_x + i, center_y + size, 1);
            SSD1306_DrawPixel(center_x - size, center_y + i, 1);
            SSD1306_DrawPixel(center_x + size, center_y + i, 1);
        }
    }


    for (uint8_t i = 0; i <= 30; i++) {
        // Главные диагонали
        SSD1306_DrawPixel(center_x + i, center_y + i, 1);
        SSD1306_DrawPixel(center_x - i, center_y - i, 1);
        SSD1306_DrawPixel(center_x + i, center_y - i, 1);
        SSD1306_DrawPixel(center_x - i, center_y + i, 1);
    }
}