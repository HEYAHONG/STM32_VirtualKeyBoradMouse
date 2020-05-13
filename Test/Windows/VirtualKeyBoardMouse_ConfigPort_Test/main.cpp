#include <iostream>
#include "libusb.h"
#include "stdio.h"

/*
��������Ҫ���Դ�������ģʽ(�谲װ����ģʽ�����������̽�ʧЧ)�µ�VirtualKeyBoardMouse�����ýӿڡ�
*/
using namespace std;

#define VID 0x483
#define PID 0x572b
#define Config_Interface 0x02
#define OUT_EndPoint 0x04
#define IN_EndPoint 0x85


int main()
{
    //��ʼ��libusb��
    int r;
    r = libusb_init(NULL);
    if(r<0)
        return r;
    //����libusb debug��Ϣ
    libusb_set_debug(NULL,0);

    //���豸
    libusb_device_handle *hdev=NULL;
    if((hdev=libusb_open_device_with_vid_pid(NULL,VID,PID))==NULL)
    {
        printf("����Ӳ���������Ƿ�װ��ȷ!\n\r");
        libusb_exit(NULL);
        return -1;
    }

    //�����˿�
    libusb_claim_interface(hdev,Config_Interface);

    {//�����д
        unsigned char out[32]="0123456789012345678",in[32]="";
        int count=0;
        if(libusb_bulk_transfer(hdev,OUT_EndPoint,out,sizeof(out),&count,1000)==0)
            printf("\n\r�������ݳɹ� - %d �ֽ�\n\r",count);
        count=0;
        if(libusb_bulk_transfer(hdev,IN_EndPoint,in,sizeof(in),&count,4000)==0)
        {
            printf("\n\r�������ݳɹ� - %d �ֽ�\n\r",count);
            printf((char *)in);
        }

    }

    //�ر�USB�豸
    libusb_close(hdev);

    //�˳�libusb
    libusb_exit(NULL);
    return 0;
}
