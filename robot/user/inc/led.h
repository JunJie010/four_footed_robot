#ifndef _LED_H
#define _LED_H


//���������ͺ궨��
#include "stm32f10x.h"                  // Device header



#define LED1_ON GPIO_ResetBits(GPIOA,GPIO_Pin_8); //�͵�ƽ����LED1
#define LED1_OFF GPIO_SetBits(GPIOA,GPIO_Pin_8);  //�ߵ�ƽϨ��LED1




void led_init(void);




#endif