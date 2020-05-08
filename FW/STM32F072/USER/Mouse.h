/*
 * Mouse.h
 *
 *  Created on: 2020年5月8日
 *      Author: Administrator
 */

#ifndef MOUSE_H_
#define MOUSE_H_
#include "stdint.h"
typedef union
{
	uint8_t byte;
	struct {
	uint8_t Left_Key:1;
	uint8_t Right_Key:1;
	uint8_t Middle_Key:1;
	uint8_t Resv:1;//保留，恒为1
	uint8_t X_Sign:1;//X变化的符号位
	uint8_t Y_Sign:1;//Y变化的符号位
	uint8_t X_overflow:1;//X溢出
	uint8_t Y_overflow:1;//Y溢出
	} flags;
} Mouse_Flags;

typedef struct {
	Mouse_Flags flags;
	uint8_t     X_Val;
	uint8_t		Y_Val;
	int8_t		wheel;//滚轮
} Mouse_Data;

//下面是一些辅助函数

//设置X的值
void Mouse_Set_X(Mouse_Data *p,int16_t X);
//设置Y的值
void Mouse_Set_Y(Mouse_Data *p,int16_t Y);
//恢复默认设置
void Mouse_Set_Default(Mouse_Data *p);

//发送鼠标数据
void Mouse_Send_Data(Mouse_Data *p);
#endif /* MOUSE_H_ */
