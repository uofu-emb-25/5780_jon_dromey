#pragma once
#include <stdint.h>

void SystemClock_Config(void);
void UserButton_Interrupt_Init(void);
void TIMER2_SETUP(void);
void TIMER3_SETUP(uint32_t initial_CCR);
void TIMER3_SETCCR1(uint32_t ch1_val);
void TIMER3_SETCCR2(uint32_t ch2_val);
int lab1_main(void);
int lab2_main(void);
int lab3_main(void);
int lab4_main(void);
int lab5_main(void);
int lab6_main(void);
int lab7_main(void);
