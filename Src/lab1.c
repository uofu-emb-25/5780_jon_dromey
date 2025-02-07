#include <stm32f0xx_hal.h>
#include <assert.h>

int lab1_main(void) {
    HAL_Init(); // reset all peripherals, init the flash and systick
    SystemClock_Config(); // configure the system clock

    //__HAL_RCC_GPIOC_CLK_ENABLE(); // enable the GPIOC clock in the RCC
    My_HAL_RCC_GPIOC_CLK_ENABLE();

    GPIOC->MODER = 0b00000000000001010101000000000000; // set pins 6 and 7 to be outputs
    GPIOA->PUPDR = 0x24000002; // set PA0 to pull down

    GPIOC->BSRR = 0b00000000000000000000000001000000; // set PC6

    
    assert(GPIOC->MODER == 0b00000000000001010101000000000000);
    assert(GPIOC->OTYPER == 0x00000000);
    assert(GPIOC->OSPEEDR == 0x00000000);
    assert(GPIOC->PUPDR == 0x00000000);

    assert(GPIOA->MODER == 0x28000000);
    //assert(GPIOA->OSPEEDR == 0x0C000000);

    while (1) {
        HAL_Delay(1000);
        if (GPIOC->IDR & 0b00000000000000000000000001000000) {
            GPIOC->BSRR = 0b00000000010000000000000010000000; // set PC7 and reset PC6
        }
        else {
            GPIOC->BSRR = 0b00000000100000000000000001000000; // set PC6 and reset PC7
        }
    }

}