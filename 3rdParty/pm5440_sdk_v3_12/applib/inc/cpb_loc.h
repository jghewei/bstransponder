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
#ifndef _CPB_LOC_H
#define _CPB_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_log.h"
/*
#include "pmc_verification.h"
*/
#include "util_global.h"
#include "cpb.h"
#include "dcs_io.h"
#include "master_dpi_io.h"
#include "slave_dpi_io.h"
#include "scpb_reg_io_inline.h"
#include "qs_io_inline.h"
#include "cpb_ctrl_io_inline.h"
#include "slave_dpi_io_inline.h"
#include "pmpm_io_inline.h"

#ifndef CPB_UNIT_TEST
#include "hostmsg.h"
#endif

/*#include "util_gen_db.h"*/




/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: cpb_mem_pool_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying memory pool usage.
*
* ELEMENTS:
*    CPB_MEM_POOL_ID_0 - Stores pointer of size: num_pages
*    CPB_MEM_POOL_ID_1 - Stores pointer of size: num_pages
*    CPB_MEM_POOL_ID_2 - Stores pointer of size: (block_per_page*num_pages) + num_pages
*    CPB_MEM_POOL_ID_3 - Stores pointer of size: num_pages*2
*
*******************************************************************************/
typedef enum
{
    CPB_MEM_POOL_ID_0 = 0,
    CPB_MEM_POOL_ID_1,
    CPB_MEM_POOL_ID_2,
    CPB_MEM_POOL_ID_3,
    
    LAST_CPB_MEM_POOL_ID
}cpb_mem_pool_id_t;


/*******************************************************************************
* ENUM: cpb_path_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Represents the status of an end to end path. Only used with crash restart.
*
* ELEMENTS:
*   CPB_PATH_STATE_DISABLED        - Path is disabled
*   CPB_PATH_STATE_ENABLED         - Path is enabled
*   CPB_PATH_STATE_PARTIAL         - Path is partially enabled
*
*******************************************************************************/
typedef enum
{
    CPB_PATH_STATE_DISABLED = 0,
    CPB_PATH_STATE_ENABLED,
    CPB_PATH_STATE_PARTIAL
}cpb_path_state_t;

/*******************************************************************************
* ENUM: cpb_pmpm_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for defining PMPM instance types
*
* ELEMENTS:
*     CPB_PMPM_INST_PMPM      - PMPM instance
*     CPB_PMPM_INST_PMPM_PL   - PMPM_PL instance
*     CPB_PMPM_INST_PMPM_EOP  - PMPM_EOP instance
*
*******************************************************************************/
typedef enum
{
    CPB_PMPM_INST_PMPM     = 0,
    CPB_PMPM_INST_PMPM_PL  = 1,
    CPB_PMPM_INST_PMPM_EOP = 2,
    
    LAST_CPB_PMPM_INST
}cpb_pmpm_inst_t;

/*
** Constants
*/

#define CPB_PMPM_PHYSICAL_PAGE_EMPTY 0
#define CPB_PHYSICAL_BLOCK_EMPTY 0
#define CPB_PMPM_PHYSICAL_PAGE_NIL 0x7F
#define CPB_BLOCK_LBA_NIL 0x1F
#define CPB_BLOCK_WQID_NIL 0xFF
#define CPB_BLOCK_RQID_NIL 0xFF
#define CPB_FIFO_PUSHID_NIL 0xFF


#define CPB_NUM_USEC_ITERATIONS 1000


#define DCS_UTIL_SCHD_CH_TYPE                0
#define DCS_DEFAULT_GROUP_ID                 0
#define DCS_BASE_CHNL_NUMBER                 0
#define DCS_CHNL_NUMBER_STEP                 1
#define DCS_CAL_ENTRY_PER_ROW                4
#define DCS_NULL_ROW_NUMBER                  0xFFFFFFFF
#define NUM_OF_DCS                           4
#define DCS_NULL_QUEUE_ID                    0x7F
#define CPB_NULL_DB_REC                      0xFF

#define DCPB_MST_PORT_0_CH_OFFSET                 0
#define DCPB_MST_PORT_1_CH_OFFSET                 0
#define DCPB_MST_PORT_2_CH_OFFSET                 32
#define DCPB_MST_PORT_3_CH_OFFSET                 128
#define DCPB_MST_PORT_4_CH_OFFSET                 0
#define DCPB_MST_PORT_5_CH_OFFSET                 0

#define DCPB_SLV_PORT_0_CH_OFFSET                 0
#define DCPB_SLV_PORT_1_CH_OFFSET                 0
#define DCPB_SLV_PORT_2_CH_OFFSET                 108
#define DCPB_SLV_PORT_3_CH_OFFSET                 12
#define DCPB_SLV_PORT_4_CH_OFFSET                 0
#define DCPB_SLV_PORT_5_CH_OFFSET                 0

#define DEFAULT_32BIT_INVALID                0xFFFFFFFF


#define CPB_MAX_HDR_SIZE                     6

/*in microseconds (us) */

#define DCS_PAGE_SWAP_POLL_DELAY             1
/* old value is 10, extended it to handle MCPB 50MHz port */
#define DCS_PAGE_SWAP_POLL_ITERATIONS        150
#define PMON_TRIG_POLL_DELAY             1
#define PMON_TRIG_POLL_ITERATIONS        150
#define CPB_REMAP_BLOCK_POLL_DELAY           1
#define CPB_REMAP_BLOCK_POLL_ITERATIONS      300

#define CPB_NUM_SIFD_INTF                    2
#define CPB_NUM_SIFD_CHNL_IN_32BIT_WORDS     5
#define CPB_BITS_IN_32_BIT_WORD              32

#define CPB_MAX_FIFO_NUM                    96 

#define CPB_NUM_SCHED_ENTRIES               96 
#define CPB_NULL_CHANNEL_ENTRY              0xFF 
#define DCPB_TOTAL_RATE_GBPS                120 
#define OCPB_TOTAL_RATE_GBPS                120
#define MCPB_TOTAL_RATE_ENET_GBPS           120
#define MCPB_TOTAL_RATE_CPU_FEGE_GBPS       2

#define DCPB_NUM_PAGES                      32 
#define DCPB_BLOCKS_PER_PAGE                16 
#define DCPB_BLOCK_SIZE                     12288
#define DCPB_MTU_SIZE                       9600
#define DCPB_MAX_CHANNEL_PORT0              149
#define DCPB_MAX_CHANNEL_PORT1              149
#define DCPB_MAX_CHANNEL_PORT2              11
#define DCPB_MAX_CHANNEL_PORT3              95
#define DCPB_MAX_CHANNEL_PORT4              11
#define DCPB_MAX_CHANNEL_PORT5              11
#define DCPB_MAX_DPI_PORT                   5
#define DCPB_NUM_PUSH_PORTS                 4
#define DCPB_NUM_QUEUE_SYSTEMS              4

#define OCPB_NUM_PAGES                      22
#define OCPB_BLOCKS_PER_PAGE                64
#define OCPB_BLOCK_SIZE                     768
#define OCPB_MTU_SIZE                       512
#define OCPB_MAX_CHANNEL_PORT0              103
#define OCPB_MAX_CHANNEL_PORT1              103
#define OCPB_MAX_CHANNEL_PORT2              95
#define OCPB_MAX_CHANNEL_PORT3              95
#define OCPB_MAX_DPI_PORT                   3
#define OCPB_NUM_PUSH_PORTS                 4
#define OCPB_NUM_QUEUE_SYSTEMS              4

#define MCPB_NUM_PAGES                      22
#define MCPB_BLOCKS_PER_PAGE                64 
#define MCPB_BLOCK_SIZE                     384
#define MCPB_MTU_SIZE                       2000
#define MCPB_MAX_SLAVE_CHANNEL_PORT0        23
#define MCPB_MAX_SLAVE_CHANNEL_PORT1        23
#define MCPB_MAX_SLAVE_CHANNEL_PORT2        23
#define MCPB_MAX_SLAVE_CHANNEL_PORT3        23
#define MCPB_MAX_SLAVE_CHANNEL_PORT4        24
#define MCPB_MAX_SLAVE_CHANNEL_PORT5        24 
#define MCPB_MAX_SLAVE_CHANNEL_PORT6        11
#define MCPB_MAX_SLAVE_CHANNEL_PORT7        11
#define MCPB_MAX_MASTER_CHANNEL_PORT0       11 
#define MCPB_MAX_MASTER_CHANNEL_PORT1       11
#define MCPB_MAX_MASTER_CHANNEL_PORT2       11 
#define MCPB_MAX_MASTER_CHANNEL_PORT3       11 
#define MCPB_MAX_MASTER_CHANNEL_PORT4       46 
#define MCPB_MAX_MASTER_CHANNEL_PORT5       45  
#define MCPB_MAX_SLAVE_DPI_PORT             7
#define MCPB_MAX_MASTER_DPI_PORT            5
#define MCPB_NUM_PUSH_PORTS                 4
#define MCPB_NUM_POP_PORTS                  3
#define MCPB_NUM_QUEUE_SYSTEMS              4
#define MCPB_NUM_SCHED_CHANS_DCS0_1         24 
#define MCPB_MAX_SCHED_ENTRIES_DCS0_1       12
#define MCPB_NUM_SCHED_CHANS_DCS2           62
#define MCPB_MAX_SCHED_ENTRIES_DCS2         37
#define MCPB_NUM_SCHED_CHANS_DCS3           25
#define MCPB_MAX_SCHED_ENTRIES_DCS3         25
#define MCPB_CPU_PORT_NUM                   4
#define MCPB_FEGE_PORT_NUM                  5
#define CPB_MCPB_POP_PORT_2                 2
#define CPB_MCPB_POP_PORT_3                 3
#define CPB_MCPB_DPI_PORT_4                 4
#define CPB_MCPB_DPI_PORT_2                 2
#define CPB_DCPB_POP_PORT_2                 2
#define CPB_DCPB_POP_PORT_3                 3
#define CPB_MCPB_CPU_TO_MGMT_CHNL           24

#define CPB_BUS_WIDTH_BYTES                 48 /* same as word size in bytes */
#define CPB_ZONE0_1_DIVISOR                 4
#define CPB_ZONE1_2_DIVISOR                 2
#define CPB_ZONE2_3_DIVISOR                 4
#define CPB_ZONE2_3_NUMERATOR               3
#define CPB_MAX_SOP_HOLE_SIZE               47
#define CPB_MAX_EOP_HOLE_SIZE               95
#define CPB_MAX_PMON_COUNTER                3
#define BITS_IN_A_BYTE                      8

#define CPB_PMON_CNT_PKT_FILT_DROP_PACKETS_MSK   0x1
#define CPB_PMON_CNT_PKT_FILT_ERR_PACKETS_MSK    0x2
#define CPB_PMON_CNT_PKT_OVERSIZED_PACKETS_MSK   0x4
#define CPB_PMON_CNT_PKT_GOOD_PACKETS_MSK       0x10

#define MCPB_TOTAL_RATE_DCS2_GBPS           (MCPB_MAX_SCHED_ENTRIES_DCS2 * 10)
#define MCPB_TOTAL_RATE_DCS3_GBPS           (MCPB_MAX_SCHED_ENTRIES_DCS3 * 10)

/*
** Macro Definitions
*/

#define CPB_SET_BIT_IN_32_BIT_WORD(word,bitmsk)      ( word |= bitmsk )
#define CPB_CLEAR_BIT_IN_32_BIT_WORD(word,bitmsk)    ( word &= ~bitmsk )
#define CPB_GET_BIT_IN_32_BIT_WORD(word,bitoff,bitmsk) ( (word & bitmsk) >> bitoff )

#define CPB_GET_32_BIT_WORD_OFFSET(channel) (channel/CPB_BITS_IN_32_BIT_WORD)
#define CPB_GET_32_BIT_WORD_INDEX(channel) (channel%CPB_BITS_IN_32_BIT_WORD)

#define CPB_INVALID_REMAP_VALUE    0xFF

#define CPB_NULL_BLOCK_ID       0xFFFFFFFF

/*
** Macro Definitions
*/


/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: mcpb_im_sel_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Local memory tracking of MCPB ingress mux register fields
*
* ELEMENTS:
*   igr_ext_sel      - Local copy IM_IGR_EXT_SEL field
*   egr_ext_sel      - Local copy IM_EGR_EXT_SEL field
*   reflect_sel      - Local copy IM_REFLECT_SEL field
*       
*******************************************************************************/
typedef struct mcpb_im_sel_struct_t {
     UINT32 igr_ext_sel;
     UINT32 egr_ext_sel;
     UINT32 reflect_sel;
} mcpb_im_sel_t;     

/*******************************************************************************
* STRUCTURE: mcpb_em_sel_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Local memory tracking of MCPB egress mux register fields
*
* ELEMENTS:
*   em_egr_ext_sel      - Local copy EM_EGR_EXT_SEL field
*   em_igr_ext_sel      - Local copy EM_IGR_EXT_SEL field
*       
*******************************************************************************/
typedef struct mcpb_em_sel_struct_t {
     UINT32 em_egr_ext_sel;
     UINT32 em_igr_ext_sel;
} mcpb_em_sel_t;    

/*******************************************************************************
* STRUCTURE: block_inverted_page_table_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Local memory tracking of Block Inverted Page Table fields
*
* ELEMENTS:
*   ipt_valid      - Local copy IPT_VALID field
*   ipt_wqid       - Local copy IPT_WQID field
*   ipt_lba        - Local copy IPT_LBA field
*   ipt_queue_next - If a used block, index of next block in the same queue.
*                    If free, index of next free block in the same page.
*   ipt_queue_prev - prev and next, double linked list.
*       
*******************************************************************************/
typedef struct block_inverted_page_table_struct_t {
    
     BOOL   ipt_valid;
     UINT32 ipt_wqid; 
     UINT32 ipt_lba;

#ifdef CPB_BLOCK_LIST
     UINT32 ipt_queue_next;
     UINT32 ipt_queue_prev;
#endif
} block_inverted_page_table_t;

/*******************************************************************************
* STRUCTURE: page_inverted_page_table_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Local memory tracking of Page Inverted Page Table fields
*
* ELEMENTS:
*   ctrl_page_valid       - Local copy PAGE_VALID field
*   ctrl_ipt_qs_id        - Local copy IPT_QS_ID field
*   ctrl_ipt_wp_id        - Local copy IPT_WP_ID field
*   free_block_num        - Number of free blocks in this page
*   first_free_block      - List head of free blocks in the page.
*                           A universal block id, not index inside page.
*       
*******************************************************************************/
typedef struct page_inverted_page_table_struct_t {
     
    BOOL   ctrl_page_valid;
    UINT32 ctrl_ipt_qs_id;
    UINT32 mfege_ctrl_ipt_qs_id;
    UINT32 ctrl_ipt_wp_id;

#ifdef CPB_BLOCK_LIST
    UINT32 free_block_num;
    UINT32 first_free_block;
#endif
} page_inverted_page_table_t;

#if defined (PMC_VERIFICATION) | defined (PMC_SW_SIMULATION)
/* Verification only -> this ctxt is used to track the channel mapping for dpi wr
   channels */
/*******************************************************************************
* STRUCTURE: cpb_dpi_wr_ch_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DPI write channel context.
*
* ELEMENTS:
*       enable            - enable
*       wr_ch_id          - write channel ID
*       rd_ch_id          - read channel ID
*       rd_port_id        - read port ID
*       
*******************************************************************************/
typedef struct cpb_dpi_wr_ch_struct_t { 
    
    BOOL                        enable;
    UINT32                      wr_ch_id;
    UINT32                      rd_ch_id;
    UINT32                      rd_port_id;

} cpb_dpi_wr_ch_t;
#endif /* PMC_VERIFICATION */
#if defined (PMC_VERIFICATION) | defined (PMC_SW_SIMULATION)
/*******************************************************************************
* STRUCTURE: cpb_dpi_wr_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DPI Write port context.
*
* ELEMENTS:
*       enable            - enable
*       wr_port_id        - write port ID
*       wr_ch[150]        - write channel context array
*       
*******************************************************************************/
typedef struct cpb_dpi_wr_port_struct_t { 

    BOOL                        enable;
    UINT32                      wr_port_id;
    cpb_dpi_wr_ch_t             wr_ch[150];

} cpb_dpi_wr_port_t;
#endif /* defined (PMC_VERIFICATION) | defined (PMC_SW_SIMULATION) */

/* this ctxt is used to track the fifo information, there is an array 
   in cpb_var_struct_t, the index is max_fif_num*QSID+rd_queue_id */
/*******************************************************************************
* STRUCTURE: cpb_fifo_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Dynamic data associated with a FIFO
*
* ELEMENTS:
*       fifo_end_addr     - FIFO end address
*       status            - See cpb_fifo_status_t
*       write_id          - FIFO write ID
*       push_port_id      - FIFO push port ID
*       chnl_type         - See cpb_dcpb_rd_chnl_t
*       
*******************************************************************************/
typedef struct cpb_fifo_var_struct_t{ 

    UINT32                       fifo_end_addr;
    cpb_fifo_status_t            status;
    UINT32                       write_id;
    UINT32                       push_port_id;
    cpb_dcpb_rd_chnl_t           chnl_type;

} cpb_fifo_var_t;

/*******************************************************************************
* STRUCTURE: dcs_use_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This struct is to illustrate the dynamically changing context.
*
* ELEMENTS:
*       alloc_mode            - Determines the allocation mode (database or local mode)
*                               CPB will run in
*       db_id                 - Database ID that will be used by the database mode 
*                               to store scheduler enteries
*       db_client_handle      - Stores the database client handle to determine how 
*                               many clients are connected to a particular database
*                               in a CPB instance
*       
*******************************************************************************/
typedef struct {

    util_schd_alloc_method_t alloc_mode;
    util_gen_db_id_t db_id;
    UINT32 db_client_handle;

} dcs_use_ctxt_t;

/*******************************************************************************
* STRUCTURE: cpb_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static CPB module instance data.
*
* ELEMENTS:
*   cpb_type                  - See cpb_instance_type_t
*   page_size                 - PMPM page size
*   fifo_null_entry           - Null entry value for usage in FIFO
*   bus_width                 - Internal bus width
*   num_queue_systems         - Number of queue systems
*   num_dcs                   - Number of DCS instances
*   num_pages                 - number of PMPM pages
*   page_null_entry           - Null entry value for usage in Page Table
*   blocks_per_page           - Number of blocks per PMPM Page
*   block_size                - Size of PMPM Page Block
*   mtu_size                  - Maximum Transmit Unit
*   max_fifo_num              - Maximum number of FIFO
*   push_port_num             - Number of Push Ports
*   pop_port_num              - Number of Pop Ports
*   num_calendar_entries      - Number of DCS Calendar entries
*   bursts_per_block          - Number of bursts within PMPM Page Block
*   max_dpi_slv_chnl_id[8]    - Number of DPI Slave Port channels per Port
*   max_dpi_mst_chnl_id[8]    - Number of DPI Master Port channels per Port
*   max_slv_dpi_port          - Number of DPI Slave Ports
*   max_mst_dpi_port          - Number of DPI Master Ports
*   dcs_use_ctxt[NUM_OF_DCS]  - See dcs_use_ctxt_t one entry per DCS instance
*   sysotn_mode               - ODUKSW major operating mode
*       
*******************************************************************************/
typedef struct cpb_cfg_struct_t{

    cpb_instance_type_t    cpb_type;
    UINT32 page_size;
    UINT32 fifo_null_entry;
    UINT32 bus_width; 
    UINT32 num_queue_systems; 
    UINT32 num_dcs;
    UINT32 num_pages;
    UINT32 page_null_entry;
    UINT32 blocks_per_page;
    UINT32 block_size;
    UINT32 mtu_size;
    UINT32 max_fifo_num;
    UINT32 push_port_num;
    UINT32 pop_port_num;
    UINT32 num_calendar_entries;
    UINT32 bursts_per_block;
    UINT32 max_dpi_slv_chnl_id[8];
    UINT32 max_dpi_mst_chnl_id[8];
    UINT32 max_slv_dpi_port;
    UINT32 max_mst_dpi_port;
    dcs_use_ctxt_t dcs_use_ctxt[NUM_OF_DCS];
    BOOL8 sysotn_mode;
   
} cpb_cfg_t;

/*******************************************************************************
* STRUCTURE: cpb_slv_port_chnl_data_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Stores register data for a DPI slave port channel.\n\n
*
*   Array index is POP QID.
*
* ELEMENTS:
*       qs_ch_en     - Tracking QSn_CH_EN field setting
*       qs_pop_qid   - Tracking QSn_POP_QID field setting
*       qs_ch_mst    - Tracking QSn_CH_MST field setting
*       
*******************************************************************************/
typedef struct cpb_slv_port_chnl_data_t {

    UINT32 qs_ch_en[4];
    UINT32 qs_pop_qid[4];
    UINT32 qs_ch_mst[4]; 

} cpb_slv_port_chnl_data_t;


/*******************************************************************************
* STRUCTURE: cpb_block_leak_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Stores block leak information. Block leak occurs if the block remap 
*   algorithm resized a FIFO up 1 block to do a remap, but failed to resize
*   it back down.
*
* ELEMENTS:
*   leak_exists - One extra block is allocated in this CPB instance
*   pop_port    - Pop port of the FIFO with the extra block
*   rid         - Read queue id of the FIFO with the extra block
*       
*******************************************************************************/
typedef struct cpb_block_leak_t {

    BOOL8 leak_exists;
    UINT32 pop_port;
    UINT32 rid;

} cpb_block_leak_t;

/*******************************************************************************
* STRUCTURE: cpb_slv_data_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Stores register data for all DPI slave port channel.  2D array for 
*   maximum CPB instantiation used.  Addressed as:
*    slv_data[DPI Slave Port][DPI Slave Channel]
*
* ELEMENTS:
*       slv_data      - see cpb_slv_port_chnl_data_t
*       
*******************************************************************************/
typedef struct cpb_slv_data_t {

    cpb_slv_port_chnl_data_t slv_data[8][256];

} cpb_slv_data_t;


typedef util_schd_dcs_calendar_entry_t* dcs_calendars_t[NUM_OF_DCS];  /*!< Array of DCS calendars per CPB instance */
typedef util_schd_dcs_calendar_entry_t* dcs_temp_calendar_t; /*!< Single DCS calendar for intermediate purposes */

/*******************************************************************************
* STRUCTURE: cpb_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CPB Module Instance dynamic data
*
* ELEMENTS:
*   parent_energy_state        -  External block energy state.  See pmc_block_energy_state_enum
*   *p_var_t_ram_data          -  RAM data pointer
*   var_t_ram_data_size        -  RAM data size
*   *block_inverted_page_table -  See block_inverted_page_table_t
*   *page_inverted_page_table  -  See page_inverted_page_table_t
*   mcpb_im_sel                -  See mcpb_im_sel_t
*   mcpb_em_sel                -  See mcpb_em_sel_t
*   dcpb_em_sifd1_sel          -  0: CBRC/ENET_SYS
*                                 1: SIFD
*   dcpb_em_sifd2_sel          -  0: CBRC/ENET_SYS
*                                 1: SIFD
*   slv_dpi_port_init_flag[8]  -  Tracks initialized state of slave DPI port
*                                 0: Disabled
*                                 1: Enabled
*   mst_dpi_port_init_flag[8]  -  Tracks initialized state of master DPI port
*                                 0: Disabled
*                                 1: Enabled
*   dcs_inst_init_flag[4]      - Tracks initialized state of DCS Instance
*                                 0: Disabled
*                                 1: Enabled
*   *fifo_var_data             -  This array contains the context for four QSs, the array is the formed by stacking 
*                                 all the fifos for the four QSs, e.g., 0~95 are for QS0, 96~191 is for QS1, read 
*                                 queue id can be used to index the fifo array
*   *cpb_connect_data          - Dynamic monitoring of CPB connections see util_global_cpb_connect_t
*   dcs_offline_calendars      -  these pointers must be part of a common block of allocated memory, so they must be 
*                                 referenced against this common block instead of seperate alloc 
*   dcs_online_calendars       -  these pointers must be part of a common block of allocated memory, so they must be 
*                                 referenced against this common block instead of seperate alloc
*   dcs_temp_calendar          - Used for swapping online and offline page and for tracking calendar differences 
*   dcpb_ingress_mux_output_source[2]   -  these pointers must be part of a common block of allocated memory, so they must be 
*                                          referenced against this common block instead of seperate alloc 
*   sifd_zone_bypass[2][5]     -  Bit mask to identify if zone bypass feature used 
*                                 for SIFD ports.  A dimension is SIFD1, SIFD2, B
*                                 dimension is 32 x 5 bits, 1 bit per channel,
*                                 excess unused.  Values 
*                                 0: CPB FIFO depth
*                                 1: SIFD XON/XOFF status
*   *cpb_mem_pool[4]           - pointer to array of memory pools used within functions
*                                used to avoid dynamic alloc/free throughout code
*   cpb_mem_pool_in_use[4]     - memory pool tracking variable.
*   cpb_oduksw_port0           - See cpb_oduksw_port0_t
*   block_leak                 - See cpb_block_leak_t
*       
*******************************************************************************/
typedef struct cpb_var_struct_t{

    pmc_block_energy_state_enum  parent_energy_state;
    void                         *p_var_t_ram_data;
    UINT32                       var_t_ram_data_size;
    block_inverted_page_table_t  *block_inverted_page_table;
    page_inverted_page_table_t   *page_inverted_page_table;
    mcpb_im_sel_t                mcpb_im_sel;
    mcpb_em_sel_t                mcpb_em_sel;
    UINT32                       dcpb_em_sifd1_sel;
    UINT32                       dcpb_em_sifd2_sel;
    BOOL                         slv_dpi_port_init_flag[8];
    BOOL                         mst_dpi_port_init_flag[8];
    BOOL8                        dcs_inst_init_flag[4];
    cpb_fifo_var_t               *fifo_var_data; 
    util_global_cpb_connect_t *cpb_connect_data;    
    dcs_calendars_t dcs_offline_calendars;
    dcs_calendars_t dcs_online_calendars;
    dcs_temp_calendar_t dcs_temp_calendar; 
    UINT32 dcpb_ingress_mux_output_source[2];
    UINT32 sifd_zone_bypass[CPB_NUM_SIFD_INTF][CPB_NUM_SIFD_CHNL_IN_32BIT_WORDS];
#if defined (PMC_VERIFICATION) | defined (PMC_SW_SIMULATION)
/*
*   dpi_wr_port[8]             -  Only used by verification... this allows testbench
*                                 to compare fixed channel map with those used by s/w 
*                                 driver for mcpb snapshot
 */
    /* Only used by verification... this allows testbench
       to compare fixed channel map with those used by s/w 
       driver for mcpb snapshot*/
    cpb_dpi_wr_port_t dpi_wr_port[8];
#endif
    UINT32                       *cpb_mem_pool[4];
    BOOL                         cpb_mem_pool_in_use[5];
    cpb_oduksw_port0_t cpb_oduksw_port0;
    cpb_block_leak_t block_leak; 

} cpb_var_t;


/*******************************************************************************
* STRUCTURE: cpb_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CPB Module Instance Handle
*
* ELEMENTS:
*       base                                 - See pmc_handle_t
*       cfg                                  - See cpb_cfg_t
*       var                                  - See cpb_var_t
*       *util_schd_handle[NUM_OF_DCS]        - see util_schd_handle_t
*       
*******************************************************************************/
struct cpb_handle_struct_t {

    pmc_handle_t base;
    cpb_cfg_t cfg;
    cpb_var_t var;
    util_schd_handle_t *util_schd_handle[NUM_OF_DCS];   

};

/*******************************************************************************
* STRUCTURE: cpb_full_path_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Represents an end to end path through the CPB. Only used for crash restart. 
*   It is used in an array indexed by master port and channel.
*
* ELEMENTS:
*       state           - See cpb_path_state_t
*       slave_port      - DPI Slave port
*       slave_channel   - DPI Slave Port Channel
*       queue_system    - Queue System
*       rid             - Read Queue ID
*       
*******************************************************************************/
typedef struct cpb_full_path_t {

    cpb_path_state_t state;
    UINT32 slave_port;
    UINT32 slave_channel;
    UINT32 queue_system;
    UINT32 rid;

} cpb_full_path_t;

/*******************************************************************************
* STRUCTURE: cpb_pmpm_use_summary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Captures the usage of PMPM pages for analysis for block remapping.
*
* ELEMENTS:
*   page_block_use - array with elements defining the allocation of each
*                    PMPM page.  Index = pmpm page number 0 to 31(max).  Each
*                    element defines the number of used blocks within a 
*                    page.
*   page_assign    - array showing the assignment of PMPM pages to QS_ID,
*                    WP_ID ports.  Array index = IPT_QS_ID * 4 + IPT_WP_ID. 
*                    Each bit offset of the UINT32 respresents a PMPM page 
*                    address.  The bits are used as a mask to show which PMPM 
*                    pages are assigned to the push/pop port pair.
*       
*******************************************************************************/
typedef struct cpb_pmpm_use_summary_t {

    UINT32 page_block_use[32];
    UINT32 page_assign[16];

} cpb_pmpm_use_summary_t;  

/*
** Global variables
*/

/*
** Function Prototypes
*/

#ifdef __cplusplus
}
#endif

#endif /* _CPB_LOC_H */

/* 
** end of file 
*/

