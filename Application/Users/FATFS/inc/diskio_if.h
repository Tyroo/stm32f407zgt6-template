#ifndef __DISKIO_IF_H__
#define __DISKIO_IF_H__

#include "ff.h"
#include "usbh_usr.h"


uint8_t SD_disk_initialize(void);
uint8_t FLASH_disk_initialize(void);
uint8_t USB_disk_initialize(void);

uint8_t SD_disk_status(void);
uint8_t FLASH_disk_status(void);
uint8_t USB_disk_status(void);

uint8_t SD_disk_read(const BYTE *buff, LBA_t sector, UINT count);
uint8_t FLASH_disk_read(const BYTE *buff, LBA_t sector, UINT count);
uint8_t USB_disk_read(const BYTE *buff, LBA_t sector, UINT count);

uint8_t SD_disk_write(const BYTE *buff, LBA_t sector, UINT count);
uint8_t FLASH_disk_write(const BYTE *buff, LBA_t sector, UINT count);
uint8_t USB_disk_write(const BYTE *buff, LBA_t sector, UINT count);

uint8_t SD_disk_ioctl (BYTE pdrv, BYTE cmd, void *buff);
uint8_t FLASH_disk_ioctl (BYTE pdrv, BYTE cmd, void *buff);
uint8_t USB_disk_ioctl (BYTE pdrv, BYTE cmd, void *buff);


#endif
