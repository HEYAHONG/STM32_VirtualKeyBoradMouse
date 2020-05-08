/*
 * KeyScan.c
 *
 *  Created on: 2020年5月8日
 *      Author: Administrator
 */

#include "main.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "stdbool.h"

void keyscan()//扫描按键并且执行动作
{
	{//扫描鼠标按键
		Mouse_Data data;
		Mouse_Set_Default(&data);
		bool R_click,M_click,L_cilck,R,L,U,D;

		R_click=M_click=L_cilck=R=L=U=D=false;

		{//第一次扫描
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)!=GPIO_PIN_RESET)
			{
				U=true;
			}

			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)!=GPIO_PIN_RESET)
			{
				D=true;
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)!=GPIO_PIN_RESET)
			{
				R=true;
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)!=GPIO_PIN_RESET)
			{
				L=true;
			}
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10)!=GPIO_PIN_RESET)
			{
				L_cilck=true;
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)!=GPIO_PIN_RESET)
			{
				M_click=true;
			}
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)!=GPIO_PIN_RESET)
			{
				R_click=true;
			}
		}

		HAL_Delay(1);//延时

		{//第二次扫描
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)!=GPIO_PIN_RESET)
			{
					if(U)
						Mouse_Set_Y(&data,-10);
			}

			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)!=GPIO_PIN_RESET)
			{
					if(D)
						Mouse_Set_Y(&data,10);
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)!=GPIO_PIN_RESET)
			{
					if(R)
						Mouse_Set_X(&data,10);
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)!=GPIO_PIN_RESET)
			{
					if(L)
						Mouse_Set_X(&data,-10);
			}
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10)!=GPIO_PIN_RESET)
			{
				if(L_cilck)
					data.flags.flags.Left_Key=1;
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)!=GPIO_PIN_RESET)
			{
				if(M_click)
					data.flags.flags.Middle_Key=1;
			}
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)!=GPIO_PIN_RESET)
			{
				if(R_click)
					data.flags.flags.Right_Key=1;
			}
		}

		Mouse_Send_Data(&data);

	}
}
