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

#ifndef _OTN_PRBS_H
#define _OTN_PRBS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_log.h"
#include "util_global.h"
#include "otn_prbs_api.h"


/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: otn_prbs_pattern_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PRBS pattern types.
*
* ELEMENTS:
*
*
*******************************************************************************/
typedef enum
{
    OTN_INVERTED_PRBS_11    = 0,
    OTN_INVERTED_PRBS_31    = 1,
    OTN_PRBS_11             = 2,
    OTN_PRBS_31             = 3,
} otn_prbs_pattern_t;
          

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* ENUM: otn_prbs_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OTN PRBS channel state
*
* ELEMENTS:
*   See Typedef.
*
*******************************************************************************/
typedef enum 
{
    OTN_PRBS_CHNL_START        = 0,
    OTN_PRBS_CHNL_EQUIPPED     = 1,
    OTN_PRBS_CHNL_OPERATIONAL  = 2,
} otn_prbs_chnl_state_t;

/*******************************************************************************
* STRUCTURE: otn_prbs_ch_ctxt_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OTN PRBS parameters specific to each channel 
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct
{
    otn_prbs_chnl_state_t           chnl_state;
} otn_prbs_ch_ctxt_struct_t;


/*******************************************************************************
* STRUCTURE: otn_prbs_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
    Static OTN_PRBS config context 
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct
{
    
    UINT32 max_chnl;
} otn_prbs_cfg_t;


/*******************************************************************************
* STRUCTURE: otn_prbs_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
    Dynamic ODUKSC config context 
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct
{
    otn_prbs_ch_ctxt_struct_t   *ch_ctxt_ptr;
      
} otn_prbs_var_t;


/*******************************************************************************
* STRUCTURE: otn_prbs_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
    OTN PRBS handle
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct otn_prbs_handle_struct_t
{
    pmc_handle_t base;
    otn_prbs_cfg_t cfg;
    otn_prbs_var_t var;
} otn_prbs_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC otn_prbs_handle_t *otn_prbs_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void otn_prbs_ctxt_destroy(otn_prbs_handle_t *otn_prbs_handle);

PUBLIC void otn_prbs_handle_init(otn_prbs_handle_t *otn_prbs_handle);
PUBLIC PMC_ERROR otn_prbs_handle_restart_init(otn_prbs_handle_t *otn_prbs_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                              pmc_energy_state_t energy_state);
                               
PUBLIC UINT32 otn_prbs_base_address_get(otn_prbs_handle_t *otn_prbs_handle);

PUBLIC PMC_ERROR otn_prbs_chnl_init(otn_prbs_handle_t *otn_prbs_handle, 
                                    UINT32 chnl, UINT32 prbs_operation,
                                    UINT32 mode);

PUBLIC PMC_ERROR otn_prbs_chnl_prov(otn_prbs_handle_t *otn_prbs_handle,
                                    UINT32 chnl, UINT32 dci_chnl, 
                                    otn_prbs_pattern_t prbs_pattern);

PUBLIC PMC_ERROR otn_prbs_ber_thres_cfg(otn_prbs_handle_t *otn_prbs_handle,
                                        UINT32 prbs_ber_lo_thres, 
                                        UINT32 prbs_ber_hi_thres);

PUBLIC PMC_ERROR otn_prbs_chnl_bit_err_inject(otn_prbs_handle_t *otn_prbs_handle,
                                              UINT32 chnl, UINT32 insert_err);                                                                            
                                                                                          
PUBLIC PMC_ERROR otn_prbs_chnl_defect_cnt_get(otn_prbs_handle_t *otn_prbs_handle,
                                              UINT32 chnl, UINT32 *defect_cnt);           

#ifdef __cplusplus
}
#endif

#endif /* _OTN_PRBS_H */

/*
** end of file
*/
