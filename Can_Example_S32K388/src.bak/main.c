/*==================================================================================================
*   Project              : RTD AUTOSAR 4.9
*   Platform             : CORTEXM
*   Peripheral           : FLEXCAN
*   Dependencies         : 
*
*   Autosar Version      : 4.9.0
*   Autosar Revision     : ASR_REL_4_9_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 7.0.1
*   Build Version        : S32K3_RTD_7_0_1_D2602_ASR_REL_4_9_REV_0000_20260206
*
*   Copyright 2020 - 2026 NXP
*
*   NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcu.h"
#include "Platform.h"
#include "Can_43_FLEXCAN.h"
#include "SchM_Can_43_FLEXCAN.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

#define CAN_NUMBER_OF_MSG     (10U)
#define TIMEOUT_VALUE         (1000U)
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

uint8 Can_au8Sdu[8U] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U};
volatile uint8 CanIf_u8TxConfirmCnt = 0U;
volatile uint8 CanIf_u8RxIndicationCnt = 0U;
volatile boolean CanIf_bTxFlag = FALSE;
volatile boolean CanIf_bRxFlag = FALSE;
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

Can_PduType Can_CreatePduInfo(Can_IdType id, PduIdType swPduHandle, uint8 length, uint8* sdu)
{
    Can_PduType PduInfo;
    
    PduInfo.id = id;
    PduInfo.swPduHandle = swPduHandle;
    PduInfo.length = length;
    PduInfo.sdu = sdu;
    
    return PduInfo;
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void CanIf_ControllerBusOff(uint8 ControllerId)
{
    (void)ControllerId;
}

void CanIf_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode )
{
    (void)ControllerId;
    (void)ControllerMode;
}

void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
    CanIf_u8TxConfirmCnt++;
    if(CanIf_u8TxConfirmCnt == CAN_NUMBER_OF_MSG)
    {
        CanIf_bTxFlag = TRUE;
    }
    (void)CanTxPduId;
}

void CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr )
{
    CanIf_u8RxIndicationCnt++;
    if(CanIf_u8RxIndicationCnt == CAN_NUMBER_OF_MSG)
    {
        CanIf_bRxFlag = TRUE;
    }
    (void)Mailbox;
    (void)PduInfoPtr;
}

int main(void)
{
    uint32 u32TimeOut = 0U;
    Can_PduType Can_PduInfo[CAN_NUMBER_OF_MSG];

    /* Prepare Pdu data for transmission */
    for (uint8 MsgIdx = 1U; MsgIdx <= CAN_NUMBER_OF_MSG; MsgIdx++)
    {
        Can_PduInfo[MsgIdx - 1U] = Can_CreatePduInfo(MsgIdx, MsgIdx, 8U, Can_au8Sdu);
    }

    /* Initialize Platform driver */
    Platform_Init(NULL_PTR);

    /* Initialize Mcu driver */
#if (MCU_PRECOMPILE_SUPPORT == STD_ON)
    Mcu_Init(NULL_PTR);
#else
    Mcu_Init(&Mcu_Config_VS_0);
#endif
    Mcu_InitClock(McuClockSettingConfig_0);
#if (MCU_NO_PLL == STD_OFF)
    while ( MCU_PLL_LOCKED != Mcu_GetPllStatus() )
    {
        /* Busy wait until the System PLL is locked */
    }
    Mcu_DistributePllClock();
#endif
    Mcu_SetMode(McuModeSettingConf_0);

    /* Initialize Can driver */
#if (CAN_43_FLEXCAN_PRECOMPILE_SUPPORT == STD_ON)
    Can_43_FLEXCAN_Init(NULL_PTR);
#else
    Can_43_FLEXCAN_Init(&Can_43_FLEXCAN_Config_VS_0);
#endif

    /* Start Can controller */
    Can_43_FLEXCAN_SetControllerMode(Can_43_FLEXCANConf_CanController_CanController_0, CAN_CS_STARTED);

    /* Perform transmit multiple Pdu */
    for (uint8 MsgIdx = 1U; MsgIdx <= CAN_NUMBER_OF_MSG; MsgIdx++)
    {
        u32TimeOut = TIMEOUT_VALUE;
        Can_43_FLEXCAN_Write(Can_43_FLEXCANConf_CanHardwareObject_CanHardwareObject_1, &Can_PduInfo[MsgIdx - 1U]);
        while ((CanIf_u8TxConfirmCnt != MsgIdx) && (u32TimeOut != 0U))
        {
            Can_43_FLEXCAN_MainFunction_Write();
            u32TimeOut--;
        }
    }

    /* Stop Can controller */
    Can_43_FLEXCAN_SetControllerMode(Can_43_FLEXCANConf_CanController_CanController_0, CAN_CS_STOPPED);

    /* De-Initialize Can driver */
    Can_43_FLEXCAN_DeInit();

    /* Validate if transfer process is complete and store the result */
    Exit_Example((CanIf_bTxFlag && CanIf_bRxFlag) == TRUE);

    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */
