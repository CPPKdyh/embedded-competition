#include "uart.h"
#include <string.h>
#include "OLED.h"

int cx=0;//中心坐标x
int cy=0;//中心坐标y
int  color_type = 0;//颜色类型
uint8_t rx_flag=0;
double center_x_cm = 0;
double center_y_cm = 0;

/*

USART配置:
	通过结构体USART_InitTypeDef来确定。UART模式下的字段如下：
	USART_boundRate：波特率（每秒能传输的数据位），缺省值为9600。
	USART_WordLength：字长
	USART_StopBits：停止位
	USART_Parity：校验方式（奇偶校验）
	USART_HardwareFlowControl：硬件流控制
	USART_Mode：单/双工，即收发状态。
	最后通过USART_Init()来设置。
*/

void UART1_Init(int bound)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//打开UART时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//打开GPIOA时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无奇偶校验位
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送接收模式
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//八个数据位
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级位2
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//打开串口1的中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);//串口使能
}

void UART3_Init(int bound)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//打开UART时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//打开GPIOB时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无奇偶校验位
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送接收模式
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//八个数据位
	USART_Init(USART3,&USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级位2
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//打开串口1的中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3,ENABLE);//串口使能
}

//重写printf函数(发送)
int fputc(int ch,FILE *stream)
{
	USART_SendData(USART3,ch);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	return ch;
}

