#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"

//powerd by xujunjie

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值  
extern u8  TIM2CH4_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM2CH4_CAPTURE_VAL;	//输入捕获值 
	
double PWMZkba;
double PWMZkbb;
int a;
double  b;
double c;
int cao;
int usart_handle(void);

int min=1000,max=2000,fre=10;
int flag;
int main(void)
{ 
	long long temp=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
	
	
	
 	TIM14_PWM_Init(10000-1,840-1); 
	TIM13_PWM_Init(10000-1,840-1);	
 	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); 
	TIM2_CH4_Cap_Init(0XFFFFFFFF,84-1); 
   	while(1)
	{		
		if(USART_RX_STA&0x8000)	
		{		
			cao=usart_handle();
			if(flag==1)
			{
				min=cao;
				printf("min:%d \r\n",cao);
			}
			else if(flag==2)
			{
				max=cao;
				printf("max:%d \r\n",cao);
			}
			else if(flag==3)
			{
				if(cao<5)
				{
				cao=5;
				}
				if(cao>100)
				{
				cao=100;
				}
				c=(double)8400/cao;
				TIM14_PWM_Init(10000-1,c-1); 
				TIM13_PWM_Init(10000-1,c-1); 
				printf("fre:%d \r\n",cao);
			}
			
			
		}			
 		if(TIM5CH1_CAPTURE_STA&0X80)        
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;		   
			
			
			
			printf("HIGH5:%lld us\r\n",temp); //打印总的高电平时间
			
			if(temp<=min)
			{
				temp=min;
			}
			if(temp>=max)
			{
				temp=max;
			}
			a=temp-min;
			b=(double)a/(max-min);
			PWMZkba=1-b;
			
			TIM_SetCompare1(TIM14,10000*PWMZkba);
			
			TIM5CH1_CAPTURE_STA=0;			    
		}
		
		
		if(TIM2CH4_CAPTURE_STA&0X80)        
		{
			temp=TIM2CH4_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //溢出时间总和
			temp+=TIM2CH4_CAPTURE_VAL;		   
			
			
			
			printf("HIGH2:%lld us\r\n",temp); //打印总的高电平时间
			
			if(temp<=min)
			{
				temp=min;
			}
			if(temp>=max)
			{
				temp=max;
			}
			a=temp-min;
			b=(double)a/(max-min);
			PWMZkbb=1-b;
			
			TIM_SetCompare1(TIM13,10000*PWMZkbb);
			
			TIM2CH4_CAPTURE_STA=0;			    
		}
		
		
		
		
		
	}
}

int usart_handle(void)
{
	int fly;
	int fuck=0;
	u8 t;
	u8 len;	
	int fh;
	
	if(USART_RX_STA&0x8000)
		{			
			len=USART_RX_STA&0x3fff;
			
			if(USART_RX_BUF[0]=='i')
					{
						//fh=-1;
						fly=0;
						flag=1;
					}
					else if(USART_RX_BUF[0]=='a')
					{
					flag=2;
						fly=0;
					}
					else if(USART_RX_BUF[0]=='f')
					{
					flag=3;
						fly=0;
					}
					for(t=1;t<len;t++)
					{
						
						fly=(fly*10+ (USART_RX_BUF[t]-48));
						fuck=fly;
						
						
					}
					
					USART_RX_STA=0;
				}
			
				 
			return fuck;
			
}
