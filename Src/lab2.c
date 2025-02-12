#include <stm32f0xx_hal.h>
#include <assert.h>

void EXTI0_1_IRQHandler() {
    
    My_HAL_GPIO_TogglePin(GPIOC, 8);
    My_HAL_GPIO_TogglePin(GPIOC, 9);

    // Delay loop
    for (volatile uint32_t i = 0; i < 3000000; i++);

    My_HAL_GPIO_TogglePin(GPIOC, 8);
    My_HAL_GPIO_TogglePin(GPIOC, 9);
    EXTI->PR |= 0x01;
}

int lab2_main() {
    HAL_Init(); // reset all peripherals, init the flash and systick
    SystemClock_Config(); // configure the system clock

    My_HAL_RCC_GPIOC_CLK_ENABLE();
    My_HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef initStrC = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};

    My_HAL_GPIO_Init(GPIOC, &initStrC);

    My_HAL_GPIO_WritePin(GPIOC, 9, 1);

    GPIO_InitTypeDef initStrA = {GPIO_PIN_0,
        GPIO_MODE_INPUT,
        GPIO_SPEED_FREQ_LOW,
        GPIO_PULLDOWN};

    My_HAL_GPIO_Init(GPIOA, &initStrA);

    assert(EXTI->IMR == 0x7F840000);
    assert(EXTI->RTSR == 0x00000000);
    assert((SYSCFG->EXTICR[0] & 0x0F) == 0);
    UserButton_Interrupt_Init();
    assert(EXTI->IMR == 0x7F840001);
    assert(EXTI->RTSR == 0x00000001);
    assert((SYSCFG->EXTICR[0] & 0x0F) == 0);

    while(1)
    {
        HAL_Delay(400);
        My_HAL_GPIO_TogglePin(GPIOC, 6);
    }

}