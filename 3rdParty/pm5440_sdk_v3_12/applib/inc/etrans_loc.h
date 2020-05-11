/*******************************************************************************
*   COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
* --------------------------------------------------------------------------
*  This software embodies materials and concepts which are proprietary and
*  confidential to PMC-Sierra, Inc.
*  PMC-Sierra distributes this software to its customers pursuant to the
*  terms and conditions of the Software License Agreement
*  contained in the text file software.lic that is distributed along with
*  the software. This software can only be utilized if all
*  terms and conditions of the Software License Agreement are
*  accepted. If there are any questions, concerns, or if the
*  Software License Agreement text file, software.lic, is missing please
*  contact PMC-Sierra for assistance.
* --------------------------------------------------------------------------
*   DESCRIPTION :
*
*   NOTES:
*
*******************************************************************************/
#ifndef _ETRANS_LOC_H
#define _ETRANS_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "stdlib.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "etrans.h"
#include "etrans_io.h"
#include "etrans_mtsb_map.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define ENET_ETRANS_DP_SEL_GSUP43_C73                  0x0
#define ENET_ETRANS_DP_SEL_GSUP43_C62                  0x1
#define ENET_ETRANS_DP_SEL_GSUP43_C71                  0x2

/* 
 GSUP43_73 default configuration 
 Refer to GSUP43_C73 reg. doc. for details 
*/
/* RX */
#define ENET_ETRANS_GSUP43_C73_LOS_INV                  0x0
#define ENET_ETRANS_GSUP43_C73_RX_OS_TYPE               0x14
#define ENET_ETRANS_GSUP43_C73_RX_PKT_TYPE              0x13
#define ENET_ETRANS_GSUP43_C73_RX_MIN_OS_RPT            0x4
#define ENET_ETRANS_GSUP43_C73_RX_FULL_OS_RPT_DIS       0x0
#define ENET_ETRANS_GSUP43_C73_RX_OS_ALL_DSCRD          0x0
#define ENET_ETRANS_GSUP43_C73_RX_SEQ_OS_DSCRD_EN       0x1
#define ENET_ETRANS_GSUP43_C73_RX_SIG_OS_DSCRD_EN       0x0
#define ENET_ETRANS_GSUP43_C73_DSCRD_ZONE               0x2
#define ENET_ETRANS_GSUP43_C73_RX_125US_CNT             0x4EAC
/* TX */
#define ENET_ETRANS_GSUP43_C73_TX_OS_TYPE               0x14
#define ENET_ETRANS_GSUP43_C73_TX_PKT_TYPE              0x13
#define ENET_ETRANS_GSUP43_C73_TX_ERR_DROP              0x0
#define ENET_ETRANS_GSUP43_C73_FORCE_MIN_IPG            0x0
#define ENET_ETRANS_GSUP43_C73_UNIDIR_EN                0x1
#define ENET_ETRANS_GSUP43_C73_SEND_IDLE_EN             0x0
#define ENET_ETRANS_GSUP43_C73_SEND_LF_EN               0x0
#define ENET_ETRANS_GSUP43_C73_SEND_RF_EN               0x0
#define ENET_ETRANS_GSUP43_C73_FIFO_HMARK               0x6
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*
** Global variables
*/

/*
** Function Prototypes
*/
                                                                                                                                       
#ifdef __cplusplus
}
#endif

#endif /* _ETRANS_LOC_H */

/* 
** end of file 
*/

