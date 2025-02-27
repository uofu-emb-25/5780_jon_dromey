#include <stm32f0xx_hal.h>

int32_t command = -1;
char color = 'a';

void Transmit_Char(char toTransmit)
{
    while (!(USART3->ISR & 0x80))
        ;
    USART3->TDR = toTransmit;
}

void Transmit_String(char *toTransmit)
{

    while (*toTransmit)
    {
        Transmit_Char(*toTransmit);
        toTransmit++;
    }
}

void USART3_4_IRQHandler() {
    if (USART3->ISR & 0b0100000)
        { // Read data register not empty
            char newChar = USART3->RDR;

            if (newChar >= '0' && newChar <= '2') {
                
                if (color == 'r' || color == 'g' || color == 'b' || color == 'o') {
                    command = newChar - '0';
                }
                else {
                    Transmit_String("Color character ");
                    Transmit_Char(color);
                    Transmit_String(" not valid\n\r");
                }
            }
            else
            {
                color = newChar;
            }
        }
}

int lab4_main()
{
    HAL_Init();

    // Enable LED pins
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef initStr1 = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
                                 GPIO_MODE_OUTPUT_PP,
                                 GPIO_SPEED_FREQ_LOW,
                                 GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr1);

    // Enable the GPIOC pins 4 and 5 to act as USART RX/TX
    GPIO_InitTypeDef initStr = {GPIO_PIN_4 | GPIO_PIN_5,
                                GPIO_MODE_AF_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL,
                                GPIO_AF1_USART3};

    My_HAL_GPIO_Init(GPIOC, &initStr);

    NVIC_EnableIRQ(USART3_4_IRQn);
    NVIC_SetPriority(USART3_4_IRQn, 0);

    // Enable the clock to USART3
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    // Set BAUD rate to 115200
    USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200;
    // Enable transmitter and receiver
    USART3->CR1 |= 0b101100;
    // Enable the USART3 peripheral
    USART3->CR1 |= 0b01;

    while (1)
    {
        HAL_Delay(50);
        if (command >= 0)
        { // Read data register not empty
            uint32_t ledPin;

            switch (color)
            {
            case 'r':
                ledPin = 6;
                break;
            case 'b':
                ledPin = 7;
                break;
            case 'o':
                ledPin = 8;
                break;
            case 'g':
                ledPin = 9;
                break;
            }

            switch (command) 
            {
            case 0:
                My_HAL_GPIO_WritePin(GPIOC, ledPin, 0);
                break;
            case 1:
                
                My_HAL_GPIO_WritePin(GPIOC, ledPin, 1);
                break;
            case 2:
                My_HAL_GPIO_TogglePin(GPIOC, ledPin);
                break;
            }
            command = -1;
        }
    }
}