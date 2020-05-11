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
*     Include file for ODU_OHP block used by upper layer block.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ODU_OHP_H
#define _ODU_OHP_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "odu_ohp_api.h" 
#include "cfc.h"
#include "enet_fege.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: odu_ohp_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum listing OHP States
*
* ELEMENTS:
*   see enum.
*
*******************************************************************************/
typedef enum
{
    ODU_OHP_STATE_START        = 0,
    ODU_OHP_STATE_EQUIPPED     = 1,
    ODU_OHP_STATE_OPERATIONAL  = 2,
    LAST_ODU_OHP_STATE,
} odu_ohp_state_t;


/*
** Constants
*/
/* Number of CFCs per OHP */
#define ODU_OHP_CFC_MAX 4


/* number of OHPs */
#define ODU_OHP_MAX 3

/* number of pors per OHP */
#define ODU_OHP_PORT_MAX 2

/* maximum schedule size */
#define ODU_OHP_SCHED_SIZE_MAX 96

/* Maximum # of channels for each OHP */
#define ODU_OHP_1_CH_MAX 12
#define ODU_OHP_2_CH_MAX 96
#define ODU_OHP_3_CH_MAX 96

/* CFC FIFO end address req polling interval */
#define ODU_OHP_CFC_FIFO_POLLING_INTERVAL 1000
#define ODU_OHP_CFC_FIFO_POLLING_ITERATIONS 600

/*
** Macro Definitions
*/

#define ODU_OHP_INT_TABLE_DEFINE(tail)                                  \
    ODU_OHP_INT(odu_ohp_handle, odu_ohp_core, RXMAC_UID_FAIL, rxmac_uid_fail_i); \
    ODU_OHP_INT(odu_ohp_handle, odu_ohp_core, RXMAC_FRMRID_FAIL, rxmac_frmrid_fail_i);   \
    ODU_OHP_INT(odu_ohp_handle, odu_ohp_core, RXMAC_CHID_FAIL, rxmac_chid_fail_i);     \
    ODU_OHP_INT(odu_ohp_handle, odu_ohp_core, RXMAC_PKTERR, rxmac_pkterr_i);  \
    ODU_OHP_INT(odu_ohp_handle, odu_ohp_core, RXMAC_FIFO_ERR, rxmac_fifo_err_i); \


#define ODU_OHP_CHNL_INT_TABLE_DEFINE(tail)   \
    ODU_OHP_CHNL_INT(odu_ohp_handle, odu_ohp_core, RXFA_CFC_FULL, rxfa_cfc_full_i); \
    ODU_OHP_CHNL_INT(odu_ohp_handle, odu_ohp_core, RXFB_CFC_FULL, rxfb_cfc_full_i); \
    ODU_OHP_CHNL_INT(odu_ohp_handle, odu_ohp_core, RXFA_TXDLY, rxfa_txdly_i); \
    ODU_OHP_CHNL_INT(odu_ohp_handle, odu_ohp_core, RXFB_TXDLY, rxfa_txdly_i); \

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: odu_ohp_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining ODU_OHP parameters specific block state.
*
* ELEMENTS:
*       state         - block state
*
*******************************************************************************/
typedef struct odu_ohp_ctxt_struct_{
    odu_ohp_state_t           state;        
} odu_ohp_ctxt_t;


/*******************************************************************************
* STRUCTURE:  odu_ohp_port_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU_OHP Configuration description.
*
* ELEMENTS:
*    ohp            - OHP instance: ODU_OHP1, ODU_OHP2 or ODU_OHP3
*    port           - ODU_OHP_PORT_A or ODU_OHP_PORT_B
*    port_framer    - Framer - ODU_OHP_PORT_FRAMER_1/2/3A/3B/4/N 
*                     or ODU_OHP_PORT_UNUSED
*    src_user_id    - source user ID 
*    dest_user_id   - destination user ID
*    schedule       - schedule
*    cfc_in         - insertion CFC
*    cfc_ex         - extraction CFC
*
*******************************************************************************/
typedef struct
{  
    odu_ohp_inst_t ohp;
    odu_ohp_port_t port;
    odu_ohp_port_framer_t port_framer;
    UINT32 src_user_id;
    UINT32 dest_user_id;
    UINT8 schedule[ODU_OHP_SCHED_SIZE_MAX];
    cfc_handle_t *cfc_in;
    cfc_handle_t *cfc_ex;
} odu_ohp_port_cfg_t;

/*******************************************************************************
* STRUCTURE:  odu_ohp_blk_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU_OHP Configuration description.
*
* ELEMENTS:
*  odu_ohp_inst_t - OHP instance: ODU_OHP_1, ODU_OHP_2 or ODU_OHP_3
*  num_chnl       - Number of channels: 96 or 12 
*  db_dev_addr    - db device address
*  db_handle      - db handle
*  port_cfg       - Port configuration
*
*******************************************************************************/
typedef struct odu_ohp_blk_cfg_struct_t
{ 
    odu_ohp_inst_t  ohp;
    UINT8     num_chnl;
    UINT32*   db_dev_addr;
    UINT32 rx_db_handle;
    odu_ohp_port_cfg_t  port_cfg[ODU_OHP_PORT_MAX];
} odu_ohp_blk_cfg_t;


/*******************************************************************************
* STRUCTURE:  odu_ohp_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU_OHP variables description.
*
* ELEMENTS:
*  ctxt   - ODU OHP context 
*
*******************************************************************************/
typedef struct
{
    odu_ohp_ctxt_t   ctxt;
} odu_ohp_var_t;


/*******************************************************************************
* STRUCTURE: odu_ohp_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU_OHP handle description.
*
* ELEMENTS:
*  See typedef.
*
*******************************************************************************/
struct odu_ohp_handle_struct_t
{
    pmc_handle_t base;
    
    odu_ohp_blk_cfg_t cfg;
    odu_ohp_var_t var;
    
    cfc_handle_t *cfc_handle[ODU_OHP_CFC_MAX];
    enet_fege_handle_t *enet_fege_handle;
};


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC BOOL odu_ohp_start_state_test(odu_ohp_handle_t *odu_ohp_handle);
PUBLIC odu_ohp_handle_t *odu_ohp_ctxt_create(pmc_handle_t *parent, 
                                             UINT32 base_address, 
                                             pmc_sys_handle_t *sys_handle, 
                                             const char *tsb_name,
                                             odu_ohp_inst_t ohp_inst);

PUBLIC void odu_ohp_ctxt_destroy(odu_ohp_handle_t *odu_ohp_handle);

PUBLIC void odu_ohp_handle_init(odu_ohp_handle_t *odu_ohp_handle, 
                                UINT32 rx_db_handle);

PUBLIC void odu_ohp_alloc_init(odu_ohp_handle_t *odu_ohp_handle);

PUBLIC void odu_ohp_handle_restart_init(odu_ohp_handle_t *odu_ohp_handle, 
                                        UINT32 rx_db_handle,
                                        util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                        pmc_energy_state_t top_energy_state);

                               
PUBLIC UINT32 odu_ohp_base_address_get(odu_ohp_handle_t *odu_ohp_handle);

PUBLIC PMC_ERROR odu_ohp_init(odu_ohp_handle_t *hndl, 
                              odu_ohp_cfg_t *cfg_ptr);

PUBLIC PMC_ERROR odu_ohp_cfg(odu_ohp_handle_t *hndl, 
                             BOOL enable);

PUBLIC PMC_ERROR odu_ohp_uninit(odu_ohp_handle_t *hndl);

PUBLIC PMC_ERROR odu_ohp_init_get(odu_ohp_handle_t *hndl,
                                  odu_ohp_cfg_t *cfg_ptr,
                                  BOOL *enable);
   
PUBLIC PMC_ERROR odu_ohp_ch_cfg(odu_ohp_handle_t *hndl, 
                               odu_ohp_port_t port, 
                               UINT32 ch_id, 
                               UINT16 extract_mask,
                               UINT16 insert_mask,
                               UINT32 num_cal_entries,
                               UINT32 *cal_entries,
                               BOOL enable);

PUBLIC PMC_ERROR odu_ohp_ch_cfg_get(odu_ohp_handle_t *hndl, 
                                odu_ohp_port_t port, 
                                UINT32 ch_id, 
                                UINT16 *extract_mask,
                                UINT16 *insert_mask,
                                BOOL *enable);

PUBLIC PMC_ERROR odu_ohp_ch_insert_cfg(odu_ohp_handle_t *hndl, 
                                       odu_ohp_port_t port, 
                                       UINT32 ch_id, 
                                       UINT16 insert_mask,
                                       UINT32 num_cal_entries,
                                       UINT32 *cal_entries,
                                       BOOL enable);

PUBLIC PMC_ERROR odu_ohp_ch_insert_cfg_get(odu_ohp_handle_t *hndl, 
                                           odu_ohp_port_t port, 
                                           UINT32 ch_id, 
                                           UINT16 *insert_mask,
                                           BOOL *enable);

PUBLIC PMC_ERROR odu_ohp_ch_extract_cfg(odu_ohp_handle_t *hndl, 
                                        odu_ohp_port_t port, 
                                        UINT32 ch_id, 
                                        UINT16 extract_mask,
                                        UINT32 num_cal_entries,
                                        UINT32 *cal_entries,
                                        BOOL enable);

PUBLIC PMC_ERROR odu_ohp_ch_extract_cfg_get(odu_ohp_handle_t *hndl, 
                                            odu_ohp_port_t port, 
                                            UINT32 ch_id, 
                                            UINT16 *extract_mask,
                                            BOOL *enable);

PUBLIC void odu_ohp_chnl_clean(odu_ohp_handle_t   *hndl,
                               odu_ohp_port_t port, 
                               UINT32 *ch_entries,
                               UINT32 *chnl,
                               UINT32 num_chnl);

PUBLIC PMC_ERROR odu_ohp_ch_resize(odu_ohp_handle_t *hndl, 
                                   odu_ohp_port_t port, 
                                   UINT32 ch_id, 
                                   INT32 resize);


PUBLIC PMC_ERROR odu_ohp_int_enable(odu_ohp_handle_t    *odu_ohp_handle,
                                    odu_ohp_int_t       *int_table_ptr,
                                    odu_ohp_int_t       *int_en_table_ptr,
                                    BOOL                 enable);

PUBLIC PMC_ERROR odu_ohp_int_retrieve(odu_ohp_handle_t    *odu_ohp_handle,
                                      odu_ohp_int_t       *filt_table_ptr,
                                      odu_ohp_int_t       *int_table_ptr);

PUBLIC PMC_ERROR odu_ohp_int_clear(odu_ohp_handle_t    *odu_ohp_handle,
                                   odu_ohp_int_t       *int_table_ptr,
                                   odu_ohp_int_t       *int_en_table_ptr);


PUBLIC PMC_ERROR odu_ohp_int_chnl_enable(odu_ohp_handle_t     *odu_ohp_handle,
                                         UINT32                chnl_id,
                                         odu_ohp_int_chnl_t   *int_table_ptr,
                                         odu_ohp_int_chnl_t   *int_en_table_ptr,
                                         BOOL                  enable);

PUBLIC PMC_ERROR odu_ohp_int_chnl_enabled_check(odu_ohp_handle_t    *odu_ohp_handle,
                                                UINT32               chnl_id,
                                                odu_ohp_int_chnl_t  *int_en_table_ptr,
                                                BOOL8               *int_found_ptr);

PUBLIC PMC_ERROR odu_ohp_int_chnl_retrieve(odu_ohp_handle_t    *odu_ohp_handle,
                                           UINT32               chnl_id,
                                           odu_ohp_int_chnl_t  *filt_table_ptr,
                                           odu_ohp_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR odu_ohp_int_chnl_clear(odu_ohp_handle_t    *odu_ohp_handle,
                                        UINT32               chnl_id,
                                        odu_ohp_int_chnl_t  *int_table_ptr);
#ifdef __cplusplus
}
#endif

#endif /* _ODU_OHP_H */

/*
** end of file
*/
