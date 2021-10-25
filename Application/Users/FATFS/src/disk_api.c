#include "disk_api.h"


/*  Get disk Status API */

// 获取虚拟存储的状态
int RAM_disk_status()
{
	return 0;
}


// 获取MMC/SD卡的状态
int MMC_disk_status()
{
	return 0;
}


// 获取U盘的状态
int USB_disk_status()
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);	// 返回U盘状态
}


/* Initialize disk API */

// 初始化虚拟存储设备
int RAM_disk_initialize()
{
	return 0;
}

// 初始化MMC/SD卡设备
int MMC_disk_initialize()
{
	return 0;
}

// 初始化USB设备
int USB_disk_initialize()
{
	return 0;
}


/* Read disk API */

// 读虚拟存储设备
int RAM_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// 读MMC/SD卡设备
int MMC_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// 读USB设备
int USB_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	uint8_t Result;
	uint8_t State;
	
	Result = USBH_MSC_FAIL;
	State = USB_disk_status();
	
	if (State)
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
	
	return Result;
}


/* Write disk API */

// 写虚拟存储设备
int RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// 写MMC/SD卡设备
int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// 写USB设备
int USB_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
		uint8_t Result;
	uint8_t State;
	
	Result = USBH_MSC_FAIL;
	State = USB_disk_status();
	
	if (State)
	{
		do
		{
			Result = USBH_MSC_Write10(&USB_OTG_Core, (uint8_t*)buff, sector, 512*count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);
			if (!USB_disk_status())
			{
				Result = USBH_MSC_FAIL;	// 读写错误
				break;
			}
		}
		while(Result == USBH_MSC_BUSY);
	}
	
	return Result;
}
