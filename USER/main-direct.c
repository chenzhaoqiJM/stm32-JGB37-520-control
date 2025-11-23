
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
	int mode = 0;
	
	int speed_count = 0;
    float rpm = 0;
	float abs_rpm = 0;
	float target_speed;
	int pwm_output; // 给设备的pwm
	
	float init_pwm = 0; // 直接根据转速计算得来的
	float derivative_rpm = 0;
	float abs_derivative_rpm = 0;
	int add_value = 0;

	
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
	
		if(rpm<0){
			abs_rpm = -1.0 * rpm;
		}
		else{
			abs_rpm = rpm;
		}
		
		// 计算偏差梯度，系数可调整
		derivative_rpm = (target_speed - abs_rpm);
		
		if(derivative_rpm<0){
			abs_derivative_rpm = -1.0 * derivative_rpm;
		}
		else{
			abs_derivative_rpm = derivative_rpm;
		}
		
		
		if(abs_derivative_rpm>=0.8){
			add_value = 5;
		}
		else if(abs_derivative_rpm>=0.6 && abs_derivative_rpm<0.8){
			add_value = 4;
		}
		else if(abs_derivative_rpm>=0.4 && abs_derivative_rpm<0.6){
			add_value = 3;
		}
		else if(abs_derivative_rpm>=0.2 && abs_derivative_rpm<0.4){
			add_value = 2;
		}
		else if(abs_derivative_rpm>=0.0 && abs_derivative_rpm<0.2){
			add_value = 1;
		}
		
		
		// 设置电机转速
		if(derivative_rpm>0.1){
			pwm_output = pwm_output + add_value;
		}
		if(derivative_rpm<-0.1){
			pwm_output = pwm_output - add_value;
		}
		
		TIM_SetCompare1(TIM3, pwm_output);
		
		// 5s 打印一次
		if(time >= 50){
			time = 0;                            
			u1_printf("no data recev !!!\r\n"); 
		}
		
		// 0.5s打印一次
		//if(time%5==0){
		//	u1_printf("Number of pulses: %d, RPM: %.2f, Target=%.2f,  PWM=%d\r\n", speed_count, rpm, target_speed, pwm_output);
		//}
		u1_printf("Number of pulses: %d, RPM: %.2f, Target=%.2f,  PWM=%d\r\n", speed_count, abs_rpm, target_speed, pwm_output);
		
		if(Usart1_RxCompleted == 1){	                       //如果Usart1_RxCompleted等于1，表示接收完成了，进入if				   
			Usart1_RxBuff[Usart1_RxCounter]='\0'   ;           //加入字符串结束符
			
			
			if(sscanf((char*)Usart1_RxBuff, "%d;%f", &mode, &target_speed) == 2)
			{
				init_pwm = 10.2 * target_speed + 158;
				pwm_output = (int)init_pwm;

				switch(mode) {
					case 0:
						init_pwm = 0;
						pwm_output = 0;
						target_speed = 0;
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


