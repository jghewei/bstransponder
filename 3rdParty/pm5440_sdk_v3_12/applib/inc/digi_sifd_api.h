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
*   DESCRIPTION : This file contains definitions for C functions for
*   configuring SIFD features.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_SIFD_API_H
#define _DIGI_SIFD_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "sifd_api.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
/**/
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: digi_sifd_pmon_link_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Link table for SIFD PMON.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct digi_sifd_pmon_link_t
{
    digi_pmon_sifd_t                pmon;

    digi_enet_pmon_mode_t           enet_mode[DIGI_ENET_CHNL_MAX];
    digi_ilkn_chnl_t               *ilkn_handle[DIGI_ENET_CHNL_MAX];
    digi_enet_pmon_chnl_t           enet[DIGI_ENET_CHNL_MAX];
} digi_sifd_pmon_link_t;

/*******************************************************************************
* STRUCTURE: digi_sifd_pmon_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for SIFD PMON.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct digi_sifd_pmon_t
{
    digi_ilkn_link_t        *handle[DIGI_SIFD_ILKN_NUM];
    digi_sifd_pmon_link_t    sifd_link[DIGI_SIFD_ILKN_NUM];
} digi_sifd_pmon_t;


/*******************************************************************************
* STRUCTURE: digi_sifd_enet_pmon_int_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for SIFD ENET PMON channel interrupts.
*
* ELEMENTS:
*   chnl_handle     - pointer to SIFD ILKN1 or ILKN2 channel associated ENET PMON.
*   enet_pmon       - ENET PMON interrupt table associated to ILN channel. 
*******************************************************************************/
typedef struct digi_sifd_enet_pmon_int_chnl_t
{
    digi_ilkn_chnl_t  *chnl_handle;
    enet_int_chnl_t    enet_pmon;
}digi_sifd_enet_pmon_int_chnl_t; 

/*******************************************************************************
* STRUCTURE: digi_sifd_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for SIFD interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct digi_sifd_int_t
{
    digi_ilkn_link_t              *handle[DIGI_SIFD_ILKN_NUM];
    sifd_int_link_t                sifd_link[DIGI_SIFD_ILKN_NUM];
    digi_sifd_enet_pmon_int_chnl_t enet_pmon_int_chnl[DIGI_ENET_CHNL_MAX];
} digi_sifd_int_t; 


/*******************************************************************************
* ENUM: digi_sifd_int_summary_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to retrieve in digi_sifd_int_summary_t.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_SIFD_INT_SUMMARY_MASK_ACTIVE_LINK_NUM    = 0x01,
    DIGI_SIFD_INT_SUMMARY_MASK_ACTIVE_LINK        = 0x02,
    DIGI_SIFD_ENET_PMON_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM    = 0x04,
    DIGI_SIFD_ENET_PMON_INT_SUMMARY_MASK_ACTIVE_CHNL        = 0x08,
} digi_sifd_int_summary_mask_t;


/*******************************************************************************
* STRUCTURE: digi_sifd_int_summary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Interrupt summary information for sifd channels
*
* ELEMENTS:
*   mask                       - digi_sifd_int_summary_mask_t bitmask to
*                                indicate which fields have been returned
*
*   active_link_num            - Number of links in sifd that have an active
*                                enabled interrupt
*
*   active_link                - Array of sifd link handles that have an
*                                have an active enabled interrupt. This allows the user
*                                to iteratively call digi_sifd_int_link_filt_retrieve()
*                                with the provided channel handles.
*
*   active_enet_pmon_chnl_num  - Number of ILKN channels in sifd that have an ENET PMON
*                                active enabled interrupt
*
*   active_enet_pmon_chnl       - Array of sifd ilkn channel handles that have an
*                                 have an ENENT PMON  active enabled interrupt. 
*                                 This allows the user to iteratively call 
*                                 digi_sifd_enet_pmon_int_chnl_filt_retrieve()
*                                 with the provided channel handles.
*******************************************************************************/
typedef struct digi_sifd_int_summary_t
{
    UINT32               mask;

    UINT32               active_link_num;
    sifd_int_link_t     *active_link[DIGI_SIFD_ILKN_NUM];

    UINT32               active_enet_pmon_chnl_num;
    digi_ilkn_chnl_t    *active_enet_pmon_chnl[DIGI_ENET_CHNL_MAX];
} digi_sifd_int_summary_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/

/*
** SIFD realted functions 
*/
PUBLIC PMC_ERROR digi_sifd_ilkn_core_diag_lpbk(digi_handle_t *digi_handle,
                                               digi_ilkn_link_t *ilkn_inst_data_ptr,
                                               BOOL enable);

PUBLIC PMC_ERROR digi_sifd_ilkn_fifo_size_get(digi_handle_t       *digi_handle,
                                              digi_ilkn_chnl_t    *chnl_ctxt_ptr,
                                              UINT32              *num_fifo_block_ptr,
                                              UINT32              *num_fifo_48_byte_incr_ptr);
    
PUBLIC PMC_ERROR digi_sifd_ilkn_fifo_thresh_get(digi_handle_t       *digi_handle,
                                                digi_ilkn_chnl_t    *chnl_ctxt_ptr,
                                                UINT32              *xon_thresh_ptr,
                                                UINT32              *xoff_thresh_ptr);
    
PUBLIC PMC_ERROR digi_sifd_ilkn_fifo_thresh_set(digi_handle_t       *digi_handle,
                                                digi_ilkn_chnl_t    *chnl_ctxt_ptr,
                                                UINT32               xon_thresh,
                                                UINT32               xoff_thresh);

PUBLIC PMC_ERROR digi_sifd_ilkn_mon_prog_test_patt_cfg(digi_handle_t      *digi_handle,
                                                       digi_ilkn_link_t    *ilkn_inst_ptr,
                                                       BOOL8               enable,
                                                       UINT32              pattern_a,
                                                       UINT32              pattern_b,
                                                       UINT8               rep_a,
                                                       UINT8               rep_b);

PUBLIC PMC_ERROR digi_sifd_ilkn_gen_prog_test_patt_cfg(digi_handle_t      *digi_handle,
                                                       digi_ilkn_link_t    *ilkn_inst_ptr,
                                                       BOOL8               enable,
                                                       UINT32              pattern_a,
                                                       UINT32              pattern_b,
                                                       UINT8               rep_a,
                                                       UINT8               rep_b);


PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_mon_cfg(digi_handle_t           *digi_handle,
                                             digi_ilkn_link_t        *ilkn_ins_ptr,
                                             util_serdes_patt_cfg_t  *cfg_ptr);
PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_mon_cfg_get(digi_handle_t          *digi_handle,
                                                 digi_ilkn_link_t       *ilkn_inst_ptr,
                                                 util_serdes_patt_cfg_t *cfg_ptr);

PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_gen_cfg(digi_handle_t             *digi_handle,
                                             digi_ilkn_link_t          *ilkn_inst_ptr,
                                             util_serdes_patt_cfg_t    *cfg_ptr);
PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_gen_cfg_get(digi_handle_t          *digi_handle,
                                                 digi_ilkn_link_t       *ilkn_inst_ptr,
                                                 util_serdes_patt_cfg_t *cfg_ptr);

PUBLIC PMC_ERROR digi_sifd_ilkn_prbs_status_get(digi_handle_t      *digi_handle,
                                                digi_ilkn_link_t   *ilkn_inst_ptr,
                                                UINT32             *status);
PUBLIC PMC_ERROR digi_dcpb_pmon_cfg(digi_handle_t *digi_handle,
                                    cpb_dcpb_dpi_port_type_t port,
                                    UINT32 counter_num,
                                    cpb_pmon_mode pmon_mode,
                                    BOOL count_drop_packets,
                                    BOOL count_err_packets,
                                    BOOL count_oversize_packets,
                                    BOOL count_good_packets );
PUBLIC PMC_ERROR digi_ocpb_pmon_cfg(digi_handle_t *digi_handle,
                                    cpb_ocpb_dpi_port_type_t port,
                                    UINT32 counter_num,
                                    cpb_pmon_mode pmon_mode,
                                    BOOL count_drop_packets,
                                    BOOL count_err_packets,
                                    BOOL count_oversize_packets,
                                    BOOL count_good_packets );
PUBLIC PMC_ERROR digi_cpb_pmon_chnl_add_drop_cfg(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *switch_data,
                                                 UINT32 counter_num,
                                                 BOOL is_dcpb,
                                                 BOOL add_channel);
PUBLIC PMC_ERROR digi_dcpb_pmon_latch_trig_set(digi_handle_t *digi_handle, 
                                               cpb_dcpb_dpi_port_type_t port,
                                               UINT32 counter_mask);
PUBLIC PMC_ERROR digi_ocpb_pmon_latch_trig_set(digi_handle_t *digi_handle, 
                                               cpb_ocpb_dpi_port_type_t port,
                                               UINT32 counter_mask);
PUBLIC PMC_ERROR digi_dcpb_pmon_counter_get(digi_handle_t *digi_handle, 
                                            cpb_dcpb_dpi_port_type_t port, 
                                            UINT32 counter_mask,
                                            UINT32 * count0,
                                            UINT32 * count1,
                                            UINT32 * count2,
                                            UINT32 * count3);
PUBLIC PMC_ERROR digi_ocpb_pmon_counter_get(digi_handle_t *digi_handle, 
                                            cpb_ocpb_dpi_port_type_t port, 
                                            UINT32 counter_mask,
                                            UINT32 * count0,
                                            UINT32 * count1,
                                            UINT32 * count2,
                                            UINT32 * count3);

PUBLIC PMC_ERROR digi_sifd_ilkn_chnl_cnt_cfg(digi_handle_t       *digi_handle,
                                             digi_ilkn_link_t *ilkn_ctxt_ptr,
                                             digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                             sifd_ilkn_dir_t dir);
PUBLIC PMC_ERROR digi_sifd_ilkn_get_chnl_counts(digi_handle_t *digi_handle,
                                                digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                sifd_ilkn_dir_t dir,
                                                UINT64 *p_byte_count, 
                                                UINT64 *p_pkt_count, 
                                                UINT64 *p_err_count, 
                                                UINT64 *p_b_max_count, 
                                                UINT64 *p_b_min_count, 
                                                UINT32 *p_byte_status, 
                                                UINT32 *p_pkt_status, 
                                                UINT32 *p_err_status, 
                                                UINT32 *p_b_max_status, 
                                                UINT32 *p_b_min_status);
PUBLIC PMC_ERROR digi_sifd_ilkn_get_rx_intf_counts(digi_handle_t *digi_handle,
                                                   digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                   UINT64 *p_ib_fc_err_cnt, 
                                                   UINT64 *p_crc24_cnt, 
                                                   UINT32 *p_ib_fc_stat, 
                                                   UINT32 *p_crc24_stat);
PUBLIC PMC_ERROR digi_sifd_ilkn_get_serdes_lane_counts(digi_handle_t *digi_handle,
                                             digi_ilkn_link_t *ilkn_ctxt_ptr,
                                             UINT32 lane, 
                                             UINT32 counter, 
                                             UINT64 *p_cnt_val, 
                                             UINT32 *p_cnt_stat);
PUBLIC PMC_ERROR digi_sifd_ilkn_copi_lowpwr_cfg(digi_handle_t *digi_handle,
                                                digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                BOOL8 lowpwr_flag);
PUBLIC PMC_ERROR digi_sifd_pmon_retrieve(digi_handle_t       *digi_handle,
                                         digi_sifd_pmon_t    *pmon_table_ptr);
PUBLIC PMC_ERROR digi_sifd_pmon_link_get(digi_handle_t                *digi_handle,
                                         digi_ilkn_link_t             *inst_handle,
                                         digi_sifd_pmon_t             *pmon_table_ptr,
                                         digi_sifd_pmon_link_t       **pmon_link_table_pptr);
PUBLIC PMC_ERROR digi_sifd_pmon_enet_chnl_get(digi_handle_t                *digi_handle,
                                              digi_ilkn_chnl_t             *chnl_handle,
                                              digi_sifd_pmon_link_t        *pmon_table_ptr,
                                              digi_enet_pmon_mode_t        *enet_mode_ptr,
                                              digi_enet_pmon_chnl_t       **enet_pmon_pptr);
PUBLIC PMC_ERROR digi_sifd_int_register_cb(digi_handle_t      *digi_handle,
                                             void               *token,
                                             UINT32              token_size,
                                             digi_int_cb_fcn     user_cb);
PUBLIC PMC_ERROR digi_sifd_int_link_register_cb(digi_handle_t      *digi_handle,
                                                void               *token,
                                                UINT32              token_size,
                                                digi_int_cb_fcn     user_cb);
PUBLIC PMC_ERROR digi_sifd_int_link_enable(digi_handle_t           *digi_handle,
                                           digi_ilkn_link_t        *inst_handle,   
                                           sifd_int_link_t         *int_table_ptr);
PUBLIC PMC_ERROR digi_sifd_int_link_disable(digi_handle_t             *digi_handle,
                                              digi_ilkn_link_t        *inst_handle,   
                                              sifd_int_link_t         *int_table_ptr);
PUBLIC PMC_ERROR digi_sifd_int_clear(digi_handle_t             *digi_handle,
                                       digi_sifd_int_t         *int_table_ptr);
PUBLIC PMC_ERROR digi_sifd_int_link_clear(digi_handle_t           *digi_handle,
                                          digi_ilkn_link_t        *inst_handle,
                                          sifd_int_link_t         *int_table_ptr);
PUBLIC PMC_ERROR digi_sifd_int_retrieve(digi_handle_t       *digi_handle,
                                        digi_sifd_int_t   *int_table_ptr);
PUBLIC PMC_ERROR digi_sifd_int_filt_retrieve(digi_handle_t           *digi_handle,
                                             sifd_int_link_t         *filt_table_ptr,   
                                             digi_sifd_int_t         *int_table_ptr);
PUBLIC PMC_ERROR digi_sifd_int_link_filt_retrieve(digi_handle_t         *digi_handle,
                                                  digi_ilkn_link_t      *chnl_handle,
                                                  sifd_int_link_t       *filt_table_ptr,
                                                  sifd_int_link_t       *int_table_ptr);
PUBLIC PMC_ERROR digi_sifd_int_summary_get(digi_handle_t            *digi_handle,
                                           UINT32                    summary_mask,
                                           digi_sifd_int_summary_t  *int_summary_ptr);
PUBLIC PMC_ERROR digi_sifd_int_link_get(digi_handle_t              *digi_handle,
                                        digi_ilkn_link_t           *inst_handle,
                                        digi_sifd_int_t            *int_table_ptr,
                                        sifd_int_link_t           **int_link_table_pptr);
PUBLIC PMC_ERROR digi_sifd_int_enabled_get(digi_handle_t       *digi_handle,
                                             digi_sifd_int_t   *int_table_ptr);
PUBLIC PMC_ERROR digi_sifd_int_enabled_check(digi_handle_t      *digi_handle);


PUBLIC PMC_ERROR digi_sifd_int_enet_pmon_chnl_register_cb(digi_handle_t      *digi_handle,
                                                          void               *token,
                                                          UINT32              token_size,
                                                          digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_enable(digi_handle_t             *digi_handle,
                                                     digi_ilkn_chnl_t          *chnl_handle,
                                                     enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_disable(digi_handle_t             *digi_handle,
                                                     digi_ilkn_chnl_t          *chnl_handle,
                                                     enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_clear(digi_handle_t           *digi_handle,
                                                    digi_ilkn_chnl_t        *chnl_handle,
                                                    enet_int_chnl_t         *int_table_ptr);

PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_filt_retrieve(digi_handle_t     *digi_handle,
                                                            digi_ilkn_chnl_t  *chnl_handle,
                                                            enet_int_chnl_t   *filt_table_ptr,
                                                            enet_int_chnl_t   *int_table_ptr);

PUBLIC PMC_ERROR digi_sifd_enet_pmon_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                            digi_sifd_int_summary_t  *int_summary_ptr);

PUBLIC PMC_ERROR digi_sifd_int_link_enabled_check(digi_handle_t            *digi_handle,
                                                  digi_sifd_int_summary_t  *int_summary_ptr);

PUBLIC PMC_ERROR digi_ilkn_oduk_rx_2b_header_get(digi_handle_t *digi_handle,
                                                 digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                 UINT16 *rx_2b_header);
PUBLIC PMC_ERROR digi_ilkn_oduk_rx_2b_header_set(digi_handle_t *digi_handle,
                                                 digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                 UINT16 rx_2b_header);
PUBLIC PMC_ERROR digi_ilkn_oduk_tx_2b_header_get(digi_handle_t *digi_handle,
                                                 digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                 UINT16 *tx_2b_header);
PUBLIC PMC_ERROR digi_ilkn_oduk_tx_2b_header_set(digi_handle_t *digi_handle,
                                                 digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                                 UINT16 tx_2b_header);




#ifdef __cplusplus
}
#endif

#endif /* _DIGI_SIFD_API_H */

/*
** end of file
*/

