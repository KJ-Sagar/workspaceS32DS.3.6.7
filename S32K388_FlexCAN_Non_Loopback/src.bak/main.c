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
#include "Mcal.h"
#include "Clock_Ip.h"
#include "FlexCAN_Ip.h"
#include "IntCtrl_Ip.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

#define RX_MB_IDX                  (0U)
#define TX_MB_IDX                  (1U)
#define FLEXCAN_NUMBER_OF_MSG      (10U)
#define TIMEOUT_VALUE              (10000U)
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

uint8 au8Data[8U] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U};
volatile uint8 u8TxConfirmCnt = 0U;
volatile uint8 u8RxIndicationCnt = 0U;
volatile boolean bTxFlag = FALSE;
volatile boolean bRxFlag = FALSE;
Flexcan_Ip_MsgBuffType aRxDataBuffer[FLEXCAN_NUMBER_OF_MSG];
Flexcan_Ip_DataInfoType RxInfo = {
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .data_length = 8U,
        .is_polling = FALSE,
        .is_remote = FALSE
};
Flexcan_Ip_DataInfoType TxInfo = {
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .data_length = 8U,
        .is_polling = TRUE,
        .is_remote = FALSE
};
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

void FlexCAN_UserCallback(uint8 instance,
                          Flexcan_Ip_EventType eventType,
                          uint32 buffIdx,
                          const struct FlexCANState *driverState)

{
    if (FLEXCAN_EVENT_RX_COMPLETE == eventType)
    {
        u8RxIndicationCnt++;
        if (u8RxIndicationCnt == FLEXCAN_NUMBER_OF_MSG)
        {
            bRxFlag = TRUE;
        }
        else
        {
            /* Prepare to receive the next message */
            FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MB_IDX, &aRxDataBuffer[u8RxIndicationCnt], FALSE);
        }
    }
    if (FLEXCAN_EVENT_TX_COMPLETE == eventType)
    {
        u8TxConfirmCnt++;
        if (u8TxConfirmCnt == FLEXCAN_NUMBER_OF_MSG)
        {
            bTxFlag = TRUE;
        }
    }
    (void)instance;
    (void)buffIdx;
    (void)driverState;
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

int main(void)
{
    uint32 u32TimeOut = 0U;

    /* Initialize Clock */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);

    /* Initialize Interrupt */
    IntCtrl_Ip_Init(&IntCtrlConfig_0);

    /* Initialize FlexCAN driver */
    FlexCAN_Ip_Init(INST_FLEXCAN_0, &FlexCAN_State0, &FlexCAN_Config0);

    /* Set Rx filter mask type */
    FlexCAN_Ip_SetRxMaskType(INST_FLEXCAN_0, FLEXCAN_RX_MASK_INDIVIDUAL);

    /* Set Rx individual mask value */
    /* Expect to receive all IDs, mask = 0x0 */
    FlexCAN_Ip_SetRxIndividualMask(INST_FLEXCAN_0, RX_MB_IDX, 0x0U);

    /* Start FlexCAN controller */
    FlexCAN_Ip_SetStartMode(INST_FLEXCAN_0);

    /* Configure Rx message buffer */
    FlexCAN_Ip_ConfigRxMb(INST_FLEXCAN_0, RX_MB_IDX, &RxInfo, 0x0U);

    /* Start trigger to receive messages */
    FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MB_IDX, &aRxDataBuffer[0], FALSE);

    /* Perform transmit multiple messages */
    for (uint8 MsgIdx = 1U; MsgIdx <= x; MsgIdx++)
    {
        u32TimeOut = TIMEOUT_VALUE;
        FlexCAN_Ip_Send(INST_FLEXCAN_0, TX_MB_IDX, &TxInfo, MsgIdx, au8Data);
        while ((u8TxConfirmCnt != MsgIdx) && (u32TimeOut != 0U))
        {
            FlexCAN_Ip_MainFunctionWrite(INST_FLEXCAN_0, TX_MB_IDX);
            u32TimeOut--;
        }
    }

    /* Stop FlexCAN controller */
    FlexCAN_Ip_SetStopMode(INST_FLEXCAN_0);

    /* De-Initialize FlexCAN driver */
    FlexCAN_Ip_Deinit(INST_FLEXCAN_0);

    /* Validate if transfer process is complete and store the result */
    Exit_Example((bTxFlag && bRxFlag) == TRUE);

    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */
