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
*     This head file includes card specific modification information.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_CUSTOMER_H
#define _DIGI_ADPT_CUSTOMER_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_adpt_com.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: digi_trigger_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type defining PMON trigger mode.
*
* ELEMENTS:
*   DIGI_TRIGGER_MANUAL      - manual trigger PMON.
*   DIGI_TRIGGER_AUTOMATIC   - automatic trigger PMON.
*
*******************************************************************************/
typedef enum
{
    DIGI_TRIGGER_MANUAL = 0,
    DIGI_TRIGGER_AUTOMATIC,

    LAST_DIGI_TRIGGER,      /* this is for out of range values error handling */
} digi_trigger_mode_t;

/*******************************************************************************
* ENUM: digi_warm_restart_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type defining warm restart type.
*
* ELEMENTS:
*   DIGI_WARM_CTXT_IN_PLACE  - context in-place for warm restart. Non-volatile 
*                              memory is required for data recovery in the
*                              system. The virtual/physical memory should be 
*                              always kept the same after warm restart.
*   DIGI_WARM_CTXT_COPY      - context copy for warm restart. Storage, such as 
*                              flash, is required for data recovery in the
*                              system. 
*
*******************************************************************************/
typedef enum
{
    DIGI_WARM_CTXT_IN_PLACE = 0,
    DIGI_WARM_CTXT_COPY,

    LAST_DIGI_WARM_CTXT,      /* this is for out of range values error handling */
} digi_warm_restart_type_t;

/*******************************************************************************
* ENUM: digi_cpb_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for CPB switch source. The
*   element terms have a syntax:
*      DIGI_CPB_SRC_<application>
*
* ELEMENTS:
*   DIGI_CPB_SRC_CBR          - CPB switch from CBR
*   DIGI_CPB_SRC_ENET         - CPB switch from ENET
*   DIGI_CPB_SRC_MAP          - CPB switch from MAPOTN
*   DIGI_CPB_SRC_ILKN         - CPB switch from ILKN
*
*******************************************************************************/
typedef enum
{
    DIGI_CPB_SRC_CBR   = 0x0,
    DIGI_CPB_SRC_ENET,
    DIGI_CPB_SRC_MAP,
    DIGI_CPB_SRC_ILKN,
    
    LAST_DIGI_CPB_SRC, /* this is for out of range values error handling */
} digi_cpb_src_t;

/*
** Macro Definitions
*/
#define ADPT_FILE_NAME_LEN   32
#define ADPT_DEV_NUMBER_MAX  100
#define ADPT_DIGI_DEV_MAX    8
#define ADPT_INVALID_DEV_ID  0xFFFFFFFF

#define HW_CARD_6x10G_TRANSPONDER_SYS_CLIENT    1002 /* for DIGI60 */
#define HW_CARD_6x10G_TRANSPONDER               1003 /* for DIGI60 */
#define HW_CARD_1x100GE_OTN_TRIBUTARY           1004
#define HW_CARD_2x40G_OTN_TRIBUTARY             1005
#define HW_CARD_10x10G_ANY_OTN_TRIBUTARY        1006
#define HW_CARD_1x100G_TRANSPONDER              1007 /* with CXP */
#define HW_CARD_10x10G_ANY_MUXPONDER_TO_OTU4    1008 /* OTU4: SYSOTN */
#define HW_CARD_10x10G_CLIENT_MUXPONDER_TO_OTU4 1009 /* OTU4: LINEOTN */
#define HW_CARD_10x10G_TRANSPONDER              1010
#define HW_CARD_3x40G_CFP                       1011 /* 100G <> 3x40G CFP */
#define HW_CARD_100G_TRANSPONDER_TO_OTU4        1012 /* OTU4: SYSOTN */
#define HW_CARD_2x40G_TO_OTU4                   1013 /* OTU4: SYSOTN */
#define HW_CARD_SFI_2x40G_TO_OTU4               1014 /* 40G: SFI-51/SFI-S, OTU4: SYSOTN(SFI-S) */
#define HW_CARD_12x10G_ANY_OTN_TRIBUTARY        1015
#define HW_CARD_3x40G_OTN_TRIBUTARY             1016

/*
** Structures and Unions
*/



/*******************************************************************************
* STRUCTURE: digi_dev_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the device necessary information.
*
* ELEMENTS:
*   pmon_work_order    - bitmask of hardware block to enable PMON for. A '1' in
*                        a bit location indicates that the PMON should be enabled
*                        for that block. The bit field definition is as follows.
*                        Bit0  -- LINEOTN
*                        Bit1  -- SYSOTN
*                        Bit2  -- CoreOTN Errored Block Counts
*                        Bit8  -- CBRC
*                        Bit9  -- MGMT Port
*                        Bit10 -- ENET_LINE
*                        Bit11 -- ENET_SYS
*                        Bit12 -- MAPOTN
*                        Bit13 -- MCPB
*                        Bit14 -- SIFD
*
*   manual_trig_msk    - Manual Trigger Mask. Bitmask of PMON to trigger.
*                        A '1' in a bit position will trigger an update 
*                        of the respective PMON.
*                        Bit0 -- OTN PMON. Latches the pN_EBC and pF_EBC 
*                                counts in LINEOTN, SYSOTN and COREOTN,
*                                and the Ethernet counts in COREOTN OHP 
*                                and RCP_RI and RCP_DI. Also latches the 
*                                pN_TSE counts in COREOTN if RCP is not 
*                                enabled. This trigger can only be triggered 
*                                if Enhanced or Bursty dDEG is not enabled.
*                        Bit1 -- COREOTN Defects. This trigger can only be 
*                                triggered if RCP is not enabled.
*                        Bit2 -- Non-OTN PMON. Latches all other non-OTN
*                                PMON counts. This trigger can only be used
*                                if pmon_trig_mode = 0 (manual trigger) in
*                                function digi_pmon_collection_mode_cfg().
*
*   int_type           -  INTERRUPT_MODE        Description
*                         --------------        -----------
*                         CPUIF_INT_DISABLE     Polling mode
*                         CPUIF_INT_MSI_X       PCIe MSI-X interrupts
*                         CPUIF_INT_MSI         PCIe MSI interrupts
*                         CPUIF_INT_LEGACY      PCIe Legacy interrupts
*                         CPUIF_INT_DEVICE_PIN  Device pin interrupt
*
*   sifd_1_cfg.ilkn.tx_flow_control_calendar.entry:
*   DIGI_SIFD_ILKN_NULL_CAL_ENTRY      - calendar entry is unused
*   DIGI_SIFD_ILKN_LINK_FLOW_CONTROL   - calendar entry is passing flow control
*                                        for the link
*   channel number                     - CPB or ODUKSW DPI channel ID that relates 
*                                        to this flow control calendar entry.
*                                        0 - 149 is the CPB DPI channel ID
*                                        150 - 253 is the ODUKSW DPI channel ID 
*                                        offset by 150 (ODUKSW DPI channel ID = 
*                                        BCW channel - 150)
*
*******************************************************************************/
typedef struct
{
    /* card identity */
    UINT32                     card_id;
    digi_prod_app_t            card_type;
    /* file name for data storage */
    char                       file_name[ADPT_FILE_NAME_LEN];
    /* warm restart type */
    digi_warm_restart_type_t   warm_type;
    /* OPSA parameter */
    /* packet size: 128, 256, 512 */
    UINT32                     pkt_size;
    /* header size: 0 ~ 16 */
    UINT32                     header_size;
    /* maximum multiplex stages */
    digi_otn_mux_stages_t      mux_stage;

    /* PMON trigger mode */
    digi_trigger_mode_t        pmon_trig_mode;
    /* manual trigger mask */
    UINT32                     manual_trig_msk;
    /* trigger period: 250, 500 or 1000(ms) */
    UINT32                     trig_period;
    /* PMON work order */
    UINT32                     pmon_work_order;
    /* Ethernet PMON enable/disable in Tx */
    BOOL                       tx_enet_pmon;
    /* interrupt process type */
    cpuif_int_t                int_type;
    /* static SerDes adaptation */
    BOOL                       static_serdes_adpt;
    /* FW path and file name */
    char*                      fw_filename;

    /* line serdes port information */
    UINT32                     num_line_serdes_port;
    digi_serdes_intf_t         serdes_type[DIGI_SERDES_XFI_LANES_MAX];
    UINT8                      line_cage_id[DIGI_SERDES_XFI_LANES_MAX];
    UINT32                     line_port_lane_tx[DIGI_SERDES_XFI_LANES_MAX][DIGI_SERDES_XFI_LANES_MAX];
    UINT32                     line_port_lane_rx[DIGI_SERDES_XFI_LANES_MAX][DIGI_SERDES_XFI_LANES_MAX];
    UINT8                      line_port_los_pin[DIGI_SERDES_XFI_LANES_MAX];
    BOOL                       line_port_los_inv;

    /* systm serdes port information */
    UINT32                     num_sys_serdes_port;
    UINT8                      sys_cage_id[DIGI_SERDES_SYS_LANES_MAX];
    UINT32                     sys_port_lane_tx[DIGI_SERDES_SYS_LANES_MAX][DIGI_SERDES_SYS_LANES_MAX];
    UINT32                     sys_port_lane_rx[DIGI_SERDES_SYS_LANES_MAX][DIGI_SERDES_SYS_LANES_MAX];
    UINT8                      sys_port_los_pin[DIGI_SERDES_SYS_LANES_MAX];
    BOOL                       sys_port_los_inv;



    /* interlaken information */
    ilkn_sifd_type_t            sifd_type;
    digi_sifd_ilkn_1_cfg_t      sifd_1_cfg;
    digi_sifd_ilkn_calendar32_t sifd_1_tx_fc;
    digi_sifd_ilkn_calendar32_t sifd_1_rx_fc;
    digi_sifd_ilkn_2_cfg_t      sifd_2_cfg;
    digi_sifd_ilkn_calendar32_t sifd_2_tx_fc;
    digi_sifd_ilkn_calendar32_t sifd_2_rx_fc;
} digi_dev_info_t;

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_dev_info_get(UINT32 dev_id, digi_dev_info_t **card_info_ptr);
PUBLIC PMC_ERROR adpt_handle_allocate(UINT32 dev_id, UINT32 data_size, void **data_ptr);
PUBLIC PMC_ERROR adpt_handle_free(UINT32 dev_id, void **data_ptr);
PUBLIC PMC_ERROR adpt_database_save(char *file_name, UINT32 data_size, void *data_ptr);
PUBLIC PMC_ERROR adpt_database_restore(char *file_name, UINT32 data_size, void *data_ptr);
PUBLIC PMC_ERROR adpt_opsa_pkt_size_get(UINT32 dev_id,
                                     digi_oduksw_stream_t oduk_stream,
                                     UINT32 *pkt_size,
                                     UINT32 *header_size);
PUBLIC PMC_ERROR adpt_opsa_oduksw_fifo_get(UINT32 dev_id,
                                        UINT32 src_chnl_id,
                                        digi_oduksw_stream_t oduk_stream,
                                        UINT32 ts_number,
                                        UINT32 *halt_buffer,
                                        UINT32 *latency_s);
PUBLIC PMC_ERROR adpt_cpb_fifo_get(digi_cpb_stream_t cpb_stream,
                                digi_cpb_src_t source,
                                UINT32 ts_number,
                                UINT32 *halt_buffer_ptr);
PUBLIC const char* adpt_serdes_csv_name_get(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_serdes_port_get(UINT32 dev_id, UINT32 rx_serdes_port, UINT32 *tx_serdes_port);
PUBLIC PMC_ERROR adpt_defect_opt_en_get(UINT32 dev_id, BOOL *opt_en);
PUBLIC PMC_ERROR adpt_fw_filename_set(UINT32 dev_id, char const *file_name);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_CUSTOMER_H */

/* 
** end of file 
*/

