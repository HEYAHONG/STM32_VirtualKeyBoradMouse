/*
 * KeyBoard.h
 *
 *  Created on: 2020年4月26日
 *      Author: Administrator
 */

#ifndef USER_KEYBOARD_H_
#define USER_KEYBOARD_H_
#include "stdint.h"

typedef union
{
uint8_t byte;
struct
{
uint8_t Left_Control:1;
uint8_t Left_Shift:1;
uint8_t Left_Alt:1;
uint8_t Left_GUI:1;
uint8_t Right_Control:1;
uint8_t Right_Shift:1;
uint8_t Right_Alt:1;
uint8_t Right_GUI:1;
} key;
} KB_Special_Key;//特殊按键定义


typedef struct
{
	KB_Special_Key SKey;//特殊按键
	uint8_t		   resv;//保留,恒为0
	uint8_t		   Key[6];//普通按键
} KeyBoard_Data;//8字节数据

//下面是一些辅助函数
//初始化KeyBoard_Data
void KeyBoard_Set_Default(KeyBoard_Data *p);

//发送按键
void KeyBoard_Send_Data(KeyBoard_Data *p);



#endif /* USER_KEYBOARD_H_ */
