/***************************************************************************************
 * 描述    ：W5500的端口0工作在客户端模式,主动与《TCP&UDP测试工具》上创建的服务端连接,
 *			 并且定时给服务端发送字符串"\r\nWelcome To YiXinElec!\r\n",同时将接
 *			 收到服务端发来的数据回发给服务端。
 * 实验平台：芯航线FPGA学习套件核心板 + 芯航线通信扩展卡模块（W5500）
 *
 * 淘宝    ：https://xiaomeige.taobao.com/
 ***************************************************************************************/

/*例程网络参数*/
//网关：192.168.90.1
//掩码:	255.255.255.0
//物理地址：0C 29 AB 7C 00 01
//本机IP地址:192.168.90.199
//端口0的端口号：5000
//端口0的目的IP地址：192.168.90.101
//端口0的目的端口号：6000

#include <string.h>
#include  <stdio.h>
#include  "system.h"
#include  "altera_avalon_pio_regs.h"
#include  "alt_types.h"
#include  "sys/alt_irq.h"
#include  "priv/alt_busy_sleep.h"
#include  "priv/alt_iic_isr_register.h"
#include "altera_avalon_spi_regs.h"  //定义了SPI寄存器的基本信息
#include "altera_avalon_spi.h"        //提供了访问SPI的函数声明
#include "W5500.h"

void Delay(unsigned int d); //延时函数(ms)

/*******************************************************************************
 * 函数名  : main
 * 描述    : 主函数，用户程序从main函数开始运行
 * 输入    : 无
 * 输出    : 无
 * 返回值  : int:返回值为一个16位整形数
 * 说明    : 无
 *******************************************************************************/
int main(void) {
	printf("hello");
	unsigned int W5500_Send_Delay_Counter = 0;

	Load_Net_Parameters(); //装载网络参数
	printf("1");
	W5500_Hardware_Reset(); //硬件复位W5500
	printf("2");
	W5500_Initialization(); //W5500初始化配置
	printf("3");

	while (1) {
		W5500_Socket_Set(); //W5500端口初始化配置

		W5500_Interrupt_Process(); //W5500中断处理程序框架

		if ((S0_Data & S_RECEIVE) == S_RECEIVE) //如果Socket0接收到数据
		{
			S0_Data &= ~S_RECEIVE;
			Process_Socket_Data(0); //W5500接收并发送接收到的数据
		} else if (W5500_Send_Delay_Counter >= 30000) //定时发送字符串
				{
			if (S0_State == (S_INIT | S_CONN)) {
				S0_Data &= ~S_TRANSMITOK;
				memcpy(Tx_Buffer, "\r\nhello, I am Xiao Meige\r\n", 25);
				Write_SOCK_Data_Buffer(0, Tx_Buffer, 25);//指定Socket(0~7)发送数据处理,端口0发送23字节数据
			}
			W5500_Send_Delay_Counter = 0;
		}
		W5500_Send_Delay_Counter++;
	}
}

/*******************************************************************************
 * 函数名  : Delay
 * 描述    : 延时函数(ms)
 * 输入    : d:延时系数，单位为毫秒
 * 输出    : 无
 * 返回    : 无
 * 说明    :
 *******************************************************************************/
void Delay(unsigned int x) {
	unsigned int i, j;

	for (j = 0; j < 500; j++)
		for (i = 0; i < x; i++)
			;
}
