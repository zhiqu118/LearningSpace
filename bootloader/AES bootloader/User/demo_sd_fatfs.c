/*
*********************************************************************************************************
*
*	??????? : SD??Fat???????SD??????????�
*	??????? : demo_sd_fatfs.c
*	??    ?? : V1.0
*	?    ?? : SD????????????????????:
*              1 - ???SD??????????????
*              2 - ???????????, ��???????
*
*	????? :
*		?��??   ????         ????        ???
*		V1.0    2022-06-26   Eric2013    ???????
*
*	Copyright (C), 2022-2030, ?????????? www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "ff.h"		
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/aes.h"


/*
*********************************************************************************************************
*	                                        ????
*********************************************************************************************************
*/
/* ???????????????????????? */
static void LoadFirmware(void);
static void DispMenu(void);
static void ViewRootDir(void);
static void JumpToApp(void);
static void BootHexCrcVeriy(void);

/*
*********************************************************************************************************
*	                                       ????
*********************************************************************************************************
*/
#define AppAddr  0x08100000    /* APP??? */

/*
*********************************************************************************************************
*	                                       ????
*********************************************************************************************************
*/
/* ???? */
mbedtls_aes_context aes;

unsigned char key[] =
{
  0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

unsigned char iv[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

FATFS fs;
FIL file;
DIR DirInf;
FILINFO FileInf;
__IO uint32_t uwCRCValue;
__IO uint32_t uwExpectedCRCValue;
__IO uint32_t uwAppSize;

ALIGN_32BYTES(uint8_t output[4096]);  /* ??????????? */
ALIGN_32BYTES(uint8_t tempbuf[4096]);  /* ??????????? */
char DiskPath[4];                      /* SD?????????��?????????0??????"0:/" */

/* FatFs API?????? */
static const char * FR_Table[]= 
{
	"FR_OK?????",				                             /* (0) Succeeded */
	"FR_DISK_ERR????????????",			                 /* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR?????????",				                     /* (2) Assertion failed */
	"FR_NOT_READY????????????��???",			             /* (3) The physical drive cannot work */
	"FR_NO_FILE???????????",				                 /* (4) Could not find the file */
	"FR_NO_PATH??��????????",				                 /* (5) Could not find the path */
	"FR_INVALID_NAME????��?????",		                     /* (6) The path name format is invalid */
	"FR_DENIED?????????????????????????????",         /* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST????????????",			                     /* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT?????????????????��",		         /* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED????????????��????",		             /* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE?????????????��",		                 /* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED?????????????",			                 /* (12) The volume has no work area */
	"FR_NO_FILESYSTEM???????��??FAT??",		             /* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED?????????????f_mkfs()?????",	         /* (14) The f_mkfs() aborted due to any parameter error */
	"FR_TIMEOUT????��????????????��???????????",		 /* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED?????????????????????????",				 /* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE????????????????????",		     /* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES?????????????????_FS_SHARE", /* (18) Number of open files > _FS_SHARE */
	"FR_INVALID_PARAMETER????????��"	                     /* (19) Given parameter is invalid */
};

/*
*********************************************************************************************************
*	?? ?? ??: DemoFatFS
*	???????: FatFS??????????????
*	??    ??: ??
*	?? ?? ?: ??
*********************************************************************************************************
*/
void DemoFatFS(void)
{
	uint8_t cmd;

	/* ????????��??????????????????????? */
	DispMenu();
	
	/* ???SD?????? */
	FATFS_LinkDriver(&SD_Driver, DiskPath);
	
	bsp_StartAutoTimer(0, 500);	/* ????1??500ms?????????????? */
	
	while (1)
	{
		/* ?��??????????? */
		if (bsp_CheckTimer(0))	
		{            
			/* ???500ms ??????? */  
			bsp_LedToggle(2);
		}

		if (comGetChar(COM1, &cmd))	/* ??????????????(?????????) */
		{
			printf("\r\n");
			switch (cmd)
			{
				case '1':
					printf("??1 - ???SD????????????????\r\n");
					ViewRootDir();
					break;
				
				case '2':
					printf("??2 - ??????????????, ��?????????\r\n");
					LoadFirmware();		
					BootHexCrcVeriy();
					JumpToApp();
					break;
	
				default:
					DispMenu();
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: DispMenu
*	???????: ?????????????
*	??    ?��???
*	?? ?? ?: ??
*********************************************************************************************************
*/
static void DispMenu(void)
{
	printf("?????????????:\r\n");
	printf("1 - ???SD??????????????\r\n");
	printf("2 - ???????????, ��??????\r\n");
}

/*
*********************************************************************************************************
*	?? ?? ??: JumpToApp
*	???????: ????????JumpToApp
*	??    ??: ??
*	?? ?? ?: ??
*********************************************************************************************************
*/
extern uint8_t BSP_SD_DeInit(void);
static void JumpToApp(void)
{
	uint32_t i=0;
	void (*AppJump)(void);         /* ?????????????? */
    
	BSP_SD_DeInit();
	
    /* ???????��? */
	DISABLE_INT(); 
    
    /* ??????????????????????HSI??? */
	HAL_RCC_DeInit();
    
	/* ???��????????��?????? */
	SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

	/* ????????��??????????��?????? */
	for (i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}	

	/* ???????��? */
	ENABLE_INT();

	/* ???????��?????????MSP?????+4???��?��????????? */
	AppJump = (void (*)(void)) (*((uint32_t *) (AppAddr + 4)));

	/* ???????????? */
	__set_MSP(*(uint32_t *)AppAddr);
	
	/* ??RTOS???????????????????????????????????MSP??? */
	__set_CONTROL(0);

	/* ???????BootLoader */
	AppJump(); 

	/* ?????????????????��????????????????????????? */
	while (1)
	{

	}
}

/*
*********************************************************************************************************
*	?? ?? ??: BootHexCrcVeriy
*	???????: ???CRC��??
*	??    ??: ??
*	?? ?? ?: ??
*********************************************************************************************************
*/
static void BootHexCrcVeriy(void)
{
	CRC_HandleTypeDef   CrcHandle = {0};
	
	/* ???bin?????CRC */
	uwExpectedCRCValue  = *(__IO uint32_t *)(AppAddr + uwAppSize - 4);
	
	/* ????????CRC */
	__HAL_RCC_CRC_CLK_ENABLE();      
	
	CrcHandle.Instance = CRC;
	CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;
	CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
	CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
	CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
	CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;

	if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
	{
        Error_Handler(__FILE__, __LINE__);
	}

	/* ????????????CRC??? */
	uwCRCValue = HAL_CRC_Calculate(&CrcHandle, (uint32_t *)(AppAddr), uwAppSize/4 - 1);
	
	printf("???APP???CRC��??? = 0x%x\r\n", uwExpectedCRCValue);
	printf("????APP???CRC��??? = 0x%x\r\n", uwCRCValue);	
	
	if (uwCRCValue != uwExpectedCRCValue)
	{
		printf("��?????\r\n");
        Error_Handler(__FILE__, __LINE__);		
	}
	else
	{
		printf("��???????????????APP\r\n");
	}
	
	printf("=================================================\r\n");
}

/*
*********************************************************************************************************
*	?? ?? ??: LoadFirmware
*	???????: ???????
*	??    ??: ??
*	?? ?? ?: ??
*********************************************************************************************************
*/
static void LoadFirmware(void)
{
	FRESULT result;
	float FinishPecent;
	uint32_t bw;
	char path[64];
	uint32_t SectorCount = 0;
	uint32_t SectorRemain = 0;
	FILINFO fno;
	uint32_t i = 0;
	uint8_t ucState;
    uint32_t Count = 0;
    uint32_t TotalSize = 0;
	
	
	mbedtls_aes_init(&aes);
	mbedtls_aes_setkey_dec(&aes, key, 128);
	
	/* ??1????????????? ***************************************************************/
	result = f_mount(&fs, DiskPath, 0);		
	if (result != FR_OK)
	{
		printf("???????????? (%s)\r\n", FR_Table[result]);
	}

	/* ??2????????? ***************************************************************/
	sprintf(path, "%saes.bin", DiskPath);
	result = f_open(&file, path, FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : 128.bin\r\n");
		return;
	}
	
	/* ??2????????????��??APP????�� ***********************************************/
	f_stat(path, &fno);

	/* ????????��, ????4G */
	printf("APP?????��??%d\r\n", (int)fno.fsize);
	
	uwAppSize = fno.fsize;

	/* ??4???????????? **************************************************************/
	SectorCount = fno.fsize/(128*1024);
	SectorRemain = fno.fsize%(128*1024);	
	
	for(i = 0; i < SectorCount; i++)
	{
		printf("??????????? = %08x\r\n", AppAddr + i*128*1024);
		bsp_EraseCpuFlash((uint32_t)(AppAddr + i*128*1024));
	}
	
	if(SectorRemain)
	{
		printf("?????????????? = %08x\r\n", AppAddr + i*128*1024);
		bsp_EraseCpuFlash((uint32_t)(AppAddr + i*128*1024));
	}
	
	/* ??5??????APP???��?????Flash ********************************************/
	for(;;)
	{
		/* ?????????????????buf */
		result = f_read(&file, &tempbuf, sizeof(tempbuf), &bw);
		mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, sizeof(tempbuf), iv, tempbuf, output);
		
		
		/* ????????????????????? */
		if ((result != FR_OK)||bw == 0)
		{
			printf("APP??????????\r\n");
			printf("=================================================\r\n");
			break;
		}
		
		/* ??????Flash */
		TotalSize += bw;
		ucState = bsp_WriteCpuFlash((uint32_t)(AppAddr + Count*sizeof(tempbuf)),  (uint8_t *)output, bw);
		
		/* ?????????0???????????? */
		if(ucState != 0)
		{
			printf("???????\r\n");
			break;
		}
		
		/* ?????????? */
		Count = Count + 1;
		FinishPecent = (float)(TotalSize) / fno.fsize;
		printf("???AES?????????:%02d%%\r\n", (uint8_t)(FinishPecent*100));
	}
	
	/* ??????*/
	f_close(&file);

	/* ��??????? */
	f_mount(NULL, DiskPath, 0);
}

/*
*********************************************************************************************************
*	?? ?? ??: ViewRootDir
*	???????: ???SD??????????????
*	??    ??: ??
*	?? ?? ?: ??
*********************************************************************************************************
*/
extern SD_HandleTypeDef uSdHandle;
static void ViewRootDir(void)
{
	FRESULT result;
	uint32_t cnt = 0;
	FILINFO fno;
	
 	/* ????????? */
	result = f_mount(&fs, DiskPath, 0);	/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("???????????? (%s)\r\n", FR_Table[result]);
	}

	/* ???????? */
	result = f_opendir(&DirInf, DiskPath); /* ??????????????????????? */
	if (result != FR_OK)
	{
		printf("????????  (%s)\r\n", FR_Table[result]);
		return;
	}

	printf("????        |  ?????�� | ??????? | ???????\r\n");
	for (cnt = 0; ;cnt++)
	{
		result = f_readdir(&DirInf, &FileInf); 		/* ???????????????????? */
		if (result != FR_OK || FileInf.fname[0] == 0)
		{
			break;
		}

		if (FileInf.fname[0] == '.')
		{
			continue;
		}

		/* ?��?????????????? */
		if (FileInf.fattrib & AM_DIR)
		{
			printf("(0x%02d)??  ", FileInf.fattrib);
		}
		else
		{
			printf("(0x%02d)???  ", FileInf.fattrib);
		}

		f_stat(FileInf.fname, &fno);
		
		/* ????????��, ????4G */
		printf(" %10d", (int)fno.fsize);


		printf("  %s\r\n", (char *)FileInf.fname);	/* ??????? */
	}
 
    /* ??????????? */
    if(uSdHandle.SdCard.CardSpeed == CARD_NORMAL_SPEED)
    {
        printf("Normal Speed Card <12.5MB/S, MAX Clock < 25MHz, Spec Version 1.01\r\n");           
    }
    else if (uSdHandle.SdCard.CardSpeed == CARD_HIGH_SPEED)
    {
        printf("High Speed Card <25MB/s, MAX Clock < 50MHz, Spec Version 2.00\r\n");            
    }
    else if (uSdHandle.SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED)
    {
        printf("UHS-I SD Card <50MB/S for SDR50, DDR50 Cards, MAX Clock < 50MHz OR 100MHz\r\n");
        printf("UHS-I SD Card <104MB/S for SDR104, MAX Clock < 108MHz, Spec version 3.01\r\n");   
    }    

    
	/* ��??????? */
	 f_mount(NULL, DiskPath, 0);
}

/***************************** ?????????? www.armfly.com (END OF FILE) *********************************/
