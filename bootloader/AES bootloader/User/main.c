/*
*********************************************************************************************************
*
*	模锟斤拷锟斤拷锟斤拷 : 锟斤拷锟斤拷锟斤拷模锟斤拷
*	锟侥硷拷锟斤拷锟斤拷 : main.c
*	锟斤拷    锟斤拷 : V1.0
*	说    锟斤拷 : 锟斤拷锟斤拷SD锟斤拷锟斤拷BootLoader锟斤拷锟教ｏ拷锟斤拷AES锟斤拷锟斤拷
*              实锟斤拷目锟侥ｏ拷
*                1. 学习使锟斤拷SD锟斤拷锟斤拷BootLoader实锟街ｏ拷锟斤拷AES锟斤拷锟斤拷
*              实锟斤拷锟斤拷锟斤拷锟�
*                锟矫伙拷通锟斤拷锟斤拷锟皆端达拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷1-2锟斤拷锟斤拷锟斤拷锟藉即锟斤拷
*                1 - 锟斤拷示SD锟斤拷锟斤拷目录锟铰碉拷锟侥硷拷锟斤拷
*                2 - 锟斤拷锟斤拷锟教硷拷锟斤拷锟斤拷, 校锟斤拷锟斤拷锟阶�
*              注锟斤拷锟斤拷锟筋：
*                1. 锟斤拷实锟斤拷锟狡硷拷使锟矫达拷锟斤拷锟斤拷锟斤拷SecureCRT锟斤拷锟斤拷H7-TOOL锟斤拷位锟斤拷锟斤拷锟斤拷锟介看锟斤拷印锟斤拷息锟斤拷锟斤拷锟斤拷锟斤拷115200锟斤拷锟斤拷锟斤拷位8锟斤拷锟斤拷偶校锟斤拷位锟睫ｏ拷停止位1锟斤拷
*                2. 锟斤拷亟锟斤拷嗉�锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷TAB锟斤拷锟斤拷为4锟斤拷锟侥讹拷锟斤拷锟侥硷拷锟斤拷要锟斤拷锟斤拷锟斤拷锟斤拷示锟斤拷锟斤拷锟诫。
*
*	锟睫改硷拷录 :
*		锟芥本锟斤拷   锟斤拷锟斤拷         锟斤拷锟斤拷        说锟斤拷
*		V1.0    2022-06-26   Eric2013     1. CMSIS锟斤拷锟斤拷锟芥本 V5.7.0
*                                         2. HAL锟斤拷姹� V1.9.0
*                                         3. FatFS锟芥本V0.12c
*                                         
*	Copyright (C), 2020-2030, 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷 www.armfly.com
*
*********************************************************************************************************
*/	
#include "bsp.h"			 /* 锟阶诧拷硬锟斤拷锟斤拷锟斤拷 */


/* 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟教凤拷锟斤拷锟斤拷锟斤拷 */
#define EXAMPLE_NAME	"V7-锟斤拷锟斤拷SD锟斤拷锟斤拷BootLoader锟斤拷锟斤拷AES锟斤拷锟斤拷"
#define EXAMPLE_DATE	"2022-06-26"
#define DEMO_VER		"1.0"

static void PrintfLogo(void);
extern void DemoFatFS(void);

/*
*********************************************************************************************************
*	锟斤拷 锟斤拷 锟斤拷: main
*	锟斤拷锟斤拷说锟斤拷: c锟斤拷锟斤拷锟斤拷锟�
*	锟斤拷    锟斤拷: 锟斤拷
*	锟斤拷 锟斤拷 值: 锟斤拷锟斤拷锟斤拷锟�(锟斤拷锟借处锟斤拷)
*********************************************************************************************************
*/
int main(void)
{
	bsp_Init();		/* 硬锟斤拷锟斤拷始锟斤拷 */
	
	PrintfLogo();	/* 锟斤拷印锟斤拷锟斤拷锟斤拷锟狡和版本锟斤拷锟斤拷息 */

	DemoFatFS();    /* SD锟斤拷锟斤拷锟斤拷 */

	// lbh  2023.10.23锟睫革拷
}

/*
*********************************************************************************************************
*	锟斤拷 锟斤拷 锟斤拷: PrintfLogo
*	锟斤拷锟斤拷说锟斤拷: 锟斤拷印锟斤拷锟斤拷锟斤拷锟狡猴拷锟斤拷锟教凤拷锟斤拷锟斤拷锟斤拷, 锟斤拷锟较达拷锟斤拷锟竭后，达拷PC锟斤拷锟侥筹拷锟斤拷锟秸讹拷锟斤拷锟斤拷锟斤拷锟皆观诧拷锟斤拷
*	锟斤拷    锟斤拷: 锟斤拷
*	锟斤拷 锟斤拷 值: 锟斤拷
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	printf("*************************************************************\r\n");
	printf("lbh first change");
	
	/* 锟斤拷锟紺PU ID */
	{
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;
		
		CPU_Sn0 = *(__IO uint32_t*)(0x1FF1E800);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FF1E800 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FF1E800 + 8);

		printf("CPU : STM32H743XIH6, BGA240, 锟斤拷频: %dMHz\r\n", SystemCoreClock / 1000000);
		printf("UID = %08X %08X %08X\r\n", CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("*************************************************************\r\n");
	printf("* 锟斤拷锟斤拷锟斤拷锟斤拷   : %s\r\n", EXAMPLE_NAME);	/* 锟斤拷印锟斤拷锟斤拷锟斤拷锟斤拷 */
	printf("* 锟斤拷锟教版本   : %s\r\n", DEMO_VER);		/* 锟斤拷印锟斤拷锟教版本 */
	printf("* 锟斤拷锟斤拷锟斤拷锟斤拷   : %s\r\n", EXAMPLE_DATE);	/* 锟斤拷印锟斤拷锟斤拷锟斤拷锟斤拷 */

	/* 锟斤拷印ST锟斤拷HAL锟斤拷姹� */
	printf("* HAL锟斤拷姹�  : V1.9.0 (STM32H7xx HAL Driver)\r\n");
	printf("* \r\n");	/* 锟斤拷印一锟叫空革拷 */
	printf("* QQ    : 1295744630 \r\n");
	printf("* 锟斤拷锟斤拷  : armfly\r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* 微锟脚癸拷锟节猴拷: anfulai_com \r\n");
	printf("* 锟皆憋拷锟斤拷: anfulai.taobao.com\r\n");
	printf("* Copyright www.armfly.com 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷\r\n");
	printf("*************************************************************\r\n");
}

/***************************** 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷 www.armfly.com (END OF FILE) *********************************/