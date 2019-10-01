/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
#include "sdio_sd.h"	/* Example: MMC/SDC control */
#include "rtc.h"		/* RTC functions */

/* Definitions of physical drive number for each media */

#define MMC		0
#define ATA		1
#define USB		2

static volatile DSTATUS d_stat = STA_NOINIT;

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive number (0..) */
)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	d_stat = STA_NOINIT;

	switch (pdrv) {
	case MMC :
	    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
	    if( SD_Init() == 0)
	    {
	      d_stat &= ~STA_NOINIT;
	    }
		return d_stat;

	case ATA :
		//result = ATA_disk_initialize();
		// translate the result code here
		return d_stat;

	case USB :
		//result = USB_disk_initialize();
		// translate the result code here
		return d_stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number (0..) */
)
{
	d_stat = STA_NOINIT;

	switch (pdrv) {
	case MMC :
		if (SD_GetStatus() == 0)
		{
		  d_stat &= ~STA_NOINIT;
		}
		return d_stat;

	case ATA :
		//result = ATA_disk_status();
		// translate the result code here
		return d_stat;

	case USB :
		//result = USB_disk_status();
		// translate the result code here
		return d_stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive number (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res = RES_ERROR;
    	SD_Error sdstatus = SD_OK;

	switch (pdrv) {
	case MMC :
	    SD_ReadMultiBlocks(buff, sector << 9, 512, count);
	    /* Check if the Transfer is finished */
	    sdstatus =  SD_WaitReadOperation();
	    while(SD_GetStatus() != SD_TRANSFER_OK);
	    if (sdstatus == SD_OK)
	    {
	    	res = RES_OK;
	    }
		return res;

	case ATA :
		// translate the arguments here
		//result = ATA_disk_read(buff, sector, count);
		// translate the result code here
		return res;

	case USB :
		// translate the arguments here
		//result = USB_disk_read(buff, sector, count);
		// translate the result code here
		return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive number (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res = RES_ERROR;
	SD_Error sdstatus = SD_OK;

	switch (pdrv) {
	case MMC :
	    SD_WriteMultiBlocks((BYTE *)buff, sector << 9, 512, count);
	    /* Check if the Transfer is finished */
	    sdstatus = SD_WaitWriteOperation();
	    while(SD_GetStatus() != SD_TRANSFER_OK);
	    if (sdstatus == SD_OK)
	    {
	      res = RES_OK;
	    }
		return res;

	case ATA :
		// translate the arguments here
		//result = ATA_disk_write(buff, sector, count);
		// translate the result code here
		return res;

	case USB :
		// translate the arguments here
		//result = USB_disk_write(buff, sector, count);
		// translate the result code here
		return res;
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive number (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_ERROR;
	SD_CardInfo SDCardInfo;

	switch (pdrv) {
	case MMC :
		  if (d_stat & STA_NOINIT) return RES_NOTRDY;

		  switch (cmd) {
		  case CTRL_SYNC :		/* Make sure that no pending write process */
		    res = RES_OK;
		    break;

		  case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
		      SD_GetCardInfo(&SDCardInfo);
		      *(DWORD*)buff = SDCardInfo.CardCapacity / 512;

		    res = RES_OK;
		    break;

		  case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
		    *(WORD*)buff = 512;
		    res = RES_OK;
		    break;

		  case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
		      *(DWORD*)buff = 512;
			res = RES_OK;
		    break;

		  default:
		    res = RES_PARERR;
		  }
		return res;

	case ATA :
		// pre-process here
		//result = ATA_disk_ioctl(cmd, buff);
		// post-process here
		return res;

	case USB :
		// pre-process here
		//result = USB_disk_ioctl(cmd, buff);
		// post-process here
		return res;
	}
	return RES_PARERR;
}
#endif

#if !_FS_READONLY
DWORD get_fattime (void)
{
	DWORD res;
	//RTC_t rtc;
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);


	res  =   (((DWORD)ts_year + 20) << 25) //years from 1980
			| ((DWORD)ts_month << 21)
			| ((DWORD)ts_mday  << 16)
			|  (WORD)(ts_hour  << 11)
			|  (WORD)(ts_min   << 5)
			|  (WORD)(ts_sec   >> 1);

	return res;
}
#endif
