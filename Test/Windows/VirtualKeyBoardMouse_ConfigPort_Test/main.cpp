#include <iostream>
#include "libusb.h"
#include "stdio.h"

/*
本程序主要测试处于配置模式(需安装配置模式驱动，鼠标键盘将失效)下的VirtualKeyBoardMouse的配置接口。
*/
using namespace std;

#define VID 0x483
#define PID 0x572b
#define Config_Interface 0x02
#define OUT_EndPoint 0x04
#define IN_EndPoint 0x85


int main()
{
    //初始化libusb库
    int r;
    r = libusb_init(NULL);
    if(r<0)
        return r;
    //设置libusb debug信息
    libusb_set_debug(NULL,0);

    //打开设备
    libusb_device_handle *hdev=NULL;
    if((hdev=libusb_open_device_with_vid_pid(NULL,VID,PID))==NULL)
    {
        printf("请检查硬件和驱动是否安装正确!\n\r");
        libusb_exit(NULL);
        return -1;
    }

    //声明端口
    libusb_claim_interface(hdev,Config_Interface);

    {//测设读写
        unsigned char out[32]="0123456789012345678",in[32]="";
        int count=0;
        if(libusb_bulk_transfer(hdev,OUT_EndPoint,out,sizeof(out),&count,1000)==0)
            printf("\n\r发送数据成功 - %d 字节\n\r",count);
        count=0;
        if(libusb_bulk_transfer(hdev,IN_EndPoint,in,sizeof(in),&count,4000)==0)
        {
            printf("\n\r接收数据成功 - %d 字节\n\r",count);
            printf((char *)in);
        }

    }

    //关闭USB设备
    libusb_close(hdev);

    //退出libusb
    libusb_exit(NULL);
    return 0;
}
