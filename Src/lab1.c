#include <stm32f0xx_hal.h>

int lab1_main(void) {
    HAL_Init(); // reset all peripherals, init the flash and systick
    SystemClock_Config(); // configure the system clock

    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Set up a configuration struct to pass the initialization function
    GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9, 
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL};

    HAL_GPIO_Init(GPIOC, &initStr);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    while (1) {
        HAL_Delay(200);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
    }

}