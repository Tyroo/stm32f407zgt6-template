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
	return 0;
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
	return 0;
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
	return 0;
}
