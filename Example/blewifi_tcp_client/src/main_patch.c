/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  main_patch.c
*
*  Project:
*  --------
*  OPL1000 Project - the main patch implement file
*
*  Description:
*  ------------
*  This implement file is include the main patch function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include "sys_init.h"
#include "sys_init_patch.h"
#include "hal_system.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#ifdef __BLEWIFI_TRANSPARENT__
#include "at_cmd_app_patch.h"
#include "hal_pin_config_project_transparent.h"
#else
#include "hal_pin_config_project.h"
#endif
#include "at_cmd_common_patch.h"
#include "mw_fim.h"
#include "mw_fim_default.h"
#include "hal_dbg_uart.h"
#include "hal_vic.h"
#include "boot_sequence.h"

#include "blewifi_app.h"
#include "blewifi_configuration.h"
#include "mw_fim_default_version_project.h"
#include "mw_fim_default_group11_project.h"
#include "uart.h"


//#include "hal_wdt.h"
// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern uint8_t* g_ucaMemPartAddr;
extern uint32_t g_ulMemPartTotalSize;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
static E_IO01_UART_MODE g_eAppIO01UartMode;


// Sec 7: declaration of static function prototype
void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
void __Patch_EntryPoint(void) __attribute__((used));
static void Main_PinMuxUpdate(void);
static void Main_FlashLayoutUpdate(void);
static void Main_MiscDriverConfigSetup(void);
static void Main_AtUartDbgUartSwitch(void);
static void Main_AppInit_patch(void);
#ifdef __BLEWIFI_TRANSPARENT__
static int Main_BleWifiInit(void);
#endif
static void Main_ApsUartRxDectecConfig(void);
static void Main_ApsUartRxDectecCb(E_GpioIdx_t tGpioIdx);

/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   __Patch_EntryPoint
*
* DESCRIPTION:
*   the entry point of SW patch
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void __Patch_EntryPoint(void)
{
    // don't remove this code
    SysInit_EntryPoint();

#ifdef SWITCH_TO_32K_RC
    // Uncomment this function when the device is without 32k XTAL.
    Sys_SwitchTo32kRC();
#endif

    // update the pin mux
    Hal_SysPinMuxAppInit = Main_PinMuxUpdate;

    // update the flash layout
    MwFim_FlashLayoutUpdate = Main_FlashLayoutUpdate;

    // the initial of driver part for cold and warm boot
    Sys_MiscDriverConfigSetup = Main_MiscDriverConfigSetup;

    // update the switch AT UART / dbg UART function
    at_cmd_switch_uart1_dbguart = Main_AtUartDbgUartSwitch;

    // modify the heap size, from 0x43C000 to 0x44F000
    g_ucaMemPartAddr = (uint8_t*) 0x42E000;
    g_ulMemPartTotalSize = 0x21000;

    Sys_SetUnsuedSramEndBound(0x42E000);

    // application init
    Sys_AppInit = Main_AppInit_patch;
#ifdef __BLEWIFI_TRANSPARENT__
    at_blewifi_init_adpt = Main_BleWifiInit;
#endif
}

/*************************************************************************
* FUNCTION:
*   Main_PinMuxUpdate
*
* DESCRIPTION:
*   update the flash layout
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_PinMuxUpdate(void)
{
    Hal_Pin_ConfigSet(0, HAL_PIN_TYPE_IO_0, HAL_PIN_DRIVING_IO_0);
    Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_IO_1, HAL_PIN_DRIVING_IO_1);
    Hal_Pin_ConfigSet(2, HAL_PIN_TYPE_IO_2, HAL_PIN_DRIVING_IO_2);
    Hal_Pin_ConfigSet(3, HAL_PIN_TYPE_IO_3, HAL_PIN_DRIVING_IO_3);
    Hal_Pin_ConfigSet(4, HAL_PIN_TYPE_IO_4, HAL_PIN_DRIVING_IO_4);
    Hal_Pin_ConfigSet(5, HAL_PIN_TYPE_IO_5, HAL_PIN_DRIVING_IO_5);
    Hal_Pin_ConfigSet(6, HAL_PIN_TYPE_IO_6, HAL_PIN_DRIVING_IO_6);
    Hal_Pin_ConfigSet(7, HAL_PIN_TYPE_IO_7, HAL_PIN_DRIVING_IO_7);
    Hal_Pin_ConfigSet(8, HAL_PIN_TYPE_IO_8, HAL_PIN_DRIVING_IO_8);
    Hal_Pin_ConfigSet(9, HAL_PIN_TYPE_IO_9, HAL_PIN_DRIVING_IO_9);
    Hal_Pin_ConfigSet(10, HAL_PIN_TYPE_IO_10, HAL_PIN_DRIVING_IO_10);
    Hal_Pin_ConfigSet(11, HAL_PIN_TYPE_IO_11, HAL_PIN_DRIVING_IO_11);
    Hal_Pin_ConfigSet(12, HAL_PIN_TYPE_IO_12, HAL_PIN_DRIVING_IO_12);
    Hal_Pin_ConfigSet(13, HAL_PIN_TYPE_IO_13, HAL_PIN_DRIVING_IO_13);
    Hal_Pin_ConfigSet(14, HAL_PIN_TYPE_IO_14, HAL_PIN_DRIVING_IO_14);
    Hal_Pin_ConfigSet(15, HAL_PIN_TYPE_IO_15, HAL_PIN_DRIVING_IO_15);
    Hal_Pin_ConfigSet(16, HAL_PIN_TYPE_IO_16, HAL_PIN_DRIVING_IO_16);
    Hal_Pin_ConfigSet(17, HAL_PIN_TYPE_IO_17, HAL_PIN_DRIVING_IO_17);
    Hal_Pin_ConfigSet(18, HAL_PIN_TYPE_IO_18, HAL_PIN_DRIVING_IO_18);
    Hal_Pin_ConfigSet(19, HAL_PIN_TYPE_IO_19, HAL_PIN_DRIVING_IO_19);
    Hal_Pin_ConfigSet(20, HAL_PIN_TYPE_IO_20, HAL_PIN_DRIVING_IO_20);
    Hal_Pin_ConfigSet(21, HAL_PIN_TYPE_IO_21, HAL_PIN_DRIVING_IO_21);
    Hal_Pin_ConfigSet(22, HAL_PIN_TYPE_IO_22, HAL_PIN_DRIVING_IO_22);
    Hal_Pin_ConfigSet(23, HAL_PIN_TYPE_IO_23, HAL_PIN_DRIVING_IO_23);

    g_eAppIO01UartMode = HAL_PIN_0_1_UART_MODE;
}

/*************************************************************************
* FUNCTION:
*   Main_FlashLayoutUpdate
*
* DESCRIPTION:
*   update the flash layout
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_FlashLayoutUpdate(void)
{
#ifdef __BLEWIFI_TRANSPARENT__
    MwFim_GroupInfoUpdate(0, 8, (T_MwFimFileInfo *)g_taMwFimGroupTable11_project);
    MwFim_GroupVersionUpdate(0, 8, MW_FIM_VER11_PROJECT);
#else
    g_taMwFimZoneInfoTable[1].ulBaseAddr = 0x00090000;
    g_taMwFimZoneInfoTable[1].ulBlockNum = 9;

    // Register FIM table
    MwFim_GroupInfoUpdate(1, 1, (T_MwFimFileInfo *)g_taMwFimGroupTable11_project);
    
    // Update FIM Version
    MwFim_GroupVersionUpdate(1, 1, MW_FIM_VER11_PROJECT);
    MwFim_GroupVersionUpdate(1, 2, MW_FIM_VER12_PROJECT);
    MwFim_GroupVersionUpdate(1, 3, MW_FIM_VER13_PROJECT);
    MwFim_GroupVersionUpdate(1, 4, MW_FIM_VER14_PROJECT);
    MwFim_GroupVersionUpdate(1, 5, MW_FIM_VER15_PROJECT);
    MwFim_GroupVersionUpdate(1, 6, MW_FIM_VER16_PROJECT);
    MwFim_GroupVersionUpdate(1, 7, MW_FIM_VER17_PROJECT);
    MwFim_GroupVersionUpdate(1, 8, MW_FIM_VER18_PROJECT);
#endif
}

/*************************************************************************
* FUNCTION:
*   Main_MiscDriverConfigSetup
*
* DESCRIPTION:
*   the initial of driver part for cold and warm boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_MiscDriverConfigSetup(void)
{
    //Hal_Wdt_Stop();   //disable watchdog here.

    // IO 1 : detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        Hal_DbgUart_RxIntEn(0);

        if (HAL_PIN_TYPE_IO_1 == PIN_TYPE_UART_APS_RX)
        {
            Main_ApsUartRxDectecConfig();
        }
    }

#if (IOT_UART0_EN == 1)
    // UART Init
    Iot_Uart0_Init();
#endif
}

/*************************************************************************
* FUNCTION:
*   Main_AtUartDbgUartSwitch
*
* DESCRIPTION:
*   switch the UART1 and dbg UART
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AtUartDbgUartSwitch(void)
{
    if (g_eAppIO01UartMode == IO01_UART_MODE_AT)
    {
        Hal_Pin_ConfigSet(0, PIN_TYPE_UART_APS_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(1, PIN_TYPE_UART_APS_RX, PIN_DRIVING_LOW);

        Hal_Pin_ConfigSet(8, PIN_TYPE_UART1_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(9, PIN_TYPE_UART1_RX, PIN_DRIVING_HIGH);

        Hal_DbgUart_RxIntEn(1);
    }
    else
    {
        Hal_DbgUart_RxIntEn(0);

        Hal_Pin_ConfigSet(0, PIN_TYPE_UART1_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(1, PIN_TYPE_UART1_RX, PIN_DRIVING_LOW);

        Hal_Pin_ConfigSet(8, PIN_TYPE_UART_APS_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(9, PIN_TYPE_UART_APS_RX, PIN_DRIVING_HIGH);
    }

    g_eAppIO01UartMode = (E_IO01_UART_MODE)!g_eAppIO01UartMode;
}

/*************************************************************************
* FUNCTION:
*   Main_AppInit_patch
*
* DESCRIPTION:
*   the initial of application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AppInit_patch(void)
{
    // add the application initialization from here
    printf("AppInit\n");

#ifdef __BLEWIFI_TRANSPARENT__
    // the blewifi init will be triggered by AT Cmd
#else
	BleWifiAppInit();
#endif
}

/*************************************************************************
* FUNCTION:
*   Main_BleWifiInit
*
* DESCRIPTION:
*   the initial of application by AT Cmd
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
#ifdef __BLEWIFI_TRANSPARENT__
static int Main_BleWifiInit(void)
{
	BleWifiAppInit();

    return 0;
}
#endif

/*************************************************************************
* FUNCTION:
*   Main_ApsUartRxDectecConfig
*
* DESCRIPTION:
*   detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_ApsUartRxDectecConfig(void)
{
    E_GpioLevel_t eGpioLevel;

    Hal_Pin_ConfigSet(1, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_LOW);
    eGpioLevel = Hal_Vic_GpioInput(GPIO_IDX_01);
    if (GPIO_LEVEL_HIGH == eGpioLevel)
    {
        // it is connected
        Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_IO_1, HAL_PIN_DRIVING_IO_1);
        Hal_DbgUart_RxIntEn(1);
    }
    else //if (GPIO_LEVEL_LOW == eGpioLevel)
    {
        // it is not conncected, set the high level to trigger the GPIO interrupt
        Hal_Vic_GpioCallBackFuncSet(GPIO_IDX_01, Main_ApsUartRxDectecCb);
        //Hal_Vic_GpioDirection(GPIO_IDX_01, GPIO_INPUT);
        Hal_Vic_GpioIntTypeSel(GPIO_IDX_01, INT_TYPE_LEVEL);
        Hal_Vic_GpioIntInv(GPIO_IDX_01, 0);
        Hal_Vic_GpioIntMask(GPIO_IDX_01, 0);
        Hal_Vic_GpioIntEn(GPIO_IDX_01, 1);
    }
}

/*************************************************************************
* FUNCTION:
*   Main_ApsUartRxDectecCb
*
* DESCRIPTION:
*   detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
*
* PARAMETERS
*   1. tGpioIdx : Index of call-back GPIO
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_ApsUartRxDectecCb(E_GpioIdx_t tGpioIdx)
{
    // disable the GPIO interrupt
    Hal_Vic_GpioIntEn(GPIO_IDX_01, 0);

    // it it connected
    Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_IO_1, HAL_PIN_DRIVING_IO_1);
    Hal_DbgUart_RxIntEn(1);
}
