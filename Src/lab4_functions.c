#include <stm32f0xx_hal.h>

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