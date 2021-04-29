#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "TIM2.h"
#include "TIM3.h"



int encoder_num;
int flag;
float Kp=20;				//����ϵ��
float Kd=1;					//΢��ϵ��
float Ki=0.5;					//����ϵ��

int exp_spd=400;				//�����ٶ�
int spd_now=0;				//�����ٶ�


//PID���ƺ���
void PID_control(int exp)
{
	int PID_out;					//PID�����ֵ
	static int error_pre=0;			//��һ�ε�ƫ��
	static int error_now=0;			//��һ�ε�ƫ��
	static int error_iteg=0;		//������
	
	spd_now=exp*600/28;			//���תһȦ7�����壬һ�������ȡ4�Σ����ǵ�λʱ��10ms�ڣ�ת��Ϊ����ÿתҪ��600
	printf("%d,%d \n",spd_now,exp_spd);		//��ӡ�����ٶ�
	error_now=exp_spd-spd_now;			//��������
	error_iteg+=error_now;				//�ӷ��������	
	PID_out=Kp*error_now+Ki*error_iteg+Kd*(error_now-error_pre);	//��깫ʽ
	error_pre=error_now;	//�ⳡ�����������Ϊ�´ε��ϴ����
	if(PID_out>1000)	PID_out=1000;
	if(PID_out<0)		PID_out=0;
	TIM_SetCompare1(TIM4,PID_out);			//PB6��PWm���
	
}







 int main(void)
 {		
// 	u16 led0pwmval=0;
//	u8 dir=1;	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	TIM2_Int_Init(99,7199); //10ms=(99+1)*(7199+1)/72
	encoder_tim3_init();		//TIM3������ģʽ��ʼ��
	TIM4_PWM_Init(1000,0);	 //1KHZ PWM���
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
   	while(1)
	{
		LED0=1;
		if(flag==1)
		{
			PID_control(encoder_num);
			flag=0;
		}
		
//		printf("%d rd\r\n",encoder_num);//��ӡ
//		if(dir)led0pwmval++;
//		else led0pwmval--;
//		LED0=!LED0;
// 		if(led0pwmval>400)dir=0;
//		if(led0pwmval==0)dir=1;	
//		TIM_SetCompare1(TIM4,led0pwmval);	//PB6��PWm���
	}	 
 }

