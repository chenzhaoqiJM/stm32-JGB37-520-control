
/*-----------------------------------------------------*/
/*                     程序结构                        */
/*-----------------------------------------------------*/
/*USER     ：包含程序的main函数，是整个程序的入口      */
/*HARDWARE ：包含开发板各种功能外设的驱动程序          */
/*CORE     ：包含STM32的核心程序，官方提供，我们不修改 */
/*STLIB    ：官方提供的库文件，我们不修改              */
/*-----------------------------------------------------*/
/*                                                     */
/*           程序main函数，入口函数源文件              */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h" 
#include "main.h"       
#include "delay.h"      
#include "usart1.h"     
#include "timer4.h"     
#include "pwm.h"
#include "led.h"        
#include <stdlib.h>

/*-------------------------------------------------*/
/* 函数名：读取编码器当前计数值                     */
/* 返回值：当前累计的脉冲数 (带方向)                */
/*-------------------------------------------------*/
int Read_Encoder(void)
{
    int encoder_count;
    
    // 将无符号的寄存器值强制转换为有符号整型
    // 这样反转时，0 会变成 -1 (0xFFFF)
    encoder_count = (short)TIM_GetCounter(TIM2);
    
    // 读完后通常清零，为了测量“这段时间内的速度”
    // 如果是做位置控制（如舵机），则不需要清零
    TIM_SetCounter(TIM2, 0); 
    
    return encoder_count;
}


int main(void) 
{
    unsigned int time;     //定义一个变量待用
	int value = 0;
	int mode = 0;
	int pwm = 0;
	int speed_count = 0;
    float rpm = 0;
	
	Delay_Init();           //延时功能初始化              
	Usart1_Init(9600);      //串口1功能初始化，波特率9600
	TIM4_Init(300,7200);    //TIM4初始化，定时时间 300*7200*1000/72000000 = 30ms
	
    // 初始化
    Encoder_TIM2_Init(); // 此时 TIM2 开始在后台默默数脉冲
	
	PWM_T3CH1_PA6_Init(1000,72); //72M/72=1Mhz的计数频率,重装1000，所以PWM频率为 1M/1000=1Khz.	
	
	LED_Init();     //LED指示灯初始化
	
	Usart1_RxCompleted = 0; //初始化串口1接收完成标志，设置为0，表示没有接收完成
	Usart1_RxCounter = 0;   //初始化串口1接收量，设置为0
	
	while(1)                //主循环
	{		
		time++;            //time值+1
		Delay_Ms(100);      //延时100ms
		
		// 1. 读取过去 100ms 内经过了多少个脉冲，并清零
        speed_count = Read_Encoder(); 
        
        // 2. 计算转速 (假设减速比为 30)，单位: 转/s
        rpm = (float)speed_count * 10 / (11 * 4 * 6.3);
        
        // 3. 可以在这里根据 rpm 调整 PWM 来控制电机速度 (PID控制)
		
		if(time >= 50){                          //每延时10ms，time+1，如果大于50，表示至少过去了500ms，进入if
			time = 0;                            //time等于0
			u1_printf("no data recev !!!\r\n");     //串口1输出信息
		}
		
		u1_printf("Number of pulses: %d, RPM: %.2f\r\n", speed_count, rpm);
		
		if(Usart1_RxCompleted == 1){	                       //如果Usart1_RxCompleted等于1，表示接收完成了，进入if				   
			Usart1_RxBuff[Usart1_RxCounter]='\0'   ;           //加入字符串结束符
			
			
			if(sscanf((char*)Usart1_RxBuff, "%d;%d", &mode, &pwm) == 2)
			{
				TIM_SetCompare1(TIM3, pwm);

				switch(mode) {
					case 0:
						LED1_OFF;
						LED2_OFF;
						break;
					case 1:
						LED1_ON;
						LED2_OFF;
						break;
					case 2:
						LED1_OFF;
						LED2_ON;
						break;
					default:
						break;
				}
			}
			
			u1_printf("get data: %s\r\n",Usart1_RxBuff);     //串口1输出信息
			Usart1_RxCompleted = 0;                            //串口1接收完成标志设置为0
			Usart1_RxCounter = 0;                              //串口1接收量设置为0	
		}			
	}
	
}


