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

#ifndef _ODU_STRUCT_H
#define _ODU_STRUCT_H 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/*
** Include Files
*/
#include <stdlib.h>
#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"

#include "util_global.h"
#include "odu_struct_api.h"



/*
** Enumerated Types
*/

/*
** Constants
*/


/*
** Macro Definitions
*/


/*
** Structures and Unions
*/



/* 
** Function Prototypes
*/
    

PUBLIC PMC_ERROR odu_struct_odu_di_rcp_cfg(coreotn_handle_t           *coreotn_handle,  
                                           UINT32                      di_rcp_pdu,
                                           BOOL8                       enable);  

PUBLIC void odu_struct_odu_rcp_di_pdu_msk_get(coreotn_handle_t           *coreotn_handle,
                                              UINT32                     *pdu_msk_ptr,
                                              UINT16                     *sdh_cnt_sel_ptr);
PUBLIC PMC_ERROR odu_struct_ri_rcp_resize(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          UINT32 *ch_entries);
PUBLIC void odu_struct_odu_rcp_di_sdh_cnt_sel_set(coreotn_handle_t           *coreotn_handle,
                                                  UINT16                     sdh_cnt_sel);

PUBLIC odu_struct_frm_id_t odu_struct_frm_id_get(coreotn_handle_t *coreotn_handle,
                                                 odu_struct_t *odu_struct);

PUBLIC UINT32 odu_struct_ch_id_get(odu_struct_t *odu_struct);
 
PUBLIC PMC_ERROR odu_struct_prbs_inst_get_helper(coreotn_handle_t       *coreotn_handle,
                                                 odu_struct_odu_level_t  odu_level,
                                                 coreotn_prbs_inst_t    *prbs_inst_ptr);


PUBLIC PMC_ERROR odu_struct_rcp_en_get(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       UINT32 *rcp_en);
/* 
** end of file 
*/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODU_STRUCT_H */

