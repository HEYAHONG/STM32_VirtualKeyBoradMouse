/*
 * VKB_Config.c
 *
 *  Created on: 2020年5月11日
 *      Author: Administrator
 */

#include "VKBM_Config.h"
#include "usbd_hid.h"
void VKBM_KeyBoard_Receive(uint8_t *buf,size_t size)//键盘数据接收
{

}
void VKBM_Config_Receive(uint8_t *buf,size_t size)//配置数据接收
{

	//测试配置通道
	VKBM_Config_Send(buf,size);

}

extern USBD_HandleTypeDef hUsbDeviceFS;
void VKBM_Config_Send(uint8_t *buf,size_t size)//配置数据发送
{
	USBD_HID2_SendReport(&hUsbDeviceFS,buf,size>64?64:size);
}
