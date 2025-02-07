#include <stm32f0xx_hal.h>
#include <assert.h>

int lab1_main(void) {
    HAL_Init(); // reset all peripherals, init the flash and systick
    SystemClock_Config(); // configure the system clock

    //__HAL_RCC_GPIOC_CLK_ENABLE(); // enable the GPIOC clock in the RCC
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    My_HAL_RCC_GPIOA_CLK_ENABLE();

    // GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9,
    //                             GPIO_MODE_OUTPUT_PP,
    //                             GPIO_SPEED_FREQ_LOW,
    //                             GPIO_NOPULL};

    My_HAL_GPIO_Init();

    GPIOC->BSRR = 0b00000000000000000000000001000000; // set PC6

    
    assert(GPIOC->MODER == 0b00000000000001010101000000000000);
    assert(GPIOC->OTYPER == 0x00000000);
    assert(GPIOC->OSPEEDR == 0x00000000);
    assert(GPIOC->PUPDR == 0x00000000);

    assert(GPIOA->MODER == 0x28000000);
    //assert(GPIOA->OSPEEDR == 0x0C000000); // not sure why this doesn't work

    int time_delay = 50;

    while (1) {
        HAL_Delay(time_delay);
        time_delay += 50;
        // //if (GPIOC->IDR & 0b00000000000000000000000001000000) {
        // if (My_HAL_GPIO_ReadPin(GPIOC, 6)) {
        //     //GPIOC->BSRR = 0b00000000010000000000000010000000; // set PC7 and reset PC6
        //     My_HAL_GPIO_WritePin(GPIOC, 7, 1);
        //     My_HAL_GPIO_WritePin(GPIOC, 6, 0);
        // }
        // else {
        //     // GPIOC->BSRR = 0b00000000100000000000000001000000; // set PC6 and reset PC7
        //     My_HAL_GPIO_WritePin(GPIOC, 7, 0);
        //     My_HAL_GPIO_WritePin(GPIOC, 6, 1);
        // }
        My_HAL_GPIO_TogglePin(GPIOC, 6);
        My_HAL_GPIO_TogglePin(GPIOC, 7);
    }

}