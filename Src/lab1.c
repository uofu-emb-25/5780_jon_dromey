#include <stm32f0xx_hal.h>
#include <assert.h>

int lab1_main(void) {
    HAL_Init(); // reset all peripherals, init the flash and systick
    SystemClock_Config(); // configure the system clock

    //__HAL_RCC_GPIOC_CLK_ENABLE(); // enable the GPIOC clock in the RCC
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    My_HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef initStrC = {GPIO_PIN_6 | GPIO_PIN_7,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL};

    My_HAL_GPIO_Init(GPIOC, &initStrC);


    GPIO_InitTypeDef initStrA = {GPIO_PIN_0,
        GPIO_MODE_INPUT,
        GPIO_SPEED_FREQ_LOW,
        GPIO_PULLDOWN};

    My_HAL_GPIO_Init(GPIOA, &initStrA);

    GPIOC->BSRR = 0b00000000000000000000000001000000; // set PC6

    // My_HAL_GPIO_WritePin(GPIOC, 6, 1);

    
    // assert(GPIOC->MODER == 0b00000000000001010101000000000000);
    // assert(GPIOC->OTYPER == 0x00000000);
    // assert(GPIOC->OSPEEDR == 0x00000000);
    // assert(GPIOC->PUPDR == 0x00000000);

    // assert(GPIOA->MODER == 0x28000000);
    // assert(GPIOA->OSPEEDR == 0x0C000000); // not sure why this doesn't work
    int debouncer = 0;

    while (1) {
        debouncer = (debouncer << 1);

        if (My_HAL_GPIO_ReadPin(GPIOA, 0)) {
            debouncer |= 0x01;
        }

        if (debouncer == 0x7FFFFFFF) {
            My_HAL_GPIO_TogglePin(GPIOC, 6);
            My_HAL_GPIO_TogglePin(GPIOC, 7);
        }

        //HAL_Delay(5);
    }

}