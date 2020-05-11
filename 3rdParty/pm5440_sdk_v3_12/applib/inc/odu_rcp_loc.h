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
#ifndef _ODU_RCP_LOC_H
#define _ODU_RCP_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "odu_rcp.h"
#include "odu_rcp_map.h"
#include "di_rcp_io.h"
#include "ri_rcp_io.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: odu_rcp_di_pdu_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying ODU_RCP DI PDU types.
*
* ELEMENTS:
*   DIGI_ODU_RCP_DI_PDU_LINE_SIDE_OTU_HO_ODUK_CHNL    - Line Side OTU + Channel based HO ODUk
*   DIGI_ODU_RCP_DI_PDU_SYS_SIDE_OTU_HO_ODUK_CHNL     - System Side OTU + Channel based HO ODUk
*   IGI_ODU_RCP_DI_PDU_STG_1_FRM_HO_ODUK_CHNL         - Channel based HO ODUk Rx Framer Stage 1 (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_N_FRM_HO_ODUK_CHNL        - Channel based HO ODUk Rx Framer Stage N (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_1_FRM_HO_ODUK_POOL        - Pool based HO ODUk Rx Framer Stage 1
*   DIGI_ODU_RCP_DI_PDU_STG_N_FRM_HO_ODUK_POOL        - Pool based HO ODUk Rx Framer Stage N
*   DIGI_ODU_RCP_DI_PDU_STG_1_FRM_HO_ODUK_EBC_POOL    - Pool based HO ODUk Rx Framer Stage 1 (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_N_FRM_HO_ODUK_EBC_POOL    - Pool based HO ODUk Rx Framer Stage N (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_CHNL           - Channel based ODUk Rx Framer Stage 2
*   DIGI_ODU_RCP_DI_PDU_STG_3A_FRM_ODUK_CHNL          - Channel based ODUk Rx Framer Stage 3a
*   DIGI_ODU_RCP_DI_PDU_STG_3B_FRM_ODUK_CHNL          - Channel based ODUk Rx Framer Stage 3b
*   DIGI_ODU_RCP_DI_PDU_STG_4_FRM_ODUK_CHNL           - Channel based ODUk Rx Framer Stage 4
*   DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_EBC_CHNL       - Channel based ODUk Rx Framer Stage 2 (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_3A_FRM_ODUK_EBC_CHNL      - Channel based ODUk Rx Framer Stage 3a (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_3B_FRM_ODUK_EBC_CHNL      - Channel based ODUk Rx Framer Stage 3b (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_4_FRM_ODUK_EBC_CHNL       - Channel based ODUk Rx Framer Stage 4 (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_POOL           - Pool based ODUk Rx Framer Stage 2
*   DIGI_ODU_RCP_DI_PDU_STG_3A_FRM_ODUK_POOL          - Pool based ODUk Rx Framer Stage 3a
*   DIGI_ODU_RCP_DI_PDU_STG_3B_FRM_ODUK_POOL          - Pool based ODUk Rx Framer Stage 3b
*   DIGI_ODU_RCP_DI_PDU_STG_4_FRM_ODUK_POOL           - Pool based ODUk Rx Framer Stage 4
*   DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_EBC_POOL       - Pool based ODUk Rx Framer Stage 2 (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_3A_FRM_ODUK_EBC_POOL      - Pool based ODUk Rx Framer Stage 3a (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_3B_FRM_ODUK_EBC_POOL      - Pool based ODUk Rx Framer Stage 3b (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_STG_4_FRM_ODUK_EBC_POOL       - Pool based ODUk Rx Framer Stage 4 (NEBC & FEBC)
*   DIGI_ODU_RCP_DI_PDU_GFPF                          - GFP-F
*   DIGI_ODU_RCP_DI_PDU_CLIENTS                       - Clients (ENET, SDH, FC1200,FC800, PRBS)
*   DIGI_ODU_RCP_DI_PDU_SDH                           - SDH B1 & B2 & M0/M1
*   DIGI_ODU_RCP_DI_PDU_PRBS                          - PRBS nTSE
*
*******************************************************************************/
typedef enum {
        DIGI_ODU_RCP_DI_PDU_LINE_SIDE_OTU_HO_ODUK_CHNL = 0,
        DIGI_ODU_RCP_DI_PDU_SYS_SIDE_OTU_HO_ODUK_CHNL = 1,
        DIGI_ODU_RCP_DI_PDU_STG_1_FRM_HO_ODUK_CHNL = 2,
        DIGI_ODU_RCP_DI_PDU_STG_N_FRM_HO_ODUK_CHNL = 3,
        DIGI_ODU_RCP_DI_PDU_STG_1_FRM_HO_ODUK_POOL = 4,
        DIGI_ODU_RCP_DI_PDU_STG_N_FRM_HO_ODUK_POOL = 5,
        DIGI_ODU_RCP_DI_PDU_STG_1_FRM_HO_ODUK_EBC_POOL = 6,
        DIGI_ODU_RCP_DI_PDU_STG_N_FRM_HO_ODUK_EBC_POOL = 7,
        DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_CHNL = 8,
        DIGI_ODU_RCP_DI_PDU_STG_3A_FRM_ODUK_CHNL = 9,
        DIGI_ODU_RCP_DI_PDU_STG_3B_FRM_ODUK_CHNL = 10,
        DIGI_ODU_RCP_DI_PDU_STG_4_FRM_ODUK_CHNL = 11,
        DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_EBC_CHNL = 12,
        DIGI_ODU_RCP_DI_PDU_STG_3A_FRM_ODUK_EBC_CHNL = 13,
        DIGI_ODU_RCP_DI_PDU_STG_3B_FRM_ODUK_EBC_CHNL = 14,
        DIGI_ODU_RCP_DI_PDU_STG_4_FRM_ODUK_EBC_CHNL = 15,
        DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_POOL = 16,
        DIGI_ODU_RCP_DI_PDU_STG_3A_FRM_ODUK_POOL = 17,
        DIGI_ODU_RCP_DI_PDU_STG_3B_FRM_ODUK_POOL = 18,
        DIGI_ODU_RCP_DI_PDU_STG_4_FRM_ODUK_POOL = 19,
        DIGI_ODU_RCP_DI_PDU_STG_2_FRM_ODUK_EBC_POOL = 20,
        DIGI_ODU_RCP_DI_PDU_STG_3A_FRM_ODUK_EBC_POOL = 21,
        DIGI_ODU_RCP_DI_PDU_STG_3B_FRM_ODUK_EBC_POOL = 22,
        DIGI_ODU_RCP_DI_PDU_STG_4_FRM_ODUK_EBC_POOL = 23,
        DIGI_ODU_RCP_DI_PDU_GFPF = 24,
        DIGI_ODU_RCP_DI_PDU_CLIENTS = 25,
        DIGI_ODU_RCP_DI_PDU_SDH = 26,
        DIGI_ODU_RCP_DI_PDU_PRBS = 27,
        LAST_ODU_RCP_DI_PDU
} digi_odu_rcp_di_pdu_type_t;

/*
** struct Types
*/


/*
** Constants
*/
#define ODU_RCP_STG_1_N_DRCOHM_IDX 0
#define ODU_RCP_STG_2_DRCOHM_IDX 1
#define ODU_RI_RCP_CFC_PAGE_SIZE 2
#define ODU_RI_RCP_STG1N_NUM_CHNL  12
#define ODU_RI_RCP_STG234_NUM_CHNL 96
#define ODU_RI_RCP_NULL_CHNL 0x7F




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

#endif /* _ODUJAT_LOC_H */

/* 
** end of file 
*/

