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

//一般按键的枚举变量0x00(No Event)到0x65(APP)
typedef enum
{
	NO_EVENT=0x00,
	Overrun_Error=0x01,
	POST_Fail=0x02,
	ErrorUndefined=0x03,
	a_A=0x04,
	b_B=0x05,
	c_C=0x06,
	d_D=0x07,
	e_E=0x08,
	f_F=0x09,
	g_G=0x0a,
	h_H=0x0b,
	i_I=0x0c,
	j_J=0x0d,
	k_K=0x0e,
	l_L=0x0f,
	m_M=0x10,
	n_N=0x11,
	o_O=0x12,
	p_P=0x13,
	q_Q=0x14,
	r_R=0x15,
	s_S=0x16,
	t_T=0x17,
	u_U=0x18,
	v_V=0x19,
	w_W=0x1a,
	x_X=0x1b,
	y_Y=0x1c,
	z_Z=0x1d,
	Key_num_1=0x1e,//1!
	Key_num_2=0x1f,//2@
	Key_num_3=0x20,//3#
	Key_num_4=0x21,//4$
	Key_num_5=0x22,//5%
	Key_num_6=0x23,//6^
	Key_num_7=0x24,//7&
	Key_num_8=0x25,//8*
	Key_num_9=0x26,//9(
	Key_num_0=0x27,//0)
	Return=0x28,
	Escape=0x29,
	Backspace=0x2A,
	Tab=0x2B,
	Space=0x2c,
	Key_2D=0x2d,//-_
	Key_2E=0x2e,//=+
	Key_2F=0x2f,//[{
	Key_30=0x30,//]}
	Key_31=0x31,//\|
	Europe_1=0x32,
	Key_33=0x33,// ;:
	Key_34=0x34,// '"
	Key_35=0x35,// `~
	Key_36=0x36,//,<
	Key_37=0x37,//.>
	Key_38=0x38,///?
	Caps_Lock=0x39,
	F1=0x3a,
	F2=0x3b,
	F3=0x3c,
	F4=0x3d,
	F5=0x3e,
	F6=0x3f,
	F7=0x40,
	F8=0x41,
	F9=0x42,
	F10=0x43,
	F11=0x44,
	F12=0x45,
	Print_Screen=0x46,
	Scroll_Lock=0x47,
	Break=0x48,//Ctrl-Pause
	Pause=0x48,
	Insert=0x49,
	Home=0x4a,
	Page_Up=0x4b,
	Delete=0x4c,
	End=0x4d,
	Page_Down=0x4e,
	Right_Arrow=0x4f,
	Left_Arrrow=0x50,
	Down_Arrow=0x51,
	Up_Arrow=0x52,
	Num_Lock=0x53,
	Key_Keypad_54=0x54,// /
	Key_Keypad_55=0x55,// *
	Key_Keypad_56=0x56,// -
	Key_Keypad_57=0x57,// +
	Key_Keypad_Enter=0x58,
	Key_Keypad_num_1=0x59,//End
	Key_Keypad_num_2=0x5a,//Down
	Key_Keypad_num_3=0x5b,//PageDn
	Key_Keypad_num_4=0x5c,//Left
	Key_Keypad_num_5=0x5d,
	Key_Keypad_num_6=0x5e,//Right
	Key_Keypad_num_7=0x5f,//Home
	Key_Keypad_num_8=0x60,//Up
	Key_Keypad_num_9=0x61,//PageUp
	Key_Keypad_num_0=0x62,//Insert
	Key_Keypad_dot=0x63,//delete
	Europe_2=0x64,
	App=0x65
	//HID描述符规定的最大按键为0x65

} KB_Key;

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

uint8_t KeyBoard_Set_key(KeyBoard_Data *p,uint8_t key);//设置一个普通按键，成功返回非0



#endif /* USER_KEYBOARD_H_ */
