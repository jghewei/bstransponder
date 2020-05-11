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
*     The file describes the misc Run-Time functionality.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_MISC_H
#define _DIGI_ADPT_MISC_H
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
PUBLIC PMC_ERROR adpt_pgmrclk_div_cfg(UINT32 dev_id,
                                   digi_pgmrclk_div_id_t div_id,
                                   digi_pgmrclk_input_source_t clk_source,
                                   UINT32 clk_source_idx,
                                   DOUBLE input_clk_freq,
                                   DOUBLE output_clk_freq,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_pgmrclk_div_cfg_get(UINT32 dev_id,
                                          digi_pgmrclk_div_id_t div_id,
                                          digi_pgmrclk_input_source_t *clk_source,
                                          UINT32 *clk_source_idx,
                                          DOUBLE *input_clk_freq,
                                          DOUBLE *output_clk_freq,
                                          BOOL *enable);
PUBLIC PMC_ERROR adpt_pgmrclk_output_cfg(UINT32 dev_id,
                                      digi_pgmrclk_div_id_t div_id,
                                      BOOL enable);
PUBLIC PMC_ERROR adpt_m1_rate_monitor_cfg_set(UINT32 dev_id,
                                          digi_m1_pmm_lite_clk_ref_t ref_clk_src,
                                          DOUBLE ref_clk_freq,
                                          BOOL enable);
PUBLIC PMC_ERROR adpt_m1_rate_monitor_cfg_get(UINT32 dev_id,
                                          digi_m1_pmm_lite_clk_ref_t *ref_clk_src,
                                          DOUBLE *ref_clk_freq,
                                          BOOL *enable);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_MISC_H */

/* 
** end of file 
*/

