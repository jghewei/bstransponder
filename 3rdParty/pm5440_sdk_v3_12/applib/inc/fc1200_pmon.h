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

#ifndef _FC1200_PMON_H
#define _FC1200_PMON_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "fc1200_pmon_api.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define FC1200_PMON_MAX_INDEX                  12
#define FC1200_PMON_RESET                      0x1
#define FC1200_PMON_ACTIVE                     0x0
#define FC1200_PMON_ENABLE                     0x1
#define FC1200_PMON_DISABLE                    0x0



/*
** Macro Definitions
*/
#define FC1200_PMON_INT_TABLE_DEFINE_FW() \
    FC1200_PMON_INT(fc1200_pmon_handle, fc1200_pmon, HIGH_BER_INT , high_ber_int_i ); \
    FC1200_PMON_INT(fc1200_pmon_handle, fc1200_pmon, LOSS_SYNC_INT, loss_sync_int_i); \
    FC1200_PMON_INT(fc1200_pmon_handle, fc1200_pmon, LOS_INT      , los_int_i      ); \

#define FC1200_PMON_STATUS_TABLE_DEFINE() \
    FC1200_PMON_STATUS(fc1200_pmon_handle, fc1200_pmon, HIGH_BER_INT_V , high_ber_int_v ); \
    FC1200_PMON_STATUS(fc1200_pmon_handle, fc1200_pmon, LOSS_SYNC_INT_V, loss_sync_int_v); \
    FC1200_PMON_STATUS(fc1200_pmon_handle, fc1200_pmon, LOS_INT_V      , los_int_v      ); \

/*
** Structures and Unions
*/



/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC fc1200_pmon_handle_t *fc1200_pmon_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void fc1200_pmon_ctxt_destroy(fc1200_pmon_handle_t *fc1200_pmon_handle);
PUBLIC void fc1200_pmon_handle_init(fc1200_pmon_handle_t *fc1200_pmon_handle);
PUBLIC BOOL fc1200_pmon_start_state_test(fc1200_pmon_handle_t *fc1200_pmon_handle);
PUBLIC PMC_ERROR fc1200_pmon_init(fc1200_pmon_handle_t    *fc1200_pmon_handle_ptr,
                                  pmc_energy_state_t       energy_state);
PUBLIC PMC_ERROR fc1200_pmon_reset(fc1200_pmon_handle_t    *fc1200_pmon_handle_ptr,
                                   UINT32                   index,
                                   BOOL                     reset);
PUBLIC PMC_ERROR fc1200_pmon_dlolb_cfg(fc1200_pmon_handle_t        *fc1200_handle_ptr,
                                       UINT32                       index,
                                       fc1200_pmon_dlolb_cfg_t     *cfg_ptr);
PUBLIC PMC_ERROR fc1200_pmon_dlolb_cfg_get(fc1200_pmon_handle_t        *fc1200_handle_ptr,
                                           UINT32                       index,
                                           fc1200_pmon_dlolb_cfg_t     *cfg_ptr);
PUBLIC PMC_ERROR fc1200_pmon_dlolb_force(fc1200_pmon_handle_t        *fc1200_handle_ptr,
                                         UINT32                       index,
                                         BOOL                         enable);
PUBLIC PMC_ERROR fc1200_pmon_dlolb_force_get(fc1200_pmon_handle_t        *fc1200_handle_ptr,
                                             UINT32                       index,
                                             BOOL                        *enable_ptr);
PUBLIC PMC_ERROR fc1200_pmon_invalid_block_mode_set(fc1200_pmon_handle_t             *fc1200_handle_ptr,
                                                    UINT32                            index,
                                                    fc1200_pmon_invalid_block_mode_t  mode);
PUBLIC PMC_ERROR fc1200_pmon_invalid_block_mode_get(fc1200_pmon_handle_t             *fc1200_handle_ptr,
                                                    UINT32                            index,
                                                    fc1200_pmon_invalid_block_mode_t *mode_ptr);
PUBLIC PMC_ERROR fc1200_pmon_int_chnl_enable(fc1200_pmon_handle_t    *fc1200_pmon_handle,
                                             UINT32                   index,
                                             fc1200_pmon_int_chnl_t  *int_table_ptr,
                                             fc1200_pmon_int_chnl_t  *int_en_table_ptr,
                                             BOOL                     enable);
PUBLIC PMC_ERROR fc1200_pmon_int_chnl_clear(fc1200_pmon_handle_t    *fc1200_pmon_handle,
                                            UINT32                   index,
                                            fc1200_pmon_int_chnl_t  *int_table_ptr,
                                            fc1200_pmon_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR fc1200_pmon_int_chnl_retrieve(fc1200_pmon_handle_t    *fc1200_pmon_handle,
                                               UINT32                   index,
                                               fc1200_pmon_int_chnl_t  *filt_table_ptr,
                                               fc1200_pmon_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR fc1200_pmon_int_chnl_enabled_check(fc1200_pmon_handle_t     *fc1200_pmon_handle,
                                                    UINT32                    index,
                                                    fc1200_pmon_int_chnl_t   *int_en_table_ptr,
                                                    BOOL                     *int_found_ptr);


#ifdef __cplusplus
}
#endif

#endif /* _FC1200_PMON_H */

/*
** end of file
*/
