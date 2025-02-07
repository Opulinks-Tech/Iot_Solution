/******************************************************************************
*  Copyright 2017 - 2020, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2020
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim_default_group08_project.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
const T_MwFim_GP08_Boot_Status g_tMwFimDefaultGp08BootStatus = {0};

// the address buffer of device schedule
uint32_t g_ulaMwFimAddrBufferGP08BootStatus[MW_FIM_GP08_BOOT_STATUS_NUM];

// the information table of group 08
const T_MwFimFileInfo g_taMwFimGroupTable08_project[] =
{
    {MW_FIM_IDX_GP08_PROJECT_BOOT_STATUS, MW_FIM_GP08_BOOT_STATUS_NUM,  MW_FIM_GP08_BOOT_STATUS_SIZE, (uint8_t*)&g_tMwFimDefaultGp08BootStatus, g_ulaMwFimAddrBufferGP08BootStatus},

    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
