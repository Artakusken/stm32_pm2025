#include <stdint.h>
#include <stm32f10x.h>
#include <stdbool.h>

// Точная задержка в микросекундах (при тактовой частоте 72 МГц)
void delay_us(uint32_t us) {
    us *= 72;
    for(volatile uint32_t i = 0; i < us; i++) {
        __asm volatile ("nop");
    }
}

// Задержка в миллисекундах
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        delay_us(1000);
    }
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        GPIOC->ODR ^= (1 << 13);  // Переключение светодиода
        TIM2->SR &= ~TIM_SR_UIF;  // Сброс флага прерывания
    }
}


void toggleSpeed(void) {
    static uint8_t direction = 0;  // 0 - замедление, 1 - ускорение
    static uint8_t currentSpeed = 0;

    // Остановка таймера для изменения параметров
    TIM2->CR1 &= ~TIM_CR1_CEN;

    // Значения предделителя
    const uint16_t prescalerValues[] = {
            1023,
            2046,
            4092,
            8184,
            16368,
            32736,
            65472
    };

    if (direction == 0) {  // Замедление
        if (currentSpeed < 6) {
            currentSpeed++;
        } else {
            direction = 1;
            currentSpeed--;
        }
    } else {  // Ускорение
        if (currentSpeed > 0) {
            currentSpeed--;
        } else {
            direction = 0;
            currentSpeed++;
        }
    }

    // Установка нового значения предделителя
    TIM2->PSC = prescalerValues[currentSpeed];

    // Принудительное обновление регистров таймера
    TIM2->EGR |= TIM_EGR_UG;

    // Запуск таймера
    TIM2->CR1 |= TIM_CR1_CEN;
}

void GPIO_Init(void) {
    // Тактирования портов A и C
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

    // Настройка PA1 как входа с подтяжкой к питанию (кнопка)
    GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->ODR |= (1 << 1);

    // Настройка PC13 (встроенный светодиод) как выхода
    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
    GPIOC->ODR |= (1 << 13);
}

void TIM2_Init(void) {
    // Включение тактирования TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Сброс таймера
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;


    TIM2->PSC = 1023;  // Предделитель
    TIM2->ARR = 4095;  // Автоматическая перезагрузка
    TIM2->DIER |= TIM_DIER_UIE; // Разрешение прерывания по обновлению

    // Принудительное обновление регистров и запуск
    TIM2->EGR |= TIM_EGR_UG;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void NVIC_Init(void) {
    // Очистка и разрешение прерывания TIM2
    NVIC_ClearPendingIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void Button_Handler(void) {
    static bool buttonPrevState = true;
    static bool buttonPressed = false;

    // (1 - не нажата, 0 - нажата)
    bool buttonCurrentState = (GPIOA->IDR & (1 << 1)) != 0;

    if (buttonPrevState && !buttonCurrentState) {
        delay_ms(100);

        // Повторная проверка состояния после задержки
        buttonCurrentState = (GPIOA->IDR & (1 << 1)) != 0;

        if (!buttonCurrentState) {
            toggleSpeed();
            buttonPressed = true;
        }
    }

    buttonPrevState = buttonCurrentState;
}


int main(void) {
    GPIO_Init();
    TIM2_Init();
    NVIC_Init();

    while (1) {
        Button_Handler();
    }
}