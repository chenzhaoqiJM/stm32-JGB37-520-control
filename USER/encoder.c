#include "stm32f10x.h"

/*-------------------------------------------------*/
/* 函数名：编码器模式初始化 (使用 TIM2)             */
/* 说  明：PA0 接 A相，PA1 接 B相                   */
/*-------------------------------------------------*/
void Encoder_TIM2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    // 1. 开启时钟：TIM2 和 GPIOA
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2. 配置 GPIO (PA0, PA1) 为浮空输入或上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 推荐使用上拉输入，防止干扰
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 时基配置
    // ARR 设置为最大值 65535，防止计数器很快溢出
    // PSC 设置为 0，不需要分频，我们需要读每一个脉冲
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 4. 关键步骤：配置编码器接口模式
    // TIM_EncoderMode_TI12: 同时检测 TI1 和 TI2 上的边缘 (即 4倍频模式)
    // TIM_ICPolarity_Rising: 这里指信号不反相
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    // 5. 清零计数器，准备开始
    TIM_SetCounter(TIM2, 0);

    // 6. 开启定时器
    TIM_Cmd(TIM2, ENABLE);
}