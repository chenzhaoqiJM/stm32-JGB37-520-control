/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现LED功能的头文件                */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

#define LED1_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_3, (BitAction)x)  //设置PC3 的电平，可以点亮熄灭LED1
#define LED2_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_2, (BitAction)x)  //设置PC2 的电平，可以点亮熄灭LED2


#define LED1_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_3)  //PC3 控制LED1,读取电平状态，可以判断LED1是点亮还是熄灭
#define LED2_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_2)  //PC2 控制LED2,读取电平状态，可以判断LED2是点亮还是熄灭

#define LED1_OFF       GPIO_ResetBits(GPIOC, GPIO_Pin_3)         //共阳极，拉低PC3电平
#define LED1_ON      GPIO_SetBits(GPIOC, GPIO_Pin_3)           //共阳极，拉高PC3电平

#define LED2_OFF       GPIO_ResetBits(GPIOC, GPIO_Pin_2)         //共阳极，拉低PC2电平
#define LED2_ON      GPIO_SetBits(GPIOC, GPIO_Pin_2)           //共阳极，拉高PC2电平


void LED_Init(void);               //初始化	
void LED_AllOn(void);              //点亮所有LED
void LED_AllOff(void);             //熄灭所有LED

#endif
