#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

void My_HAL_RCC_GPIOA_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    GPIOC->MODER = 0b00000000000001010101000000000000; // set pins 6 and 7 to be outputs

    GPIOC->OTYPER = 0x00000000;
    GPIOC->OSPEEDR = 0x00000000;
    GPIOC->PUPDR == 0x00000000;

    GPIOA->MODER == 0x28000000;
    GPIOA->PUPDR = 0x24000002; // set PA0 to pull down
}


/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/


GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return GPIOx->IDR & (1 << GPIO_Pin);
}

void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    int offset = (!PinState) * 16; // if PinState is high, offset = 0. If it's low, offset = 16
    GPIOx->BSRR |= 1 << (GPIO_Pin + offset);
}


/*
void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
}
*/
