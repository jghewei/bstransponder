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
#ifndef _EMAC_LOC_H
#define _EMAC_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "stdlib.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "emac.h"
#include "emac_core_io.h"
#include "emac_io.h"
#include "emac_mac_io.h"
#include "emac_pcs_io.h"
#include "emac_prbs_io.h"
#include "emac_mac_regs.h"

/*
** Enumerated Types
*/

/*
** Constants
*/

/*
** Macro Definitions
*/
/* EMAC_INT(handle, tsb_name, tsb_field, struct_field) */
#define EMAC_INT_TABLE_DEFINE() \
    EMAC_INT_RANGE_LOS(emac_handle, emac, field_range, PHY_LOS      , phy_los_i);           \
    EMAC_INT_RANGE(emac_handle, emac, field_range, RX_LOC_FAULT     , rx_loc_fault_i);      \
    EMAC_INT_RANGE(emac_handle, emac, field_range, RX_REM_FAULT     , rx_rem_fault_i);      \
    EMAC_INT_RANGE(emac_handle, emac, field_range, HI_BER           , hi_ber_i);            \
    EMAC_INT_RANGE(emac_handle, emac, field_range, BLOCK_LOCK       , block_lock_i);        \
    EMAC_INT_RANGE(emac_handle, emac, field_range, EMAC_TX_OVERFLOW , emac_tx_overflow_i);  \
    EMAC_INT_RANGE(emac_handle, emac, field_range, EMAC_TX_UNDERFLOW, emac_tx_underflow_i); \
    EMAC_INT_40G(emac_handle, emac, ALIGN_STATUS , align_status_i);                         \

#define EMAC_STATUS_TABLE_DEFINE() \
    EMAC_STATUS_RANGE_LOS(emac_handle, emac, field_range, PHY_LOS_V , phy_los_v);           \
    EMAC_STATUS_RANGE(emac_handle, emac, field_range, RX_LOC_FAULT_V, rx_loc_fault_v);      \
    EMAC_STATUS_RANGE(emac_handle, emac, field_range, RX_REM_FAULT_V, rx_rem_fault_v);      \
    EMAC_STATUS_RANGE(emac_handle, emac, field_range, HI_BER_V      , hi_ber_v);            \
    EMAC_STATUS_RANGE(emac_handle, emac, field_range, BLOCK_LOCK_V  , block_lock_v);        \
    EMAC_STATUS(emac_handle, emac_pcs, LINK_STATUS,           link_status);                 \
    EMAC_STATUS(emac_handle, emac_pcs, ALL_LANES_LOCK,        all_lanes_lock);              \
    EMAC_STATUS(emac_handle, emac_pcs, LANE_19_12_BLOCK_LOCK, lane_19_12_block_lock);       \
    EMAC_STATUS(emac_handle, emac_pcs, LANE_7_0_BLOCK_LOCK,   lane_7_0_block_lock);         \
    EMAC_STATUS(emac_handle, emac_pcs, LANE_ALIGN_LOCK_19_12, lane_align_lock_19_12);       \
    EMAC_STATUS(emac_handle, emac_pcs, LANE_ALIGN_LOCK_7_0,   lane_align_lock_7_0);         \
    EMAC_STATUS_40G(emac_handle, emac, ALIGN_STATUS_V, align_status_v);                     \




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

#endif /* _EMAC_LOC_H */

/* 
** end of file 
*/

