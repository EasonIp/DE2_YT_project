/***************************************************************************************
 * ����    ��W5500�Ķ˿�0�����ڿͻ���ģʽ,�����롶TCP&UDP���Թ��ߡ��ϴ����ķ��������,
 *			 ���Ҷ�ʱ������˷����ַ���"\r\nWelcome To YiXinElec!\r\n",ͬʱ����
 *			 �յ�����˷��������ݻط�������ˡ�
 * ʵ��ƽ̨��о����FPGAѧϰ�׼����İ� + о����ͨ����չ��ģ�飨W5500��
 *
 * �Ա�    ��https://xiaomeige.taobao.com/
 ***************************************************************************************/

/*�����������*/
//���أ�192.168.90.1
//����:	255.255.255.0
//������ַ��0C 29 AB 7C 00 01
//����IP��ַ:192.168.90.199
//�˿�0�Ķ˿ںţ�5000
//�˿�0��Ŀ��IP��ַ��192.168.90.101
//�˿�0��Ŀ�Ķ˿ںţ�6000

#include <string.h>
#include  <stdio.h>
#include  "system.h"
#include  "altera_avalon_pio_regs.h"
#include  "alt_types.h"
#include  "sys/alt_irq.h"
#include  "priv/alt_busy_sleep.h"
#include  "priv/alt_iic_isr_register.h"
#include "altera_avalon_spi_regs.h"  //������SPI�Ĵ����Ļ�����Ϣ
#include "altera_avalon_spi.h"        //�ṩ�˷���SPI�ĺ�������
#include "W5500.h"

void Delay(unsigned int d); //��ʱ����(ms)

/*******************************************************************************
 * ������  : main
 * ����    : ���������û������main������ʼ����
 * ����    : ��
 * ���    : ��
 * ����ֵ  : int:����ֵΪһ��16λ������
 * ˵��    : ��
 *******************************************************************************/
int main(void) {
	printf("hello");
	unsigned int W5500_Send_Delay_Counter = 0;

	Load_Net_Parameters(); //װ���������
	printf("1");
	W5500_Hardware_Reset(); //Ӳ����λW5500
	printf("2");
	W5500_Initialization(); //W5500��ʼ������
	printf("3");

	while (1) {
		W5500_Socket_Set(); //W5500�˿ڳ�ʼ������

		W5500_Interrupt_Process(); //W5500�жϴ���������

		if ((S0_Data & S_RECEIVE) == S_RECEIVE) //���Socket0���յ�����
		{
			S0_Data &= ~S_RECEIVE;
			Process_Socket_Data(0); //W5500���ղ����ͽ��յ�������
		} else if (W5500_Send_Delay_Counter >= 30000) //��ʱ�����ַ���
				{
			if (S0_State == (S_INIT | S_CONN)) {
				S0_Data &= ~S_TRANSMITOK;
				memcpy(Tx_Buffer, "\r\nhello, I am Xiao Meige\r\n", 25);
				Write_SOCK_Data_Buffer(0, Tx_Buffer, 25);//ָ��Socket(0~7)�������ݴ���,�˿�0����23�ֽ�����
			}
			W5500_Send_Delay_Counter = 0;
		}
		W5500_Send_Delay_Counter++;
	}
}

/*******************************************************************************
 * ������  : Delay
 * ����    : ��ʱ����(ms)
 * ����    : d:��ʱϵ������λΪ����
 * ���    : ��
 * ����    : ��
 * ˵��    :
 *******************************************************************************/
void Delay(unsigned int x) {
	unsigned int i, j;

	for (j = 0; j < 500; j++)
		for (i = 0; i < x; i++)
			;
}