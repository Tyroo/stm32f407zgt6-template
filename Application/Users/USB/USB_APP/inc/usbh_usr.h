/**
  ******************************************************************************
  * @file    usbh_usr.h
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   Header file for usbh_usr.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USH_USR_H__
#define __USH_USR_H__

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "ff.h"
#include "usbh_core.h"
#include "usb_conf.h"
#include "usbh_msc_core.h"
#include "usb_hcd_int.h"
#include "usart.h"

/** @addtogroup USBH_USER
  * @{
  */

/** @addtogroup USBH_MSC_DEMO_USER_CALLBACKS
  * @{
  */

/** @defgroup USBH_USR
  * @brief This file is the Header file for usbh_usr.c
  * @{
  */ 


/** @defgroup USBH_USR_Exported_Types
  * @{
  */ 


extern  USBH_Usr_cb_TypeDef USR_USBH_cb;


typedef struct
{
	uint8_t Mode   :4;	// 0：USB HOST MSC模式(默认模式,接U盘)，1：USB HOST HID模式(驱动鼠标键盘等)，2：USB Device MSC模式(USB读卡器)
	uint8_t State  :8;	// 0：断开状态，1：连接状态，3：出现错误
	uint8_t Type   :4;	// 0: 无设备，1：SD卡，2：Flash设备，3：USB闪存设备
} USB_ManageType;

/**
  * @}
  */ 



/** @defgroup USBH_USR_Exported_Defines
  * @{
  */ 
/* State Machine for the USBH_USR_ApplicationState */
#define USH_USR_FS_INIT                   0	// 初始化
#define USH_USR_FS_WAIT_OPERATION		  1	// 正在对USB设备进行操作
#define USH_USR_FS_OK_OPERATION			  2 // 操作完成
#define USH_USR_FS_ERR_OPERATION          3 // 操作失败
/**
  * @}
  */ 

/** @defgroup USBH_USR_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_USR_Exported_Variables
  * @{
  */ 
extern uint8_t USBH_USR_ApplicationState;
/**
  * @}
  */ 

/** @defgroup USBH_USR_Exported_FunctionsPrototype
  * @{
  */ 
void USBH_USR_ApplicationSelected(void);
void USBH_USR_Init(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed); 
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);
int USBH_USR_MSC_Application(void);
void delay(__IO uint32_t nCount);
/**
  * @}
  */ 

int USBH_User_App(void);

#endif /*__USH_USR_H__*/

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

