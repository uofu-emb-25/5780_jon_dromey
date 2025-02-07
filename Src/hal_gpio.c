#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_RCC_GPIOC_CLK_ENABLE(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

void My_HAL_RCC_GPIOA_CLK_ENABLE(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    // GPIOC->MODER = 0b00000000000001010101000000000000; // set pins 6 and 7 to be outputs

    // GPIOC->OTYPER = 0x00000000;
    // GPIOC->OSPEEDR = 0x00000000;
    // GPIOC->PUPDR == 0x00000000;

    // GPIOA->MODER == 0x28000000;
    // GPIOA->PUPDR = 0x24000002; // set PA0 to pull down

    // uint32_t position = 0x00u;
    // uint32_t iocurrent;
    // uint32_t temp;

    uint32_t position = 0x00u;
    uint32_t iocurrent;
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
    assert_param(IS_GPIO_MODE(GPIO_Init->Mode));

    /* Configure the port pins */
    while (((GPIO_Init->Pin) >> position) != 0x00u)
    {
        /* Get current io position */
        iocurrent = (GPIO_Init->Pin) & (1uL << position);

        if (iocurrent != 0x00u)
        {
            /*--------------------- GPIO Mode Configuration ------------------------*/
            /* In case of Output or Alternate function mode selection */
            if (((GPIO_Init->Mode & GPIO_MODE) == MODE_OUTPUT) ||
                ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF))
            {
                /* Check the Speed parameter */
                assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));
                /* Configure the IO Speed */
                temp = GPIOx->OSPEEDR;
                temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2u));
                temp |= (GPIO_Init->Speed << (position * 2u));
                GPIOx->OSPEEDR = temp;

                /* Configure the IO Output Type */
                temp = GPIOx->OTYPER;
                temp &= ~(GPIO_OTYPER_OT_0 << position);
                temp |= (((GPIO_Init->Mode & OUTPUT_TYPE) >> OUTPUT_TYPE_Pos) << position);
                GPIOx->OTYPER = temp;
            }

            if ((GPIO_Init->Mode & GPIO_MODE) != MODE_ANALOG)
            {
                /* Check the Pull parameter */
                assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

                /* Activate the Pull-up or Pull down resistor for the current IO */
                temp = GPIOx->PUPDR;
                temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2u));
                temp |= ((GPIO_Init->Pull) << (position * 2u));
                GPIOx->PUPDR = temp;
            }

            /* In case of Alternate function mode selection */
            if ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF)
            {
                /* Check the Alternate function parameters */
                assert_param(IS_GPIO_AF_INSTANCE(GPIOx));
                assert_param(IS_GPIO_AF(GPIO_Init->Alternate));

                /* Configure Alternate function mapped with the current IO */
                temp = GPIOx->AFR[position >> 3u];
                temp &= ~(0xFu << ((position & 0x07u) * 4u));
                temp |= ((GPIO_Init->Alternate) << ((position & 0x07u) * 4u));
                GPIOx->AFR[position >> 3u] = temp;
            }

            /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
            temp = GPIOx->MODER;
            temp &= ~(GPIO_MODER_MODER0 << (position * 2u));
            temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2u));
            GPIOx->MODER = temp;

            /*--------------------- EXTI Mode Configuration ------------------------*/
            /* Configure the External Interrupt or event for the current IO */
            if ((GPIO_Init->Mode & EXTI_MODE) != 0x00u)
            {
                /* Enable SYSCFG Clock */
                __HAL_RCC_SYSCFG_CLK_ENABLE();

                temp = SYSCFG->EXTICR[position >> 2u];
                temp &= ~(0x0FuL << (4u * (position & 0x03u)));
                temp |= (GPIO_GET_INDEX(GPIOx) << (4u * (position & 0x03u)));
                SYSCFG->EXTICR[position >> 2u] = temp;

                /* Clear Rising Falling edge configuration */
                temp = EXTI->RTSR;
                temp &= ~(iocurrent);
                if ((GPIO_Init->Mode & TRIGGER_RISING) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->RTSR = temp;

                temp = EXTI->FTSR;
                temp &= ~(iocurrent);
                if ((GPIO_Init->Mode & TRIGGER_FALLING) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->FTSR = temp;

                /* Clear EXTI line configuration */
                temp = EXTI->EMR;
                temp &= ~(iocurrent);
                if ((GPIO_Init->Mode & EXTI_EVT) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->EMR = temp;

                temp = EXTI->IMR;
                temp &= ~(iocurrent);
                if ((GPIO_Init->Mode & EXTI_IT) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->IMR = temp;
            }
        }

        position++;
    }
}

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/

GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    return GPIOx->IDR & (1 << GPIO_Pin);
}

void My_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    int offset = (!PinState) * 16; // if PinState is high, offset = 0. If it's low, offset = 16
    GPIOx->BSRR |= 1 << (GPIO_Pin + offset);
}

void My_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    // int pinState = My_HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
    GPIOx->ODR ^= 1 << GPIO_Pin;
}
