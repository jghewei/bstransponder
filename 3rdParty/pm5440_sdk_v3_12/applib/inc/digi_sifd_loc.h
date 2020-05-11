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

#ifndef _DIGI_SIFD_LOC_H
#define _DIGI_SIFD_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_enet_loc.h" /* Required for digi_enet_client_chnl_def_t */

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
/*******************************************************************************
* ENUM: digi_sifd_ilkn_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This enumeration is used to identify an interlaken instance within SIFD.
*
* ELEMENTS:
*   DIGI_SIFD_ILKN1 - Interlaken 1 Instance
*   DIGI_SIFD_ILKN2 - Interlaken 2 Instance
*   LAST_DIGI_SIFD_ILKN - Element used to identify end of enum list
*
*******************************************************************************/
typedef enum {
    DIGI_SIFD_ILKN1     = 0,
    DIGI_SIFD_ILKN2     = 1,

    LAST_DIGI_SIFD_ILKN
} digi_sifd_ilkn_inst_t;
/*******************************************************************************
* ENUM: digi_sifd_path_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This enumeration is used to identify the major Interlaken path.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {

    DIGI_SIFD_PATH_NOT_IN_USE = 0,
    DIGI_SIFD_PATH_ILKN_1,
    DIGI_SIFD_PATH_ILKN_2,

    LAST_DIGI_SIFD_PATH
} digi_sifd_path_t;


/*******************************************************************************
* STRUCTURE: digi_ilkn_chnl_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common Interlaken Channel properties for rx/tx
*
* ELEMENTS:
* 
*   ilkn_bcw_chnl       - Defines the burst control word channel that will be
*                         associated with the outbound channel.
*   ilkn_2b_header      - Defines the 2B header for source CPB/ODUKSW channels.
*   ilkn_4b_header      - Defines the 4B header for source CPB/ODUKSW channels.
*
*******************************************************************************/
typedef struct digi_ilkn_chnl_info_t
{
    UINT32 ilkn_bcw_chnl;
    UINT16 ilkn_2b_header;
    UINT32 ilkn_4b_header;
} digi_ilkn_chnl_info_t;

/*******************************************************************************
* STRUCTURE: digi_ilkn_chnl_prov_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining provisioning state of ILKN channel in RX and TX direction.
*
* ELEMENTS:
*   rx_prov     - TRUE when ILKN channel is provisioned in RX direction
*   tx_prov     - FALSE when ILKN channel is provisioned in RX direction
*
*******************************************************************************/
typedef struct digi_ilkn_chnl_prov_state_t
{
    BOOL8 rx_prov;
    BOOL8 tx_prov;
} digi_ilkn_chnl_prov_state_t;

/*******************************************************************************
* STRUCTURE: digi_ilkn_chnl_def_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common Interlaken Channel properties
*
* ELEMENTS:
* 
*       switch_data     - a util_global_switch_data_def_t structure.
*       sifd_path       - Identifies the major Interlaken path.
*       enet_pmon       - 10GE/40GE/100GE passive monitoring of CBR Ethernet
*                         signals
*       target          - Subsystem to source/sink data for the channel\n
*                         See digi_ilkn_chnl_target_t for more information.
*   tx_type_header      - Defines if the device appends the four byte TYPE
*                            field for data transfers across the ILKN interface.
*                            Valid values are: \n
*                             1 -- ILKN data tranfers have TYPE header \n
*                             0 -- ILKN data transfer have no TYPE header 
*   rx_type_header       - Specifies the presence of TYPE header on data sent
*                            from backplane to ILKN interface \n
*                            Valid values are: \n
*                             1 -- ILKN data tranfers have TYPE header \n
*                             0 -- ILKN data transfer have no TYPE header 
*   rate                 - channel rate.  specified as an integer fraction of
*                          96. Used to allocate scheduling for signal
*                          transferred towards backplane.  The following must
*                          be true\n
*                          rate/96 >= (signal_rate Gpbs)/(120 Gbps
*   rx_fc_reach_extend  -  Configure data source (MAPOTN/ENET) to directly
*                          use ILKN XOFF indication for channel to backpressure
*                          FE transmitter.  Used in cases where large 
*                          downstream channel buffering is present as required 
*                          for reach extension applications. \n
*                          Valid values are: \n
*                            0 --  Reach Extension Flow Control disabled \n
*                            1 --  Reach Extension Flow Control enabled
*       tx_ilkn_info    - An internal structure capturing the tx_ilkn_info.
*                         See digi_ilkn_chnl_info_t for more information.
*       rx_ilkn_info    - An internal structure capturing the rx_ilkn_info.
*                         See digi_ilkn_chnl_info_t for more information.
*       enet_pmon_data  - a digi_enet_client_chnl_def_t structure pointing 
*                         to the enet_line/enet_sys structure if not NULL 
*                         so it can be demapped when it is no longer required
*
*******************************************************************************/
typedef struct digi_ilkn_chnl_def_t
{
    util_global_switch_data_def_t switch_data;
    digi_sifd_path_t sifd_path;
    digi_enet_pmon_type_t enet_pmon;
    digi_ilkn_chnl_target_t target;
    BOOL tx_type_header;
    BOOL rx_type_header;
    UINT8 rate;
    BOOL rx_fc_reach_extend;
    digi_ilkn_chnl_info_t tx_ilkn_info;
    digi_ilkn_chnl_info_t rx_ilkn_info;
    digi_enet_client_chnl_def_t* enet_pmon_data;
} digi_ilkn_chnl_def_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
#ifndef DOXYGEN_PUBLIC_ONLY

PUBLIC BOOL digi_sifd_db_test_update(digi_handle_t *digi_handle,
                                      digi_sifd_ilkn_inst_t sifd_ilkn_inst,
                                      UINT32 schd_addr,
                                      UINT32 chnl);
PUBLIC PMC_ERROR digi_sifd_db_prov_cfg(digi_handle_t *digi_handle,
                                        digi_sifd_ilkn_inst_t sifd_ilkn_inst,
                                        UINT32 chnl,
                                        UINT8 num_cal_entry_req);
PUBLIC PMC_ERROR digi_sifd_db_clear(digi_handle_t *digi_handle,
                                     digi_sifd_ilkn_inst_t sifd_ilkn_inst,
                                     UINT32 chnl);
PUBLIC PMC_ERROR digi_ilkn_unused_chnl_get(digi_handle_t* digi_handle, 
                                            digi_ilkn_chnl_target_t target,
                                            digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst,
                                            UINT32* chnl_index);
PUBLIC digi_ilkn_chnl_def_t* digi_ilkn_cpb_chnl_handle_get(digi_handle_t *digi_handle, 
                                                            digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst, 
                                                            UINT32 channel);

PUBLIC digi_ilkn_chnl_def_t* digi_ilkn_oduksw_chnl_handle_get(digi_handle_t *digi_handle, 
                                                               digi_sifd_ilkn_inst_t digi_sifd_ilkn_inst, 
                                                               UINT32 channel);

PUBLIC digi_ilkn_chnl_def_t* digi_ilkn_1_chnl_get(digi_handle_t *digi_handle, 
                                                   UINT32 channel);

PUBLIC digi_ilkn_chnl_def_t* digi_ilkn_2_chnl_get(digi_handle_t *digi_handle, 
                                                   UINT32 channel);

PUBLIC void digi_sifd_flow_control_calendar_2B_header_get(digi_handle_t*                digi_handle,
                                                          sifd_ilkn_inst_t              ilkn_inst,
                                                          BOOL8                         is_tx_data_dir,
                                                          digi_sifd_ilkn_calendar32_t*  ilkn_fc_ptr);
PUBLIC PMC_ERROR digi_ilkn_2B_header_to_dpi_map(digi_handle_t *digi_handle, 
                                                sifd_ilkn_inst_t ilkn_inst,
                                                BOOL8 is_tx_dir,
                                                digi_sifd_ilkn_calendar32_t *fc_from_top, 
                                                sifd_ilkn_calendar_t *fc_for_context);
PUBLIC PMC_ERROR digi_ilkn_bcw_to_dpi_map(digi_handle_t *digi_handle,
                                          UINT32 fc_cal_length,                                          
                                          UINT8 *bcw_to_dpi_map_ptr, 
                                          digi_sifd_ilkn_calendar32_t *ilkn_fc_ptr, 
                                          sifd_ilkn_calendar_t *fc_ptr);
PUBLIC PMC_ERROR digi_sifd_fc_calendar_save(digi_handle_t *digi_handle, 
                                                   sifd_ilkn_inst_t sifd_path,
                                                   digi_sifd_ilkn_calendar32_t *ilkn_tx_fc_ptr, 
                                                   digi_sifd_ilkn_calendar32_t *ilkn_rx_fc_ptr);
PUBLIC PMC_ERROR digi_sifd_fc_calendar_update(digi_handle_t *digi_handle, 
                                                  sifd_ilkn_inst_t ilkn_inst,
                                                  UINT32 cpb_egress_port,
                                                  BOOL8 is_tx_data_dir,
                                              BOOL8 is_oduksw,
                                              UINT32 deactv_chnl);
PUBLIC void digi_sifd_fc_calendar_bulk_update(digi_handle_t *digi_slice_handle,
                                              util_global_oduk_port_t oduksw_port_num,
                                              util_global_datapath_dir_t port_direction);
PUBLIC PMC_ERROR digi_copy_struct_ilkn_1_cfg(sifd_ilkn_1_cfg_t       *sifd_ilkn_1_cfg,
                                              digi_sifd_ilkn_1_cfg_t  *digi_sifd_ilkn_1_cfg_ptr);
PUBLIC PMC_ERROR digi_copy_struct_ilkn_2_cfg(sifd_ilkn_2_cfg_t       *sifd_ilkn_2_cfg,
                                              digi_sifd_ilkn_2_cfg_t  *digi_sifd_ilkn_2_cfg_ptr);
PUBLIC PMC_ERROR digi_ilkn_1_unused_chnl_get(digi_handle_t* digi_handle, 
                                              digi_ilkn_chnl_target_t target,
                                              UINT32* chnl_index);

PUBLIC PMC_ERROR digi_ilkn_2_unused_chnl_get(digi_handle_t* digi_handle, 
                                              digi_ilkn_chnl_target_t target,
                                              UINT32* chnl_index);

PUBLIC PMC_ERROR digi_ilkn_chnl_cfg_params_validate(digi_handle_t *digi_handle,
                                                     digi_ilkn_link_t *ilkn_ctxt_ptr,
                                                     digi_ilkn_chnl_target_t target,
                                                     UINT32 tx_ilkn_bcw_chnl,
                                                     UINT16 tx_2b_header,
                                                     UINT32 tx_4b_header,
                                                     UINT32 rx_ilkn_bcw_chnl,
                                                     UINT16 rx_2b_header,
                                                     UINT32 rx_4b_header,
                                                     digi_ilkn_chnl_t **chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_sifd_chnl_index_get(digi_handle_t       *digi_handle,
                                          digi_ilkn_chnl_t    *chnl_handle,
                                          UINT32              *index_ptr);

PUBLIC PMC_ERROR digi_ilkn_inst_get(digi_ilkn_chnl_def_t   *chnl_ctxt_ptr,
                                    digi_sifd_ilkn_inst_t  *sifd_ilkn_inst_ptr);


PUBLIC util_global_chnl_provisioned_state_t digi_ilkn_chnl_prov_state_new_val_get(util_global_chnl_provisioned_state_t  cur_state,
                                                                                  BOOL8                                 use_rx_chnl_state,
                                                                                  BOOL8                                 prov_rx_chnl,
                                                                                  BOOL8                                 use_tx_chnl_state,
                                                                                  BOOL8                                 prov_tx_chnl);


PUBLIC PMC_ERROR digi_sifd_fc_calendar_chnl_entries_remove(digi_handle_t         *digi_handle,
                                                           digi_ilkn_chnl_def_t  *chnl_data_ptr);

PUBLIC PMC_ERROR digi_sifd_enet_pmon_handle_get(digi_handle_t          *digi_handle,
                                                 digi_ilkn_chnl_t     *chnl_handle,
                                                 enet_handle_t          **enet_ptr,
                                                 UINT32                  *link);

#endif

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_SIFD_LOC_H */

/* 
** end of file 
*/

