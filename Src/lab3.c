#include <stm32f0xx_hal.h>

void TIM2_IRQHandler()
{
    // Toggle pin 9
    My_HAL_GPIO_TogglePin(GPIOC, 9);
    My_HAL_GPIO_TogglePin(GPIOC, 8);

    // Clear the pending bit in the status register
    TIM2->SR &= 0xFFFFFFFE;
}

int lab3_main() 
{
    HAL_Init(); // reset all peripherals, init the flash and systick

    // Enable GPIOC
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef initStr1 = {GPIO_PIN_8 | GPIO_PIN_9,
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr1);

    // Turn the green LED on
    My_HAL_GPIO_WritePin(GPIOC, 9, 1);

    // Setup timer two to create an interrupt at 4 Hz
    TIMER2_SETUP();

    // Setup timer three to generate a PWM signal to the blue and red LED's
    TIMER3_SETUP(1000);

    GPIO_InitTypeDef initStr2 = {GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL,
        GPIO_AF0_TIM3};

    My_HAL_GPIO_Init(GPIOC, &initStr2);


    int dir = 50;
    while(1) {
        if (TIM3->CCR1 <= 50) {
            dir = 50;
        }
        else if (TIM3->CCR1 >= 1200) {
            dir = -50;
        }
        TIM3->CCR1 += dir;
        TIM3->CCR2 += dir;
        HAL_Delay(25 * (1250 - TIM3->CCR1) / 1250 + 25);
    };
}

