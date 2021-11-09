#include "disk_api.h"


extern USB_ManageType	USB_Manage;

/*  Get disk Status API */

// 获取虚拟存储的状态
uint8_t SD_disk_status()
{
	return 0;
}


// 获取MMC/SD卡的状态
uint8_t FLASH_disk_status()
{
	return 0;
}


// 获取U盘的状态
uint8_t USB_disk_status()
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);
//	return ((uint8_t)HCD_IsDeviceConnected(&USB_OTG_Core));	// 返回U盘状态
}


/* Initialize disk API */

// 初始化虚拟存储设备
uint8_t SD_disk_initialize()
{
	return 0;
}

// 初始化MMC/SD卡设备
uint8_t FLASH_disk_initialize()
{
	return 0;
}

// 初始化USB设备
uint8_t USB_disk_initialize()
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);
}


/* Read disk API */

// 读虚拟存储设备
uint8_t SD_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// 读MMC/SD卡设备
uint8_t FLASH_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// 读USB设备
uint8_t USB_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	uint8_t Result;
	uint8_t State;
	
	Result = USBH_MSC_OK;
	State = USB_disk_status();
	
	if ((State) && (USB_Manage.State == 1))
	{
		do
		{
			Result = USBH_MSC_Read10(&USB_OTG_Core, (uint8_t*)buff, sector, 512*count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);
			if (!USB_disk_status())
			{
				Result = USBH_MSC_FAIL;	// 读写错误
				break;
			}
		}
		while(Result == USBH_MSC_BUSY);
	}
	else
	{
		Result = USBH_MSC_FAIL;
	}
	
	return Result;
}


/* Write disk API */

// 写虚拟存储设备
uint8_t SD_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// 写MMC/SD卡设备
uint8_t FLASH_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// 写USB设备
uint8_t USB_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	uint8_t Result;
	uint8_t State;
	
	Result = USBH_MSC_OK;
	State = USB_disk_status();
	
	if ((State) && (USB_Manage.State == 1))
	{
		do
		{
			Result = USBH_MSC_Write10(&USB_OTG_Core, (uint8_t*)buff, sector, 512*count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);
			if (!USB_disk_status())
			{
				Result = USBH_MSC_FAIL;	// 写错误
				break;
			}
		}
		while(Result == USBH_MSC_BUSY);
	}
	else
	{
		Result = USBH_MSC_FAIL;
	}
	
	return Result;
}


uint8_t SD_disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	uint8_t res;
	
	switch(cmd)
	{
		case 0:
			res = 0; 
			break;	 
		case 1:
			*(DWORD*)buff = 512; 
			res = 0;
			break;	 
		case 2:
			*(WORD*)buff = 8;
			res = 0;
			break;	 
		case 3:
			*(DWORD*)buff = 512 * 12;
			res = 0;
			break;
		default:
			res = 4;
			break;
	}
	return res;
}
	
	
uint8_t FLASH_disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	uint8_t res;
	
	switch(cmd)
	{
		case 0:
			res = 0; 
			break;	 
		case 1:
			*(WORD*)buff = 512;
			res = 0;
			break;	 
		case 2:
			*(WORD*)buff = 8;
			res = 0;
			break;	 
		case 3:
			*(DWORD*)buff = 2048 * 12;
			res = 0;
			break;
		default:
			res = 4;
			break;
	}
	return res;
}


uint8_t USB_disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	uint8_t res;
	
	switch(cmd)
	{
		case 0:
			res = 0; 
			break;	 
		case 1:
			*(WORD*)buff = 512;
			res = 0;
			break;	 
		case 2:
			*(WORD*)buff = 512;
			res = 0;
			break;	 
		case 3:
			*(DWORD*)buff=USBH_MSC_Param.MSCapacity;
			res = 0;
			break;
		default:
			res = 4;
			break;
	}		
    return res;
}
