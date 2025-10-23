#ifndef _WIFI_H
#define _WIFI_H

//���������ͺ궨��
#include "stm32f10x.h"                  // Device header

typedef struct{
	
	u8 buff[512];   //�����ַ���
	u16 len;        //��¼�������ݳ���
	u8 flag;        //������ɱ�־λ

}U2_STRUCT;

extern U2_STRUCT u2;

u8 ESP8266_Init(void);
void usart2_send_str(u8 *data);
u8 ESP8266_ConnectWifi(u8 *ssid,u8 *pwd);
u8 ESP8266_ConnectServer(u8 *ip,u8 *port);
u8 Esp8266_ConnectServer(u8 *ip,u8 *port);
#endif
