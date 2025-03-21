#include <stm32f0xx_hal.h>
#include <assert.h>

void writeRegister(uint32_t device, uint32_t reg, uint8_t toSend)
{
    /* Clear the NBYTES and SADD bit fields
     * The NBYTES field begins at bit 16, the SADD at bit 0
     */
    I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
    // Set NBYTES and SADD
    I2C2->CR2 |= (2 << 16) | (device << 1);

    // Set the read bit low
    I2C2->CR2 &= ~(0b01 << 10);

    // Set start bit
    I2C2->CR2 |= (0b01 << 13);

    // Wait for either NACKF or TXIS
    while (!(I2C2->ISR & 0b10010))
        ;

    if (I2C2->ISR & 0b10000) // NACKF
    {
        // Send stop bit
        I2C2->CR2 |= (0b01 << 14);
        My_HAL_GPIO_WritePin(GPIOC, 6, 1); // Turn on red LED for NACKF
    }
    else if (I2C2->ISR & 0b10) // TXIS
    {
        // Write the register address into the transmit data regiester
        I2C2->TXDR = reg;

        // Wait for either NACKF or TXIS
        while (!(I2C2->ISR & 0b10010))
            ;

        if (I2C2->ISR & 0b10000) // NACKF
        {
            // Send stop bit
            I2C2->CR2 |= (0b01 << 14);
            My_HAL_GPIO_WritePin(GPIOC, 6, 1); // Turn on red LED for NACKF
        }
        else if (I2C2->ISR & 0b10) // TXIS
        {
            // Write the register content into the transmit data regiester
            I2C2->TXDR = toSend;

            // Wait for TC bit
            while (!(I2C2->ISR & (1 << 6)))
                ;

            // Send stop bit
            I2C2->CR2 |= (0b01 << 14);
        }
    }
}

uint32_t readXYData(uint32_t device)
{
    uint8_t sensorValues[4];
    /* Clear the NBYTES and SADD bit fields
     * The NBYTES field begins at bit 16, the SADD at bit 0
     */
    I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
    // Set SADD bits to 0x69 and NBYTES to 1
    I2C2->CR2 |= (1 << 16) | (device << 1);

    // Set the read bit low
    I2C2->CR2 &= ~(0b01 << 10);

    // Set start bit
    I2C2->CR2 |= (0b01 << 13);

    // Wait for either NACKF or TXIS
    while (!(I2C2->ISR & 0b10010))
        ;

    if (I2C2->ISR & 0b10000) // NACKF
    {
        // Send stop bit
        I2C2->CR2 |= (0b01 << 14);
    }
    else if (I2C2->ISR & 0b10) // TXIS
    {
        // Write the address to read into the transmit data regiester
        I2C2->TXDR = 0xA8;

        // Wait for TC bit
        while (!(I2C2->ISR & (1 << 6)))
            ;

        // Set SADD bits to 0x69 and NBYTES to 1
        I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
        I2C2->CR2 |= (4 << 16) | (device << 1);
        // Set the read bit high
        I2C2->CR2 |= (0b01 << 10);
        // Set the start bit
        I2C2->CR2 |= (0b01 << 13);

        for (uint32_t i = 0; i < 4; i++)
        {
            // Wait for either NACKF or RXNE
            while (!(I2C2->ISR & 0b10100))
                ;
            sensorValues[i] = I2C2->RXDR;
        }

        while (!(I2C2->ISR & 1 << 6))
            ; // Wait for TC bit

        // Send stop bit
        I2C2->CR2 |= (0b01 << 14);
    }
    uint32_t returnValue = 0;
    for (int i = 0; i < 4; i++) 
    {
        returnValue |= (sensorValues[i] << 8*i);
    }
    return returnValue;
}

int lab5_main()
{
    HAL_Init();

    // Enable GPIO B
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // Enable the GPIOB pins. 11 = SDA
    GPIO_InitTypeDef initStr1 = {GPIO_PIN_11,
                                 GPIO_MODE_AF_OD,
                                 GPIO_SPEED_FREQ_HIGH,
                                 GPIO_NOPULL,
                                 GPIO_AF1_I2C2};
    My_HAL_GPIO_Init(GPIOB, &initStr1);

    // Enable the GPIOB pins. 13 = SCL
    GPIO_InitTypeDef initStr2 = {GPIO_PIN_13,
                                 GPIO_MODE_AF_OD,
                                 GPIO_SPEED_FREQ_HIGH,
                                 GPIO_NOPULL,
                                 GPIO_AF5_I2C2};
    My_HAL_GPIO_Init(GPIOB, &initStr2);

    // Set PB14 to output mode, push-pull output type, and initialize/set the pin high
    GPIO_InitTypeDef initStr3 = {GPIO_PIN_14,
                                 GPIO_MODE_OUTPUT_PP,
                                 GPIO_SPEED_FREQ_LOW,
                                 GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOB, &initStr3);
    My_HAL_GPIO_WritePin(GPIOB, 14, 1);

    // PC0 to output mode, push-pull output type, and initialize/set the pin high.
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIO_InitTypeDef initStr4 = {GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
                                 GPIO_MODE_OUTPUT_PP,
                                 GPIO_SPEED_FREQ_LOW,
                                 GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr4);
    My_HAL_GPIO_WritePin(GPIOC, 0, 1);

    // Enable I2C2 through the RCC
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    //              SCLL     SCLH      SDADEL      SCLDEL       PRESC
    I2C2->TIMINGR = 0x13 | 0xF << 8 | 0x2 << 16 | 0x4 << 20 | 0x1 << 28;
    // Enable I2C
    I2C2->CR1 |= 0b01;

    // Write the needed info into CR1 of the gyroscope

    writeRegister(0x69, 0x20, 0b00001011);

    while(1) {
        HAL_Delay(100);
        uint32_t sensorData = readXYData(0x69);
        int16_t xData = sensorData & 0xFFFF;
        int16_t yData = (sensorData & 0xFFFF0000) >> 16;

        int16_t bias = 25;
        xData += bias;
        yData += bias;
        int16_t threshold = 1600;

        My_HAL_GPIO_WritePin(GPIOC, 6, 0);
        My_HAL_GPIO_WritePin(GPIOC, 7, 0);
        My_HAL_GPIO_WritePin(GPIOC, 8, 0);
        My_HAL_GPIO_WritePin(GPIOC, 9, 0);

        if (xData > threshold)
        {
            My_HAL_GPIO_WritePin(GPIOC, 8, 1);
        }
        else if (xData < threshold * -1) {
            My_HAL_GPIO_WritePin(GPIOC, 9, 1);
        }

        if (yData > threshold)
        {
            My_HAL_GPIO_WritePin(GPIOC, 6, 1);
        }
        else if (yData < threshold * -1) {
            My_HAL_GPIO_WritePin(GPIOC, 7, 1);
        }
    }

    
}