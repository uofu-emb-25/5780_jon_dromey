#include <stm32f0xx_hal.h>

// Sine Wave: 8-bit, 32 samples/cycle
const uint8_t sine_table[32] = {127,151,175,197,216,232,244,251,254,251,244,
    232,216,197,175,151,127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102};
    

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


    // Enable the GPIOC pins 4 and 5 to act as USART RX/TX
    GPIO_InitTypeDef initStr = {GPIO_PIN_4 | GPIO_PIN_5,
                                GPIO_MODE_AF_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL,
                                GPIO_AF1_USART3};

    My_HAL_GPIO_Init(GPIOC, &initStr);


    // Enable the clock to USART3
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    // Set BAUD rate to 115200
    USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200;
    // Enable transmitter and receiver
    USART3->CR1 |= 0b101100;
    // Enable the USART3 peripheral
    USART3->CR1 |= 0b01;

    // Configure ADC
    GPIO_InitTypeDef initStr2 = {GPIO_PIN_0,
                                 GPIO_MODE_ANALOG,
                                 GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr2);

    // Enable the ADC in the RCC
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // continuous, 8-bit,
    ADC1->CFGR1 |= ((0b01 << 13) | (0b10 << 3));
    // Select channel 10
    ADC1->CHSELR |= (0b01 << 10);
    // Start callibration
    ADC1->CR |= (0b01 << 31);
    // Wait for calibration to complete
    while (ADC1->CR & (0b01 << 31))
        ;
    // Enable the peripheral
    ADC1->CR |= 0b01;
    // Wait for the peripheral to be ready
    while (!(ADC1->ISR & 0b01))
        ;
    
    // Start the conversion
    ADC1->CR |= (0b01 << 2);



    // Set PA4 to analog function for the DAC
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIO_InitTypeDef initStr3 = {GPIO_PIN_4,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOA, &initStr3);

    // Enable the DAC
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    // Select software trigger and enable the DAC
    DAC1->CR |= ((0b111 << 3) | 0b01);


    uint16_t wave_index = 0;
    uint8_t delay = 1;
    while (1)
    {

        // HAL_Delay(0) seems to cause a 1 ms delay??
        HAL_Delay(0);

        // Loop through the sine table and write the values to the DAC data reg
        DAC1->DHR8RD = sine_table[wave_index];
        wave_index++;
        if (wave_index > 31)
        {
            wave_index = 0;
        }

        // Read from the ADC
        uint32_t ADC_reading = ADC1->DR;

        // Turn on the leds according to different thresholds
        if (ADC_reading > 254)
        {
            My_HAL_GPIO_WritePin(GPIOC, 8, 1);
        }
        else
        {
            My_HAL_GPIO_WritePin(GPIOC, 8, 0);
        }

        if (ADC_reading > 200)
        {
            My_HAL_GPIO_WritePin(GPIOC, 7, 1);
        }
        else
        {
            My_HAL_GPIO_WritePin(GPIOC, 7, 0);
        }

        if (ADC_reading > 110)
        {
            My_HAL_GPIO_WritePin(GPIOC, 9, 1);
        }
        else
        {
            My_HAL_GPIO_WritePin(GPIOC, 9, 0);
        }

        if (ADC_reading > 20)
        {
            My_HAL_GPIO_WritePin(GPIOC, 6, 1);
        }
        else
        {
            My_HAL_GPIO_WritePin(GPIOC, 6, 0);
        }
    }
}