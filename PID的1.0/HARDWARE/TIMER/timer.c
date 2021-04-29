#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTE精英STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	//结构体声明//
	GPIO_InitTypeDef GPIO_InitStructure;            //声明GPIO
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //声明定时器
	TIM_OCInitTypeDef  TIM_OCInitStructure;         //声明PWM通道
	
	//**????**//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//定时器4时钟开启
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //GPIOB时钟开启
	
	//****//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //PB.6 端口复用
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50Mhz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //初始换疓PIOB6
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	//****//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PB.7 端口复用
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //初始化GPIOB.7
	GPIO_SetBits(GPIOB,GPIO_Pin_7);

  //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM4 Channel1 PWM 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1
	
	//初始化TIM4 Channel2 PWM 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4_CH1在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4_CH1在CCR2上的预装载寄存器
   
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4定时器
	
}
