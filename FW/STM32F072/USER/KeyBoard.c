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

uint8_t KeyBoard_Set_key(KeyBoard_Data *p,uint8_t key)//设置一个普通按键，成功返回非0
{
if(key==0) return 1;
for(uint8_t i=0;i<6;i++)
{
	if(p->Key[i]==0x00)
	{
		p->Key[i]=key;
		return 1;
	}
}
return 0;
}
extern USBD_HandleTypeDef hUsbDeviceFS;
//发送按键
void KeyBoard_Send_Data(KeyBoard_Data *p)
{
	USBD_HID1_SendReport(&hUsbDeviceFS,(uint8_t *)p,sizeof(KeyBoard_Data));
}
