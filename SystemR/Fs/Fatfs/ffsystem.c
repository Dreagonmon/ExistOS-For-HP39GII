/*------------------------------------------------------------------------*/
/* Sample Code of OS Dependent Functions for FatFs                        */
/* (C)ChaN, 2018                                                          */
/*------------------------------------------------------------------------*/


#include "ff.h"
#include "FreeRTOS.h"
#include "sys_clock.h"
#include "uclock.h"


#if FF_USE_LFN == 3	/* Dynamic memory allocation */

/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/

void* ff_memalloc (	/* Returns pointer to the allocated memory block (null if not enough core) */
	UINT msize		/* Number of bytes to allocate */
)
{
	//return malloc(msize);	/* Allocate a new memory block with POSIX API */
	return pvPortMalloc(msize);	/* Allocate a new memory block with POSIX API */
}


/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/

void ff_memfree (
	void* mblock	/* Pointer to the memory block to free (nothing to do if null) */
)
{
	//free(mblock);	/* Free the memory block with POSIX API */
	vPortFree(mblock);	/* Free the memory block with POSIX API */
}

#endif



#if FF_FS_REENTRANT	/* Mutal exclusion */

/*------------------------------------------------------------------------*/
/* Create a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to create a new
/  synchronization object for the volume, such as semaphore and mutex.
/  When a 0 is returned, the f_mount() function fails with FR_INT_ERR.
*/

//const osMutexDef_t Mutex[FF_VOLUMES];	/* Table of CMSIS-RTOS mutex */


int ff_cre_syncobj (	/* 1:Function succeeded, 0:Could not create the sync object */
	BYTE vol,			/* Corresponding volume (logical drive number) */
	FF_SYNC_t* sobj		/* Pointer to return the created sync object */
)
{
	/* Win32 */
//	*sobj = CreateMutex(NULL, FALSE, NULL);
//	return (int)(*sobj != INVALID_HANDLE_VALUE);

	/* uITRON */
//	T_CSEM csem = {TA_TPRI,1,1};
//	*sobj = acre_sem(&csem);
//	return (int)(*sobj > 0);

	/* uC/OS-II */
//	OS_ERR err;
//	*sobj = OSMutexCreate(0, &err);
//	return (int)(err == OS_NO_ERR);

	/* FreeRTOS */
	*sobj = xSemaphoreCreateMutex();
	return (int)(*sobj != NULL);

	/* CMSIS-RTOS */
//	*sobj = osMutexCreate(&Mutex[vol]);
//	return (int)(*sobj != NULL);
}


/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to delete a synchronization
/  object that created with ff_cre_syncobj() function. When a 0 is returned,
/  the f_mount() function fails with FR_INT_ERR.
*/

int ff_del_syncobj (	/* 1:Function succeeded, 0:Could not delete due to an error */
	FF_SYNC_t sobj		/* Sync object tied to the logical drive to be deleted */
)
{
	/* Win32 */
//	return (int)CloseHandle(sobj);

	/* uITRON */
//	return (int)(del_sem(sobj) == E_OK);

	/* uC/OS-II */
//	OS_ERR err;
//	OSMutexDel(sobj, OS_DEL_ALWAYS, &err);
//	return (int)(err == OS_NO_ERR);

	/* FreeRTOS */
  vSemaphoreDelete(sobj);
	return 1;

	/* CMSIS-RTOS */
//	return (int)(osMutexDelete(sobj) == osOK);
}


/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a 0 is returned, the file function fails with FR_TIMEOUT.
*/

int ff_req_grant (	/* 1:Got a grant to access the volume, 0:Could not get a grant */
	FF_SYNC_t sobj	/* Sync object to wait */
)
{
	/* Win32 */
//	return (int)(WaitForSingleObject(sobj, FF_FS_TIMEOUT) == WAIT_OBJECT_0);

	/* uITRON */
//	return (int)(wai_sem(sobj) == E_OK);

	/* uC/OS-II */
//	OS_ERR err;
//	OSMutexPend(sobj, FF_FS_TIMEOUT, &err));
//	return (int)(err == OS_NO_ERR);

	/* FreeRTOS */
	return (int)(xSemaphoreTake(sobj, FF_FS_TIMEOUT) == pdTRUE);

	/* CMSIS-RTOS */
//	return (int)(osMutexWait(sobj, FF_FS_TIMEOUT) == osOK);
}


/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
	FF_SYNC_t sobj	/* Sync object to be signaled */
)
{
	/* Win32 */
//	ReleaseMutex(sobj);

	/* uITRON */
//	sig_sem(sobj);

	/* uC/OS-II */
//	OSMutexPost(sobj);

	/* FreeRTOS */
	xSemaphoreGive(sobj);

	/* CMSIS-RTOS */
//	osMutexRelease(sobj);
}

/*------------------------------------------------------------------------*/
/* Get RTC Time in FAT32 Format                                           */
/*------------------------------------------------------------------------*/
/*
->> higher bit
The date on which the file was created.
Year 	7 bits (0~127) + 1980
Month 	4 bits (1~12)
Day 	5 bits (1~31)
The time that the file was created. Multiply Seconds by 2.
Hour 	5 bits (0~23)
Minutes 	6 bits (0~59)
Seconds 	5 bits (0~29, 2-second increments)
->> lower bit
*/

uint32_t get_fattime() {
	int64_t tms = rtc_time() & INT64_MAX;
	struct utm *now = pvPortMalloc(sizeof(struct utm));
	if (now == NULL) {
		return 0; // mem full
	}
	uclock_secs_to_tm(tms, now);
	uint32_t timestamp = (uint32_t)(((now->tm_year + 1900 - 1980) & 0b1111111) << 25);
	timestamp |= ((uint32_t)((now->tm_mon + 1) & 0b1111) << 21);
	timestamp |= ((uint32_t)((now->tm_mday) & 0b11111) << 16);
	timestamp |= ((uint32_t)((now->tm_hour) & 0b11111) << 11);
	timestamp |= ((uint32_t)((now->tm_min) & 0b111111) << 5);
	timestamp |= ((uint32_t)((now->tm_sec / 2) & 0b11111) << 0);
	vPortFree(now);
	return timestamp;
}

#endif
