/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现LED功能的源文件                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化LED函数                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //定义一个设置GPIO的变量
	
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);   //使能GPIOC端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;   //设置PC3 PC2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //推免输出方式
	GPIO_Init(GPIOC, &GPIO_InitStructure);            		 //设置PC1 PC2
	
	LED_AllOff();                                            //所有LED熄灭
}

/*-------------------------------------------------*/
/*函数名：点亮所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	LED1_ON;
	LED2_ON;
}

/*-------------------------------------------------*/
/*函数名：熄灭所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
	LED1_OFF;
	LED2_OFF;
}



