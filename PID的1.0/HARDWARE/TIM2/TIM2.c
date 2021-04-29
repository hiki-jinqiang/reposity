#include "TIM2.h"
#include "TIM3.h"
#include "led.h"
#include "usart.h"

extern int flag;

void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}


//void PID_control(int exp_spd)
//{
//	int PID_out;					//PID�����ֵ
//	static int error_pre=0;			//��һ�ε�ƫ��
//	static int error_now=0;			//��һ�ε�ƫ��
//	static int error_iteg=0;		//������
//	
//	spd_now=exp_spd*600/28;			//���תһȦ7�����壬һ�������ȡ4�Σ����ǵ�λʱ��10ms�ڣ�ת��Ϊ����ÿתҪ��600
//	printf("%d rd\r\n",spd_now);		//��ӡ�����ٶ�
//	error_now=exp_spd-spd_now;			//��������
//	error_iteg+=error_now;				//�ӷ��������
//	PID_out=Kp*error_now+Ki*error_iteg+Kd*(error_now-error_pre);	//��깫ʽ
//	error_pre=error_now;				//�ⳡ�����������Ϊ�´ε��ϴ����
//	if(PID_out>9999)	PID_out=9999;
//	if(PID_out<-9999)	PID_out=-9999;
//	printf("%d PID\r\n",PID_out);			//��ӡPID�����ֵ

//}

void TIM2_IRQHandler(void)   //TIM2�жϺ���
{
	
		extern int encoder_num;
		extern int flag;
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)  //���TIM3�����жϷ������
		{
			encoder_num = (int)((int16_t)(TIM3->CNT)); // ��ȡת�٣�ע�����ݽṹ��
			TIM_SetCounter(TIM3, 0);
			flag=1;
			LED1=!LED1;						//��֤�Ƿ���������
		}
				TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //�����־λ

	
}



