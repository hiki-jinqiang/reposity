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
float Kp=20;				//比例系数
float Kd=1;					//微分系数
float Ki=0.5;					//积分系数

int exp_spd=400;				//期望速度
int spd_now=0;				//现在速度


//PID控制函数
void PID_control(int exp)
{
	int PID_out;					//PID输出的值
	static int error_pre=0;			//上一次的偏差
	static int error_now=0;			//这一次的偏差
	static int error_iteg=0;		//积分项
	
	spd_now=exp*600/28;			//电机转一圈7个脉冲，一个脉冲读取4次，但是单位时间10ms内，转化为分钟每转要乘600
	printf("%d,%d \n",spd_now,exp_spd);		//打印现在速度
	error_now=exp_spd-spd_now;			//现在误差定义
	error_iteg+=error_now;				//加法替代积分	
	PID_out=Kp*error_now+Ki*error_iteg+Kd*(error_now-error_pre);	//灵魂公式
	error_pre=error_now;	//这场的现在误差作为下次的上次误差
	if(PID_out>1000)	PID_out=1000;
	if(PID_out<0)		PID_out=0;
	TIM_SetCompare1(TIM4,PID_out);			//PB6的PWm输出
	
}







 int main(void)
 {		
// 	u16 led0pwmval=0;
//	u8 dir=1;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	TIM2_Int_Init(99,7199); //10ms=(99+1)*(7199+1)/72
	encoder_tim3_init();		//TIM3编码器模式初始化
	TIM4_PWM_Init(1000,0);	 //1KHZ PWM输出
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
   	while(1)
	{
		LED0=1;
		if(flag==1)
		{
			PID_control(encoder_num);
			flag=0;
		}
		
//		printf("%d rd\r\n",encoder_num);//打印
//		if(dir)led0pwmval++;
//		else led0pwmval--;
//		LED0=!LED0;
// 		if(led0pwmval>400)dir=0;
//		if(led0pwmval==0)dir=1;	
//		TIM_SetCompare1(TIM4,led0pwmval);	//PB6的PWm输出
	}	 
 }

