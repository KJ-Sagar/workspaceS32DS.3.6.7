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
#define FLEXCAN_FIRST_STD_ID       (1U)
#define FLEXCAN_NUMBER_OF_MSG      (10U)
#define FLEXCAN_DATA_LENGTH        (8U)
#define TIMEOUT_VALUE              (10000U)
#define SEMIHOST_SYS_WRITE0        (0x04U)
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

uint8 au8Data[FLEXCAN_DATA_LENGTH] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U};
volatile uint8 u8TxConfirmCnt = 0U;
volatile uint8 u8RxIndicationCnt = 0U;
volatile boolean bTxFlag = FALSE;
volatile boolean bRxFlag = FALSE;
Flexcan_Ip_MsgBuffType aRxDataBuffer[FLEXCAN_NUMBER_OF_MSG];
Flexcan_Ip_DataInfoType RxInfo = {
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .data_length = FLEXCAN_DATA_LENGTH,
        .is_polling = FALSE,
        .is_remote = FALSE
};
Flexcan_Ip_DataInfoType TxInfo = {
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .data_length = FLEXCAN_DATA_LENGTH,
        .is_polling = TRUE,
        .is_remote = FALSE
};
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

static void Semihost_Write0(const char *pText)
{
    register uint32 u32Operation __asm("r0") = SEMIHOST_SYS_WRITE0;
    register const char *pParameter __asm("r1") = pText;

    __asm volatile ("bkpt 0xAB"
                    : "+r" (u32Operation)
                    : "r" (pParameter)
                    : "memory");
}

static void Trace_AppendChar(char **ppCursor, char cValue)
{
    **ppCursor = cValue;
    (*ppCursor)++;
}

static void Trace_AppendString(char **ppCursor, const char *pText)
{
    while (*pText != '\0')
    {
        Trace_AppendChar(ppCursor, *pText);
        pText++;
    }
}

static void Trace_AppendHexNibble(char **ppCursor, uint8 u8Value)
{
    char cHexValue;

    u8Value &= 0x0FU;
    if (u8Value < 10U)
    {
        cHexValue = (char)('0' + (char)u8Value);
    }
    else
    {
        cHexValue = (char)('A' + (char)(u8Value - 10U));
    }
    Trace_AppendChar(ppCursor, cHexValue);
}

static void Trace_AppendHexByte(char **ppCursor, uint8 u8Value)
{
    Trace_AppendHexNibble(ppCursor, (uint8)(u8Value >> 4U));
    Trace_AppendHexNibble(ppCursor, u8Value);
}

static void Trace_AppendStdId(char **ppCursor, uint32 u32MessageId)
{
    Trace_AppendHexNibble(ppCursor, (uint8)(u32MessageId >> 8U));
    Trace_AppendHexNibble(ppCursor, (uint8)(u32MessageId >> 4U));
    Trace_AppendHexNibble(ppCursor, (uint8)u32MessageId);
}

static const char * Trace_StatusString(Flexcan_Ip_StatusType eStatus)
{
    const char *pStatusText = "ERROR";

    switch (eStatus)
    {
        case FLEXCAN_STATUS_SUCCESS:
            pStatusText = "OK";
            break;
        case FLEXCAN_STATUS_BUSY:
            pStatusText = "BUSY";
            break;
        case FLEXCAN_STATUS_TIMEOUT:
            pStatusText = "TIMEOUT";
            break;
        case FLEXCAN_STATUS_BUFF_OUT_OF_RANGE:
            pStatusText = "BUFF_OUT_OF_RANGE";
            break;
        case FLEXCAN_STATUS_NO_TRANSFER_IN_PROGRESS:
            pStatusText = "NO_TRANSFER";
            break;
        case FLEXCAN_STATUS_ERROR:
        default:
            pStatusText = "ERROR";
            break;
    }

    return pStatusText;
}

static void Trace_CanFrame(uint32 u32MessageId,
                           const uint8 *pData,
                           uint8 u8Length,
                           Flexcan_Ip_StatusType eStatus)
{
    char aTraceLine[96U];
    char *pCursor = aTraceLine;

    Trace_AppendString(&pCursor, "TX id=0x");
    Trace_AppendStdId(&pCursor, u32MessageId);
    Trace_AppendString(&pCursor, " dlc=");
    Trace_AppendChar(&pCursor, (char)('0' + u8Length));
    Trace_AppendString(&pCursor, " data=");

    for (uint8 u8Idx = 0U; u8Idx < u8Length; u8Idx++)
    {
        if (u8Idx != 0U)
        {
            Trace_AppendChar(&pCursor, ' ');
        }
        Trace_AppendHexByte(&pCursor, pData[u8Idx]);
    }

    Trace_AppendString(&pCursor, " status=");
    Trace_AppendString(&pCursor, Trace_StatusString(eStatus));
    Trace_AppendString(&pCursor, "\r\n");
    Trace_AppendChar(&pCursor, '\0');

    Semihost_Write0(aTraceLine);
}

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
    boolean bTransferSuccess = TRUE;

    /* Initialize Clock */
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);

    /* Initialize Interrupt */
    IntCtrl_Ip_Init(&IntCtrlConfig_0);

    /* Initialize FlexCAN driver */
    if (FLEXCAN_STATUS_SUCCESS != FlexCAN_Ip_Init(INST_FLEXCAN_0, &FlexCAN_State0, &FlexCAN_Config0))
    {
        bTransferSuccess = FALSE;
    }

    /* Set Rx filter mask type */
    if (FLEXCAN_STATUS_SUCCESS != FlexCAN_Ip_SetRxMaskType(INST_FLEXCAN_0, FLEXCAN_RX_MASK_INDIVIDUAL))
    {
        bTransferSuccess = FALSE;
    }

    /* Set Rx individual mask value */
    /* Expect to receive all IDs, mask = 0x0 */
    if (FLEXCAN_STATUS_SUCCESS != FlexCAN_Ip_SetRxIndividualMask(INST_FLEXCAN_0, RX_MB_IDX, 0x0U))
    {
        bTransferSuccess = FALSE;
    }

    /* Start FlexCAN controller */
    if (FLEXCAN_STATUS_SUCCESS != FlexCAN_Ip_SetStartMode(INST_FLEXCAN_0))
    {
        bTransferSuccess = FALSE;
    }

    /* Configure passive Rx message buffer for optional replies from a VDK CAN peer. */
    if (FLEXCAN_STATUS_SUCCESS != FlexCAN_Ip_ConfigRxMb(INST_FLEXCAN_0, RX_MB_IDX, &RxInfo, 0x0U))
    {
        bTransferSuccess = FALSE;
    }

    /* Start trigger to receive messages */
    if (FLEXCAN_STATUS_SUCCESS != FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MB_IDX, &aRxDataBuffer[0], FALSE))
    {
        bTransferSuccess = FALSE;
    }

    Semihost_Write0("FlexCAN normal-mode TX trace\r\n");

    /* Perform transmit multiple messages */
    for (uint8 MsgIdx = 0U; MsgIdx < FLEXCAN_NUMBER_OF_MSG; MsgIdx++)
    {
        const uint32 u32MessageId = FLEXCAN_FIRST_STD_ID + MsgIdx;
        const uint8 u8ExpectedConfirmCnt = MsgIdx + 1U;
        Flexcan_Ip_StatusType eTxStatus = FLEXCAN_STATUS_SUCCESS;

        u32TimeOut = TIMEOUT_VALUE;
        eTxStatus = FlexCAN_Ip_Send(INST_FLEXCAN_0, TX_MB_IDX, &TxInfo, u32MessageId, au8Data);
        while ((FLEXCAN_STATUS_SUCCESS == eTxStatus) && (u8TxConfirmCnt != u8ExpectedConfirmCnt) && (u32TimeOut != 0U))
        {
            FlexCAN_Ip_MainFunctionWrite(INST_FLEXCAN_0, TX_MB_IDX);
            u32TimeOut--;
        }

        if ((FLEXCAN_STATUS_SUCCESS == eTxStatus) && (u8TxConfirmCnt != u8ExpectedConfirmCnt))
        {
            eTxStatus = FLEXCAN_STATUS_TIMEOUT;
            (void)FlexCAN_Ip_AbortTransfer(INST_FLEXCAN_0, TX_MB_IDX);
        }

        if (FLEXCAN_STATUS_SUCCESS != eTxStatus)
        {
            bTransferSuccess = FALSE;
        }

        Trace_CanFrame(u32MessageId, au8Data, FLEXCAN_DATA_LENGTH, eTxStatus);
    }

    /* Stop FlexCAN controller */
    FlexCAN_Ip_SetStopMode(INST_FLEXCAN_0);

    /* De-Initialize FlexCAN driver */
    FlexCAN_Ip_Deinit(INST_FLEXCAN_0);

    /* Validate if transfer process is complete and store the result */
    Exit_Example((bTransferSuccess && bTxFlag) == TRUE);

    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */
