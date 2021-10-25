#ifndef __DISK_API_H__
#define __DISK_API_H__

#include "ff.h"
#include "usbh_usr.h"


int RAM_disk_initialize(void);
int MMC_disk_initialize(void);
int USB_disk_initialize(void);

int RAM_disk_status(void);
int MMC_disk_status(void);
int USB_disk_status(void);

int RAM_disk_read(const BYTE *buff, LBA_t sector, UINT count);
int MMC_disk_read(const BYTE *buff, LBA_t sector, UINT count);
int USB_disk_read(const BYTE *buff, LBA_t sector, UINT count);

int RAM_disk_write(const BYTE *buff, LBA_t sector, UINT count);
int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count);
int USB_disk_write(const BYTE *buff, LBA_t sector, UINT count);


#endif
