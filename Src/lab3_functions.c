#include <stm32f0xx_hal.h>

void TIMER2_SETUP() 
{
       // Enable timer 2 in the RCC
       RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
       // Set prescaler to 7999 for 1 ms timer period
       TIM2->PSC = 7999;
       // Set ARR to 250 to get a 4 Hz event
       TIM2->ARR = 250;
       // Allow TIM2 to generate interrupts
       TIM2->DIER |= 0x01;
       // Enable timer 2 in the CR1
       TIM2->CR1 |= 0x01;
       // Enable timer 2 to create interrupts in the NVIC
       NVIC_EnableIRQ(TIM2_IRQn);
}

void TIMER3_SETUP(uint32_t initial_CCR)
{
    // Enable timer 3 in the RCC
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // Set prescaler to 7999 for 1 ms timer period
    TIM3->PSC = 7;
    // Set ARR to 250 to get a 4 Hz event
    TIM3->ARR = 1250;
    // Set channels 1 and 2 to output, channel 1 to pwm mode 2, and channel 2 to pwm mode 1
    TIM3->CCMR1 |= 0b0110100001111000;
    // Enable channels 1 and 2
    TIM3->CCER |= 0b10001;
    // Set the capture/compare value to 20% of ARR on both channels
    TIM3->CCR1 = initial_CCR;
    TIM3->CCR2 = initial_CCR;
    // Enable timer 3 in the CR1
    TIM3->CR1 |= 0x01;
}

void TIMER3_SETCCR1(uint32_t ch1_val)
{
    TIM3->CCR1 = ch1_val;
}

void TIMER3_SETCCR2(uint32_t ch2_val)
{
    TIM3->CCR2 = ch2_val;
}