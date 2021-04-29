#include "TIM2.h"
#include "TIM3.h"
#include "led.h"
#include "usart.h"

extern int flag;

void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
}


//void PID_control(int exp_spd)
//{
//	int PID_out;					//PID输出的值
//	static int error_pre=0;			//上一次的偏差
//	static int error_now=0;			//这一次的偏差
//	static int error_iteg=0;		//积分项
//	
//	spd_now=exp_spd*600/28;			//电机转一圈7个脉冲，一个脉冲读取4次，但是单位时间10ms内，转化为分钟每转要乘600
//	printf("%d rd\r\n",spd_now);		//打印现在速度
//	error_now=exp_spd-spd_now;			//现在误差定义
//	error_iteg+=error_now;				//加法替代积分
//	PID_out=Kp*error_now+Ki*error_iteg+Kd*(error_now-error_pre);	//灵魂公式
//	error_pre=error_now;				//这场的现在误差作为下次的上次误差
//	if(PID_out>9999)	PID_out=9999;
//	if(PID_out<-9999)	PID_out=-9999;
//	printf("%d PID\r\n",PID_out);			//打印PID输出的值

//}

void TIM2_IRQHandler(void)   //TIM2中断函数
{
	
		extern int encoder_num;
		extern int flag;
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)  //检查TIM3更新中断发生与否
		{
			encoder_num = (int)((int16_t)(TIM3->CNT)); // 读取转速（注意数据结构）
			TIM_SetCounter(TIM3, 0);
			flag=1;
			LED1=!LED1;						//验证是否正常工作
		}
				TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除标志位

	
}



