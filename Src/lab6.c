#include <stm32f0xx_hal.h>

int lab6_main()
{
    HAL_Init();
    // Enable GPIO C
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    // Enable all the pins for the LEDs
    GPIO_InitTypeDef initStr1 = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
                                 GPIO_MODE_OUTPUT_PP,
                                 GPIO_SPEED_FREQ_LOW,
                                 GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr1);

    GPIO_InitTypeDef initStr2 = {GPIO_PIN_0,
                                 GPIO_MODE_ANALOG,
                                 GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr2);



    while(1)
    {
        // HAL_Delay(500);
        // TIM3->CCR1 += 188;
        // if (TIM3->CCR1 > 2050) {
        //     TIM3->CCR1 = 550;
        // }
    }
}