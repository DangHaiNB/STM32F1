#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"

// Config GPIO pin
void gpio_init(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &gpio);
}

/*TIMER*/
void timer_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef timer;
    timer.TIM_Prescaler = 35999;
    timer.TIM_Period = 1999;
    timer.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM2, &timer);
}

void delay(unsigned int time)
{
    timer_init();
    TIM_Cmd(TIM2, ENABLE);
    TIM_SetCounter(TIM2, time);
    while (TIM_GetCounter(TIM2) > 0)
        ;
    TIM_Cmd(TIM2, DISABLE);
}

// Toggle Led
void gpio_run_8(void)
{
    GPIO_SetBits(GPIOE, GPIO_Pin_8);
    delay_ms(1000);
    GPIO_ResetBits(GPIOE, GPIO_Pin_8);
    delay_ms(1000);
}

void gpio_run_9(void)
{
    GPIO_SetBits(GPIOE, GPIO_Pin_9);
    delay_ms(1000);
    GPIO_ResetBits(GPIOE, GPIO_Pin_9);
    delay_ms(1000);
}

/*2 Timer dong bo*/
void timer_config(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM1->CR1 |= TIM_CR1_CEN;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->PSC = 7199;
    TIM1->ARR = 999;
    TIM1->CCR1 = TIM1->ARR / 2;

    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->PSC = 7199;
    TIM2->ARR = 999;
    TIM2->CCR1 = TIM2->ARR / 2;
}

// PWM PortA
void pwm_config(void)
	{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef timerInit;
	TIM_OCInitTypeDef pwmInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	timerInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerInit.TIM_Period = 10000 - 1;
	timerInit.TIM_Prescaler = 7200 - 1;
	
	TIM_TimeBaseInit(TIM2, &timerInit);
	
	TIM_Cmd(TIM2, ENABLE);
	
	/* Ch1 with duty 10 % */
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = (10*10000)/100;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM2, &pwmInit);
	
	/* Ch2 with duty 25 % */
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = (25*10000)/100;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC2Init(TIM2, &pwmInit);
	
	/* Ch3 with duty 40 % */
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = (40*10000)/100;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM2, &pwmInit);

	/* Ch1 with duty 80 % */
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = (80*10000)/100;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM2, &pwmInit);
}
	
/*Blink all led port E*/
#define LED_NUM 8
const long led_mask[] = {1 << 15, 1 << 14, 1 << 13, 1 << 12, 1 << 11, 1 << 10, 1 << 9, 1 << 8};
void blinkLed(void)
{
    int AD_val, i;
    int num = -1;
    int dir = 1;

    SystemInit();

    /* Setup GPIO for LEDs                                                      */
    RCC->APB2ENR |= 1 << 6;  /* Enable GPIOE clock                 */
    GPIOE->CRH = 0x33333333; /* Configure the GPIO for LEDs        */

    /* Setup and initialize ADC converter                                       */
    RCC->APB2ENR |= 1 << 9;   /* Enable ADC1 clock                  */
    GPIOC->CRL &= 0xFFF0FFFF; /* Configure PC4 as ADC.14 input      */
    ADC1->SQR1 = 0x00000000;  /* Regular channel 1 conversion       */
    ADC1->SQR2 = 0x00000000;  /* Clear register                     */
    ADC1->SQR3 = 14 << 0;     /* SQ1 = channel 14                   */
    ADC1->SMPR1 = 5 << 12;    /* Channel 14 sample time is 55.5 cyc */
    ADC1->SMPR2 = 0x00000000; /* Clear register                     */
    ADC1->CR1 = 1 << 8;       /* Scan mode on                       */
    ADC1->CR2 = (1 << 20) |   /* Enable external trigger            */
                (7 << 17) |   /* EXTSEL = SWSTART                   */
                (1 << 1) |    /* Continuous conversion              */
                (1 << 0);     /* ADC enable                         */
    ADC1->CR2 |= 1 << 3;      /* Initialize calibration registers   */
    while (ADC1->CR2 & (1 << 3))
        ;                /* Wait for initialization to finish  */
    ADC1->CR2 |= 1 << 2; /* Start calibration                  */
    while (ADC1->CR2 & (1 << 2))
        ;                 /* Wait for calibration to finish     */
    ADC1->CR2 |= 1 << 22; /* Start first conversion             */

    for (;;)
    { /* Loop forever                       */
        if (ADC1->SR & (1 << 1))
        {                               /* If conversion has finished         */
            AD_val = ADC1->DR & 0x0FFF; /* Read AD converted value            */
            ADC1->CR2 |= 1 << 22;       /* Start new conversion               */
        }

        /* Calculate 'num': 0, 1, ... , LED_NUM-1, LED_NUM-1, ... , 1, 0, 0, ...  */
        num += dir;
        if (num >= LED_NUM)
        {
            dir = -1;
            num = LED_NUM - 1;
        }
        else if (num < 0)
        {
            dir = 1;
            num = 0;
        }

        GPIOE->BSRR = led_mask[num]; /* Turn LED on                        */
        delay_ms(500);
        for (i = 0; i < ((AD_val << 8) + 100000); i++)
            ;
        GPIOE->BSRR = led_mask[num] << 16; /* Turn LED off                       */
        delay_ms(500);
    }
}


int main(void)
{
  gpio_init();
	timer_config();
//	pwm_config();
//	GPIO_ResetBits(GPIOE, GPIO_Pin_8);

    while(1)
    {
        //			gpio_run_8();
        //			gpio_run_9();
        if (TIM1->SR & TIM_SR_UIF)
        {
            GPIOE->ODR ^= GPIO_ODR_ODR8;
            TIM1->SR &= ~TIM_SR_UIF;
        }
        if (TIM2->SR & TIM_SR_UIF)
        {
            GPIOE->ODR ^= GPIO_ODR_ODR9;
            TIM2->SR &= ~TIM_SR_UIF;
        }
        delay_ms(500);
    }
    //		blinkLed();
}
