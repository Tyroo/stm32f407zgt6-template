/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */

#define DEV_SD		0	/* Example: Map MMC/SD card to physical drive 0 */
#define DEV_FLASH	1	/* Example: Map extern FLASH to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	uint8_t result;

	switch (pdrv) {
	case DEV_SD :
		result = SD_disk_status();

		// translate the reslut code here
		stat = result;
		return stat;

	case DEV_FLASH :
		result = FLASH_disk_status();

		// translate the reslut code here
		stat = result;
		return stat;

	case DEV_USB :
		result = (1 - USB_disk_status());

		// translate the reslut code here
		stat = result;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	uint8_t result;

	switch (pdrv) {
	case DEV_SD :
		result = SD_disk_initialize();
		stat = result;
		// translate the reslut code here
		return stat;

	case DEV_FLASH :
		result = FLASH_disk_initialize();
		stat = result;
		// translate the reslut code here
		return stat;

	case DEV_USB :
		result = (1 - USB_disk_initialize());
		// translate the reslut code here
		stat = result;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	uint8_t result;

	switch (pdrv) {
	case DEV_SD :
		// translate the arguments here

		result = SD_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_FLASH :
		// translate the arguments here

		result = FLASH_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		result = USB_disk_read(buff, sector, count);
		res = (DRESULT)result;
		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	uint8_t result;

	switch (pdrv) {
	case DEV_SD :
		// translate the arguments here
		result = SD_disk_write(buff, sector, count);
		res = (DRESULT)result;
		// translate the reslut code here
		return res;

	case DEV_FLASH :
		// translate the arguments here
		result = FLASH_disk_write(buff, sector, count);
		res = (DRESULT)result;
		// translate the reslut code here
		return res;

	case DEV_USB :
		// translate the arguments here
		result = USB_disk_write(buff, sector, count);
		res = (DRESULT)result;
		// translate the reslut code here
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {
	case DEV_SD :
		// Process of the command for the RAM drive
		res = (DRESULT)SD_disk_ioctl(pdrv, cmd, buff);
		return res;

	case DEV_FLASH :
		// Process of the command for the MMC/SD card
		res = (DRESULT)FLASH_disk_ioctl(pdrv, cmd, buff);
		return res;

	case DEV_USB :
		// Process of the command the USB drive
		res = (DRESULT)USB_disk_ioctl(pdrv, cmd, buff);
		return res;
	}

	return RES_PARERR;
}


/*-----------------------------------------------------------------------*/
/* Get time Functions                                               */
/*-----------------------------------------------------------------------*/
DWORD get_fattime(void)
{
	return 0;
}

