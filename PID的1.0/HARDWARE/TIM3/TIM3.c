#include "TIM3.h"
#include "led.h"
#include "usart.h"

extern int flag;

//�����������ó�ʼ��
void encoder_tim3_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	//???
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//��ʱ��3ʱ��ʹ��

	// ��ʼ��������GPIO��

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//�������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//GPIOA6��GPIOA7
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ��ʱ����������ʼ��
	//����ʱ����Ԫ
	TIM_DeInit(TIM3);								//��λTIM3��ؼĴ���
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);			//��ʼ��
	TIM_TimeBaseStructure.TIM_Prescaler = 0;						//��Ƶֵ
	TIM_TimeBaseStructure.TIM_Period = 65535;						//�Զ�װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; // �Լ����õ�
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//??:TIM_ICPolarity_Rising ������
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising); // ������ģʽ
	
	//?????
	TIM_ICStructInit(&TIM_ICInitStructure);	//�ýṹ��ȱʡֵĬ������
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 |TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection =TIM_ICSelection_DirectTI;	//����
  	TIM_ICInitStructure.TIM_ICFilter = 0;	//�����˲�����ֵ
  	TIM_ICInit(TIM3, &TIM_ICInitStructure);	//����ͨ��
	TIM_SetCounter(TIM3, 0);				//��ʼֵΪ0
	TIM_Cmd(TIM3, ENABLE);				//ʹ��
}


