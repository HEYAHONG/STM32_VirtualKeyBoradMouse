/*
 * Mouse.c
 *
 *  Created on: 2020年5月8日
 *      Author: Administrator
 */

#include "Mouse.h"
#include "usbd_hid.h"

//设置X的值
void Mouse_Set_X(Mouse_Data *p,int16_t X)
{
	//设置一些应该不变的值
	p->flags.flags.X_overflow=0;
	p->flags.flags.Resv=1;

	if(X<0)
	{
		p->flags.flags.X_Sign=1;
	}
	else
	{
		p->flags.flags.X_Sign=0;
	}

	int16_t X_data=X;
	uint16_t *X_p=&X_data;

	p->X_Val=((*X_p)&0xff);
}
//设置Y的值
void Mouse_Set_Y(Mouse_Data *p,int16_t Y)
{
	//设置一些应该不变的值
	p->flags.flags.Y_overflow=0;
	p->flags.flags.Resv=1;

	if(Y<0)
	{
		p->flags.flags.Y_Sign=1;
	}
	else
	{
		p->flags.flags.Y_Sign=0;
	}
	int16_t Y_data=Y;
	uint16_t *Y_p=&Y_data;

	p->Y_Val=((*Y_p)&0xff);
}

//恢复默认设置
void Mouse_Set_Default(Mouse_Data *p)
{
	p->wheel=0;
	p->flags.flags.Left_Key=0;
	p->flags.flags.Middle_Key=0;
	p->flags.flags.Right_Key=0;

	Mouse_Set_X(p,0);
	Mouse_Set_Y(p,0);
}

extern USBD_HandleTypeDef hUsbDeviceFS;
//发送鼠标数据
void Mouse_Send_Data(Mouse_Data *p)
{
	USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t *)p,sizeof(Mouse_Data));
}
