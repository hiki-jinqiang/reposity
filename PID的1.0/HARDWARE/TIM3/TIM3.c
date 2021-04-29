#include "TIM3.h"
#include "led.h"
#include "usart.h"

extern int flag;

//编码器的设置初始化
void encoder_tim3_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	//???
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//定时器3时钟使能

	// 初始化编码器GPIO口

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//复用输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//GPIOA6，GPIOA7
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 定时器编码器初始化
	//配置时基单元
	TIM_DeInit(TIM3);								//复位TIM3相关寄存器
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);			//初始化
	TIM_TimeBaseStructure.TIM_Prescaler = 0;						//分频值
	TIM_TimeBaseStructure.TIM_Period = 65535;						//自动装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; // 自己设置的
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//??:TIM_ICPolarity_Rising 不反向
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising); // 编码器模式
	
	//?????
	TIM_ICStructInit(&TIM_ICInitStructure);	//该结构按缺省值默认填入
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 |TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection =TIM_ICSelection_DirectTI;	//设置
  	TIM_ICInitStructure.TIM_ICFilter = 0;	//配置滤波器的值
  	TIM_ICInit(TIM3, &TIM_ICInitStructure);	//设置通道
	TIM_SetCounter(TIM3, 0);				//初始值为0
	TIM_Cmd(TIM3, ENABLE);				//使能
}


