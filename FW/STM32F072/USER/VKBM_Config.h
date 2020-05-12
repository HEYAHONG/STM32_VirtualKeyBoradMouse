/*
 * VKB_Config.h
 *
 *  Created on: 2020年5月11日
 *      Author: Administrator
 */

#ifndef VKBM_CONFIG_H_
#define VKBM_CONFIG_H_
#include "main.h"
#include "stdbool.h"
void VKBM_KeyBoard_Receive(uint8_t *buf,size_t size);//键盘数据接收
void VKBM_Config_Receive(uint8_t *buf,size_t size);//配置数据接收
void VKBM_Config_Send(uint8_t *buf,size_t size);//配置数据发送


#endif /* VKB_CONFIG_H_ */
