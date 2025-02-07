#include <stm32f0xx_hal.h>

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

    while(1)
    {
        HAL_Delay(400);
        My_HAL_GPIO_TogglePin(GPIOC, 6);
    }

}