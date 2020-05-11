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
*     The head file describes the G.HAO Run-Time functionality.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_GHAO_H
#define _DIGI_ADPT_GHAO_H
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
PUBLIC PMC_ERROR adpt_ghao_en_set(UINT32 dev_id, 
                                UINT32 chnl_id,
                                UINT32 new_rx_ts_mask[3],
                                UINT32 new_tx_ts_mask[3],
                                BOOL enable);
PUBLIC PMC_ERROR adpt_ghao_step_lcr_size(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_ghao_dir_t ghao_dir,
                                     digi_ghao_step_t ghao_step);
PUBLIC PMC_ERROR adpt_ghao_step_lcr_knife_edge(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           digi_ghao_dir_t ghao_dir,
                                           digi_ghao_step_t ghao_step);
PUBLIC PMC_ERROR adpt_ghao_step_bwr_setup(UINT32 dev_id, 
                                       UINT32 chnl_id,
                                       digi_ghao_dir_t ghao_dir,
                                       digi_ghao_step_t ghao_step);
PUBLIC PMC_ERROR adpt_ghao_step_bwr_ramp_start(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           digi_ghao_dir_t ghao_dir);
PUBLIC PMC_ERROR adpt_ghao_step_bwr_ramp_end(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           digi_ghao_dir_t ghao_dir,
                                           digi_ghao_step_t ghao_step);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_GHAO_H */

/* 
** end of file 
*/

