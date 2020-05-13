/*
 * VKB_Config.c
 *
 *  Created on: 2020年5月11日
 *      Author: Administrator
 */

#include "VKBM_Config.h"
#include "usbd_hid.h"

bool config_mode=false;
void VKBM_KeyBoard_Receive(uint8_t *buf,size_t size)//键盘数据接收
{

}
void VKBM_Config_Receive(uint8_t *buf,size_t size)//配置数据接收
{
	//进入配置模式,停用按键扫描
	config_mode=true;

	//测试配置通道
	VKBM_Config_Send(buf,size);

}

extern USBD_HandleTypeDef hUsbDeviceFS;
void VKBM_Config_Send(uint8_t *buf,size_t size)//配置数据发送
{
	USBD_HID2_SendReport(&hUsbDeviceFS,buf,size>64?64:size);
}

void VKBM_Config_Send_Finished()//发送完成
{
	//config_mode=false;
}
