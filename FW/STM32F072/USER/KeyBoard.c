/*
 * KeyBoard.c
 *
 *  Created on: 2020年5月8日
 *      Author: Administrator
 */


#include "KeyBoard.h"
#include "string.h"
#include "usbd_hid.h"

//初始化KeyBoard_Data
void KeyBoard_Set_Default(KeyBoard_Data *p)
{
	memset(p,0,sizeof(KeyBoard_Data));
}

extern USBD_HandleTypeDef hUsbDeviceFS;
//发送按键
void KeyBoard_Send_Data(KeyBoard_Data *p)
{
	USBD_HID1_SendReport(&hUsbDeviceFS,(uint8_t *)p,sizeof(KeyBoard_Data));
}
