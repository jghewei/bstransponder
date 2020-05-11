/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*   DESCRIPTION:
*     The file describes the interlaken Run-Time functionality of SIFD subsystem.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_INTERLAKEN_H
#define _DIGI_ADPT_INTERLAKEN_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Enumerated Types
*/

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_ilkn_info_set(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_oduk_t oduk_type, 
                                DOUBLE oduflex_rate);
PUBLIC PMC_ERROR adpt_ilkn_cfg_rx_reset(UINT32 dev_id, 
                                   ilkn_sifd_type_t sifd_id, 
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_ilkn_fifo_thresh_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT32 xon_thresh,
                                     UINT32 xoff_thresh);
PUBLIC PMC_ERROR adpt_ilkn_fifo_thresh_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT32 *xon_thresh,
                                     UINT32 *xoff_thresh);
PUBLIC PMC_ERROR adpt_ilkn_fifo_size_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    UINT32 *num_fifo_blk,
                                    UINT32 *num_fifo_incr);
PUBLIC PMC_ERROR adpt_ilkn_prbs_set(UINT32 dev_id,
                                 ilkn_sifd_type_t sifd_id,
                                 digi_serdes_dir_t dir,
                                 digi_serdes_prbs_t prbs_mode,
                                 BOOL inv_cfg,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_ilkn_rx_state_get(UINT32 dev_id,
                                   ilkn_sifd_type_t sifd_id,
                                   UINT32 *status);
PUBLIC PMC_ERROR adpt_ilkn_2b_header_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_serdes_dir_t dir,
                                     UINT16 header);
PUBLIC PMC_ERROR adpt_ilkn_2b_header_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_serdes_dir_t dir,
                                     UINT16 *header);
PUBLIC PMC_ERROR adpt_ilkn_copi_lowpwr_cfg(UINT32 dev_id,
                                           ilkn_sifd_type_t sifd_id,
                                           BOOL lowpwr_flag);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_INTERLAKEN_H */

/* 
** end of file 
*/

