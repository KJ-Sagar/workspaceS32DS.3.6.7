/*
*   (c) Copyright 2020 - 2026 NXP
*   All Rights Reserved.
*
*   NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
*
*   This file contains sample code only. It is not part of the production code deliverables.
*/

#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*   Project              : RTD AUTOSAR 4.9
*   Platform             : CORTEXM
*   Peripheral           : GMAC
*   Dependencies         : none
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
#include "Mcal.h"
#include "Gmac_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Clock_Ip.h"
#include "OsIf.h"

#include "check_example.h"
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define DEMO_ETH_RING          (0U)
#define DEMO_FRAME_LEN         (60U)
#define DEMO_FRAME_COUNT       (16U)
#define DEMO_RX_POLL_LIMIT     (1000000UL)
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

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
static void BuildDemoFrame(uint8 *Data,
                           const uint8 *DstMac,
                           const uint8 *SrcMac,
                           uint32 Sequence)
{
    uint8 Index;

    for (Index = 0U; Index < 6U; Index++)
    {
        Data[Index] = DstMac[Index];
        Data[Index + 6U] = SrcMac[Index];
    }

    Data[12U] = 0x88U;
    Data[13U] = 0xB5U;
    Data[14U] = (uint8)(Sequence >> 24U);
    Data[15U] = (uint8)(Sequence >> 16U);
    Data[16U] = (uint8)(Sequence >> 8U);
    Data[17U] = (uint8)Sequence;

    for (Index = 18U; Index < DEMO_FRAME_LEN; Index++)
    {
        Data[Index] = (uint8)(Sequence + Index);
    }
}

static boolean CheckDemoFrame(const uint8 *Data,
                              const uint8 *DstMac,
                              const uint8 *SrcMac,
                              uint32 Sequence)
{
    uint8 Expected[DEMO_FRAME_LEN];
    uint8 Index;
    boolean Match = TRUE;

    BuildDemoFrame(Expected, DstMac, SrcMac, Sequence);

    for (Index = 0U; Index < DEMO_FRAME_LEN; Index++)
    {
        if (Data[Index] != Expected[Index])
        {
            Match = FALSE;
            break;
        }
    }

    return Match;
}

/**
* @brief        Transmit ordered Ethernet frames from GMAC0 to GMAC1.
* @details      Requires the board pins/PHY path to connect GMAC0 Tx traffic to GMAC1 Rx.
*/
int main(void)
{
    Gmac_Ip_TxOptionsType TxOptions = {TRUE, GMAC_CRC_AND_PAD_INSERTION, GMAC_CHECKSUM_INSERTION_DISABLE};
    Gmac_Ip_BufferType TxBuffer = {0};
    Gmac_Ip_BufferType RxBuffer = {0};
    Gmac_Ip_TxInfoType TxInfo  = {0};
    Gmac_Ip_RxInfoType RxInfo  = {0};
    Gmac_Ip_StatusType Status;
    uint8 TxMacAddr[6U] = {0U};
    uint8 RxMacAddr[6U] = {0U};
    uint32 FrameIndex;
    uint32 PollCount;
    boolean Result = TRUE;

    OsIf_Init(NULL_PTR);
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS_PortContainer_0_VS_0, g_pin_mux_InitConfigArr_PortContainer_0_VS_0);
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0U]);

    Gmac_Ip_Init(INST_GMAC_0, &Gmac_0_ConfigPB_VS_0);
    Gmac_Ip_Init(INST_GMAC_1, &Gmac_1_ConfigPB_VS_0);

    Gmac_Ip_GetMacAddr(INST_GMAC_0, TxMacAddr);
    Gmac_Ip_GetMacAddr(INST_GMAC_1, RxMacAddr);

    for (FrameIndex = 0U; FrameIndex < DEMO_FRAME_COUNT; FrameIndex++)
    {
        TxBuffer.Length = DEMO_FRAME_LEN;
        Status = Gmac_Ip_GetTxBuff(INST_GMAC_0, DEMO_ETH_RING, &TxBuffer, NULL_PTR);
        if ((GMAC_STATUS_SUCCESS != Status) || (TxBuffer.Length < DEMO_FRAME_LEN))
        {
            Result = FALSE;
            break;
        }

        BuildDemoFrame(TxBuffer.Data, RxMacAddr, TxMacAddr, FrameIndex);

        TxBuffer.Length = DEMO_FRAME_LEN;
        if (GMAC_STATUS_SUCCESS != Gmac_Ip_SendFrame(INST_GMAC_0, DEMO_ETH_RING, &TxBuffer, &TxOptions))
        {
            Result = FALSE;
            break;
        }

        do {
            Status = Gmac_Ip_GetTransmitStatus(INST_GMAC_0, DEMO_ETH_RING, &TxBuffer, &TxInfo);
        } while (Status == GMAC_STATUS_BUSY);

        if ((GMAC_STATUS_SUCCESS != Status) || (0U != TxInfo.ErrMask))
        {
            Result = FALSE;
            break;
        }

        PollCount = 0UL;
        do {
            Status = Gmac_Ip_ReadFrame(INST_GMAC_1, DEMO_ETH_RING, &RxBuffer, &RxInfo);
            PollCount++;
        } while ((Status == GMAC_STATUS_RX_QUEUE_EMPTY) && (PollCount < DEMO_RX_POLL_LIMIT));

        if ((GMAC_STATUS_SUCCESS != Status) ||
            (0U != RxInfo.ErrMask) ||
            (RxInfo.PktLen < DEMO_FRAME_LEN) ||
            (FALSE == CheckDemoFrame(RxBuffer.Data, RxMacAddr, TxMacAddr, FrameIndex)))
        {
            Result = FALSE;
            break;
        }

        Gmac_Ip_ProvideRxBuff(INST_GMAC_1, DEMO_ETH_RING, &RxBuffer);
    }

    Gmac_Ip_DisableController(INST_GMAC_0);
    Gmac_Ip_DisableController(INST_GMAC_1);

    Exit_Example(Result);

    return 0;
}
#ifdef __cplusplus
}
#endif
