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
*   DESCRIPTION :   This file contains the definition of the enumerations
*                   used in the CPB subsystem API source code. It also exports 
*                   the operations of CPB subsystem for use in datapath APIs or
*                   internal testing.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _CPB_H
#define _CPB_H


#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "cpb_api.h"

#include "util_schd.h"
#include "util_schd_api.h"

#ifndef CPB_UNIT_TEST
#include "hostmsg.h"
#endif

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: cpb_xfer_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying burst transfer size.
*
* ELEMENTS:
*    CPB_SEGMENT               - Word Transfers
*    CPB_PACKET                - Packet Transfers
*    CPB_SEGMENT_BURST_128     - Segment Burst 128 Byte Transfers
*    CPB_SEGMENT_BURST_192     - Segment Burst 192 Byte Transfers
*    CPB_SEGMENT_BURST_256     - Segment Burst 256 Byte Transfers
*    CPB_PACKET_BURST_128      - Packet Burst 128 Byte Transfers
*    CPB_PACKET_BURST_192      - Packet Burst 192 Byte Transfers
*    CPB_PACKET_BURST_256      - Packet Burst 256 Byte Transfers
*
*******************************************************************************/
typedef enum
{
    CPB_SEGMENT = 0,
    CPB_PACKET,
    CPB_SEGMENT_BURST_128,
    CPB_SEGMENT_BURST_192,
    CPB_SEGMENT_BURST_256,
    CPB_PACKET_BURST_128,
    CPB_PACKET_BURST_192,
    CPB_PACKET_BURST_256,

    LAST_CPB_BURST_XFER     /* this is for out of range values error handling */
    
} cpb_xfer_mode_t;

/*******************************************************************************
* ENUM: cpb_xfer_mode_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying interface transfer mode.
*
* ELEMENTS:
*    CPB_XFER_MODE_TYPE_PACKET       - Packet Transfers
*    CPB_XFER_MODE_TYPE_SEGMENT      - Segment Transfers
*
*******************************************************************************/
typedef enum
{
    CPB_XFER_MODE_TYPE_SEGMENT = 0,
    CPB_XFER_MODE_TYPE_PACKET,

    LAST_CPB_XFER_MODE_TYPE
    
}cpb_xfer_mode_type_t;


/*******************************************************************************
* ENUM: cpb_burst_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying burst transfer size.
*
* ELEMENTS:
*    CPB_BURST_WORD          - Word Transfers
*    CPB_BURST_128           - 128 Byte Burst Transfers
*    CPB_BURST_192           - 192 Byte Burst Transfers
*    CPB_BURST_256           - 256 Byte Burst Transfers
*
*******************************************************************************/
typedef enum
{
    CPB_BURST_WORD = 0,
    CPB_BURST_128,
    CPB_BURST_192,
    CPB_BURST_256,

    LAST_CPB_BURST
    
}cpb_burst_mode_t;


/*******************************************************************************
* ENUM: cpb_xoff_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying xoff boundary
*
* ELEMENTS:
*    CPB_XOFF_SEG_BOUNDARY     - Stop on word boundary
*    CPB_XOFF_BRST_BOUNDARY    - Stop on burst boundary
*    CPB_XOFF_PKT_BOUNDARY     - Stop on packet boundary
*
*******************************************************************************/
typedef enum
{
    CPB_XOFF_SEG_BOUNDARY  = 0,
    CPB_XOFF_BRST_BOUNDARY,
    CPB_XOFF_PKT_BOUNDARY,

    LAST_CPB_XOFF_BOUNDARY        /* this is for out of range values error handling */
    
} cpb_xoff_mode_t;

/*******************************************************************************
* ENUM: cpb_dpi_chnl_zone_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying slave DPI port zone reporting
*    behavior.
*
* ELEMENTS:
*     CPB_SLV_CHNL_ZONE_FIFO                - Generate Zone based on
*                                            FIFO depth
*     CPB_SLV_CHNL_ZONE_FIFO_AND_MSTR_XOFF  - Generate ZONE based on
*                                            FIFO depth and downstream
*                                            master DPI port XOFF 
*                                            assertion
*     CPB_SLV_CHNL_ZONE_FORCE               - Force Zone to fixed port
*                                             value
*
*******************************************************************************/
typedef enum
{
    CPB_SLV_CHNL_ZONE_FIFO = 0,
    CPB_SLV_CHNL_ZONE_FIFO_AND_MSTR_XOFF,
    CPB_SLV_CHNL_ZONE_FORCE,

    LAST_CPB_SLV_CHNL_ZONE        /* this is for out of range values error handling */
    
}cpb_dpi_chnl_zone_mode_t;

/*******************************************************************************
* ENUM: cpb_xoff_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying transfer XOFF types.
*
* ELEMENTS:
*    CPB_XOFF_TYPE_LINK    - Link Level Flow Control
*    CPB_XOFF_TYPE_CHANNEL - Channel Level Flow Control
*
*******************************************************************************/
typedef enum
{
    CPB_XOFF_TYPE_LINK = 0,
    CPB_XOFF_TYPE_CHANNEL,

    LAST_CPB_XOFF_TYPE        /* this is for out of range values error handling */
    
} cpb_xoff_type_t;

/*******************************************************************************
* ENUM: cpb_credit_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying DCS credit type.
*
* ELEMENTS:
*    CPB_CREDIT_BURST    - CPB Credit Burst
*    CPB_CREDIT_PACKET   - CPB Credit Packet
*
*******************************************************************************/
typedef enum
{
    CPB_CREDIT_TYPE_BURST = 0,
    CPB_CREDIT_TYPE_PACKET,

    LAST_CPB_CREDIT_TYPE        /* this is for out of range values error handling */
    
} cpb_credit_type_t;


/*******************************************************************************
* ENUM: cpb_dpi_slv_drop_zone_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying drop zone config.
*
* ELEMENTS:
*    CPB_SLV_DPI_DROP_ZONE_0       - Enable drop for fifo at zone=0
*    CPB_SLV_DPI_DROP_ZONE_1       - Enable drop for fifo at zone=1
*    CPB_SLV_DPI_DROP_ZONE_2       - Enable drop for fifo at zone=2
*    CPB_SLV_DPI_DROP_ZONE_3       - Enable drop for fifo at zone=3
*    CPB_SLV_DPI_DROP_ZONE_DISABLE - Disable drop based on fifo level
*******************************************************************************/
typedef enum
{
    CPB_SLV_DPI_DROP_ZONE_0 = 0,
    CPB_SLV_DPI_DROP_ZONE_1,
    CPB_SLV_DPI_DROP_ZONE_2,
    CPB_SLV_DPI_DROP_ZONE_3,
    CPB_SLV_DPI_DROP_ZONE_DISABLE,

    LAST_CPB_SLV_DPI_DROP_ZONE        /* this is for out of range values error handling */
    
}cpb_dpi_slv_drop_zone_t;

/*******************************************************************************
* ENUM: cpb_instance_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the CPB instance.
*
* ELEMENTS:
*    CPB_INSTANCE_DCPB - Datapath CPB
*    CPB_INSTANCE_OCPB - ODUk CPB
*    CPB_INSTANCE_MCPB - Management CPB
*
*******************************************************************************/
typedef enum
{
    CPB_INSTANCE_DCPB      = 0,
    CPB_INSTANCE_OCPB,
    CPB_INSTANCE_MCPB,

    LAST_CPB_INSTANCE
} cpb_instance_type_t;

/*******************************************************************************
* ENUM: cpb_schd_populate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the CPB DCS instance scheduler 
*    content source generation mode.
*
* ELEMENTS:
*    CPB_SCHD_POPULATE_LOCAL  - Use local scheduler content generation
*                               method
*    CPB_SCHD_POPULATE_DB     - Use database contents to generate
*                               scheduler contents
*
*******************************************************************************/
typedef enum
{
    CPB_SCHD_POPULATE_LOCAL      = 0,
    CPB_SCHD_POPULATE_DB,

    LAST_CPB_SCHD_POPULATE
} cpb_schd_populate_t;

/*******************************************************************************
* ENUM: cpb_mcpb_ingr_input_port_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the MCPB slave dpi port type.
*
* ELEMENTS:
*    CPB_MCPB_ING_LINE_IM_EXT - line side ingress mux extract
*    CPB_MCPB_ING_SYS_IM_EXT  - system side ingress mux extract
*    CPB_MCPB_ING_LINE_EM_EXT - line side egress mux extract
*    CPB_MCPB_ING_SYS_EM_EXT  - system side egress mux extract
*    CPB_MCPB_ING_CPU_INS     - CPU insert
*    CPB_MCPB_ING_FEGE_INS    - FEGE insert
*    CPB_MCPB_ING_LINE_REF    - line side reflect
*    CPB_MCPB_ING_SYS_REF     - system side reflect
*
*******************************************************************************/
typedef enum
{
    /* Enum values map to physical device resources */
    CPB_MCPB_ING_LINE_IM_EXT        = 0,
    CPB_MCPB_ING_SYS_IM_EXT         = 1,
    CPB_MCPB_ING_LINE_EM_EXT        = 2, 
    CPB_MCPB_ING_SYS_EM_EXT         = 3,
    CPB_MCPB_ING_CPU_INS            = 4,
    CPB_MCPB_ING_FEGE_INS           = 5,
    CPB_MCPB_ING_LINE_REF           = 6,
    CPB_MCPB_ING_SYS_REF            = 7,
    
    LAST_CPB_MCPB_INGRESS_PORT
}cpb_mcpb_ingr_input_port_type_t;

/*******************************************************************************
* ENUM: cpb_mcpb_egr_output_port_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the MCPB master dpi port type.
*
* ELEMENTS:
*    CPB_MCPB_EGR_SYS_ING  - system side ingress mux
*    CPB_MCPB_EGR_LINE_ING - line side ingress mux
*    CPB_MCPB_EGR_SYS_EG   - system side egress mux extract
*    CPB_MCPB_EGR_LINE_EG  - line side egress mux
*    CPB_MCPB_EGR_CPU      - CPU
*    CPB_MCPB_EGR_FEGE     - FEGE
*
*******************************************************************************/
typedef enum
{
    /* Enum values map to physical device resources */
    CPB_MCPB_EGR_SYS_ING        = 1,
    CPB_MCPB_EGR_LINE_ING       = 0,
    CPB_MCPB_EGR_SYS_EG         = 3, 
    CPB_MCPB_EGR_LINE_EG        = 2,
    CPB_MCPB_EGR_CPU            = 4,
    CPB_MCPB_EGR_FEGE           = 5,
    
    LAST_CPB_MCPB_EGRESS_PORT
}cpb_mcpb_egr_output_port_type_t;

/*******************************************************************************
* ENUM: cpb_fifo_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the status of a FIFO in SCPB 
*    block.
*
* ELEMENTS:
*    CPB_FIFO_NOT_USED   - FIFO is not used
*    CPB_FIFO_BEING_USED - FIFO is being used
*
*******************************************************************************/
typedef enum
{
    CPB_FIFO_NOT_USED      = 0,
    CPB_FIFO_BEING_USED
}cpb_fifo_status_t;

/*******************************************************************************
* ENUM: cpb_target_ram_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the target RAM type.
*
* ELEMENTS:
*    CPB_RAM_RPL     - RPL ram
*    CPB_RAM_STANDBY -standby RAM
*    CPB_RAM_ACTIVE  - active RAM
*    CPB_RAM_ALL     - All RAM
*
*******************************************************************************/
typedef enum
{
    CPB_RAM_RPL       = 0,
    CPB_RAM_STANDBY   = 1,
    CPB_RAM_ACTIVE    = 2,
    CPB_RAM_ALL       = 3
}cpb_target_ram_t;

/*******************************************************************************
* ENUM: cpb_ram_power_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the CPB RAM power mode.
*
* ELEMENTS:
*    CPB_RAM_POWER_NORMAL      - RAM power normal mode
*    CPB_RAM_POWER_LIGHT_SLEEP - RAM power light sleep mode
*    CPB_RAM_POWER_DEEP_SLEEP  - RAM power deep sleep mode
*    CPB_RAM_POWER_SHUTDOWN    - RAM power shutdown mode
*
*******************************************************************************/
typedef enum
{
    CPB_RAM_POWER_NORMAL         = 0,
    CPB_RAM_POWER_LIGHT_SLEEP    = 1,
    CPB_RAM_POWER_DEEP_SLEEP     = 2,
    CPB_RAM_POWER_SHUTDOWN       = 3,
    
    
    LAST_CPB_RAM_POWER
}cpb_ram_power_mode_t;

/*******************************************************************************
* ENUM: cpb_dcpb_rd_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the channel type for each read 
*    queue id.
*
* ELEMENTS:
*   CPB_FIFO_INVALID_CH     - The fifo is not used
*   CPB_FIFO_SECONDARY_CH   - This is a secondary reader
*   CPB_SLAVE_ZONE_PRIMARY  - Defines if the egress mux output port channel is the 
*                             DPI slave port channel ZONE source for all channels 
*                             associated with the DPI slave port channel. A slave 
*                             zone primary channel is also a reader primary channel
*   CPB_READER_PRIMARY      - Defines if the egress mux output port channel is the 
*                             primary reader for the queue system it is configured in
*   LAST_CPB_RD_CHNL_TYPE   - Used for error checking
*
*******************************************************************************/
typedef enum
{
    CPB_FIFO_INVALID_CH       = 0, 
    CPB_FIFO_SECONDARY_CH     = 1, 
    CPB_SLAVE_ZONE_PRIMARY    = 2, 
    CPB_READER_PRIMARY        = 3, 
    
    LAST_CPB_RD_CHNL_TYPE
}cpb_dcpb_rd_chnl_t;

/*******************************************************************************
* ENUM: cpb_dcs_instance_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the mode of a DCS instance
*
* ELEMENTS:
*    CPB_DCS_DISABLED  - Disabled
*    CPB_DCS_EN_N_SIFD - Enabled (MAPOTN, CBRC, ENET_LINE, ENET_SYS mode)
*    CPB_DCS_EN_SIFD   - Enabled (SIFD mode)
*
*******************************************************************************/
typedef enum
{
    CPB_DCS_DISABLED       = 0,
    CPB_DCS_EN_N_SIFD      = 1,
    CPB_DCS_EN_SIFD        = 2,
    LAST_CPB_DCS_MODE
}cpb_dcs_instance_mode_t;


/*******************************************************************************
* ENUM: cpb_port_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the port type.
*
* ELEMENTS:
*    CPB_PORT_TYPE_DPI_SLAVE  - DPI Slave Port
*    CPB_PORT_TYPE_DPI_MASTER - DPI Master Port
*    CPB_PORT_TYPE_DCS        - DCS Port
*
*******************************************************************************/
typedef enum
{
    CPB_PORT_TYPE_DPI_SLAVE     = 0, 
    CPB_PORT_TYPE_DPI_MASTER    = 1, 
    CPB_PORT_TYPE_DCS           = 2,
    LAST_CPB_PORT_TYPE
}cpb_port_type_t;

/*******************************************************************************
* ENUM: cpb_dcpb_igr_sifd_sel
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the DCPB push port slave DPI
*    data source.  Applicable to push port 0 and 1.
*
* ELEMENTS:
*    CPB_DCPB_IGR_SIFD_SEL_CBRC_ENET  - CBRC_ENET
*    CPB_DCPB_IGR_SIFD_SEL_SIFD       - SIFD
*
*******************************************************************************/
typedef enum
{
    CPB_DCPB_IGR_SIFD_SEL_CBRC_ENET      = 0,
    CPB_DCPB_IGR_SIFD_SEL_SIFD           = 1,    
    
    LAST_CPB_DCPB_IGR_SIFD_SEL
}cpb_dcpb_igr_sifd_sel_t;


/*******************************************************************************
* ENUM: cpb_dcs_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the DCPB dpi port type.
*
* ELEMENTS:
*    CPB_DCS_INST_0 - DCS Instance 0
*    CPB_DCS_INST_1 - DCS Instance 1
*    CPB_DCS_INST_2 - DCS Instance 2
*    CPB_DCS_INST_3 - DCS Instance 3
*
*******************************************************************************/
typedef enum {
    CPB_DCS_INST_0 = 0,
    CPB_DCS_INST_1 = 1,
    CPB_DCS_INST_2 = 2,
    CPB_DCS_INST_3 = 3,
    LAST_CPB_DCS_INST
} cpb_dcs_inst_t;

/*******************************************************************************
* ENUM: cpb_mcpb_mux_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*     Defines the muxing mode for the management CPB. Ports can be directed to
*     ENET system or ENET line.
*
* ELEMENTS:
*    
*     CPB_MCPB_MUX_MODE_DISABLED  - Disabled
*     CPB_MCPB_MUX_MODE_ENET_SYS  - Enabled (ENET_SYS sources if MCPB instantiation)
*     CPB_MCPB_MUX_MODE_ENET_LINE - Enabled (ENET_LINE sources if MCPB instantiation)
*    
*******************************************************************************/
typedef enum {
    CPB_MCPB_MUX_MODE_DISABLED  = 0,
    CPB_MCPB_MUX_MODE_ENET_SYS  = 1,
    CPB_MCPB_MUX_MODE_ENET_LINE = 2,
    LAST_CPB_MCPB_MUX_MODE
} cpb_mcpb_mux_mode_t;

#ifndef DOXYGEN_PUBLIC_ONLY
/*
** Constants
*/
#define CPBO_NUM_WORDS_PER_BLOCK    16
#define CPB_INDEX_LAST              0xFF

/*
** Macro Definitions
*/
/* CPB_INT(handle, tsb_name, tsb_field, struct_field) */
#define CPB_INT_TABLE_DEFINE()                                                 \
    CPB_INT_RANGE(cpb_handle, slave_dpi, field, CH_DROP_OVRSIZE,   ch_drop_ovrsize_i); \
    CPB_INT_RANGE(cpb_handle, slave_dpi, field, CH_DROP_CLIENT,    ch_drop_client_i ); \
    CPB_INT_RANGE(cpb_handle, slave_dpi, field, CH_DROP_FULL,      ch_drop_full_i   ); \

/*
** Structures and Unions
*/

/*
** Global variables
*/

/*
** Function Prototypes
*/

/*
 * Exported Interface 
 */

PUBLIC cpb_handle_t *cpb_ctxt_create(pmc_handle_t *parent, UINT32 base_address, 
                                     pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void cpb_ctxt_destroy(cpb_handle_t *cpb_handle);
PUBLIC void cpb_handle_init( cpb_handle_t *cpb_handle,
                             cpb_instance_type_t cpb_type,
                             cpb_schd_populate_t schd_populate_mode[4],
                             pmc_block_energy_state_enum parent_energy_state,
                             void *p_var_t_ram_data,
                             UINT32 var_t_ram_data_size );

PUBLIC void cpb_init(cpb_handle_t *h, pmc_energy_state_t energy_state);
PUBLIC void cpb_alloc_init(cpb_handle_t *cpb_handle);
PUBLIC PMC_ERROR cpb_handle_restart_init(cpb_handle_t *cpb_handle,
                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                         pmc_energy_state_t energy_state);
PUBLIC BOOL8 cpb_start_state_test(cpb_handle_t *h);
PUBLIC PMC_ERROR cpb_dcs_db_entry_set( cpb_handle_t *cpb_handle,
                                       UINT32 dcs_instance,
                                       UINT32 schd_addr,
                                       UINT32 egress_mux_output_port,
                                       UINT32 egress_mux_output_chnl );
PUBLIC PMC_ERROR cpb_dcs_db_entry_set_all( cpb_handle_t *cpb_handle,
                                           UINT32 dcs_instance,
                                           UINT32 egress_mux_output_port,
                                           UINT32 egress_mux_output_chnl,
                                           UINT32 num_cal_entries );
PUBLIC PMC_ERROR cpb_dcs_db_entry_clear( cpb_handle_t *cpb_handle,
                                         UINT32 dcs_instance,
                                         UINT32 egress_mux_output_port,
                                         UINT32 egress_mux_output_chnl );
PUBLIC PMC_ERROR cpb_dcs_db_calendar_entry_clear( cpb_handle_t *cpb_handle,
                                                  UINT32 dcs_instance,
                                                  UINT32 egress_mux_output_port,
                                                  UINT32 egress_mux_output_chnl,
                                                  UINT32 calendar_entry);
PUBLIC PMC_ERROR cpb_dcs_db_entry_get( cpb_handle_t *cpb_handle,
                                       UINT32 dcs_instance,
                                       UINT32 schd_addr,
                                       UINT32 *egress_mux_output_port_ptr,
                                       UINT32 *egress_mux_output_chnl_ptr,
                                       UINT32 *rd_channel_id_ptr );
PUBLIC PMC_ERROR cpb_dcs_db_entry_all_get( cpb_handle_t *cpb_handle,
                                       UINT32 dcs_instance,
                                       UINT32 *rd_channel_id_ptr );
PUBLIC PMC_ERROR cpb_chnl_prov( cpb_handle_t *cpb_handle,
                                UINT32 ingress_mux_input_port,
                                UINT32 ingress_mux_input_port_chnl,
                                UINT32 ingress_mux_egress_port,
                                UINT32 egress_mux_input_port,
                                UINT32 egress_mux_output_port,
                                UINT32 egress_mux_output_port_chnl,
                                UINT32 rate,
                                util_schd_chnl_rate_exp_t rate_units,
                                UINT32 cpb_data,
                                UINT32 halt_buffer,
                                UINT32 hdr_en,
                                BOOL8 backup_mode );
PUBLIC void cpb_chnl_backup_swap(cpb_handle_t *cpb_handle, UINT32 ingress_mux_input_port, UINT32 ingress_mux_input_port_chnl, BOOL8 use_backup);
PUBLIC PMC_ERROR cpb_chnl_backup_status(cpb_handle_t *cpb_handle, UINT32 ingress_mux_input_port, UINT32 ingress_mux_input_port_chnl, BOOL8 *use_backup_ptr);
PUBLIC PMC_ERROR cpb_chnl_activate( cpb_handle_t *cpb_handle,
                                    UINT32 egress_mux_output_port,
                                    UINT32 egress_mux_output_port_chnl );
PUBLIC PMC_ERROR cpb_egress_mux_input_init( cpb_handle_t *cpb_handle,
                                            UINT32 input_port,
                                            cpb_xfer_mode_t cpb_xfer_mode,
                                            cpb_dcs_instance_mode_t mode);
PUBLIC PMC_ERROR cpb_egress_mux_output_init( cpb_handle_t *cpb_handle,
                                             UINT32 output_port,
                                             cpb_xfer_mode_t sifd_xfer,
                                             UINT32 mode );
PUBLIC PMC_ERROR cpb_ingress_mux_input_init( cpb_handle_t *h,
                                             UINT32 input_port,
                                             cpb_xfer_mode_t cpb_xfer_mode,
                                             UINT32 hdr_removal_size);
PUBLIC PMC_ERROR cpb_dcpb_ingress_mux_output_init( cpb_handle_t *cpb_handle,
                                                   UINT32 output_port,
                                                   cpb_dcpb_igr_sifd_sel_t source );
PUBLIC PMC_ERROR cpb_mcpb_mux_mode_init(cpb_handle_t *h,
                                         cpb_mcpb_mux_mode_t mux_mode);
PUBLIC cpb_mcpb_mux_mode_t cpb_mcpb_mux_mode_init_get(cpb_handle_t *h);
PUBLIC PMC_ERROR cpb_chnl_deactivate( cpb_handle_t *cpb_handle,
                                      UINT32 egress_mux_output_port,
                                      UINT32 egress_mux_output_port_chnl );
PUBLIC PMC_ERROR cpb_chnl_switch( cpb_handle_t *cpb_handle,
                                  UINT32 egress_mux_output_port,
                                  UINT32 egress_mux_output_port_chnl );
PUBLIC PMC_ERROR cpb_mcpb_chnl_prov( cpb_handle_t *cpb_handle, 
                                     UINT32 egress_mux_output_port,
                                     UINT32 egress_mux_output_port_chnl,
                                     UINT32 rate,
                                     util_schd_chnl_rate_exp_t rate_units,
                                     UINT32 halt_buffer);
PUBLIC PMC_ERROR cpb_ocpb_chnl_prov( cpb_handle_t *cpb_handle,
                                     UINT32 input_port,
                                     UINT32 input_port_chnl,
                                     UINT32 output_port,
                                     UINT32 output_port_chnl,
                                     UINT32 rate,
                                     util_schd_chnl_rate_exp_t rate_units,
                                     UINT32 halt_buffer, 
                                     UINT32 hdr_en);
PUBLIC PMC_ERROR cpb_chnl_deprov( cpb_handle_t *cpb_handle,
                                  UINT32 egress_mux_output_port,
                                  UINT32 egress_mux_output_port_chnl,
                                  BOOL8  backup_mode);
PUBLIC PMC_ERROR cpb_chnl_deprov_super_deprov( cpb_handle_t *cpb_handle,
                                               UINT32 egress_mux_output_port,
                                               UINT32 egress_mux_output_port_chnl );                                  
PUBLIC void cpb_slv_chnl_qs_ch_en_get(cpb_handle_t *cpb_handle,
                                      UINT32 ingress_mux_input_port,
                                      UINT32 ingress_mux_input_port_chnl,
                                      UINT8  *qs_ch_en_ptr);
PUBLIC void cpb_slv_chnl_qs_ch_en_set(cpb_handle_t *cpb_handle,
                                      UINT32 ingress_mux_input_port,
                                      UINT32 ingress_mux_input_port_chnl,
                                      UINT32 enable);
PUBLIC PMC_ERROR cpb_chnl_source_and_type_test ( cpb_handle_t *cpb_handle,
                                                 UINT32 egress_mux_output_port,
                                                 UINT32 egress_mux_output_port_chnl,
                                                 UINT32 *ingress_mux_input_port,
                                                 UINT32 *ingress_mux_input_port_chnl,
                                                 BOOL8 *is_broadcast,
                                                 BOOL8 *is_multicast);
PUBLIC PMC_ERROR cpb_chnl_source_type_and_primary_test(cpb_handle_t *cpb_handle,
                                                       UINT32 egress_mux_output_port,
                                                       UINT32 egress_mux_output_port_chnl,
                                                       UINT32 *ingress_mux_input_port,
                                                       UINT32 *ingress_mux_input_port_chnl,
                                                       BOOL8 *is_broadcast,
                                                       BOOL8 *is_multicast,
                                                       BOOL8 *is_reader_primary,
                                                       BOOL8 *is_slave_zone_primary);
PUBLIC PMC_ERROR cpb_ingress_mux_input_port_data_get( cpb_handle_t *cpb_handle, 
                                                      UINT32 egress_mux_input_port, 
                                                      UINT32 rd_queue_id,
                                                      UINT32 *p_wr_queue_id, 
                                                      UINT32 *p_ingress_mux_input_port, 
                                                      UINT32 *p_ingress_mux_input_port_chnl );
PUBLIC BOOL8 cpb_mcpb_enet_connect_start_state_test(cpb_handle_t *cpb_handle);
PUBLIC BOOL8 cpb_mcpb_enet_channel_start_state_test(cpb_handle_t *cpb_handle,
                                                    UINT32 channel);
PUBLIC BOOL8 cpb_mcpb_enet_channel_start_state_filtered_test(cpb_handle_t *cpb_handle,
                                                             UINT32 channel,
                                                             UINT32 dcs_mask,
                                                             UINT32 target_mask);
PUBLIC BOOL8 cpb_mcpb_cpu_mgmt_start_state_test(cpb_handle_t *cpb_handle);
PUBLIC PMC_ERROR cpb_chnl_hole_inst_tail_cfg( cpb_handle_t *h,
                                              UINT32 egress_mux_output_port,
                                              UINT32 egress_mux_output_port_chnl,
                                              BOOL8   hole_inst_eop_size);
PUBLIC PMC_ERROR cpb_chnl_hole_inst_head_cfg( cpb_handle_t *h,
                                              UINT32 egress_mux_output_port,
                                              UINT32 egress_mux_output_port_chnl,
                                              UINT32 hole_inst_sop_size);
PUBLIC BOOL8 cpb_port_init_state_get(cpb_handle_t *cpb_handle,
                                    cpb_port_type_t port_type,
                                    UINT32 port_id );
PUBLIC PMC_ERROR cpb_chnl_range_check( cpb_handle_t *h,
                                       UINT32 input_port,
                                       UINT32 input_port_chnl,
                                       UINT32 output_port,
                                       UINT32 output_port_chnl);
PUBLIC PMC_ERROR cpb_dcs_egress_dpi_chnl_source_get(  cpb_handle_t *cpb_handle, 
                                                      UINT32 egress_mux_output_port, 
                                                      UINT32 egress_mux_output_port_chnl,
                                                      UINT32 *p_egress_mux_input_port, 
                                                      UINT32 *p_rd_channel_id, 
                                                      UINT32 *p_rd_queue_id );                                       
PUBLIC PMC_ERROR cpb_queue_resize_wrapper ( cpb_handle_t *cpb_handle,
                                            UINT32 egress_mux_output_port,
                                            UINT32 egress_mux_output_port_chnl,
                                            UINT32 num_blocks);    
PUBLIC PMC_ERROR cpb_dcs_db_num_entry_get(cpb_handle_t *cpb_handle,
                                          UINT32 dcs_instance,
                                          UINT32 egress_mux_output_port,
                                          UINT32 egress_mux_output_chnl,
                                          UINT32 *num_entries);
PUBLIC PMC_ERROR cpb_dcs_db_num_entry_by_rd_chnl_id_get(cpb_handle_t *cpb_handle,
                                                        UINT32 dcs_instance,
                                                        UINT32 rd_channel_id,
                                                        UINT32 *num_entries);
PUBLIC void cpb_sifd_chnl_ctxt_set(cpb_handle_t *cpb_handle, UINT32 port, UINT32 chnl, UINT32 zone_bypass);
PUBLIC void cpb_sifd_chnl_ctxt_get(cpb_handle_t *cpb_handle, UINT32 port, UINT32 chnl, UINT32 *zone_bypass_ptr);
PUBLIC void cpb_connect_state_get(cpb_handle_t *cpb_handle, util_global_cpb_connect_t *conn_ptr);
PUBLIC void cpb_connect_data_state_get(cpb_handle_t *cpb_handle, util_global_cpb_connect_t **conn_pptr);
PUBLIC void cpb_fifo_num_blocks_by_input_get(cpb_handle_t *h,
                                             UINT32 slave_port,
                                             UINT32 slave_chnl,
                                             UINT32 *num_block_ptr,
                                             UINT32 *num_burst_ptr);
PUBLIC UINT32 cpb_dcs_page_update(cpb_handle_t *cpb_handle, UINT32 dcs_inst);
PUBLIC UINT32 cpb_dcs_page_swap_and_offline_update(cpb_handle_t *cpb_handle,
                                                   UINT32 dcs_inst,
                                                   BOOL8* is_cal_updated_ptr,
                                                   BOOL8 is_cal_chnl_updated_arr[]);
PUBLIC PMC_ERROR cpb_queue_flush_and_ptr_reset(cpb_handle_t *h,
                                               UINT32 slave_port,
                                               UINT32 slave_chnl);
PUBLIC PMC_ERROR cpb_chnl_activate_super_prov( cpb_handle_t *cpb_handle,
                                               UINT32 egress_mux_output_port,
                                               UINT32 egress_mux_output_port_chnl );
PUBLIC PMC_ERROR cpb_chnl_deactivate_super_prov( cpb_handle_t *cpb_handle,
                                                 UINT32 egress_mux_output_port,
                                                 UINT32 egress_mux_output_port_chnl );  
PUBLIC PMC_ERROR cpb_fifo_var_data_and_chnl_state_reset( cpb_handle_t *cpb_handle,
                                                         UINT32 egress_mux_input_port,
                                                         UINT32 rd_queue_id,
                                                         UINT32 queue_index );   
PUBLIC PMC_ERROR cpb_connect_data_delete( cpb_handle_t *cpb_handle,
                                          UINT32 ingress_mux_input_port,
                                          UINT32 ingress_mux_input_port_chnl,
                                          UINT32 egress_mux_output_port,
                                          UINT32 egress_mux_output_port_chnl );
PUBLIC PMC_ERROR cpb_connect_data_delete_super_deprov( cpb_handle_t *cpb_handle,
                                                       UINT32 ingress_mux_input_port,
                                                       UINT32 ingress_mux_input_port_chnl,
                                                       UINT32 egress_mux_output_port,
                                                       UINT32 egress_mux_output_port_chnl,
                                                       BOOL8 super_deprov);
PUBLIC PMC_ERROR cpb_dcpb_pmon_cfg(cpb_handle_t *cpb_handle, 
                                   cpb_dcpb_dpi_port_type_t port, 
                                   UINT32 counter_num,
                                   cpb_pmon_mode pmon_mode,
                                   BOOL8 count_drop_packets,
                                   BOOL8 count_err_packets,
                                   BOOL8 count_oversize_packets,
                                   BOOL8 count_good_packets);
PUBLIC PMC_ERROR cpb_ocpb_pmon_cfg(cpb_handle_t *cpb_handle, 
                                   cpb_ocpb_dpi_port_type_t port, 
                                   UINT32 counter_num,
                                   cpb_pmon_mode pmon_mode,
                                   BOOL8 count_drop_packets,
                                   BOOL8 count_err_packets,
                                   BOOL8 count_oversize_packets,
                                   BOOL8 count_good_packets);
PUBLIC PMC_ERROR cpb_mcpb_pmon_cfg (cpb_handle_t *cpb_handle,
                                    cpb_mcpb_ingr_input_port_type_t port,
                                    UINT32 counter_num,
                                    cpb_pmon_mode pmon_mode,
                                    BOOL8 count_drop_packets,
                                    BOOL8 count_err_packets,
                                    BOOL8 count_oversize_packets,
                                    BOOL8 count_good_packets);
PUBLIC PMC_ERROR cpb_pmon_latch_trig_set(cpb_handle_t *cpb_handle, 
                                         UINT32 port, 
                                         UINT32 counter_mask);
PUBLIC PMC_ERROR cpb_pmon_counter_get(cpb_handle_t *cpb_handle, 
                                      UINT32 port, 
                                      UINT32 counter_mask,
                                      UINT32 * count0,
                                      UINT32 * count1,
                                      UINT32 * count2,
                                      UINT32 * count3);
PUBLIC PMC_ERROR cpb_pmon_chnl_add_drop_cfg(cpb_handle_t *cpb_handle, 
                                            UINT32 port, 
                                            UINT32 counter_num,
                                            UINT32 channel,
                                            BOOL8 add_channel);
PUBLIC void cpb_ocpb_slv_chnl_qs_ch_en_get_all(cpb_handle_t *cpb_handle,
                                               UINT32 ingress_mux_input_port,
                                               UINT32 *ingress_mux_input_port_chnl_ptr,
                                               BOOL8  enable,
                                               UINT8  *qs_ch_en_ptr);
/*
 * Exported PMC Internal Testing Interface 
 */                                    
PUBLIC PMC_ERROR cpb_queue_num_block_get( cpb_handle_t *cpb_handle,
                                          UINT32 pop_port_id,
                                          UINT32 read_qid,
                                          UINT32 *num_current_blocks_ptr );
PUBLIC PMC_ERROR cpb_queue_resize( cpb_handle_t *cpb_handle,
                                   UINT32 pop_port_id,
                                   UINT32 read_qid,
                                   BOOL8  page_management_en,
                                   UINT32 num_blocks );


#if defined (PMC_VERIFICATION) | defined (PMC_SW_SIMULATION)
PUBLIC PMC_ERROR cpb_fifo_queue_cfg_get(cpb_handle_t *cpb_handle,
                                        UINT32 queue_system_id,
                                        UINT32 read_qid,
                                        UINT32 *push_port_id_ptr,
                                        UINT32 *wr_queue_id_ptr, 
                                        UINT32 *end_addr_ptr);
PUBLIC PMC_ERROR cpb_dpi_chnl_cfg_get(cpb_handle_t *cpb_handle, 
                                            UINT32 wr_port_id,
                                            UINT32 wr_chan_id, 
                                            UINT32 *rd_port_id_ptr, 
                                            UINT32 *rd_chan_id_ptr);
#endif

/*
 * Register Access Operations
 */ 
PUBLIC PMC_ERROR cpb_dpi_slave_port_burst_cfg( cpb_handle_t *h, UINT32 port, cpb_burst_mode_t burst_mode);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_zone_ovrd_cfg(cpb_handle_t  *h, UINT32 port, UINT32 chnl, cpb_dpi_chnl_zone_mode_t mode, UINT32 dpi_mstr_port, UINT32 dpi_mstr_chnl);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_zone_thresh_cfg_check(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 zone_0_1, UINT32 zone_1_2, UINT32 zone_2_3);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_zone_thresh_cfg(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 zone_0_1, UINT32 zone_1_2, UINT32 zone_2_3);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_zone_thresh_cfg_get(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 *zone_0_1, UINT32 *zone_1_2, UINT32 *zone_2_3);
PUBLIC PMC_ERROR cpb_dpi_mstr_port_chnl_zone_mode_cfg(cpb_handle_t  *h, UINT32 port, UINT32 chnl, cpb_dpi_chnl_zone_mode_t mode);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_map_cfg(cpb_handle_t  *h, UINT32 port, UINT32 chnl, UINT32 dpi_mstr_port, UINT32 dpi_mstr_chnl);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_zone_type_cfg_get(cpb_handle_t  *h, UINT32 port, UINT32 chnl, cpb_dpi_chnl_zone_mode_t *mode);
PUBLIC PMC_ERROR cpb_dpi_slave_port_zone_cfg(cpb_handle_t *h, UINT32 port, UINT32 num_chnl, UINT32 ovrd_val, cpb_burst_mode_t burst_mode);
PUBLIC PMC_ERROR cpb_dpi_slave_port_hdr_remove_cfg(cpb_handle_t *h, UINT32 port, UINT32 hdr_size);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_hdr_strip_cfg(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 strip);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_rd_queue_map_cfg(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 queue_system, UINT32 rd_queue_id);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_enbl(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 queue_system,  UINT32 enable);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_zone_source_cfg(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 operation, UINT32 master_queue);
PUBLIC PMC_ERROR cpb_dpi_slave_port_chnl_ing_mux_output_port_sel_cfg(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 operation, UINT32 output_port);
PUBLIC PMC_ERROR cpb_dpi_mstr_port_chnl_zone_0_1_zone_1_2_thresh_cfg(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 zone_0_1, UINT32 zone_1_2);
PUBLIC PMC_ERROR cpb_dpi_mstr_port_chnl_zone_thresh_cfg(cpb_handle_t *h, UINT32 port, UINT32 chnl, UINT32 zone_0_1, UINT32 zone_1_2, UINT32 zone_2_3);
PUBLIC PMC_ERROR cpb_dpi_mstr_port_zone_cfg(cpb_handle_t *h, UINT32 port, UINT32 enable);
PUBLIC PMC_ERROR cpb_mux_cpbd_src_sel_cfg(cpb_handle_t *h, UINT32 port, UINT32 src);
PUBLIC PMC_ERROR cpb_mux_cpbd_dest_sel_cfg(cpb_handle_t *h, UINT32 port, UINT32 dest);
PUBLIC PMC_ERROR cpb_mux_mgmt_src_sel_cfg(cpb_handle_t *h, UINT32 source);
PUBLIC PMC_ERROR cpb_mux_mgmt_dest_sel_cfg(cpb_handle_t *h, UINT32 dest);
PUBLIC PMC_ERROR cpb_dcs_xoff_cfg( cpb_handle_t *h,
                                   UINT32 slice,
                                   cpb_xoff_type_t xoff_type,
                                   cpb_xoff_mode_t xfer_term );
PUBLIC PMC_ERROR cpb_dcs_credit_cfg( cpb_handle_t *h,
                                   UINT32 slice,
                                   cpb_credit_type_t credit_type,
                                   UINT32 credit_mode );
PUBLIC PMC_ERROR cpb_dcs_burst_xfer_mode_cfg( cpb_handle_t *h,
                                              UINT32 slice,
                                              cpb_xfer_mode_type_t xfer_mode,
                                              cpb_burst_mode_t burst_mode );
PUBLIC cpb_xfer_mode_type_t cpb_dcs_burst_xfer_mode_get( cpb_handle_t *h,
                                                         UINT32 slice);
PUBLIC PMC_ERROR cpb_dcs_page_swap(cpb_handle_t *cpb_handle, UINT32 slice);
PUBLIC PMC_ERROR cpb_dcs_page_ctrl_cfg(cpb_handle_t *h, UINT32 slice, UINT32 mode, UINT32 ptr );
PUBLIC PMC_ERROR cpb_dcs_page_align(cpb_handle_t *h, UINT32 slice);
PUBLIC PMC_ERROR cpb_dcs_queue_reset(cpb_handle_t *h, UINT32 slice, UINT32 queue_id);
PUBLIC PMC_ERROR cpb_dcs_ram_power_cfg(cpb_handle_t *h, UINT32 slice, cpb_target_ram_t ram, cpb_ram_power_mode_t mode);
PUBLIC PMC_ERROR cpb_dcs_enable(cpb_handle_t *h, UINT32 slice, UINT32 enable);
PUBLIC PMC_ERROR cpb_dcs_poll_swap_success( cpb_handle_t *cpb_handle, UINT32 dcs_inst );
PUBLIC PMC_ERROR cpb_dcs_swap_poll( cpb_handle_t *cpb_handle, UINT32 dcs_inst );
PUBLIC PMC_ERROR cpb_xoff_mask_set(cpb_handle_t *h,
                                   UINT32 dcs_inst,
                                   UINT32 dpi_mst_port,
                                   UINT32 dpi_mst_channel,
                                   BOOL8 set_mask);
PUBLIC void cpb_x_default_register_start_set(cpb_handle_t *cpb_handle);
PUBLIC BOOL8 cpb_dcs_status_get(cpb_handle_t *cpb_handle,
                                UINT32 input_port);
PUBLIC PMC_ERROR cpb_sysotn_mode_set( cpb_handle_t *cpb_handle, 
                                      BOOL8 sysotn_mode);
PUBLIC PMC_ERROR cpb_int_chnl_retrieve(cpb_handle_t    *cpb_handle,
                                       UINT32           port,
                                       UINT32           port_chnl,
                                       cpb_int_chnl_t  *filt_table_ptr,
                                       cpb_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR cpb_int_chnl_clear(cpb_handle_t    *cpb_handle,
                                    UINT32           port,
                                    UINT32           port_chnl,
                                    cpb_int_chnl_t  *int_table_ptr,
                                    cpb_int_chnl_t  *int_en_table_ptr);
PUBLIC util_schd_chnl_state_t cpb_channel_state_get(cpb_handle_t * cpb_handle,
                                                    UINT32 cpb_port,
                                                    UINT32 cpb_channel,
                                                    BOOL8 is_egress_port);
PUBLIC PMC_ERROR cpb_dcs_calendar_cfg( cpb_handle_t *cpb_handle, 
                                       UINT32 dcs_inst,
                                       UINT32 rd_queue_id);
PUBLIC PMC_ERROR cpb_dsc_calendar_pinch_mode_set(cpb_handle_t *cpb_handle,
                                                 UINT32 output_port,
                                                 util_global_calendar_pinch_t cal_pinch_type);
PUBLIC PMC_ERROR cpb_ghao_queue_resize_zone_update ( cpb_handle_t *cpb_handle,
                                                     UINT32 egress_mux_output_port,
                                                     UINT32 egress_mux_output_port_chnl,
                                                     UINT32 halt_buffer,
                                                     UINT32 oduflex_num_ts,
                                                     BOOL increase);
PUBLIC PMC_ERROR cpb_pmpm_recover_pages(cpb_handle_t *cpb_handle,
                                        UINT32 *num_pages_freed,
                                        UINT32 *num_pages_failed);
PUBLIC void cpb_oduksw_port0_set(cpb_handle_t *cpb_handle,
                                 cpb_oduksw_port0_t cpb_oduksw_port0);
PUBLIC PMC_ERROR cpb_determine_num_blocks( cpb_handle_t *cpb_handle,
                                            UINT32 dcs_inst,
                                            UINT32 push_port_id,
                                            UINT32 rd_chnl_id,
                                            UINT32 rate, UINT32 rate_units,
                                            UINT32 halt_buffer_size,
                                            BOOL8 ilkn_source,
                                            UINT32 *num_blocks );
PUBLIC cpb_oduksw_port0_t cpb_oduksw_port0_get(cpb_handle_t *cpb_handle);
#ifndef CPB_UNIT_TEST
PUBLIC PMC_ERROR cpb_fw_opsa_debug_status_internal_get(cpb_handle_t *cpb_handle,
                                                         hostmsg_opsa_subsystem_cfg_t subsystem,
                                                         UINT8 channel,
                                                         fw_opsa_status_t *fw_opsa_status_ptr);
PUBLIC PMC_ERROR cpb_fw_opsa_cfg_mpma_port_add(cpb_handle_t *cpb_handle,
                                               hostmsg_opsa_subsystem_cfg_t subsystem,
                                               UINT32 *chnl_add_msk_ptr,
                                               UINT8  *qs_ch_en_ptr);
#endif
PUBLIC BOOL cpb_remap_clean(cpb_handle_t * cpb_handle,
                            UINT32 * master_port,
                            UINT32 * master_channel);
PUBLIC void cpb_debug_page_block_alloc_print(cpb_handle_t *cpb_handle);
PUBLIC PMC_ERROR mcpb_dpi_slave_port_chnl_flush(cpb_handle_t *handle,
                                                UINT32        egr_port,
                                                UINT32        egr_chnl,
                                                UINT32        ingr_port,
                                                UINT32        ingr_chnl);
PUBLIC void cpb_oduksw_port_bulk_update_req_set(cpb_handle_t *cpb_handle,
                                                util_global_oduk_port_t port_type,
                                                util_global_datapath_dir_t port_direction,
                                                BOOL8 update_required);

PUBLIC BOOL8 cpb_oduksw_port_bulk_update_req_get(cpb_handle_t *cpb_handle,
                                                 util_global_oduk_port_t port_type,
                                                 util_global_datapath_dir_t update_required);

PUBLIC void cpb_oduksw_port_bulk_update_req_init(cpb_handle_t *cpb_handle);
#endif /*OXYGEN_PUBLIC_ONLY*/

#ifdef __cplusplus
}
#endif



#endif /* _CPB_H */

/*
** end of file
*/
