#include "disk_api.h"


extern USB_ManageType	USB_Manage;

/*  Get disk Status API */

// ��ȡ����洢��״̬
uint8_t SD_disk_status()
{
	return 0;
}


// ��ȡMMC/SD����״̬
uint8_t FLASH_disk_status()
{
	return 0;
}


// ��ȡU�̵�״̬
uint8_t USB_disk_status()
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);
//	return ((uint8_t)HCD_IsDeviceConnected(&USB_OTG_Core));	// ����U��״̬
}


/* Initialize disk API */

// ��ʼ������洢�豸
uint8_t SD_disk_initialize()
{
	return 0;
}

// ��ʼ��MMC/SD���豸
uint8_t FLASH_disk_initialize()
{
	return 0;
}

// ��ʼ��USB�豸
uint8_t USB_disk_initialize()
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);
}


/* Read disk API */

// ������洢�豸
uint8_t SD_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// ��MMC/SD���豸
uint8_t FLASH_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// ��USB�豸
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
				Result = USBH_MSC_FAIL;	// ��д����
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

// д����洢�豸
uint8_t SD_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// дMMC/SD���豸
uint8_t FLASH_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// дUSB�豸
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
				Result = USBH_MSC_FAIL;	// д����
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
