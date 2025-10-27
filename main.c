#include <stdint.h>
#include <stm32f10x.h>

void delay(uint32_t ticks) {
	for (int i=0; i<ticks; i++) {
		__NOP();
	}
}

int __attribute((noreturn)) main(void) {
	// Тактирование для портов AFIO, PA, PC
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	GPIOC->CRH &= ~GPIO_CRH_CNF13;        // очистка CNF
	GPIOC->CRH |= GPIO_CRH_MODE13_0;      // MODE = 01 (10MHz)

	GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0); // PA0
	GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1); // PA1
	GPIOA->CRL |= GPIO_CRL_CNF0_1;
	GPIOA->CRL |= GPIO_CRL_CNF1_1;
	GPIOA->ODR |= (1U << 0);
	GPIOA->ODR |= (1U << 1U);

	uint32_t delays[] = {
		2000000000,
		200000000,
		40000000,
		8000000};
	uint8_t mode = 0;

	while (1)
	{
		// Кнопка A (PA0) — увеличить частоту (кнопку лучше удерживать)
		if (!(GPIOA->IDR & (1U << 0)))
		{
			if (mode < sizeof(delays) / sizeof(delays[0]) - 1)
			{
				mode++;
			}
		}

		// Кнопка C (PA1) — уменьшить частоту (кнопку лучше удерживать)
		if (!(GPIOA->IDR & (1U << 1)))
		{
			if (mode > 1)
			{
				mode--;
			}
		}

		// Включаем/Выключаем LED
		GPIOC->ODR ^= (1U << 13U);
		delay(delays[mode]);
	}
}