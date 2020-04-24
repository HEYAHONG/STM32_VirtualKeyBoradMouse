/**
  ******************************************************************************
  * @file    usbd_hid.c
  * @author  MCD Application Team
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                HID Class  Description
  *          ===================================================================
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick
  *             - Collection : Application
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid.h"
#include "usbd_ctlreq.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_HID
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_HID_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_HID_Private_Defines
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_HID_Private_Macros
  * @{
  */
/**
  * @}
  */




/** @defgroup USBD_HID_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_HID_Init(USBD_HandleTypeDef *pdev,
                              uint8_t cfgidx);

static uint8_t  USBD_HID_DeInit(USBD_HandleTypeDef *pdev,
                                uint8_t cfgidx);

static uint8_t  USBD_HID_Setup(USBD_HandleTypeDef *pdev,
                               USBD_SetupReqTypedef *req);

static uint8_t  *USBD_HID_GetFSCfgDesc(uint16_t *length);

static uint8_t  *USBD_HID_GetHSCfgDesc(uint16_t *length);

static uint8_t  *USBD_HID_GetOtherSpeedCfgDesc(uint16_t *length);

static uint8_t  *USBD_HID_GetDeviceQualifierDesc(uint16_t *length);

static uint8_t  USBD_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
/**
  * @}
  */

/** @defgroup USBD_HID_Private_Variables
  * @{
  */

USBD_ClassTypeDef  USBD_HID =
{
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL, /*EP0_TxSent*/
  NULL, /*EP0_RxReady*/
  USBD_HID_DataIn, /*DataIn*/
  USBD_HID_DataOut, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,
  USBD_HID_GetHSCfgDesc,
  USBD_HID_GetFSCfgDesc,
  USBD_HID_GetOtherSpeedCfgDesc,
  USBD_HID_GetDeviceQualifierDesc,
};


//修改FS下的设置

/* USB HID device FS Configuration Descriptor */
__ALIGN_BEGIN static  uint8_t USBD_HID_CfgFSDesc[USB_HID_CONFIG_DESC_SIZ]  __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x03,         /*bNumInterfaces: 31 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xE0,         /*bmAttributes: bus powered and Support Remote Wake-up */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

  /************** Descriptor of Joystick Mouse interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x01,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: HID*/
  0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x02,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Joystick Mouse HID ********************/
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Mouse endpoint ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

  HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_EPIN_SIZE, /*wMaxPacketSize: 4 Byte max */
  0x00,
  HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
  /* 34 */
//添加新接口interface
    0x09,         /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
    0x01,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/

    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    HID_KeyBoard_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
    0x00,

    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

    HID_EPIN1_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    HID_EPIN1_SIZE, /*wMaxPacketSize: 4 Byte max */
    0x00,
    HID_FS_BINTERVAL,          /*bInterval: Polling Interval */

	 0x07,          /*bLength: Endpoint Descriptor size*/
	 USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

	 HID_EPOUT1_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
	 0x03,          /*bmAttributes: Interrupt endpoint*/
	 HID_EPOUT1_SIZE, /*wMaxPacketSize: 4 Byte max */
	 0x00,
	 HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
 //添加新接口interface
	     0x09,         /*bLength: Interface Descriptor size*/
	     USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
	     0x02,         /*bInterfaceNumber: Number of Interface*/
	     0x00,         /*bAlternateSetting: Alternate setting*/
	     0x02,         /*bNumEndpoints*/
	     0x03,         /*bInterfaceClass: HID*/
	     0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
	     0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	     0,            /*iInterface: Index of string descriptor*/

	     0x09,         /*bLength: HID Descriptor size*/
	     HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
	     0x11,         /*bcdHID: HID Class Spec release number*/
	     0x01,
	     0x00,         /*bCountryCode: Hardware target country*/
	     0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
	     0x22,         /*bDescriptorType*/
	     HID_Config_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
	     0x00,

	     0x07,          /*bLength: Endpoint Descriptor size*/
	     USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

	     HID_EPIN2_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
	     0x03,          /*bmAttributes: Interrupt endpoint*/
	     HID_EPIN2_SIZE, /*wMaxPacketSize: 4 Byte max */
	     0x00,
	     HID_FS_BINTERVAL,          /*bInterval: Polling Interval */

	 	 0x07,          /*bLength: Endpoint Descriptor size*/
	 	 USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

	 	 HID_EPOUT2_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
	 	 0x03,          /*bmAttributes: Interrupt endpoint*/
	 	 HID_EPOUT2_SIZE, /*wMaxPacketSize: 4 Byte max */
	 	 0x00,
	 	 HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
};

/* USB HID device HS Configuration Descriptor */
__ALIGN_BEGIN static  uint8_t USBD_HID_CfgHSDesc[USB_HID_CONFIG_DESC_SIZ]  __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x03,         /*bNumInterfaces: 3 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xE0,         /*bmAttributes: bus powered and Support Remote Wake-up */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

  /************** Descriptor of Joystick Mouse interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x01,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: HID*/
  0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x02,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Joystick Mouse HID ********************/
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Mouse endpoint ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

  HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_EPIN_SIZE, /*wMaxPacketSize: 4 Byte max */
  0x00,
  HID_HS_BINTERVAL,          /*bInterval: Polling Interval */
  /* 34 */

  //添加新接口interface
      0x09,         /*bLength: Interface Descriptor size*/
      USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
      0x01,         /*bInterfaceNumber: Number of Interface*/
      0x00,         /*bAlternateSetting: Alternate setting*/
      0x02,         /*bNumEndpoints*/
      0x03,         /*bInterfaceClass: HID*/
      0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
      0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
      0,            /*iInterface: Index of string descriptor*/

      0x09,         /*bLength: HID Descriptor size*/
      HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
      0x11,         /*bcdHID: HID Class Spec release number*/
      0x01,
      0x00,         /*bCountryCode: Hardware target country*/
      0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
      0x22,         /*bDescriptorType*/
	  HID_KeyBoard_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
      0x00,

      0x07,          /*bLength: Endpoint Descriptor size*/
      USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

      HID_EPIN1_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
      0x03,          /*bmAttributes: Interrupt endpoint*/
      HID_EPIN1_SIZE, /*wMaxPacketSize: 4 Byte max */
      0x00,
      HID_HS_BINTERVAL,          /*bInterval: Polling Interval */

  	 0x07,          /*bLength: Endpoint Descriptor size*/
  	 USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

  	 HID_EPOUT1_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  	 0x03,          /*bmAttributes: Interrupt endpoint*/
  	 HID_EPOUT1_SIZE, /*wMaxPacketSize: 4 Byte max */
  	 0x00,
  	 HID_HS_BINTERVAL,          /*bInterval: Polling Interval */

//添加新接口interface
	 	     0x09,         /*bLength: Interface Descriptor size*/
	 	     USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
	 	     0x02,         /*bInterfaceNumber: Number of Interface*/
	 	     0x00,         /*bAlternateSetting: Alternate setting*/
	 	     0x02,         /*bNumEndpoints*/
	 	     0x03,         /*bInterfaceClass: HID*/
	 	     0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
	 	     0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	 	     0,            /*iInterface: Index of string descriptor*/

	 	     0x09,         /*bLength: HID Descriptor size*/
	 	     HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
	 	     0x11,         /*bcdHID: HID Class Spec release number*/
	 	     0x01,
	 	     0x00,         /*bCountryCode: Hardware target country*/
	 	     0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
	 	     0x22,         /*bDescriptorType*/
	 	     HID_Config_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
	 	     0x00,

	 	     0x07,          /*bLength: Endpoint Descriptor size*/
	 	     USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

	 	     HID_EPIN2_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
	 	     0x03,          /*bmAttributes: Interrupt endpoint*/
	 	     HID_EPIN2_SIZE, /*wMaxPacketSize: 4 Byte max */
	 	     0x00,
	 	     HID_HS_BINTERVAL,          /*bInterval: Polling Interval */

	 	 	 0x07,          /*bLength: Endpoint Descriptor size*/
	 	 	 USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

	 	 	 HID_EPOUT2_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
	 	 	 0x03,          /*bmAttributes: Interrupt endpoint*/
	 	 	 HID_EPOUT2_SIZE, /*wMaxPacketSize: 4 Byte max */
	 	 	 0x00,
	 	 	 HID_HS_BINTERVAL,          /*bInterval: Polling Interval */
};

/* USB HID device Other Speed Configuration Descriptor */
__ALIGN_BEGIN static  uint8_t USBD_HID_OtherSpeedCfgDesc[USB_HID_CONFIG_DESC_SIZ]  __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x03,         /*bNumInterfaces: 3 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xE0,         /*bmAttributes: bus powered and Support Remote Wake-up */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

  /************** Descriptor of Joystick Mouse interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x01,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: HID*/
  0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x02,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Joystick Mouse HID ********************/
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Mouse endpoint ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

  HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_EPIN_SIZE, /*wMaxPacketSize: 4 Byte max */
  0x00,
  HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
  /* 34 */
  //添加新接口interface
      0x09,         /*bLength: Interface Descriptor size*/
      USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
      0x01,         /*bInterfaceNumber: Number of Interface*/
      0x00,         /*bAlternateSetting: Alternate setting*/
      0x02,         /*bNumEndpoints*/
      0x03,         /*bInterfaceClass: HID*/
      0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
      0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
      0,            /*iInterface: Index of string descriptor*/

      0x09,         /*bLength: HID Descriptor size*/
      HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
      0x11,         /*bcdHID: HID Class Spec release number*/
      0x01,
      0x00,         /*bCountryCode: Hardware target country*/
      0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
      0x22,         /*bDescriptorType*/
	  HID_KeyBoard_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
      0x00,

      0x07,          /*bLength: Endpoint Descriptor size*/
      USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

      HID_EPIN1_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
      0x03,          /*bmAttributes: Interrupt endpoint*/
      HID_EPIN1_SIZE, /*wMaxPacketSize: 4 Byte max */
      0x00,
      HID_FS_BINTERVAL,          /*bInterval: Polling Interval */

  	 0x07,          /*bLength: Endpoint Descriptor size*/
  	 USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

  	 HID_EPOUT1_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  	 0x03,          /*bmAttributes: Interrupt endpoint*/
  	 HID_EPOUT1_SIZE, /*wMaxPacketSize: 4 Byte max */
  	 0x00,
  	 HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
	 //添加新接口interface
	 	     0x09,         /*bLength: Interface Descriptor size*/
	 	     USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
	 	     0x02,         /*bInterfaceNumber: Number of Interface*/
	 	     0x00,         /*bAlternateSetting: Alternate setting*/
	 	     0x02,         /*bNumEndpoints*/
	 	     0x03,         /*bInterfaceClass: HID*/
	 	     0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
	 	     0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	 	     0,            /*iInterface: Index of string descriptor*/

	 	     0x09,         /*bLength: HID Descriptor size*/
	 	     HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
	 	     0x11,         /*bcdHID: HID Class Spec release number*/
	 	     0x01,
	 	     0x00,         /*bCountryCode: Hardware target country*/
	 	     0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
	 	     0x22,         /*bDescriptorType*/
	 	     HID_Config_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
	 	     0x00,

	 	     0x07,          /*bLength: Endpoint Descriptor size*/
	 	     USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

	 	     HID_EPIN2_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
	 	     0x03,          /*bmAttributes: Interrupt endpoint*/
	 	     HID_EPIN2_SIZE, /*wMaxPacketSize: 4 Byte max */
	 	     0x00,
	 	     HID_FS_BINTERVAL,          /*bInterval: Polling Interval */

	 	 	 0x07,          /*bLength: Endpoint Descriptor size*/
	 	 	 USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

	 	 	 HID_EPOUT2_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
	 	 	 0x03,          /*bmAttributes: Interrupt endpoint*/
	 	 	 HID_EPOUT2_SIZE, /*wMaxPacketSize: 4 Byte max */
	 	 	 0x00,
	 	 	 HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
};


/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static  uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ]  __ALIGN_END  =
{
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
};

//添加键盘定义
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static  uint8_t USBD_HID1_Desc[USB_HID_DESC_SIZ]  __ALIGN_END  =
{
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_KeyBoard_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
};

//添加配置接口定义
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static  uint8_t USBD_HID2_Desc[USB_HID_DESC_SIZ]  __ALIGN_END  =
{
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_Config_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static  uint8_t USBD_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC]  __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

__ALIGN_BEGIN static  uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE]  __ALIGN_END =
{
  0x05,   0x01,
  0x09,   0x02,
  0xA1,   0x01,
  0x09,   0x01,

  0xA1,   0x00,
  0x05,   0x09,
  0x19,   0x01,
  0x29,   0x03,

  0x15,   0x00,
  0x25,   0x01,
  0x95,   0x03,
  0x75,   0x01,

  0x81,   0x02,
  0x95,   0x01,
  0x75,   0x05,
  0x81,   0x01,

  0x05,   0x01,
  0x09,   0x30,
  0x09,   0x31,
  0x09,   0x38,

  0x15,   0x81,
  0x25,   0x7F,
  0x75,   0x08,
  0x95,   0x03,

  0x81,   0x06,
  0xC0,   0x09,
  0x3c,   0x05,
  0xff,   0x09,

  0x01,   0x15,
  0x00,   0x25,
  0x01,   0x75,
  0x01,   0x95,

  0x02,   0xb1,
  0x22,   0x75,
  0x06,   0x95,
  0x01,   0xb1,

  0x01,   0xc0
};

//添加键盘定义
__ALIGN_BEGIN static  uint8_t HID_KeyBoard_ReportDesc[HID_KeyBoard_REPORT_DESC_SIZE]  __ALIGN_END =
{
		 	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		    0x09, 0x06,                    // USAGE (Keyboard)
		    0xa1, 0x01,                    // COLLECTION (Application)
		    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
		    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
		    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
		    0x75, 0x01,                    //   REPORT_SIZE (1)
		    0x95, 0x08,                    //   REPORT_COUNT (8)
		    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
		    0x95, 0x01,                    //   REPORT_COUNT (1)
		    0x75, 0x08,                    //   REPORT_SIZE (8)
		    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
		    0x95, 0x05,                    //   REPORT_COUNT (5)
		    0x75, 0x01,                    //   REPORT_SIZE (1)
		    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
		    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
		    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
		    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
		    0x95, 0x01,                    //   REPORT_COUNT (1)
		    0x75, 0x03,                    //   REPORT_SIZE (3)
		    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
		    0x95, 0x06,                    //   REPORT_COUNT (6)
		    0x75, 0x08,                    //   REPORT_SIZE (8)
		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
		    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
		    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
		    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
		    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
		    0xc0                           // END_COLLECTION
};

//添加配置接口定义
__ALIGN_BEGIN static  uint8_t HID_Config_ReportDesc[HID_Config_REPORT_DESC_SIZE]  __ALIGN_END =
{
		  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		    0x09, 0x00,                    // USAGE (Undefined)
		    0xa1, 0x01,                    // COLLECTION (Application)
		    0x85, 0x01,                    //   REPORT_ID (1)
		    0x09, 0x00,                    //   USAGE (Undefined)
		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
		    0x75, 0x08,                    //   REPORT_SIZE (8)
		    0x95, 0x08,                    //   REPORT_COUNT (8)
		    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
		    0x85, 0x02,                    //   REPORT_ID (2)
		    0x09, 0x00,                    //   USAGE (Undefined)
		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
		    0x75, 0x08,                    //   REPORT_SIZE (8)
		    0x95, 0x08,                    //   REPORT_COUNT (8)
		    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
		    0xc0                           // END_COLLECTION

};

/**
  * @}
  */

/** @defgroup USBD_HID_Private_Functions
  * @{
  */
static uint8_t hid1_rx_buff[HID_EPOUT1_SIZE]={};
static uint8_t hid2_rx_buff[HID_EPOUT2_SIZE]={};
static void set_rx_buff(USBD_HandleTypeDef * pdev)
{//设置接收缓冲区(防止接收时崩溃)


	  USBD_LL_PrepareReceive(pdev, HID_EPOUT1_ADDR,hid1_rx_buff,HID_EPOUT1_SIZE);


	  USBD_LL_PrepareReceive(pdev, HID_EPOUT2_ADDR,hid2_rx_buff,HID_EPOUT2_SIZE);
}

/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  /* Open EP IN */
  USBD_LL_OpenEP(pdev, HID_EPIN_ADDR, USBD_EP_TYPE_INTR, HID_EPIN_SIZE);
  pdev->ep_in[HID_EPIN_ADDR & 0xFU].is_used = 1U;

  USBD_LL_OpenEP(pdev, HID_EPIN1_ADDR, USBD_EP_TYPE_INTR, HID_EPIN1_SIZE);
  pdev->ep_in[HID_EPIN1_ADDR & 0xFU].is_used = 1U;

  USBD_LL_OpenEP(pdev, HID_EPOUT1_ADDR, USBD_EP_TYPE_INTR, HID_EPOUT1_SIZE);
  pdev->ep_out[HID_EPOUT1_ADDR & 0xFU].is_used = 1U;

  USBD_LL_OpenEP(pdev, HID_EPIN2_ADDR, USBD_EP_TYPE_INTR, HID_EPIN2_SIZE);
    pdev->ep_in[HID_EPIN2_ADDR & 0xFU].is_used = 1U;

  USBD_LL_OpenEP(pdev, HID_EPOUT2_ADDR, USBD_EP_TYPE_INTR, HID_EPOUT2_SIZE);
  pdev->ep_out[HID_EPOUT2_ADDR & 0xFU].is_used = 1U;


  //初始化端点
  USBD_LL_FlushEP(pdev,HID_EPIN_ADDR);
  USBD_LL_FlushEP(pdev,HID_EPIN1_ADDR);
  USBD_LL_FlushEP(pdev,HID_EPIN2_ADDR);
  USBD_LL_FlushEP(pdev,HID_EPOUT1_ADDR);
  USBD_LL_FlushEP(pdev,HID_EPOUT2_ADDR);

  //准备接收缓冲
  set_rx_buff(pdev);

  pdev->pClassData = USBD_malloc(sizeof(USBD_HID_HandleTypeDef));

  if (pdev->pClassData == NULL)
  {
    return USBD_FAIL;
  }

  ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;

  return USBD_OK;
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_DeInit(USBD_HandleTypeDef *pdev,
                                uint8_t cfgidx)
{
  /* Close HID EPs */
  USBD_LL_CloseEP(pdev, HID_EPIN_ADDR);
  pdev->ep_in[HID_EPIN_ADDR & 0xFU].is_used = 0U;

  USBD_LL_CloseEP(pdev, HID_EPIN1_ADDR);
  pdev->ep_in[HID_EPIN1_ADDR & 0xFU].is_used = 0U;

  USBD_LL_CloseEP(pdev, HID_EPOUT1_ADDR);
  pdev->ep_out[HID_EPOUT1_ADDR & 0xFU].is_used = 0U;

  USBD_LL_CloseEP(pdev, HID_EPIN2_ADDR);
   pdev->ep_in[HID_EPIN2_ADDR & 0xFU].is_used = 0U;

   USBD_LL_CloseEP(pdev, HID_EPOUT2_ADDR);
   pdev->ep_out[HID_EPOUT2_ADDR & 0xFU].is_used = 0U;
  /* FRee allocated memory */
  if (pdev->pClassData != NULL)
  {
    USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }

  return USBD_OK;
}

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_HID_Setup(USBD_HandleTypeDef *pdev,
                               USBD_SetupReqTypedef *req)
{
  USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef *) pdev->pClassData;
  uint16_t len = 0U;
  uint8_t *pbuf = NULL;
  uint16_t status_info = 0U;
  USBD_StatusTypeDef ret = USBD_OK;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
    case USB_REQ_TYPE_CLASS :
      switch (req->bRequest)
      {
        case HID_REQ_SET_PROTOCOL:
          hhid->Protocol = (uint8_t)(req->wValue);
          break;

        case HID_REQ_GET_PROTOCOL:
          USBD_CtlSendData(pdev, (uint8_t *)(void *)&hhid->Protocol, 1U);
          break;

        case HID_REQ_SET_IDLE:
          hhid->IdleState = (uint8_t)(req->wValue >> 8);
          break;

        case HID_REQ_GET_IDLE:
          USBD_CtlSendData(pdev, (uint8_t *)(void *)&hhid->IdleState, 1U);
          break;

        default:
          USBD_CtlError(pdev, req);
          ret = USBD_FAIL;
          break;
      }
      break;
    case USB_REQ_TYPE_STANDARD:
      switch (req->bRequest)
      {
        case USB_REQ_GET_STATUS:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            USBD_CtlSendData(pdev, (uint8_t *)(void *)&status_info, 2U);
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
          break;

        case USB_REQ_GET_DESCRIPTOR:
          if (req->wValue >> 8 == HID_REPORT_DESC)
          {
        	  if(req->wIndex==0)//interface 0
        		  {
        		  	  len = MIN(HID_MOUSE_REPORT_DESC_SIZE, req->wLength);
        		  	  pbuf = HID_MOUSE_ReportDesc;
        		  }
        	  if(req->wIndex==1)//interface 1
        	  	  {
        		  	  	len = MIN(HID_KeyBoard_REPORT_DESC_SIZE, req->wLength);
        		        pbuf = HID_KeyBoard_ReportDesc;
        	  	  }
        	  if(req->wIndex==2)//interface 2
        	      {
        	          	len = MIN(HID_Config_REPORT_DESC_SIZE, req->wLength);
        	            pbuf = HID_Config_ReportDesc;
        	      }
          }
          else if (req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
          {
        	 if(req->wIndex==0)//interface 0
        	 {
        		 pbuf = USBD_HID_Desc;
             	 len = MIN(USB_HID_DESC_SIZ, req->wLength);
        	 }

        	 if(req->wIndex==1)//interface 1
        	 {
        		 pbuf = USBD_HID1_Desc;
        		 len = MIN(USB_HID_DESC_SIZ, req->wLength);
        	 }

        	 if(req->wIndex==2)//interface 2
        	  {
        	      pbuf = USBD_HID2_Desc;
        	      len = MIN(USB_HID_DESC_SIZ, req->wLength);
        	  }
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;
          }
          USBD_CtlSendData(pdev, pbuf, len);
          break;

        case USB_REQ_GET_INTERFACE :
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            USBD_CtlSendData(pdev, (uint8_t *)(void *)&hhid->AltSetting, 1U);
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
          break;

        case USB_REQ_SET_INTERFACE :
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            hhid->AltSetting = (uint8_t)(req->wValue);
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
          break;

        default:
          USBD_CtlError(pdev, req);
          ret = USBD_FAIL;
          break;
      }
      break;

    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
  }

  return ret;
}

/**
  * @brief  USBD_HID_SendReport
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_HID_SendReport(USBD_HandleTypeDef  *pdev,
                            uint8_t *report,
                            uint16_t len)
{
  USBD_HID_HandleTypeDef     *hhid = (USBD_HID_HandleTypeDef *)pdev->pClassData;

  if (pdev->dev_state == USBD_STATE_CONFIGURED)
  {
    if (hhid->state == HID_IDLE)
    {
      hhid->state = HID_BUSY;
      USBD_LL_Transmit(pdev,
                       HID_EPIN_ADDR,
                       report,
                       len);
    }
  }
  return USBD_OK;
}

uint8_t USBD_HID1_SendReport(USBD_HandleTypeDef  *pdev,
                            uint8_t *report,
                            uint16_t len)
{
  USBD_HID_HandleTypeDef     *hhid = (USBD_HID_HandleTypeDef *)pdev->pClassData;

  if (pdev->dev_state == USBD_STATE_CONFIGURED)
  {
    if (hhid->state == HID_IDLE)
    {
      hhid->state = HID_BUSY;
      USBD_LL_Transmit(pdev,
                       HID_EPIN1_ADDR,
                       report,
                       len);
    }
  }
  return USBD_OK;
}

uint8_t USBD_HID2_SendReport(USBD_HandleTypeDef  *pdev,
                            uint8_t *report,
                            uint16_t len)
{
  USBD_HID_HandleTypeDef     *hhid = (USBD_HID_HandleTypeDef *)pdev->pClassData;

  if (pdev->dev_state == USBD_STATE_CONFIGURED)
  {
    if (hhid->state == HID_IDLE)
    {
      hhid->state = HID_BUSY;
      USBD_LL_Transmit(pdev,
                       HID_EPIN2_ADDR,
                       report,
                       len);
    }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_HID_GetPollingInterval
  *         return polling interval from endpoint descriptor
  * @param  pdev: device instance
  * @retval polling interval
  */
uint32_t USBD_HID_GetPollingInterval(USBD_HandleTypeDef *pdev)
{
  uint32_t polling_interval = 0U;

  /* HIGH-speed endpoints */
  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Sets the data transfer polling interval for high speed transfers.
     Values between 1..16 are allowed. Values correspond to interval
     of 2 ^ (bInterval-1). This option (8 ms, corresponds to HID_HS_BINTERVAL */
    polling_interval = (((1U << (HID_HS_BINTERVAL - 1U))) / 8U);
  }
  else   /* LOW and FULL-speed endpoints */
  {
    /* Sets the data transfer polling interval for low and full
    speed transfers */
    polling_interval =  HID_FS_BINTERVAL;
  }

  return ((uint32_t)(polling_interval));
}

/**
  * @brief  USBD_HID_GetCfgFSDesc
  *         return FS configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_HID_GetFSCfgDesc(uint16_t *length)
{
  *length = sizeof(USBD_HID_CfgFSDesc);
  return USBD_HID_CfgFSDesc;
}

/**
  * @brief  USBD_HID_GetCfgHSDesc
  *         return HS configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_HID_GetHSCfgDesc(uint16_t *length)
{
  *length = sizeof(USBD_HID_CfgHSDesc);
  return USBD_HID_CfgHSDesc;
}

/**
  * @brief  USBD_HID_GetOtherSpeedCfgDesc
  *         return other speed configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_HID_GetOtherSpeedCfgDesc(uint16_t *length)
{
  *length = sizeof(USBD_HID_OtherSpeedCfgDesc);
  return USBD_HID_OtherSpeedCfgDesc;
}

/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_HID_DataIn(USBD_HandleTypeDef *pdev,
                                uint8_t epnum)
{

  /* Ensure that the FIFO is empty before a new transfer, this condition could
  be caused by  a new transfer before the end of the previous transfer */
  ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;
  return USBD_OK;
}

static uint8_t  USBD_HID_DataOut(USBD_HandleTypeDef *pdev,
                                uint8_t epnum)
{
 //获取接收的大小
 size_t data_size=USBD_LL_GetRxDataSize(pdev,epnum);
 if(epnum==HID_EPOUT1_ADDR)
{


	//再次接收
	USBD_LL_PrepareReceive(pdev, HID_EPOUT1_ADDR,hid1_rx_buff,HID_EPOUT1_SIZE);
 }
 if(epnum==HID_EPOUT2_ADDR)
 {

	 //测试发送
	 USBD_HID2_SendReport(pdev,hid2_rx_buff,data_size);

	 //再次接收
	 USBD_LL_PrepareReceive(pdev, HID_EPOUT2_ADDR,hid2_rx_buff,HID_EPOUT2_SIZE);
 }
  return USBD_OK;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t  *USBD_HID_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = sizeof(USBD_HID_DeviceQualifierDesc);
  return USBD_HID_DeviceQualifierDesc;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
