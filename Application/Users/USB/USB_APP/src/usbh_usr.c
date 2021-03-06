/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file includes the usb host library user callbacks
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
/* Includes ------------------------------------------------------------------*/

#include "usbh_usr.h"


/* USB variable define */
static uint8_t        AppState;
USBH_HOST             USB_Host;
USB_OTG_CORE_HANDLE   USB_OTG_Core;
USB_ManageType		  USB_Manage;


/* FatFs variable define */
FATFS* Fs[3];
FIL*   Fi;
FRESULT F_Sate;
UINT FatfsReadSize, FatfsWriteSize;
BYTE* ReadTextBuff;
char* WriteTextBuff;
BYTE Work[FF_MAX_SS];


void OTG_FS_IRQHandler(void)
{
    USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_USBH_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
    
};


/** @defgroup USBH_USR_Private_Functions
* @{
*/ 


/**
* @brief  USBH_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
    // USBH??????????????
	USB_Manage = ((USB_ManageType){ 0, 0, 0 });
	printf("USB????????????????\r\n");
}

/**
* @brief  USBH_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
	printf("????????USB??????????\r\n");
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
  // ????USB????????
	printf("USB??????????????????????\r\n");
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Staus
*/
void USBH_USR_DeviceDisconnected (void)
{
  // USB????????????????
	printf("USB????????????????\r\n");
}
/**
* @brief  USBH_USR_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
	printf("USB????????????\r\n");
}


/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    // ??????????USB??????????????????????????????????
	switch(DeviceSpeed)
	{
		case HPRT0_PRTSPD_HIGH_SPEED:
			printf("??????????USB??????????\r\n");
			break;
		case HPRT0_PRTSPD_FULL_SPEED:
			printf("??????????USB??????????\r\n");
			break;
		case HPRT0_PRTSPD_LOW_SPEED:
			printf("??????????USB??????????\r\n");
			break;
		default:
			printf("??????????\r\n");
			break;
	}
}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
	USBH_DevDesc_TypeDef *hs;
	hs = DeviceDesc;

	printf("VID : %04Xh\r\n", (uint32_t)(*hs).idVendor); 	// ????????????ID
	printf("PID : %04Xh\r\n", (uint32_t)(*hs).idProduct); 	// ????????ID
}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
	// ????????????????????????
	printf("USB??????????????????\r\n");
}


/**
* @brief  USBH_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
    // ??????????????????????
	USBH_InterfaceDesc_TypeDef* Id;
	Id = itfDesc;
	
	if (Id->bInterfaceClass == 0x08)
	{
		printf("??USB????????????????????????\r\n");
		USB_Manage.Type = 3;
	}
	else
	{
		printf("??USB??????HID??????\r\n");
	}
}

/**
* @brief  USBH_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  Manufacturer String 
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
    // ??????????????
//  printf("Manufacturer : %s\n", (char *)ManufacturerString);
}

/**
* @brief  USBH_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  Product String
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
    // ??????????????
//  printf("Product : %s\n", (char *)ProductString);  
}

/**
* @brief  USBH_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNum_String 
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
    //????????????
//  printf( "Serial Number : %s\n", (char *)SerialNumString);    
} 



/**
* @brief  EnumerationDone 
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
    // USB????????????????
} 


/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
    // ????????????????????
//  printf ("> Device not supported."); 
}  


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
    // ????????????????????
  return USBH_USR_RESP_OK;
}  

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Staus
*/
void USBH_USR_OverCurrentDetected (void)
{
    // ????????????????????
//  printf ("Overcurrent detected.");
}


/**
* @brief  USBH_USR_MSC_Application 
*         Demo application for mass storage
* @param  None
* @retval Staus
*/
int USBH_USR_MSC_Application(void)
{
    // U??????????????
	uint8_t Result;
	
	Result = HCD_IsDeviceConnected(&USB_OTG_Core);
	
	switch(AppState)
	{
		case USH_USR_FS_INIT:
			AppState = USH_USR_FS_WAIT_OPERATION;
			break;
		case USH_USR_FS_WAIT_OPERATION:
			if (Result) 
			{
				USB_Manage.State = 1;
			}
			else
			{
				AppState = USH_USR_FS_INIT;
			}
			break;
		case USH_USR_FS_OK_OPERATION:
			printf("USB??????????");
			Result = 0;
		case USH_USR_FS_ERR_OPERATION:
			printf("USB??????????");
			Result = 1;
		default:
			Result = 1;
			break;
	}
	
    return 0;
}


/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
  AppState = USH_USR_FS_INIT;
}


/**
* @brief  USBH_User_App
* @param  None
* @retval 0: Normal??1: Anomaly
*/ 
int USBH_User_App(void)
{
	FatfsReadSize = 10;
	
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//????????????
	{	
		F_Sate = f_open(Fi, "2:123.txt", FA_READ | FA_OPEN_EXISTING);
		F_Sate = f_read(Fi, ReadTextBuff, sizeof(ReadTextBuff), &FatfsReadSize);
		
		if (F_Sate == FR_OK)
		{
			Uart1_Send("??????????????????:");
			Uart1_Send((char*)ReadTextBuff);
			return 0;
		}
		else
		{
			return 1;
		}
		
	}
	return 1;
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
