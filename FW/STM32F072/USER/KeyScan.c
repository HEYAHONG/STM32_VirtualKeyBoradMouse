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
#include "stdint.h"

static void KB_OUT_Output(uint16_t _pin)//参数为GPIO_PIN_X(X为0~9)
{
	for(size_t i=0;i<10;i++)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0 << i,GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(GPIOA,_pin,GPIO_PIN_RESET);

}

static uint16_t KB_IN_Input()//返回的数据每位表示一位按键被按下(0~10)
{
	uint16_t ret=0;
	for(size_t i=0;i<11;i++)
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3<<i)==GPIO_PIN_RESET)
		{
			ret|=(1<<i);
		}
	}
	return ret;
}

static  KeyBoard_Data  kb_data;

static void KB_Key_Code_Process(uint8_t key)
{//按键编码处理,此处仅用于测试
/*
	if(key <= 0x65)
	{
		//KeyBoard_Set_key(&kb_data,key);
	}
*/
}

static void keyboard_key_scan()
{
	KeyBoard_Set_Default(&kb_data);
	for(uint16_t i=0;i<10;i++)
	{
		KB_OUT_Output(GPIO_PIN_0<<i);
		HAL_Delay(1);
		uint16_t input=KB_IN_Input();
		HAL_Delay(1);
		input&=KB_IN_Input();
		for(uint16_t j=0;j<11;j++)
		{
			if(input&(1<<j))
			{
				//按键编码方式，KB_OUT+KB_IN*10
				KB_Key_Code_Process(j*10+i);
			}
		}
	}
	KeyBoard_Send_Data(&kb_data);
}


static void mouse_key_scan()
{
	{//扫描鼠标按键
		static Mouse_Data data;
		Mouse_Set_Default(&data);
		bool R_click,M_click,L_cilck,R,L,U,D;//按键是否按下的标志
		static uint32_t R_Step=1,L_Step=1,U_Step=1,D_Step=1;//每次按键前进的步长
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
					{
						Mouse_Set_Y(&data,-(U_Step++));
					}
					else
					{
						U_Step=1;
					}
			}
			else
			{
				U_Step=1;
			}

			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)!=GPIO_PIN_RESET)
			{
					if(D)
					{
						Mouse_Set_Y(&data,D_Step++);
					}
					else
					{
						D_Step=1;
					}
			}
			else
			{
				D_Step=1;
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)!=GPIO_PIN_RESET)
			{
					if(R)
					{
						Mouse_Set_X(&data,R_Step++);
					}
					else
					{
						R_Step=1;
					}
			}
			else
			{
				R_Step=1;
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)!=GPIO_PIN_RESET)
			{
					if(L)
					{
						Mouse_Set_X(&data,-(L_Step++));
					}
					else
					{
						L_Step=1;
					}
			}
			else
			{
				L_Step=1;
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

extern bool config_mode;
void keyscan()//扫描按键并且执行动作
{

if(!config_mode)	mouse_key_scan();//扫描鼠标按键
if(!config_mode)	keyboard_key_scan();
}
