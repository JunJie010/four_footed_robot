#ifndef _MOTOR_H
#define _MOTOR_H

//���������ͺ궨��
#include "stm32f10x.h"                  // Device header
#include "stdbool.h"

#define MOTOR_POWER_ON  (GPIO_SetBits(GPIOA,GPIO_Pin_15))
#define MOTOR_POWER_OFF (GPIO_ResetBits(GPIOA,GPIO_Pin_15))

//  -----               -----
// |  4  |             |  0  |
// |pin8 |             |Pin6 |
//  ----- -----   ----- -----
//       |  5  | |  1  |
//       |Pin1 | |Pin4 |
//        -----   -----
//       |  6  | |  2  |
//       |Pin7 ||Pin5 |
//  ----- -----   ----- -----
// |  7  |             |  3  |
// |Pin2 |             |Pin3 |
//  -----               -----

typedef enum
{
	ARM_RIGHT_BEFORE_L = 0,
	ARM_RIGHT_BEFORE_H,
	ARM_RIGHT_AFTER_H,
	ARM_RIGHT_AFTER_L,
	ARM_LEFT_BEFORE_L,
	ARM_LEFT_BEFORE_H,
	ARM_LEFT_AFTER_H,
	ARM_LEFT_AFTER_L,
}ArmDef;

typedef enum
{
	ARM_BODYDTOP = 0,
	ARM_GOON,
	ARM_BACKWARD,
	ARM_TURNLEFT,
	ARM_TURNRIGHT,
	ARM_TEIST,
	ARM_PERISTALSIS,
	ARM_JUMP,
	ARM_SAYHELLO,
	ARM_PUSHUP
}Arm_StateDef;

typedef struct
{
	uint32_t arm_time5;
	uint32_t arm_time10;
	uint32_t arm_time20;
	//���ԳŶ�����־
	bool action_push_up;
	bool action_push_down;
	//���к�������־
	bool action_sayhello_up;
	bool action_sayhello_down;
	//����������־
	bool action_jump_up;
	bool action_jump_down;
	//�䶯������־
	bool action_Peristalsis_up;
	bool action_Peristalsis_down;
	//Ť��������־
	bool action_twisting_up;
	bool action_twisting_down;
	//��ת������־
	bool action_left_up;
	bool action_left_down;
	//��ת������־
	bool action_right_up;
	bool action_right_down;
	//���˶�����־
	bool action_back_up;
	bool action_back_down;
	//ǰ��������־
	bool action_forward_up;
	bool action_forward_down;
}Arm_Countinfo;


extern Arm_Countinfo armflag;


#define ARM_JUMP    1u
#define ARM_PUSHUP  1u
#define ARM_SAYHELLO 1u
#define ARM_TEIST   1u
#define ARM_PERISTALSIS 1u
#define ARM_TURNLEFT  1u
#define ARM_TURNRIGHT 1u


void ArmCtrl_Config(void);
void ArmCtrl_Angle (ArmDef ArmNum, float Angle);
void Arm_Zero(void);
void Arm_Li(void);
void Arm_Jump(void);
void Arm_Push(void);
void Arm_Hello(void);          //���к�
void Arm_Twisting(void);
void Arm_Peristalsis(void);    
void Arm_Left(void);           //��ת
void Arm_Right(void);          //��ת
void Arm_ss(void);
void Arm_Forward(void);        //ǰ��


void MOTOR_Power(void);











#endif
