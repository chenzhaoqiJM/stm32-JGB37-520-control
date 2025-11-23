/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现PWM功能的源文件                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化TIM3通道3的PWM(PB0)                */
/*参  数：arr：自动重装值                          */
/*参  数：psc：时钟预分频数                        */
/*返回值：无                                       */
/*-------------------------------------------------*/
void PWM_T3CH3_PB0_Init(unsigned int arr, unsigned int psc)
{		 					 	
	GPIO_InitTypeDef GPIO_InitStructure;                   //定义一个设置IO的变量
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        //定义一个设置TIM的变量
	TIM_OCInitTypeDef  TIM_OCInitStructure;                //定义一个设置TIM比较输出的变量
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;              //准备设置PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //速率50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PB0
	
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;                //定时器分频
	TIM_TimeBaseStructure.TIM_Period=arr-1;                   //自动重装载值
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);            //设置TIM3
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:低
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);                      //设置TIM3通道3的比较输出

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在通道3上的预装载寄存器 
    TIM_ARRPreloadConfig(TIM3,ENABLE);                            //ARPE使能 	
	TIM_Cmd(TIM3, ENABLE);                                        //使能TIM3										  
}

void PWM_T3CH1_PA6_Init(unsigned int arr, unsigned int psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    // 1. ????
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2. ??? GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  // PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ??????
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseStructure.TIM_Period = arr - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 4. ?? PWM ??(??1)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // 5. ?????
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}



