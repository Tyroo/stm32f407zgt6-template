#include "disk_api.h"


/*  Get disk Status API */

// ��ȡ����洢��״̬
int RAM_disk_status()
{
	return 0;
}


// ��ȡMMC/SD����״̬
int MMC_disk_status()
{
	return 0;
}


// ��ȡU�̵�״̬
int USB_disk_status()
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);	// ����U��״̬
}


/* Initialize disk API */

// ��ʼ������洢�豸
int RAM_disk_initialize()
{
	return 0;
}

// ��ʼ��MMC/SD���豸
int MMC_disk_initialize()
{
	return 0;
}

// ��ʼ��USB�豸
int USB_disk_initialize()
{
	return 0;
}


/* Read disk API */

// ������洢�豸
int RAM_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// ��MMC/SD���豸
int MMC_disk_read(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// ��USB�豸
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
				Result = USBH_MSC_FAIL;	// ��д����
				break;
			}
		}
		while(Result == USBH_MSC_BUSY);
	}
	
	return Result;
}


/* Write disk API */

// д����洢�豸
int RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// дMMC/SD���豸
int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
	return 0;
}

// дUSB�豸
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
				Result = USBH_MSC_FAIL;	// ��д����
				break;
			}
		}
		while(Result == USBH_MSC_BUSY);
	}
	
	return Result;
}
