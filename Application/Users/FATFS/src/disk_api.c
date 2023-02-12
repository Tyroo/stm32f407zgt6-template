#include "disk_api.h"


#ifdef USE_HOST_MODE
extern USB_ManageType	USB_Manage;
#endif

/************************* SD�� ***************************/
// ��ʼ������洢�豸
uint8_t SD_disk_initialize()
{
	return 0;
}

// ��ȡ����洢��״̬
uint8_t SD_disk_status()
{
	return 0;
}

// ������洢�豸
uint8_t SD_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// д����洢�豸
uint8_t SD_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
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

/************************* Flash ***************************/

// ��ʼ��Flash�豸
uint8_t FLASH_disk_initialize()
{
	return 0;
}

// ��ȡMMC/SD����״̬
uint8_t FLASH_disk_status()
{
	return 0;
}

// ��MMC/SD���豸
uint8_t FLASH_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// дMMC/SD���豸
uint8_t FLASH_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
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

/************************* USB ********************************/

// ��ʼ��USB�豸
uint8_t USB_disk_initialize()
{
#ifdef USE_HOST_MODE
	return HCD_IsDeviceConnected(&USB_OTG_Core);
#else
	return 0;
#endif
}

// ��ȡU�̵�״̬
uint8_t USB_disk_status()
{
#ifdef USE_HOST_MODE
	return HCD_IsDeviceConnected(&USB_OTG_Core);
#else
	return 0;
#endif
}

// ��USB�豸
uint8_t USB_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	uint8_t Result;
	uint8_t State;
	
	Result = USBH_MSC_OK;
	State = USB_disk_status();
	
#ifdef USE_HOST_MODE
	if ((State) && (USB_Manage.State == 1))
#else
	if (State)
#endif
	{
		do
		{
#ifdef USE_HOST_MODE
			Result = USBH_MSC_Read10(&USB_OTG_Core, (uint8_t*)buff, sector, 512*count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);
#endif
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

// дUSB�豸
uint8_t USB_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	uint8_t Result;
	uint8_t State;
	
	Result = USBH_MSC_OK;
	State = USB_disk_status();
#ifdef USE_HOST_MODE	
	if ((State) && (USB_Manage.State == 1))
#else
	if (State)
#endif
	{
		do
		{
#ifdef USE_HOST_MODE
			Result = USBH_MSC_Write10(&USB_OTG_Core, (uint8_t*)buff, sector, 512*count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);
#endif
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
		#ifdef USE_HOST_MODE
			*(DWORD*)buff=USBH_MSC_Param.MSCapacity;
		#endif
			res = 0;
			break;
		default:
			res = 4;
			break;
	}		
    return res;
}
