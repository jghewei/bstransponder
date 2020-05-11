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
*   DESCRIPTION : This file contains definitions and C constructs related to
*   DIGI PMON. These costructs are exposed to the world.
*
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_PMON_API_H
#define _DIGI_PMON_API_H

#ifdef __cplusplus
extern "C" {
#endif


#include "pmc_bitarray.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_pmon_ddeg_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   dDEG Mode to Configure/Monitor
*
* ELEMENTS:
*   DIGI_PMON_DDEG_MODE_ENH     - Enhanced dDEG
*   DIGI_PMON_DDEG_MODE_BURSTY  - Bursty (G.798) dDEG
*
*******************************************************************************/
typedef enum digi_pmon_ddeg_mode_t 
{
    DIGI_PMON_DDEG_MODE_ENH     = 0,
    DIGI_PMON_DDEG_MODE_BURSTY  = 1,
    DIGI_PMON_DDEG_MODE_MAX_NUM
} digi_pmon_ddeg_mode_t;

/*******************************************************************************
* ENUM: digi_pmon_ddeg_hw_block_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   dDEG Hardware Block to Configure
*
* ELEMENTS:
*   See typedef
*
*******************************************************************************/
typedef enum digi_pmon_ddeg_hw_block_t 
{
    DIGI_PMON_DDEG_LINEOTN                = 0,
    DIGI_PMON_DDEG_SYSOTN                 = 1,
    DIGI_PMON_DDEG_COREOTN_STG_1_PM       = 2,
    DIGI_PMON_DDEG_COREOTN_STG_1_TCM      = 3,
    DIGI_PMON_DDEG_COREOTN_STG_2_PM       = 4,
    DIGI_PMON_DDEG_COREOTN_STG_2_TCM      = 5,
    DIGI_PMON_DDEG_COREOTN_STG_N_PM       = 6,
    DIGI_PMON_DDEG_COREOTN_STG_N_TCM      = 7,
    DIGI_PMON_DDEG_COREOTN_STG_3A_4_PM    = 8,
    DIGI_PMON_DDEG_COREOTN_STG_3A_4_TCM   = 9,
    DIGI_PMON_DDEG_HW_BLOCK_MAX_NUM
} digi_pmon_ddeg_hw_block_t;

/*******************************************************************************
* ENUM: digi_pmon_ipi_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Type of IPI bit
*
* ELEMENTS:
*   See typedef
*
*******************************************************************************/
typedef enum digi_pmon_ipi_type_t 
{
    DIGI_PMON_IPI_TYPE_PMON_OTN     = 0,
    DIGI_PMON_IPI_TYPE_PMON_MISC,
    DIGI_PMON_IPI_TYPE_DDEG_ENH,
    DIGI_PMON_IPI_TYPE_DDEG_BURSTY,

    LAST_DIGI_PMON_IPI_TYPE,
} digi_pmon_ipi_type_t;

/*
** Constants
*/

/* Constants to enable/disable PMON collection blocks using digi_pmon_work_order_cfg() */
#define DIGI_PMON_WORK_ORDER_LINEOTN                     0x00000001
#define DIGI_PMON_WORK_ORDER_SYSOTN                      0x00000002 
#define DIGI_PMON_WORK_ORDER_COREOTN_ERR_BLK_COUNTS      0x00000004 
#define DIGI_PMON_WORK_ORDER_CBRC                        0x00000100 
#define DIGI_PMON_WORK_ORDER_MGMT_PORT                   0x00000200 
#define DIGI_PMON_WORK_ORDER_ENET_LINE                   0x00000400 
#define DIGI_PMON_WORK_ORDER_ENET_SYS                    0x00000800 
#define DIGI_PMON_WORK_ORDER_MAPOTN                      0x00001000 
#define DIGI_PMON_WORK_ORDER_MCPB                        0x00002000 
#define DIGI_PMON_WORK_ORDER_SIFD                        0x00004000     

/* Maximum number of links/channels in subsystems */
#define SYSOTN_LINKS_MAX_NUM             1
#define COREOTN_STG1_TCM_CH_MAX_NUM      72
#define COREOTN_STG2_TCM_CH_MAX_NUM      256
#define COREOTN_STGN_TCM_CH_MAX_NUM      6
#define COREOTN_STGN_PM_CH_MAX_NUM       1
#define COREOTN_STG3A_TCM_CH_MAX_NUM     256
#define COREOTN_STG3B_TCM_CH_MAX_NUM     256
#define COREOTN_STG4_TCM_CH_MAX_NUM      256

#define COREOTN_STG1_TSE_CH_MAX_NUM      12
#define COREOTN_STGN_TSE_CH_MAX_NUM      12
#define COREOTN_STG3B_TSE_CH_MAX_NUM     12
#define COREOTN_STG4_TSE_CH_MAX_NUM      12

/* Number of registers needed to store dDEG interrupts. 1 bit per interrupt. */
#define LINEOTN_DDEG_INT_REG_MAX_NUM      (((DIGI_OTN_SERVER_CHNL_MAX-1)/32) + 1)
#define SYSOTN_DDEG_INT_REG_MAX_NUM       (((SYSOTN_LINKS_MAX_NUM-1)/32) + 1)
#define COREOTN_STG1_TCM_INT_REG_MAX_NUM  (((COREOTN_STG1_TCM_CH_MAX_NUM-1)/32) + 1)
#define COREOTN_STG1_PM_INT_REG_MAX_NUM   (((DIGI_OTN_SERVER_HO_CHNL_MAX-1)/32) + 1)
#define COREOTN_STG2_TCM_INT_REG_MAX_NUM  (((COREOTN_STG2_TCM_CH_MAX_NUM-1)/32) + 1)
#define COREOTN_STG2_PM_INT_REG_MAX_NUM   (((DIGI_OTN_SERVER_MO_CHNL_MAX-1)/32) + 1)
#define COREOTN_STGN_TCM_INT_REG_MAX_NUM  (((COREOTN_STGN_TCM_CH_MAX_NUM-1)/32) + 1)
#define COREOTN_STGN_PM_INT_REG_MAX_NUM   (((COREOTN_STGN_PM_CH_MAX_NUM-1)/32) + 1)
#define COREOTN_STG3A_TCM_INT_REG_MAX_NUM (((COREOTN_STG3A_TCM_CH_MAX_NUM-1)/32) + 1)
#define COREOTN_STG3A_PM_INT_REG_MAX_NUM  (((DIGI_OTN_SERVER_LO_CHNL_MAX-1)/32) + 1)
#define COREOTN_STG4_TCM_INT_REG_MAX_NUM  (((COREOTN_STG4_TCM_CH_MAX_NUM-1)/32) + 1)
#define COREOTN_STG4_PM_INT_REG_MAX_NUM   (((DIGI_OTN_SERVER_LO_CHNL_MAX-1)/32) + 1)

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: digi_pmon_work_order_msg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Contains information about pmon work order configuration.
*
* ELEMENTS:
*  enable_hw_block_msk - Bitmask of hardware block(s) to enable PMON for. The
*                        following contants can be OR'd together to select the
*                        corresponding blocks to enable.
*                        DIGI_PMON_WORK_ORDER_LINEOTN\n
*                        DIGI_PMON_WORK_ORDER_SYSOTN\n
*                        DIGI_PMON_WORK_ORDER_COREOTN_ERR_BLK_COUNTS\n
*                        DIGI_PMON_WORK_ORDER_CBRC\n
*                        DIGI_PMON_WORK_ORDER_MGMT Port\n
*                        DIGI_PMON_WORK_ORDER_ENET_LINE\n
*                        DIGI_PMON_WORK_ORDER_ENET_SYS\n
*                        DIGI_PMON_WORK_ORDER_MAPOTN\n
*                        DIGI_PMON_WORK_ORDER_MCPB\n
*                        DIGI_PMON_WORK_ORDER_SIFD\n
*
*  disable_hw_block_msk - Bitmask of hardware block(s) to disable PMON for. The
*                         following contants can be OR'd together to select the
*                         corresponding blocks to disable.
*                         DIGI_PMON_WORK_ORDER_LINEOTN\n
*                         DIGI_PMON_WORK_ORDER_SYSOTN\n
*                         DIGI_PMON_WORK_ORDER_COREOTN_ERR_BLK_COUNTS\n
*                         DIGI_PMON_WORK_ORDER_CBRC\n
*                         DIGI_PMON_WORK_ORDER_MGMT Port\n
*                         DIGI_PMON_WORK_ORDER_ENET_LINE\n
*                         DIGI_PMON_WORK_ORDER_ENET_SYS\n
*                         DIGI_PMON_WORK_ORDER_MAPOTN\n
*                         DIGI_PMON_WORK_ORDER_MCPB\n
*                         DIGI_PMON_WORK_ORDER_SIFD\n
*******************************************************************************/
typedef struct digi_pmon_work_order_msg_t
{
    UINT32 enable_hw_block_msk;
    UINT32 disable_hw_block_msk;
} digi_pmon_work_order_msg_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_ddeg_chnl_bitmsk_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel bitmask for configuring dDEG PMON.
*
* ELEMENTS:
*  chnl_bitmsk - Bitmask of channels to configure
*                chnl_bitmsk[0] - Channels [31:0]\n
*                chnl_bitmsk[1] - Channels [63:32]\n
*                ..\n
*                chnl_bitmsk[7] - Channels [255:224]
*******************************************************************************/
typedef struct digi_pmon_ddeg_chnl_bitmsk_t
{
    UINT32 chnl_bitmsk[8];
} digi_pmon_ddeg_chnl_bitmsk_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_ddeg_ints_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   dDEG interrupts retrieved by firmware.
*
* ELEMENTS:
*  ddeg_mode   - dDEG mode of interrupts read
*  int_type    - Interrupt type retrieved:\n
*                0 -- Interrupt value (V-bit). Indicates whether the dDEG
*                     interrupt is asserted for the current channel.\n
*                1 --  Interrupt status (I-bit). Indicates whether there has
*                    been a change of state in the dDEG interrupt for the
*                    current channel. The I-bit is cleared after reading.
*  lineotn_int - Interrupt value/status.
*                dDEG interrupt value (V-bit) or status (I-bit).
*                I-bits are cleared after reading.
*                Each bit represents a channel in that subsystem, e.g.
*                Bit 0 in lineotn_int indicates the interrupt value for channel
*                0 in the LINEOTN subsystem.
*                The number of channels in each block are as follows:\n
*                 LINEOTN (12)\n
*                 SYSOTN (1)\n
*                 COREOTN Stage 1 PM (12)\n
*                 COREOTN Stage 1 TCM (72)\n
*                 COREOTN Stage 2 PM (96)\n
*                 COREOTN Stage 2 TCM (256)\n
*                 COREOTN Stage N PM (1)\n
*                 COREOTN Stage N TCM (6)\n
*                 COREOTN Stage 3a / 4 PM (96)\n
*                 COREOTN Stage 3a / 4 TCM (256)
*  sysotn_int - See lineotn_int description for description
*  coreotn_stg_1_tcm_int - See lineotn_int description for description
*  coreotn_stg_1_pm_int - See lineotn_int description for description  
*  coreotn_stg_2_tcm_int - See lineotn_int description for description
*  coreotn_stg_2_pm_int - See lineotn_int description for description
*  coreotn_stg_n_tcm_int - See lineotn_int description for description  
*  coreotn_stg_n_pm_int - See lineotn_int description for description   
*  coreotn_stg_3a_4_tcm_int - See lineotn_int description for description
*  coreotn_stg_3a_4_pm_int - See lineotn_int description for description
*******************************************************************************/
typedef struct digi_pmon_ddeg_ints_t
{
    digi_pmon_ddeg_mode_t ddeg_mode;
    UINT8  int_type;
    UINT32 lineotn_int;
    UINT32 sysotn_int;
    UINT32 coreotn_stg_1_tcm_int[COREOTN_STG1_TCM_INT_REG_MAX_NUM];
    UINT32 coreotn_stg_1_pm_int;
    UINT32 coreotn_stg_2_tcm_int[COREOTN_STG2_TCM_INT_REG_MAX_NUM];
    UINT32 coreotn_stg_2_pm_int[COREOTN_STG2_PM_INT_REG_MAX_NUM];
    UINT32 coreotn_stg_n_tcm_int;
    UINT32 coreotn_stg_n_pm_int;
    /* Stage 3a and 4 have the same number of channels */
    UINT32 coreotn_stg_3a_4_tcm_int[COREOTN_STG3A_TCM_INT_REG_MAX_NUM];
    UINT32 coreotn_stg_3a_4_pm_int[COREOTN_STG3A_PM_INT_REG_MAX_NUM];
} digi_pmon_ddeg_ints_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_ddeg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Pointer to dDEG enh and bursty interrupts and status.
*
* ELEMENTS:
*   i_ptr      - Pointer to PM ddeg interrupts.
*   v_ptr      - Pointer to PM ddeg status.
*   
*******************************************************************************/
typedef struct digi_pmon_ddeg_t
{
    digi_pmon_ddeg_ints_t    i_bit;
    digi_pmon_ddeg_ints_t    v_bit;
} digi_pmon_ddeg_t;


/*******************************************************************************
* STRUCTURE: digi_pmon_ddeg_stage_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Pointer to dDEG enh and bursty interrupts and status for a specific ODU
*   level.
*
* ELEMENTS:
*   i_ptr      - Pointer to PM ddeg interrupts.
*   v_ptr      - Pointer to PM ddeg status.
*   tcm_i_ptr  - Pointer to TCM ddeg interrupts.
*   tcm_v_ptr  - Pointer to TCM ddeg status.
*   
*******************************************************************************/
typedef struct digi_pmon_ddeg_stage_t
{
    pmc_bitarray_t           *i_ptr;
    pmc_bitarray_t           *v_ptr;
    pmc_bitarray_t           *tcm_i_ptr;
    pmc_bitarray_t           *tcm_v_ptr;
} digi_pmon_ddeg_stage_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_ddeg_param_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   dDEG parameters retrieved by firmware.
*
* ELEMENTS:
*  ddeg_mode   - dDEG mode of parameters read
*  ddeg_hw_block - Hardware block read
*                  (maximum number of channels):
*                  LINEOTN (12)
*                  SYSOTN (1)
*                  COREOTN Stage 1 PM (12)
*                  COREOTN Stage 1 TCM (72)
*                  COREOTN Stage 2 PM (96)
*                  COREOTN Stage 2 TCM (256)
*                  COREOTN Stage N PM (1)
*                  COREOTN Stage N TCM (6)
*                  COREOTN Stage 3a / 4 PM (96)
*                  COREOTN Stage 3a / 4 TCM (256)
*  chnl - Channel read in the specified hardware block.
*         The first channel starts from channel 0.
*  mon_intvl - Monitoring interval. If ddeg_mode = 0
*              (Enhanced dDEG), the following values are valid:\n
*                0: 10ms\n
*                1: 100ms\n
*                2: 1000ms\n
*              If ddeg_mode = 1 (Bursty dDEG), this parameter
*              is the value for DEGM (number of 1-second
*              interval). Values of 2-10 are valid.
*  mon_coreotn_stg_4 - TRUE: Monitor COREOTN Stage 4\n
*                      FALSE: Monitor COREOTN Stages 1, 2, N
*                             and 3a
*  set_threshold - Threshold of Near End Block Errors for each
*                  1-second monitoring interval used to declare
*                  a bad interval. An interval is declared bad if
*                  the number of errors >= set_threshold.
*  clear_threshold - Threshold of Near End Block Errors for each
*                    1-second monitoring interval used to declare
*                    a good interval. An interval is declared good
*                    if the number of errors < clear_threshold.
*                    clear_threshold must be less than set_threshold.
*                    Only valid if ddeg_mode = 0 (Enhanced dDEG).
*                    Bursty (G.798) dDEG always uses
*                    clear_threshold = (set_threshold - 1)
*******************************************************************************/
typedef struct digi_pmon_ddeg_param_t
{
    digi_pmon_ddeg_mode_t ddeg_mode;
    digi_pmon_ddeg_hw_block_t ddeg_hw_block;
    UINT8 chnl;
    UINT8 mon_intvl;
    BOOL mon_coreotn_stg_4;
    UINT32 set_threshold;
    UINT32 clear_threshold;
} digi_pmon_ddeg_param_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_ddeg_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   dDEG state machine variables used by firmware.
*
* ELEMENTS:
*  ddeg_hw_block       - Hardware block read
*                        (maximum number of channels):
*                        LINEOTN (12)\n
*                        SYSOTN (1)\n
*                        COREOTN Stage 1 PM (12)\n
*                        COREOTN Stage 1 TCM (72)\n
*                        COREOTN Stage 2 PM (96)\n
*                        COREOTN Stage 2 TCM (256)\n
*                        COREOTN Stage N PM (1)\n
*                        COREOTN Stage N TCM (6)\n
*                        COREOTN Stage 3a / 4 PM (96)\n
*                        COREOTN Stage 3a / 4 TCM (256)
*  chnl                - Channel read in the specified hardware block.
*                        The first channel starts from channel 0.
*  ddeg_enh_10ms_ticks - Number of ticks of the 10ms clock for
*                        enhanced dDEG
*  ddeg_enh_berm_state - State of enhanced dDEG BERM state machine.\n
*                          PMON_DDEG_CLEAR_RESET  = 0\n 
*                          PMON_DDEG_CLEAR_ACCUM  = 1\n 
*                          PMON_DDEG_SET_LOADING  = 2\n 
*                          PMON_DDEG_SET_ACCUM    = 3 
*  ddeg_enh_nebc       - Accumulated NEBC counts for Enhanced dDEG
*  ddeg_10ms_ticks     - Number of ticks of the 10ms clock for bursty
*                        (G.798) dDEG. Need 16-bits since the count goes to 1000 (10s).
*  ddeg_berm_state     - State of Bursty (G.798) BERM state machine.\n
*                          PMON_DDEG_CLEAR_RESET  = 0\n 
*                          PMON_DDEG_CLEAR_ACCUM  = 1\n 
*                          PMON_DDEG_SET_LOADING  = 2\n 
*                          PMON_DDEG_SET_ACCUM    = 3 
*  ddeg_nebc           - Accumulated NEBC counts for Bursty (G.798) dDEG
* 
*  ddeg_enh_set_thres             - enhanced dDEG set threshold
*  ddeg_enh_clear_thres           - enhanced dDEG clear threshold
*  ddeg_set_thres                 - bursty dDEG set threshold
*  ddeg_clear_thres               - bursty dDEG clear threshold
*  ddeg_enh_mon_intvl_10ms_ticks  - enhanced dDEG monitor interval
*  ddeg_mon_intvl_10ms_ticks      - bursty dDEG monitor interval
*  ddeg_enh_int_en                - enhanced dDEG interrupt enable
*  ddeg_enh_enable                - enhanced dDEG algorithm enable
*  ddeg_int_en                    - bursty dDEG interrupt enable
*  ddeg_enable                    - bursty dDEG algorithm enable
* 
*******************************************************************************/
typedef struct digi_pmon_ddeg_state_t
{
    digi_pmon_ddeg_hw_block_t ddeg_hw_block;
    UINT8   chnl;
    UINT16  ddeg_enh_10ms_ticks;  
    UINT8   ddeg_enh_berm_state;  
    UINT32  ddeg_enh_nebc;        
    UINT16  ddeg_10ms_ticks;      
    UINT8   ddeg_berm_state;      
    UINT32  ddeg_nebc;  

    UINT32  ddeg_enh_set_thres;
    UINT32  ddeg_enh_clear_thres;
    UINT32  ddeg_set_thres;
    UINT32  ddeg_clear_thres;
    UINT16  ddeg_enh_mon_intvl_10ms_ticks;
    UINT16  ddeg_mon_intvl_10ms_ticks;
    BOOL8   ddeg_enh_int_en;
    BOOL8   ddeg_enh_enable;
    BOOL8   ddeg_int_en;
    BOOL8   ddeg_enable;
} digi_pmon_ddeg_state_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_ddeg_enable_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   dDEG enable/interrupt enable status retrieved by firmware.
*
* ELEMENTS:
*  ddeg_mode   - dDEG mode of parameters read
*  ddeg_hw_block - Hardware block read
*                  (maximum number of channels):\n
*                  LINEOTN (12)\n
*                  SYSOTN (1)\n
*                  COREOTN Stage 1 PM (12)\n
*                  COREOTN Stage 1 TCM (72)\n
*                  COREOTN Stage 2 PM (96)\n
*                  COREOTN Stage 2 TCM (256)\n
*                  COREOTN Stage N PM (1)\n
*                  COREOTN Stage N TCM (6)\n
*                  COREOTN Stage 3a / 4 PM (96)\n
*                  COREOTN Stage 3a / 4 TCM (256)
*  chnl_en[]     - Enable status for each channel in the hardware block.
*                  A '1' in a bit position indicates that the channel is
*                  enabled for dDEG for the mode specified by ddeg_mode.\n
*                  chnl_en[0] - Channels [31:0]\n
*                  chnl_en[1] - Channels [63:32]\n
*                  ...\n
*                  chnl_en[7] - Channels [255:224]
*  chnl_int_en[] - Interrupt Enable status for each channel in the hardware
*                  block. A '1' in a bit position indicates that the channel
*                  is enabled for interrupt propagation for the mode
*                  specified by ddeg_mode.\n
*                  chnl_int_en[0] - Channels [31:0]\n
*                  chnl_int_en[1] - Channels [63:32]\n
*                  ...\n
*                  chnl_int_en[7] - Channels [255:224]
*******************************************************************************/
typedef struct digi_pmon_ddeg_enable_status_t
{
    digi_pmon_ddeg_mode_t ddeg_mode;
    digi_pmon_ddeg_hw_block_t ddeg_hw_block;
    UINT32 chnl_en[8];
    UINT32 chnl_int_en[8];
} digi_pmon_ddeg_enable_status_t;

/*******************************************************************************
* STRUCTURE: pmcfw_stats_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   firmware statistics.
*
* ELEMENTS:
*    sum - sum of all values (avg is sum/num)
*    num - number of samples
*    min - minimum value seen
*    max - maximum value seen
*******************************************************************************/
typedef struct pmcfw_stats_t 
{
    UINT64 sum;
    UINT32 num;
    UINT32 min;
    UINT32 max;
} pmcfw_stats_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_fw_sm_dma_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PMON parameters retrieved by firmware.
*
* ELEMENTS:
*    pmon_trig_mode      - PMON Collection Trigger.\n
*                          0: manual trigger.\n
*                          1: automatic trigger.
*    pmon_sync_en          0: Automatic PMON trigger not synchronized to
*                             PMON_SYNC_I pin.\n
*                          1: Automatic PMON trigger synchronized to external
*                             PMON_SYNC_I pin.
*    trig_period           - Trigger period in miliseconds.
*    pmon_xfer_enable      - Bitmask of hardware block that is enabled for
*                            PMON. A '1' in a bit location indicates that
*                            the PMON is enabled for that block. The
*                            bitmask definition is as follows:\n
*                             0: LINEOTN\n
*                             1: SYSOTN\n
*                             2: CoreOTN Errored Block Counts\n
*                             3: CoreOTN OHP1\n
*                             4: CoreOTN OHP2\n
*                             5: CoreOTN OHP3\n
*                             6: CoreOTN RCP_RI\n
*                             7: CoreOTN RCP_DI\n
*                             8: CBRC\n
*                             9: MGMT Port\n
*                             10: ENET_LINE\n
*                             11: ENET_SYS\n
*                             12: MAPOTN\n
*                             13: MCPB\n
*                             14: SIFD
*    rcp_enable             - 0: PMON collection through the RCP is disabled.\n
*                             1: PMON collection through the RCP is enabled.
*    ddeg_enable            - 0: Both Enhanced and Bursty dDEG are disabled
*                                for all channels.\n
*                             1: Enhanced or Bursty dDEG is enabled for at
*                                least one channel.
*    hw_ddeg_enable           -  0: Hardware dDEG LCLK configuration disabled\n
*                                1: Hardware dDEG LCLK configuration enabled
*    lclk_ ddeg_cnt           - Number of times that the latch clock for dDEG
*                               has triggered.
*    lclk_ddeg_intr_overrun   - Number of times that the dDEG task has not
*                               completed before the next LCLK pulse arrived.
*    lclk_rcp_cnt             - Number of times that the latch clock for
*                               RCP has triggered.
*    lclk_dma_xfer_cnt        - Number of times that the latch clock for
*                               PMON DMA transfer has triggered.
*    lineotn_fec_intr_cnt     - Number of times that the LINEOTN FEC
*                               interrupt has asserted.
*    lineotn_intr_overrun     - Number of times that the LINEOTN FEC transfer
*                               task has not completed before the next LCLK pulse
*                               arrived.
*    lclk_otn_intr_overrun    - Number of times that the OTN transfer task
*                               has not completed before the next LCLK pulse
*                               arrived.
*    lclk_misc_intr_overrun   - Number of times that the non-OTN transfer task
*                               has not completed before the next LCLK pulse
*                               arrived.
*    lclk_otn_buf_conflict    - Number of times that host software is still
*                               holding OTN buffer when LCLK triggered
*    lclk_misc_buf_conflict   - Number of times that host software is still
*                               holding non-OTN buffer when LCLK triggered
*    gdma_busy_num            - Number of times that GDMA is busy when
*                               firmware tries to start a transfer
*    rcp_dma_err_num          - Number of times that RCP module encountered
*                               a DMA error when attempting to transfer
*                               data to RCP
*    lineotn_fec_dma_complete - Number of cycles completed by the DMA channel
*                               that is transferring LINEOTN FEC PMON
*    otn_dma_complete         - Number of cycles completed by the DMA channel
*                               that is transferring OTN PMON
*    misc_dma_complete        - Number of cycles completed by the DMA channel
*                               that is transferring non-OTN PMON
*    rcp_dma_complete         - Number of cycles completed by the DMA channel
*                               that is transferring PMON to RCP
*    pmon_fec_update_delay    - delay in firmware waiting for FEC_DONE signal
* 
*******************************************************************************/
typedef struct digi_pmon_fw_sm_dma_status_t
{
    UINT8 pmon_trig_mode;
    UINT8 pmon_sync_en;
    UINT16 trig_period;
    UINT32 pmon_xfer_enable;
    UINT8 rcp_enable;
    UINT8 ddeg_enable;
    UINT8 hw_ddeg_enable;
    UINT32 lclk_ddeg_cnt;
    UINT32 lclk_ddeg_intr_overrun;
    UINT32 lclk_rcp_cnt;
    UINT32 lclk_dma_xfer_cnt;
    UINT32 lineotn_fec_intr_cnt;
    UINT32 lineotn_intr_overrun;
    UINT32 lclk_otn_intr_overrun;
    UINT32 lclk_misc_intr_overrun;
    UINT32 lclk_otn_buf_conflict;
    UINT32 lclk_misc_buf_conflict;
    UINT32 gdma_busy_num;
    UINT32 rcp_dma_err_num;
    UINT32 lineotn_fec_dma_complete;
    UINT32 otn_dma_complete;
    UINT32 misc_dma_complete;
    UINT32 rcp_dma_complete;
    UINT32 pmon_fec_update_delay;
    UINT32 ddeg_bursty_cnt;
    UINT32 ddeg_enh_cnt;
    UINT32 ddeg_blk_cnt;
    pmcfw_stats_t nebc_stats[1];
    pmcfw_stats_t cumulative_nebc_stats[1];
    UINT32 v_bit_set_count;
    UINT32 v_bit_clear_count;
    UINT32 cookie;
} digi_pmon_fw_sm_dma_status_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_chnl_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie. Used addition or removal of pmons.
*
*******************************************************************************/
typedef void digi_pmon_chnl_handle_t; /*!< Opaque reference to module handle */ 

/*******************************************************************************
* STRUCTURE: digi_pmon_enet_mib_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Ethernet MIB RFC-compliant counters for a single channel.
*
* ELEMENTS:
*   a_frames_tx_ok_lsb                      - aFramesTxOK (LSB 32 bits)
*   a_frames_tx_ok_msb                      - aFramesTxOK (MSB 8 bits)
*   a_frames_rx_ok_lsb                      - aFramesRxOK (LSB 32 bits)
*   a_frames_rx_ok_msb                      - aFramesRxOK (MSB 8 bits)
*   a_frame_check_sequence_errors_lsb       - aFrameCheckSequenceErrors (LSB 32 bits)
*   a_frame_check_sequence_errors_msb       - aFrameCheckSequenceErrors (MSB 8 bits)
*   a_alignment_errors_lsb                  - aAlignmentErrors (LSB 32 bits)
*   a_alignment_errors_msb                  - aAlignmentErrors (MSB 8 bits)
*   a_pause_mac_ctrl_frames_tx_lsb          - aPAUSEMACCtrlFramesTx (LSB 32 bits)
*   a_pause_mac_ctrl_frames_tx_msb          - aPAUSEMACCtrlFramesTx (MSB 8 bits)
*   a_pause_mac_ctrl_frames_rx_lsb          - aPAUSEMACCtrlFramesRx (LSB 32 bits)
*   a_pause_mac_ctrl_frames_rx_msb          - aPAUSEMACCtrlFramesRx (MSB 8 bits)
*   a_frame_too_long_errors_lsb             - aFrameTooLongErrors (LSB 32 bits)
*   a_frame_too_long_errors_msb             - aFrameTooLongErrors (MSB 8 bits)
*   a_in_range_length_errors_lsb            - aInRangeLengthErrors (LSB 32 bits)
*   a_in_range_length_errors_msb            - aInRangeLengthErrors (MSB 8 bits)
*   vlan_tx_ok_lsb                          - VLANTxOK (LSB 32 bits)
*   vlan_tx_ok_msb                          - VLANTxOK (MSB 8 bits)
*   vlan_rx_ok_lsb                          - VLANRxOK (LSB 32 bits)
*   vlan_rx_ok_msb                          - VLANRxOK (MSB 8 bits)
*   if_out_octs_lsb                         - ifOutOcts (LSB 32 bits)
*   if_out_octs_msb                         - ifOutOcts (MSB 8 bits)
*   if_in_octs_lsb                          - ifInOcts (LSB 32 bits)
*   if_in_octs_msb                          - ifInOcts (MSB 8 bits)
*   if_in_ucast_pkts_lsb                    - ifInUcastPkts (LSB 32 bits)
*   if_in_ucast_pkts_msb                    - ifInUcastPkts (MSB 8 bits)
*   if_in_multicast_pkts_lsb                - ifInMulticastPkts (LSB 32 bits)
*   if_in_multicast_pkts_msb                - ifInMulticastPkts (MSB 8 bits)
*   if_in_broadcast_pkts_lsb                - ifInBroadcastPkts (LSB 32 bits)
*   if_in_broadcast_pkts_msb                - ifInBroadcastPkts (MSB 8 bits)
*   if_out_errors_lsb                       - ifOutErrors (LSB 32 bits)
*   if_out_errors_msb                       - ifOutErrors (MSB 8 bits)
*   reserved_0                              - Reserved to align with device register map.
*   reserved_1                              - Reserved to align with device register map.
*   if_out_ucast_pkts_lsb                   - ifOutUcastPkts (LSB 32 bits)
*   if_out_ucast_pkts_msb                   - ifOutUcastPkts (MSB 8 bits)
*   if_out_multicast_pkts_lsb               - ifOutMulticastPkts (LSB 32 bits)
*   if_out_multicast_pkts_msb               - ifOutMulticastPkts (MSB 8 bits)
*   if_out_broadcast_pkts_lsb               - ifOutBroadcastPkts (LSB 32 bits)
*   if_out_broadcast_pkts_msb               - ifOutBroadcastPkts (MSB 8 bits)
*   ether_stats_drop_events_lsb             - etherStatsDropEvents (LSB 32 bits)
*   ether_stats_drop_events_msb             - etherStatsDropEvents (MSB 8 bits)
*   ether_stats_octs_lsb                    - etherStatsOcts (LSB 32 bits)
*   ether_stats_octs_msb                    - etherStatsOcts (MSB 8 bits)
*   ether_stats_pkts_lsb                    - etherStatsPkts (LSB 32 bits)
*   ether_stats_pkts_msb                    - etherStatsPkts (MSB 8 bits)
*   ether_stats_undersize_pkts_lsb          - etherStatsUndersizePkts (LSB 32 bits)
*   ether_stats_undersize_pkts_msb          - etherStatsUndersizePkts (MSB 8 bits)
*   ether_stats_pkts_64_octs_lsb            - etherStatsPkts64Octs (LSB 32 bits)
*   ether_stats_pkts_64_octs_msb            - etherStatsPkts64Octs (MSB 8 bits)
*   ether_stats_pkts_65_to_127_octs_lsb     - etherStatsPkts65to127Octs (LSB 32 bits)
*   ether_stats_pkts_65_to_127_octs_msb     - etherStatsPkts65to127Octs (MSB 8 bits)
*   ether_stats_pkts_128_to_255_octs_lsb    - etherStatsPkts128to255Octs (LSB 32 bits)
*   ether_stats_pkts_128_to_255_octs_msb    - etherStatsPkts128to255Octs (MSB 8 bits)
*   ether_stats_pkts_256_to_511_octs_lsb    - etherStatsPkts256to511Octs (LSB 32 bits)
*   ether_stats_pkts_256_to_511_octs_msb    - etherStatsPkts256to511Octs (MSB 8 bits)
*   ether_stats_pkts_512_to_1023_octs_lsb   - etherStatsPkts512to1023Octs (LSB 32 bits)
*   ether_stats_pkts_512_to_1023_octs_msb   - etherStatsPkts512to1023Octs (MSB 8 bits)
*   ether_stats_pkts_1024_to_1518_octs_lsb  - etherStatsPkts1024to1518Octs (LSB 32 bits)
*   ether_stats_pkts_1024_to_1518_octs_msb  - etherStatsPkts1024to1518Octs (MSB 8 bits)
*   ether_stats_pkts_1519_to_max_octs_lsb   - etherStatsPkts1519toMaxOcts (LSB 32 bits)
*   ether_stats_pkts_1519_to_max_octs_msb   - etherStatsPkts1519toMaxOcts (MSB 8 bits)
*   ether_stats_oversize_pkts_lsb           - etherStatsOversizePkts (LSB 32 bits)
*   ether_stats_oversize_pkts_msb           - etherStatsOversizePkts (MSB 8 bits)
*   ether_stats_jabber_lsb                  - etherStatsJabber (LSB 32 bits)
*   ether_stats_jabber_msb                  - etherStatsJabber (MSB 8 bits)
*   ether_stats_fragments_lsb               - etherStatsFragments (LSB 32 bits)
*   ether_stats_fragments_msb               - etherStatsFragments (MSB 8 bits)
*   if_in_errors_lsb                        - ifInErrors (LSB 32 bits)
*   if_in_errors_msb                        - ifInErrors (MSB 8 bits)
*   reserved_2                              - Reserved to align with device register map.
*   reserved_3                              - Reserved to align with device register map.
*   a_mac_control_frames_rx_lsb             - aMACControlFramesRx (LSB 32 bits)
*   a_mac_control_frames_rx_msb             - aMACControlFramesRx (MSB 8 bits)
*
*******************************************************************************/
typedef struct digi_pmon_enet_mib_t
{
    UINT32 a_frames_tx_ok_lsb;
    UINT32 a_frames_tx_ok_msb;
    UINT32 a_frames_rx_ok_lsb;
    UINT32 a_frames_rx_ok_msb;
    UINT32 a_frame_check_sequence_errors_lsb;
    UINT32 a_frame_check_sequence_errors_msb;
    UINT32 a_alignment_errors_lsb;
    UINT32 a_alignment_errors_msb;
    UINT32 a_pause_mac_ctrl_frames_tx_lsb;
    UINT32 a_pause_mac_ctrl_frames_tx_msb;
    UINT32 a_pause_mac_ctrl_frames_rx_lsb;
    UINT32 a_pause_mac_ctrl_frames_rx_msb;
    UINT32 a_frame_too_long_errors_lsb;
    UINT32 a_frame_too_long_errors_msb;
    UINT32 a_in_range_length_errors_lsb;
    UINT32 a_in_range_length_errors_msb;
    UINT32 vlan_tx_ok_lsb;
    UINT32 vlan_tx_ok_msb;
    UINT32 vlan_rx_ok_lsb;
    UINT32 vlan_rx_ok_msb;
    UINT32 if_out_octs_lsb;
    UINT32 if_out_octs_msb;
    UINT32 if_in_octs_lsb;
    UINT32 if_in_octs_msb;
    UINT32 if_in_ucast_pkts_lsb;
    UINT32 if_in_ucast_pkts_msb;
    UINT32 if_in_multicast_pkts_lsb;
    UINT32 if_in_multicast_pkts_msb;
    UINT32 if_in_broadcast_pkts_lsb;
    UINT32 if_in_broadcast_pkts_msb;
    UINT32 if_out_errors_lsb;
    UINT32 if_out_errors_msb;
    UINT32 reserved_0;
    UINT32 reserved_1;
    UINT32 if_out_ucast_pkts_lsb;
    UINT32 if_out_ucast_pkts_msb;
    UINT32 if_out_multicast_pkts_lsb;
    UINT32 if_out_multicast_pkts_msb;
    UINT32 if_out_broadcast_pkts_lsb;
    UINT32 if_out_broadcast_pkts_msb;
    UINT32 ether_stats_drop_events_lsb;
    UINT32 ether_stats_drop_events_msb;
    UINT32 ether_stats_octs_lsb;
    UINT32 ether_stats_octs_msb;
    UINT32 ether_stats_pkts_lsb;
    UINT32 ether_stats_pkts_msb;
    UINT32 ether_stats_undersize_pkts_lsb;
    UINT32 ether_stats_undersize_pkts_msb;
    UINT32 ether_stats_pkts_64_octs_lsb;
    UINT32 ether_stats_pkts_64_octs_msb;
    UINT32 ether_stats_pkts_65_to_127_octs_lsb;
    UINT32 ether_stats_pkts_65_to_127_octs_msb;
    UINT32 ether_stats_pkts_128_to_255_octs_lsb;
    UINT32 ether_stats_pkts_128_to_255_octs_msb;
    UINT32 ether_stats_pkts_256_to_511_octs_lsb;
    UINT32 ether_stats_pkts_256_to_511_octs_msb;
    UINT32 ether_stats_pkts_512_to_1023_octs_lsb;
    UINT32 ether_stats_pkts_512_to_1023_octs_msb;
    UINT32 ether_stats_pkts_1024_to_1518_octs_lsb;
    UINT32 ether_stats_pkts_1024_to_1518_octs_msb;
    UINT32 ether_stats_pkts_1519_to_max_octs_lsb;
    UINT32 ether_stats_pkts_1519_to_max_octs_msb;
    UINT32 ether_stats_oversize_pkts_lsb;
    UINT32 ether_stats_oversize_pkts_msb;
    UINT32 ether_stats_jabber_lsb;
    UINT32 ether_stats_jabber_msb;
    UINT32 ether_stats_fragments_lsb;
    UINT32 ether_stats_fragments_msb;
    UINT32 if_in_errors_lsb;
    UINT32 if_in_errors_msb;
    UINT32 reserved_2;
    UINT32 reserved_3;
    UINT32 a_mac_control_frames_rx_lsb;
    UINT32 a_mac_control_frames_rx_msb;
} digi_pmon_enet_mib_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_enet_errors_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Ethernet error counters for a single channel.
*   The description of each counter is the register field name.
*   Note that 100G counters are only valid on link 0 and 40G
*   counters are only valid on links 0, 4 & 8.
*
*   Note that ETRANS/EPMM CBR_40G/100G define the Ethernet to OTN datapath as
*   Tx path, and the opposite direction as Rx.
*   This defintion is exactly contrary to other ENET blocks.
*
* ELEMENTS:
*   block_error_count_40_bit_lsb                             - PMON_BLOCK_ERR_CNT (LSB 32 bits)
*   block_error_count_40_bit_msb                             - PMON_BLOCK_ERR_CNT (MSB 8 bits)
*   per_lane_bip_count                                       - LANE_BIP_ERR_CNT
*   etrans_tx_40_gbe_to_odu3_ttt_link_pcs_ber_count          - TX_PCS_BER_CTR
*   etrans_tx_40_gbe_to_odu3_ttt_pcs_blk_error_count         - TX_PCS_BLK_ERR_CTR
*   etrans_tx_40_gbe_to_odu3_ttt_lln_pcs_ber_count           - TX_PCS_BIP_ERR_CTR
*   etrans_rx_40_gbe_to_odu3_ttt_link_pcs_ber_count          - RX_1027B_BER_CTR
*   etrans_rx_40_gbe_to_odu3_ttt_pcs_blk_error_count         - RX_PCS_BLK_ERR_CTR
*   etrans_rx_40_gbe_to_odu3_ttt_lln_otn_bip_error_count     - RX_OTN_BIP_ERR_CTR
*   etrans_tx_100_gbe_to_odu4_ttt_link_pcs_ber_count         - TX_PCS_BER_CTR
*   etrans_tx_100_gbe_to_odu4_ttt_pcs_blk_error_count        - TX_PCS_BLK_ERR_CTR
*   etrans_tx_100_gbe_to_odu4_ttt_lln_pcs_ber_count          - TX_PCS_BIP_ERR_CTR
*   etrans_rx_100_gbe_to_odu4_ttt_link_pcs_ber_count         - RX_OTN_BER_CTR
*   etrans_rx_100_gbe_to_odu4_ttt_pcs_blk_error_count        - RX_PCS_BLK_ERR_CTR
*   etrans_rx_100_gbe_to_odu4_ttt_otn_bip_error_count        - RX_OTN_BIP_ERR_CTR
*   etrans_rx_10_gbe_to_odu2_gfp_os_blk_type_error_count     - BLK_TYPE_ERR_CNT
*   etrans_rx_10_gbe_to_odu2_gfp_os_hi_ber_count             - HI_BER_CNT
*   etrans_rx_10_gbe_to_odu2_gfp_os_blk_error_count          - RX_BLK_ERR_CNT
*   etrans_rx_10_gbe_to_odu2_gfp_os_os_discard_count_lsb     - RX_OS_DSCRCD_CNT (LSB 32 bits)
*   etrans_rx_10_gbe_to_odu2_gfp_os_os_discard_count_msb     - RX_OS_DSCRCD_CNT (MSB 8 bits)
*   etrans_rx_10_gbe_to_odu2_gfp_os_data_byte_count_lsb      - RX_DATA_BYTE_CNT (LSB 32 bits)
*   etrans_rx_10_gbe_to_odu2_gfp_os_data_byte_count_msb      - RX_DATA_BYTE_CNT (MSB 8 bits)
*   etrans_rx_10_gbe_to_odu2_gfp_os_os_count_lsb             - RX_OS_CNT (LSB 32 bits)
*   etrans_rx_10_gbe_to_odu2_gfp_os_os_count_msb             - RX_OS_CNT (MSB 8 bits)
*   etrans_rx_10_gbe_to_odu2_gfp_os_packet_count_lsb         - RX_PKT_CNT (LSB 32 bits)
*   etrans_rx_10_gbe_to_odu2_gfp_os_packet_count_msb         - RX_PKT_CNT (MSB 8 bits)
*   etrans_tx_10_gbe_to_odu2_gfp_os_data_byte_count_lsb      - TX_DATA_BYTE_CNT (LSB 32 bits)
*   etrans_tx_10_gbe_to_odu2_gfp_os_data_byte_count_msb      - TX_DATA_BYTE_CNT (MSB 8 bits)
*   etrans_tx_10_gbe_to_odu2_gfp_os_os_count_lsb             - TX_OS_CNT (LSB 32 bits)
*   etrans_tx_10_gbe_to_odu2_gfp_os_os_count_msb             - TX_OS_CNT (MSB 8 bits)
*   etrans_tx_10_gbe_to_odu2_gfp_os_packet_count_lsb         - TX_PKT_CNT (LSB 32 bits)
*   etrans_tx_10_gbe_to_odu2_gfp_os_packet_count_msb         - TX_PKT_CNT (MSB 8 bits)
*   epmm_tx_40_gbe_to_odu3_ttt_link_pcs_ber_count            - TX_PCS_BER_CTR
*   epmm_tx_40_gbe_to_odu3_ttt_pcs_blk_error_count           - TX_PCS_BLK_ERR_CTR
*   epmm_tx_40_gbe_to_odu3_ttt_lln_pcs_ber_count             - TX_PCS_BIP_ERR_CTR
*   epmm_rx_40_gbe_to_odu3_ttt_link_pcs_ber_count            - RX_1027B_BER_CTR
*   epmm_rx_40_gbe_to_odu3_ttt_pcs_blk_error_count           - RX_PCS_BLK_ERR_CTR
*   epmm_rx_40_gbe_to_odu3_ttt_lln_otn_bip_error_count       - RX_OTN_BIP_ERR_CTR
*   epmm_tx_100_gbe_to_odu4_ttt_link_pcs_ber_count           - TX_PCS_BER_CTR
*   epmm_tx_100_gbe_to_odu4_ttt_pcs_blk_error_count          - TX_PCS_BLK_ERR_CTR
*   epmm_tx_100_gbe_to_odu4_ttt_lln_pcs_ber_count            - TX_PCS_BIP_ERR_CTR
*   epmm_rx_100_gbe_to_odu4_ttt_link_pcs_ber_count           - RX_OTN_BER_CTR
*   epmm_rx_100_gbe_to_odu4_ttt_pcs_blk_error_count          - RX_PCS_BLK_ERR_CTR
*   epmm_rx_100_gbe_to_odu4_ttt_otn_bip_error_count          - RX_OTN_BIP_ERR_CTR
*   epmm_rx_10_gbe_to_odu2_gfp_os_blk_type_error_count       - BLK_TYPE_ERR_CNT
*   epmm_rx_10_gbe_to_odu2_gfp_os_hi_ber_count               - HI_BER_CNT
*   epmm_rx_10_gbe_to_odu2_gfp_os_blk_error_count            - RX_BLK_ERR_CNT
*   epmm_rx_10_gbe_to_odu2_gfp_os_os_discard_count_lsb       - RX_OS_DSCRCD_CNT (LSB 32 bits)
*   epmm_rx_10_gbe_to_odu2_gfp_os_os_discard_count_msb       - RX_OS_DSCRCD_CNT (MSB 8 bits)
*   epmm_rx_10_gbe_to_odu2_gfp_os_data_byte_count_lsb        - RX_DATA_BYTE_CNT (LSB 32 bits)
*   epmm_rx_10_gbe_to_odu2_gfp_os_data_byte_count_msb        - RX_DATA_BYTE_CNT (MSB 8 bits)
*   epmm_rx_10_gbe_to_odu2_gfp_os_os_count_lsb               - RX_OS_CNT (LSB 32 bits)
*   epmm_rx_10_gbe_to_odu2_gfp_os_os_count_msb               - RX_OS_CNT (MSB 8 bits)
*   epmm_rx_10_gbe_to_odu2_gfp_os_packet_count_lsb           - RX_PKT_CNT (LSB 32 bits)
*   epmm_rx_10_gbe_to_odu2_gfp_os_packet_count_msb           - RX_PKT_CNT (MSB 8 bits)
*   epmm_tx_10_gbe_to_odu2_gfp_os_data_byte_count_lsb        - TX_DATA_BYTE_CNT (LSB 32 bits)
*   epmm_tx_10_gbe_to_odu2_gfp_os_data_byte_count_msb        - TX_DATA_BYTE_CNT (MSB 8 bits)
*   epmm_tx_10_gbe_to_odu2_gfp_os_os_count_lsb               - TX_OS_CNT (LSB 32 bits)
*   epmm_tx_10_gbe_to_odu2_gfp_os_os_count_msb               - TX_OS_CNT (MSB 8 bits)
*   epmm_tx_10_gbe_to_odu2_gfp_os_packet_count_lsb           - TX_PKT_CNT (LSB 32 bits)
*   epmm_tx_10_gbe_to_odu2_gfp_os_packet_count_msb           - TX_PKT_CNT (MSB 8 bits)
*   cpb_packet_drop_count                                    - SYS_PMON_DRP_CNT
*   etime_reflect_path_good_pkts                             - REFL_PMON
*   cpb_dcpb_slv_dpi_packet_drop_count                       - CH_DROP_CNT_VAL
*******************************************************************************/
typedef struct digi_pmon_enet_errors_t
{
    UINT32 block_error_count_40_bit_lsb;
    UINT32 block_error_count_40_bit_msb;
    UINT32 per_lane_bip_count[20];

    UINT32 etrans_tx_40_gbe_to_odu3_ttt_link_pcs_ber_count;
    UINT32 etrans_tx_40_gbe_to_odu3_ttt_pcs_blk_error_count;
    UINT32 etrans_tx_40_gbe_to_odu3_ttt_lln_pcs_ber_count[4];
    UINT32 etrans_rx_40_gbe_to_odu3_ttt_link_pcs_ber_count;
    UINT32 etrans_rx_40_gbe_to_odu3_ttt_pcs_blk_error_count;
    UINT32 etrans_rx_40_gbe_to_odu3_ttt_lln_otn_bip_error_count[4];
    UINT32 etrans_tx_100_gbe_to_odu4_ttt_link_pcs_ber_count;
    UINT32 etrans_tx_100_gbe_to_odu4_ttt_pcs_blk_error_count;
    UINT32 etrans_tx_100_gbe_to_odu4_ttt_lln_pcs_ber_count[20];
    UINT32 etrans_rx_100_gbe_to_odu4_ttt_link_pcs_ber_count;
    UINT32 etrans_rx_100_gbe_to_odu4_ttt_pcs_blk_error_count;
    UINT32 etrans_rx_100_gbe_to_odu4_ttt_otn_bip_error_count[20];
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_blk_type_error_count;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_hi_ber_count;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_blk_error_count;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_os_discard_count_lsb;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_os_discard_count_msb;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_data_byte_count_lsb;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_data_byte_count_msb;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_os_count_lsb;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_os_count_msb;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_packet_count_lsb;
    UINT32 etrans_rx_10_gbe_to_odu2_gfp_os_packet_count_msb;
    UINT32 etrans_tx_10_gbe_to_odu2_gfp_os_data_byte_count_lsb;
    UINT32 etrans_tx_10_gbe_to_odu2_gfp_os_data_byte_count_msb;
    UINT32 etrans_tx_10_gbe_to_odu2_gfp_os_os_count_lsb;
    UINT32 etrans_tx_10_gbe_to_odu2_gfp_os_os_count_msb;
    UINT32 etrans_tx_10_gbe_to_odu2_gfp_os_packet_count_lsb;
    UINT32 etrans_tx_10_gbe_to_odu2_gfp_os_packet_count_msb;

    UINT32 epmm_tx_40_gbe_to_odu3_ttt_link_pcs_ber_count;
    UINT32 epmm_tx_40_gbe_to_odu3_ttt_pcs_blk_error_count;
    UINT32 epmm_tx_40_gbe_to_odu3_ttt_lln_pcs_ber_count[4];
    UINT32 epmm_rx_40_gbe_to_odu3_ttt_link_pcs_ber_count;
    UINT32 epmm_rx_40_gbe_to_odu3_ttt_pcs_blk_error_count;
    UINT32 epmm_rx_40_gbe_to_odu3_ttt_lln_otn_bip_error_count[4];
    UINT32 epmm_tx_100_gbe_to_odu4_ttt_link_pcs_ber_count;
    UINT32 epmm_tx_100_gbe_to_odu4_ttt_pcs_blk_error_count;
    UINT32 epmm_tx_100_gbe_to_odu4_ttt_lln_pcs_ber_count[20];
    UINT32 epmm_rx_100_gbe_to_odu4_ttt_link_pcs_ber_count;
    UINT32 epmm_rx_100_gbe_to_odu4_ttt_pcs_blk_error_count;
    UINT32 epmm_rx_100_gbe_to_odu4_ttt_otn_bip_error_count[20];
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_blk_type_error_count;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_hi_ber_count;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_blk_error_count;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_os_discard_count_lsb;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_os_discard_count_msb;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_data_byte_count_lsb;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_data_byte_count_msb;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_os_count_lsb;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_os_count_msb;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_packet_count_lsb;
    UINT32 epmm_rx_10_gbe_to_odu2_gfp_os_packet_count_msb;
    UINT32 epmm_tx_10_gbe_to_odu2_gfp_os_data_byte_count_lsb;
    UINT32 epmm_tx_10_gbe_to_odu2_gfp_os_data_byte_count_msb;
    UINT32 epmm_tx_10_gbe_to_odu2_gfp_os_os_count_lsb;
    UINT32 epmm_tx_10_gbe_to_odu2_gfp_os_os_count_msb;
    UINT32 epmm_tx_10_gbe_to_odu2_gfp_os_packet_count_lsb;
    UINT32 epmm_tx_10_gbe_to_odu2_gfp_os_packet_count_msb;

    UINT32 cpb_packet_drop_count;
    UINT32 etime_reflect_path_good_pkts;
    UINT32 cpb_dcpb_slv_dpi_packet_drop_count;
} digi_pmon_enet_errors_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_enet_classification_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Ethernet classification counters for a single channel.
*   The description of each counter is the register field name.
*
* ELEMENTS:
*   ingress_extract_path_packet_count       - EXT_PMON_PKT_CNT (includes all links)
*   ingress_extract_path_drop_count         - EXT_PMON_DRP_CNT
*   ingress_insert_path_packet_count        - INS_PMON_PKT_CNT
*   egress_extract_path_packet_count        - EXT_PMON_PKT_CNT (includes all links)
*   egress_insert_path_packet_count         - INS_PMON_PKT_CNT
*
*******************************************************************************/
typedef struct digi_pmon_enet_classification_t
{
    UINT32 ingress_extract_path_packet_count[64];
    UINT32 ingress_extract_path_drop_count;
    UINT32 ingress_insert_path_packet_count[2];

    UINT32 egress_extract_path_packet_count[64];
    UINT32 egress_insert_path_packet_count[2];
} digi_pmon_enet_classification_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_mapotn_counts_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining MAPOTN counters for a single channel.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*   gfp_sink_rx_byte_count_lsb                                 - RX_BYTE_CNT_LSB [31:0]
*   gfp_sink_rx_byte_count_msb                                 - RX_BYTE_CNT_MSB [8:0]
*   gfp_sink_rx_client_data_frame_count                        - RX_FRM_CNT [31:0]
*   gfp_sink_rx_idle_frame_count                               - IDLE_FRM_CNT [31:0]
*   gfp_sink_rx_ctrl_frame_count                               - CTRL_FRM_CNT [31:0]
*   gfp_sink_rx_chec_err_drop_frame_count                      - ERR_CHEC_CNT [31:0]
*   gfp_sink_rx_chec_single_corr_err_frame_count               - SINGL_ERR_CORR_CHEC_CNT [31:0]
*   gfp_sink_rx_unpack_drop_frame_count                        - UNPACK_DROP_FRM_CNT [31:0]
*   gfp_sink_n_fdis_thec                                       - ERR_THEC_CNT [31:0]
*   gfp_sink_rx_thec_single_corr_err_frame_count               - SINGL_ERR_CORR_TE_CNT [31:0]
*   gfp_sink_rx_filter_drop_frame_count                        - FILTER_DROP_FRM_CNT [31:0]
*   gfp_sink_n_fdis_ehec_exi                                   - DEXM_EHEC_CNT [31:0]
*   gfp_sink_p_fdis                                            - DUPM_FRM_CNT [31:0]
*   gfp_sink_rx_generic_filter_drop_frame_count                - DGEM_FRM_CNT [31:0]
*   gfp_sink_rx_csf_frame_count                                - CSF_FRM_CNT [31:0]
*   gfp_sink_p_fcs_error                                       - ERR_FCS_CNT [31:0]
*   gfp_sink_rx_pause_frame_drop_count                         - PAUSE_FRM_CNT [31:0]
*   gfp_sink_rx_pause_frame_frowarded_count                    - PAUSE_FRM_TR_CNT [31:0]
*   gfp_sink_rx_cpu_fifo_ovr_flw_drop_frame_count              - CPU_OVF_CNT [31:0]
*   gfp_sink_tx_frame_count                                    - EGRESS_FRM_CNT [31:0]
*   gfp_sink_rx_unpacked_frame_count                           - UNPACK_FRM_CNT [31:0]
*   gfp_sink_rx_min_frame_length_frm_drop_count                - MAXFL_CHK_CNT [31:0] 
*   gfp_sink_rx_max_frame_length_frm_drop_count                - MINFL_CHK_CNT [31:0]
*   reserved                                                   - Reserved to align with device register map. 
*   gfp_sink_rx_min_length_chk_fail_forwarded_frm_count        - FL_CHK_STAT_CNT [31:0]
*   gfp_src_tx_byte_0_count                                    - TX_BYTE_CNT0 [31:0] 
*   gfp_src_tx_byte_1_count                                    - TX_BYTE_CNT1 [7:0]  
*   gfp_src_tx_mgmt_frames                                     - MGMT_FRM_CNT [31:0]
*   gfp_src_tx_client_data_frame_count                         - TX_FRM_CNT [31:0]
*   gfp_src_tx_pause_frame_count                               - PAUSE_FRM_CNT [31:0]
*   gfp_src_tx_idle_frame_count                                - IDLE_FRM_CNT [31:0]
*   gfp_src_tx_err_frame_count                                 - ERR_FRM_CNT [31:0]
*   gfp_src_tx_min_frame_length_frm_count                      - TX_FRM_CNT_UNDERSIZE [31:0]
*   gfp_src_tx_max_frame_length_frm_count                      - TX_FRM_CNT_OVERSIZE [31:0]
*   cpb_dcpb_slv_dpi_packet_drop_count                         - CH_DROP_CNT_VAL (for DCPB)
*   cpb_ocpb_slv_dpi_packet_drop_count                         - CH_DROP_CNT_VAL (for ODUK_SW)                       
*******************************************************************************/
typedef struct digi_pmon_mapotn_counts_t
{
    /* RGFPF PMON counts */
    UINT32 gfp_sink_rx_byte_count_lsb;
    UINT32 gfp_sink_rx_byte_count_msb;
    UINT32 gfp_sink_rx_client_data_frame_count;
    UINT32 gfp_sink_rx_idle_frame_count;
    UINT32 gfp_sink_rx_ctrl_frame_count;
    UINT32 gfp_sink_rx_chec_err_drop_frame_count;
    UINT32 gfp_sink_rx_chec_single_corr_err_frame_count;
    UINT32 gfp_sink_rx_unpack_drop_frame_count;
    UINT32 gfp_sink_n_fdis_thec;
    UINT32 gfp_sink_rx_thec_single_corr_err_frame_count;
    UINT32 gfp_sink_rx_filter_drop_frame_count;
    UINT32 gfp_sink_n_fdis_ehec_exi;
    UINT32 gfp_sink_p_fdis;
    UINT32 gfp_sink_rx_generic_filter_drop_frame_count;
    UINT32 gfp_sink_rx_csf_frame_count;
    UINT32 gfp_sink_p_fcs_error;
    UINT32 gfp_sink_rx_pause_frame_drop_count;
    UINT32 gfp_sink_rx_pause_frame_frowarded_count;
    UINT32 gfp_sink_rx_cpu_fifo_ovr_flw_drop_frame_count;
    UINT32 gfp_sink_tx_frame_count;
    UINT32 gfp_sink_rx_unpacked_frame_count;
    UINT32 gfp_sink_rx_min_frame_length_frm_drop_count;
    UINT32 gfp_sink_rx_max_frame_length_frm_drop_count;
    UINT32 reserved;
    UINT32 gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[7];

    /* TGFPF PMON counts */
    UINT32 gfp_src_tx_byte_0_count;
    UINT32 gfp_src_tx_byte_1_count;
    UINT32 gfp_src_tx_mgmt_frames;
    UINT32 gfp_src_tx_client_data_frame_count;
    UINT32 gfp_src_tx_pause_frame_count;
    UINT32 gfp_src_tx_idle_frame_count;
    UINT32 gfp_src_tx_err_frame_count;
    UINT32 gfp_src_tx_min_frame_length_frm_count;
    UINT32 gfp_src_tx_max_frame_length_frm_count;

    /* CPB counts*/
    UINT32 cpb_dcpb_slv_dpi_packet_drop_count;
    UINT32 cpb_ocpb_slv_dpi_packet_drop_count;

} digi_pmon_mapotn_counts_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_mapotn_gmp_sync_loss_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining MAPOTN Loss of Sync Performance Monitor Count
*   in RXJC_PROC.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*   gmp_sync_loss_chnl_id  - PMON_SYNCLOSS_CHID [6:0]
*   gmp_sync_loss_count    - PMON_SYNCLOSS_CNT [31:0]
*
*******************************************************************************/
typedef struct digi_pmon_mapotn_gmp_sync_loss_t
{
    /* RGMP PMON counts */
    UINT32 gmp_sync_loss_chnl_id;
    UINT32 gmp_sync_loss_count;

} digi_pmon_mapotn_gmp_sync_loss_t;


/*******************************************************************************
* STRUCTURE: digi_pmon_lineotn_fec_counters_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining LINEOTN FEC error counters for a single channel.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*    fec_total_corr_errs                                 - TOTAL_CORR_ERRS[31:0]
*
*    40G Swizzle FEC:
*    fec_otuk_swz_lpa_stats_fp                           - LPA_STATS_FP[31:0]
*    fec_otuk_swz_lpa_stats_uncorr_cwds                  - LPA_STATS_UNCORR_CWDS [31:0]
*    fec_otuk_swz_lpa_stats_corr_1s                      - LPA_STATS_CORR_1S [31:0]
*    fec_otuk_swz_lpa_stats_corr_0s                      - LPA_STATS_CORR_0S [31:0]
*    fec_otuk_swz_llsa_stats_uncorr_cwds                 - LLSA_STATS_UNCORR_CWDS [31:0]
*    fec_otuk_swz_llsa_stats_lane_corr_1s[20]            - LLSA_STATS_LANE_CORR_1S [31:0]
*    fec_otuk_swz_llsa_stats_lane_corr_0s[20]            - LLSA_STATS_LANE_CORR_0S [31:0]
*
*    GFEC (10G,40G,100G):
*    fec_otuk_gfec_lpa_stats_fp                          - LPA_STATS_FP [31:0]
*    fec_otuk_gfec_lpa_stats_uncorr_cwds                 - LPA_STATS_UNCORR_CWDS [31:0]
*    fec_otuk_gfec_lpa_stats_corr_1s                     - LPA_STATS_CORR_1S [31:0]
*    fec_otuk_gfec_lpa_stats_corr_0s                     - LPA_STATS_CORR_0S [31:0]
*
*    I4 FEC (10G,40G)
*    fec_otuk_i4_fec_corr_0s_count                       - CORRECTED_ZEROES_X_COUNT [31:0]   (X=3..0)
*    fec_otuk_i4_fec_corr_1s_count                       - CORRECTED_ONES_X_COUNT [31:0]     (X=3..0)
*    fec_otuk_i4_fec_uncorr_error_count                  - UNCORR_ERROR_X_COUNT [31:0]       (X=3..0)
*    fec_otuk_i4_fec_rx_frm_pulse_count                  - RX_FRAME_PULSE_X_COUNT [31:0]     (X=3..0)
*
*    I7 FEC (10G,40G)
*    fec_otuk_i7_fec_corr_0s_count                       - CORRECTED_ZEROES_X_COUNT [31:0]   (X=3..0)
*    fec_otuk_i7_fec_corr_1s_count                       - CORRECTED_ONES_X_COUNT [31:0]     (X=3..0)
*    fec_otuk_i7_fec_uncorr_error_count                  - UNCORR_ERROR_X_COUNT [31:0]       (X=3..0)
*    fec_otuk_i7_fec_rx_frm_pulse_count                  - RX_FRAME_PULSE_X_COUNT [31:0]     (X=3..0)
*
*******************************************************************************/
typedef struct digi_pmon_lineotn_fec_counters_t
{
    UINT32 fec_total_corr_errs;

    UINT32 fec_otuk_swz_lpa_stats_fp;
    UINT32 fec_otuk_swz_lpa_stats_uncorr_cwds;
    UINT32 fec_otuk_swz_lpa_stats_corr_1s;
    UINT32 fec_otuk_swz_lpa_stats_corr_0s;
    UINT32 fec_otuk_swz_llsa_stats_uncorr_cwds;
    UINT32 fec_otuk_swz_llsa_stats_lane_corr_1s[20];
    UINT32 fec_otuk_swz_llsa_stats_lane_corr_0s[20];

    UINT32 fec_otuk_gfec_lpa_stats_fp;
    UINT32 fec_otuk_gfec_lpa_stats_uncorr_cwds;
    UINT32 fec_otuk_gfec_lpa_stats_corr_1s;
    UINT32 fec_otuk_gfec_lpa_stats_corr_0s;

    UINT32 fec_otuk_i4_fec_corr_0s_count;
    UINT32 fec_otuk_i4_fec_corr_1s_count;
    UINT32 fec_otuk_i4_fec_uncorr_error_count;
    UINT32 fec_otuk_i4_fec_rx_frm_pulse_count;

    UINT32 fec_otuk_i7_fec_corr_0s_count;
    UINT32 fec_otuk_i7_fec_corr_1s_count;
    UINT32 fec_otuk_i7_fec_uncorr_error_count;
    UINT32 fec_otuk_i7_fec_rx_frm_pulse_count;

    UINT32 fec_unused_params1[7];


    UINT32 fec_unused_params2[13];
 } digi_pmon_lineotn_fec_counters_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_lineotn_otu_frm_counters_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining LINEOTN otu framer far/near end counters for a single channel.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*    otu_frm_sk_sm_p_n_ds            - OTU_SK_SM_P_N_DS [24:24]
*    otu_frm_sk_sm_p_n_ebc           - OTU_SK_SM_P_N_EBC [23:0]
*    otu_frm_sk_sm_p_iae             - OTU_SK_SM_P_IAE [25:25]
*    otu_frm_sk_sm_p_f_ds            - OTU_SK_SM_P_F_DS [24:24]
*    otu_frm_sk_sm_p_f_ebc           - OTU_SK_SM_P_F_EBC [23:0]
*    otu_frm_sk_sm_p_biae            - OTU_SK_SM_P_BIAE [25:25]
*
*******************************************************************************/
typedef struct digi_pmon_lineotn_otu_frm_counters_t
 {
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 unused_align_32_bits0                        :6;     /*!< explicitily pad to 32 bit alignment */
    UINT32 otu_frm_sk_sm_p_iae                          :1;
    UINT32 otu_frm_sk_sm_p_n_ds                         :1;
    UINT32 otu_frm_sk_sm_p_n_ebc                        :24;

    UINT32 unused_align_32_bits1                        :6;     /*!< explicitily pad to 32 bit alignment */
    UINT32 otu_frm_sk_sm_p_biae                         :1;
    UINT32 otu_frm_sk_sm_p_f_ds                         :1;
    UINT32 otu_frm_sk_sm_p_f_ebc                        :24;
#else
    UINT32 otu_frm_sk_sm_p_n_ebc                        :24;
    UINT32 otu_frm_sk_sm_p_n_ds                         :1;
    UINT32 otu_frm_sk_sm_p_iae                          :1;
    UINT32 unused_align_32_bits0                        :6;     /*!< explicitily pad to 32 bit alignment */

    UINT32 otu_frm_sk_sm_p_f_ebc                        :24;
    UINT32 otu_frm_sk_sm_p_f_ds                         :1;
    UINT32 otu_frm_sk_sm_p_biae                         :1;
    UINT32 unused_align_32_bits1                        :6;     /*!< explicitily pad to 32 bit alignment */
#endif
 } digi_pmon_lineotn_otu_frm_counters_t;


/*******************************************************************************
* STRUCTURE: digi_pmon_coreotn_odukt_counters_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining COREOTN ODUkT far/near end counters for a TCM layer
*   of an ODUk channel.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*    odukt_sk_piae               - PIAE [21:21]
*    odukt_sk_pn_ds              - PN_DS [20:20]
*    odukt_sk_pn_ebc             - PN_EBC [19:0]
*    odukt_sk_pbiae              - PIAE [21:21]
*    odukt_sk_pf_ds              - PN_DS [20:20]
*    odukt_sk_pf_ebc             - PN_EBC [19:0]
*
*******************************************************************************/
typedef struct digi_pmon_coreotn_odukt_counters_t
 {
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 unused_align_32_bit1                       :10;
    UINT32 odukt_sk_piae                              :1;
    UINT32 odukt_sk_pn_ds                             :1;
    UINT32 odukt_sk_pn_ebc                            :20;

    UINT32 unused_align_32_bits0                      :10;
    UINT32 odukt_sk_pbiae                             :1;
    UINT32 odukt_sk_pf_ds                             :1;
    UINT32 odukt_sk_pf_ebc                            :20;
#else
    UINT32 odukt_sk_pn_ebc                            :20;
    UINT32 odukt_sk_pn_ds                             :1;
    UINT32 odukt_sk_piae                              :1;
    UINT32 unused_align_32_bit1                       :10;

    UINT32 odukt_sk_pf_ebc                            :20;
    UINT32 odukt_sk_pf_ds                             :1;
    UINT32 odukt_sk_pbiae                             :1;
    UINT32 unused_align_32_bits0                      :10;
#endif
 } digi_pmon_coreotn_odukt_counters_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_coreotn_odukp_counters_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining COREOTN ODUkP far/near end counters for a PM layer of
*   an ODUk channel.
*   The description of each counter is the register field name.
*
* ELEMENTS:
*    odukp_sk_pn_ds_pm           - PN_DS_PM [20:20]
*    odukp_sk_pn_ebc_pm          - PN_EBC_PM [19:0]
*    odukp_sk_pf_ds_pm           - PF_DS_PM [20:20]
*    odukp_sk_pf_ebc_pm          - PF_EBC_PM [19:0]
*******************************************************************************/
typedef struct digi_pmon_coreotn_odukp_counters_t
 {
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 unused_align_32_bits2                        :11;
    UINT32 odukp_sk_pn_ds_pm                            :1;
    UINT32 odukp_sk_pn_ebc_pm                           :20;

    UINT32 unused_align_32_bits0                        :11;
    UINT32 odukp_sk_pf_ds_pm                            :1;
    UINT32 odukp_sk_pf_ebc_pm                           :20;
#else
    UINT32 odukp_sk_pn_ebc_pm                           :20;
    UINT32 odukp_sk_pn_ds_pm                            :1;
    UINT32 unused_align_32_bits2                        :11;

    UINT32 odukp_sk_pf_ebc_pm                           :20;
    UINT32 odukp_sk_pf_ds_pm                            :1;
    UINT32 unused_align_32_bits0                        :11;
#endif
 } digi_pmon_coreotn_odukp_counters_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_coreotn_odukp_prbs_counter_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining COREOTN ODUkP PRBS counter for an ODUk channel.
*   The description of each counter is the register field name.
*
* ELEMENTS:
*    prbs_tse_chan_defects          - PRBS_TSE_CHAN_REG [31:0]
*******************************************************************************/
typedef struct digi_pmon_coreotn_odukp_prbs_counter_t
{
    UINT32 prbs_tse_chan_defects;

} digi_pmon_coreotn_odukp_prbs_counter_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_coreotn_fege_mstat_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Ethernet MIB RFC-compliant counters for a single channel.
*
* ELEMENTS:
*   rx_lcv_lsb;                       - RX_Line_Code_Violation (LSB 32 bits)
*   rx_lcv_msb;                       - RX_Line_Code_Violation (MSB 8 bits)
*   rx_frames_ok_lsb;                 - RXFramesOK (LSB 32 bits)
*   rx_frames_ok_msb;                 - RXFramesOK (MSB 8 bits)
*   rx_octets_ok_lsb;                 - RXOctetsOK (LSB 32 bits)
*   rx_octets_ok_msb;                 - RXOctetsOK (MSB 8 bits)
*   rx_frames_lsb;                    - RXFrames (LSB 32 bits)
*   rx_frames_msb;                    - RXFrames (MSB 8 bits)
*   rx_octets_lsb;                    - RXOctets (LSB 32 bits)
*   rx_octets_msb;                    - RXOctets (MSB 8 bits)
*   rx_unicast_frms_ok_lsb;           - RXUnicastFramesOK (LSB 32 bits)
*   rx_unicast_frmss_ok_msb;          - RXUnicastFramesOK (MSB 8 bits)
*   rx_broadcast_frms_ok_lsb;         - RXBroadcastFramesOK (LSB 32 bits)
*   rx_broadcast_frms_ok_msb;         - RXBroadcastFramesOK (MSB 8 bits)
*   rx_multicast_frms_ok_lsb;         - RXMulticastFramesOK (LSB 32 bits)
*   rx_multicast_frms_ok_msb;         - RXMulticastFramesOK (MSB 8 bits)
*   rx_tagged_frms_ok_lsb;            - RXTaggedFramesOK (LSB 32 bits)
*   rx_tagged_frms_ok_msb;            - RXTaggedFramesOK (MSB 8 bits)
*   rx_mac_pause_ctrl_frm_ok_lsb;     - RXMACPAUSEControlFramesOK (LSB 32 bits)
*   rx_mac_pause_ctrl_frm_ok_msb;     - RXMACPAUSEControlFramesOK (MSB 8 bits)
*   rx_mac_ctrl_frms_ok_lsb;          - RXNACControlFramesOK (LSB 32 bits)
*   rx_mac_ctrl_frms_ok_msb;          - RXNACControlFramesOK (MSB 8 bits)
*   rx_fcs_err_lsb;                   - RXFCSError (LSB 32 bits)
*   rx_fcs_err_msb;                   - RXFCSError (MSB 8 bits)
*   rx_reserved_lsb;                  - Reserved
*   rx_reserved_msb;                  - Reserved
*   rx_symbol_err_lsb;                - RXSymbolError (LSB 32 bits)
*   rx_symbol_err_msb;                - RXSymbolError (MSB 8 bits)
*   rx_layer1_err_lsb;                - RXlayer1Error (LSB 32 bits)
*   rx_layer1_err_msb;                - RXlayer1Error (MSB 8 bits)
*   rx_in_range_length_err_lsb;       - RXInRangeLengthError (LSB 32 bits)
*   rx_in_range_length_err_msb;       - RXInRangeLengthError (MSB 8 bits)
*   rx_long_length_err_lsb;           - RXLongLengthError (LSB 32 bits)
*   rx_long_length_err_msb;           - RXLongLengthError (MSB 8 bits)
*   rx_long_length_crc_err_lsb;       - RXLongLengthCRCError (LSB 32 bits)
*   rx_long_length_crc_err_msb;       - RXLongLengthCRCError (MSB 8 bits)
*   rx_short_length_error_lsb;        - RXShortLengthError (LSB 32 bits)
*   rx_short_length_error_msb;        - RXShortLengthError (MSB 8 bits)
*   rx_short_length_crc_error_lsb;    - RXShortLengthCRCError (LSB 32 bits)
*   rx_short_length_crc_error_msb;    - RXShortLengthCRCError (MSB 8 bits)
*   rx_frms_64_octs_lsb;              - RXFrames64Octets (LSB 32 bits)
*   rx_frms_64_octs_msb;              - RXFrames64Octest (MSB 8 bits)
*   rx_frms_65_to_127_octs_lsb;       - RXFrames65to127Octets (LSB 32 bits)
*   rx_frms_65_to_127_octs_msb;       - RXFrames65to127Octets (MSB 8 bits)
*   rx_frms_128_to_255_octs_lsb;      - RXFrames128to255Octets (LSB 32 bits)
*   rx_frms_128_to_255_octs_msb;      - RXFrames128to255Octets (MSB 8 bits)
*   rx_frms_256_to_511_octs_lsb;      - RXFrames256to511Octets (LSB 32 bits)
*   rx_frms_256_to_511_octs_msb;      - RXFrames256to511Octets (MSB 8 bits)
*   rx_frms_512_to_1023_octs_lsb;     - RXFrames512to1023Octets (LSB 32 bits)
*   rx_frms_512_to_1023_octs_msb;     - RXFrames512to1023Octets (MSB 8 bits)
*   rx_frms_1024_to_1518_octs_lsb;    - RXFrames1024to1518Octets (LSB 32 bits)
*   rx_frms_1024_to_1518_octs_msb;    - RXFrames1024to1518Octets (MSB 8 bits)
*   rx_frms_1519_to_max_lsb;          - RXFrames1519toMaxSize (LSB 32 bits)
*   rx_frms_1519_to_max_msb;          - RXFrames1519toMaxSize (MSB 8 bits)
*   rx_station_addr_fltr_lsb;         - RXStationAddressFiltered (LSB 32 bits)
*   rx_station_addr_fltr_msb;         - RXStationAddressFiltered (MSB 8 bits)
*   rx_variable_lsb;                  - RXVariable (LSB 32 bits)
*   rx_variable_msb;                  - RXVariable (MSB 8 bits)
*   rx_frms_1519_to_1522_octs_lsb;    - RXFrames1519to1522Octets (LSB 32 bits)
*   rx_frms_1519_to_1522_octs_msb;    - RXFrames1519to1522Octets (MSB 8 bits)
*   rx_frms_1523_to_1548_octs_lsb;    - RXFrames1523to1548Octets (LSB 32 bits)
*   rx_frms_1523_to_1548_octs_msb;    - RXFrames1523to1548Octets (MSB 8 bits)
*   rx_frms_1549_to_2000_octs_lsb;    - RXFrames1549to2000Octets (LSB 32 bits)
*   rx_frms_1549_to_2000_octs_msb;    - RXFrames1549to2000Octets (MSB 8 bits)
*   rx_frms_2001_to_2500_octs_lsb;    - RXFrames2001to2500Octets (LSB 32 bits)
*   rx_frms_2001_to_2500_octs_msb;    - RXFrames2001to2500Octets (MSB 8 bits)
*   rx_frms_2501_to_max_lsb;          - RXFrames2501toMaxSize (LSB 32 bits)
*   rx_frms_2501_to_max_msb;          - RXFrames2501toMaxSize (MSB 8 bits)
*   tx_frames_ok_lsb;                 - TXFramesOK (LSB 32 bits)
*   tx_frames_ok_msb;                 - TXFramesOK (MSB 8 bits)
*   tx_octets_ok_lsb;                 - TXOctetsOK (LSB 32 bits)
*   tx_octets_ok_msb;                 - TXOctetsOK (MSB 8 bits)
*   tx_octets_lsb;                    - TXOctets (LSB 32 bits)
*   tx_octets_msb;                    - TXOctets (MSB 8 bits)
*   tx_tagged_frms_ok_lsb;            - TXTaggedFramesOK (LSB 32 bits)
*   tx_tagged_frms_ok_msb;            - TXTaggedFramesOK (MSB 8 bits)
*   tx_mac_pause_ctrl_frms_lsb;       - TXMACPAUSEControlFrames (LSB 32 bits)
*   tx_mac_pause_ctrl_frms_msb;       - TXMACPAUSEControlFrames (MSB 8 bits)
*   tx_fcs_err_lsb;                   - TXFCSError (LSB 32 bits)
*   tx_fcs_err_msb;                   - TXFCSError (MSB 8 bits)
*   tx_short_length_err_lsb;          - TXShortLengthError (LSB 32 bits)
*   tx_short_length_err_msb;          - TXShortLengthError (MSB 8 bits)
*   tx_long_length_err_lsb;           - TXLongLengthError (LSB 32 bits)
*   tx_long_length_err_msb;           - TXLongLengthError (MSB 8 bits)
*   tx_system_err_lsb;                - TXSystemError (LSB 32 bits)
*   tx_system_err_msb;                - TXSystemError (MSB 8 bits)
*   tx_mac_err_lsb;                   - TXMACError (LSB 32 bits)
*   tx_mac_err_msb;                   - TXMACError (MSB 8 bits)
*   tx_jumbo_frms_lsb;                - TXJumboFramesOK (LSB 32 bits)
*   tx_jumbo_frms_msb;                - TXJumboFramesOK (MSB 8 bits)
*   tx_mac_ctrl_frms_ok_lsb;          - TXMACControlFramesOK (LSB 32 bits)
*   tx_mac_ctrl_frms_ok_msb;          - TXMACControlFramesOK (MSB 8 bits)
*   tx_unicast_frms_ok_lsb;           - TXUnicastFramesOK (LSB 32 bits)
*   tx_unicast_frms_ok_msb;           - TXUnicastFramesOK (MSB 8 bits)
*   tx_broadcast_frms_ok_lsb;         - TXBroadcastFramesOK (LSB 32 bits)
*   tx_broadcast_frms_ok_msb;         - TXBroadcastFramesOK (MSB 8 bits)
*   tx_multicast_frms_ok_lsb;         - TXMulticastFramesOK (LSB 32 bits)
*   tx_multicast_frms_ok_msb;         - TXMulticastFramesOK (MSB 8 bits)
*   tx_unicast_frms_attempted_lsb;    - TXUnicastFramesAttempted (LSB 32 bits)
*   tx_unicast_frms_attempted_msb;    - TXUnicastFramesAttempted (MSB 8 bits)
*   tx_broadcast_frms_attempted_lsb;  - TXBroadcastFramesAttempted (LSB 32 bits)
*   tx_broadcast_frms_attempted_msb;  - TXBroadcastFramesAttempted (MSB 8 bits)
*   tx_multicast_frms_attempted_lsb;  - TXMulticastFramesAttempted (LSB 32 bits)
*   tx_multicast_frms_attempted_msb;  - TXMulticastFramesAttempted (MSB 8 bits)
*   tx_frms_64_octs_lsb;              - TXFrames64Octets (LSB 32 bits)
*   tx_frms_64_octs_msb;              - TXFrames64Octets (MSB 8 bits)
*   tx_frms_65_to_127_octs_lsb;       - TXFrames65to127Octets (LSB 32 bits)
*   tx_frms_65_to_127_octs_msb;       - TXFrames65to127Octets (MSB 8 bits)
*   tx_frms_128_to_255_octs_lsb;      - TXFrames128to255Octets (LSB 32 bits)
*   tx_frms_128_to_255_octs_msb;      - TXFrames128to255Octets (MSB 8 bits)
*   tx_frms_256_to_511_octs_lsb;      - TXFrames256to511Octets (LSB 32 bits)
*   tx_frms_256_to_511_octs_msb;      - TXFrames256to511Octets (MSB 8 bits)
*   tx_frms_512_to_1023_octs_lsb;     - TXFrames512to1023Octets (LSB 32 bits)
*   tx_frms_512_to_1023_octs_msb;     - TXFrames512to1023Octets (MSB 8 bits)
*   tx_frms_1024_to_1518_octs_lsb;    - TXFrames1024to1518Octets (LSB 32 bits)
*   tx_frms_1024_to_1518_octs_msb;    - TXFrames1024to1518Octets (MSB 8 bits)
*   tx_frms_1519_to_max_lsb;          - TXFrames1519toMaxSize (LSB 32 bits)
*   tx_frms_1519_to_max_msb;          - TXFrames1519toMaxSize (MSB 8 bits)
*   tx_variable_lsb;                  - TXVariable (LSB 32 bits)
*   tx_variable_msb;                  - TXVariable (MSB 8 bits)
*   rx_jumbo_frms_ok_lsb;             - RXJumboFramesOK (LSB 32 bits)
*   rx_jumbo_frms_ok_msb;             - RXJumboFramesOK (MSB 8 bits)
*   tx_frms_1519_to_1522_octs_lsb;    - TXFrames1519to1522Octets (LSB 32 bits)
*   tx_frms_1519_to_1522_octs_msb;    - TXFrames1519to1522Octets (MSB 8 bits)
*   tx_frms_1523_to_1548_octs_lsb;    - TXFrames1523to1548Octets (LSB 32 bits)
*   tx_frms_1523_to_1548_octs_msb;    - TXFrames1523to1548Octets (MSB 8 bits)
*   tx_frms_1549_to_2000_octs_lsb;    - TXFrames1549to2000Octets (LSB 32 bits)
*   tx_frms_1549_to_2000_octs_msb;    - TXFrames1549to2000Octets (MSB 8 bits)
*   tx_frms_2001_to_2500_octs_lsb;    - TXFrames2001to2500Octets (LSB 32 bits)
*   tx_frms_2001_to_2500_octs_msb;    - TXFrames2001to2500Octets (MSB 8 bits)
*   tx_frms_2501_to_max_lsb;          - TXFrames2501toMaxSize (LSB 32 bits)
*   tx_frms_2501_to_max_msb;          - TXFrames2501toMaxSize (MSB 8 bits)
*
*******************************************************************************/
typedef struct digi_pmon_coreotn_fege_mstat_t
{
    UINT32 rx_lcv_lsb;
    UINT32 rx_lcv_msb;
    UINT32 rx_frames_ok_lsb;
    UINT32 rx_frames_ok_msb;
    UINT32 rx_octets_ok_lsb;
    UINT32 rx_octets_ok_msb;
    UINT32 rx_frames_lsb;
    UINT32 rx_frames_msb;
    UINT32 rx_octets_lsb;
    UINT32 rx_octets_msb;
    UINT32 rx_unicast_frms_ok_lsb;
    UINT32 rx_unicast_frms_ok_msb;
    UINT32 rx_broadcast_frms_ok_lsb;
    UINT32 rx_broadcast_frms_ok_msb;
    UINT32 rx_multicast_frms_ok_lsb;
    UINT32 rx_multicast_frms_ok_msb;
    UINT32 rx_tagged_frms_ok_lsb;
    UINT32 rx_tagged_frms_ok_msb;
    UINT32 rx_mac_pause_ctrl_frm_ok_lsb;
    UINT32 rx_mac_pause_ctrl_frm_ok_msb;
    UINT32 rx_mac_ctrl_frms_ok_lsb;
    UINT32 rx_mac_ctrl_frms_ok_msb;
    UINT32 rx_fcs_err_lsb;
    UINT32 rx_fcs_err_msb;
    UINT32 rx_reserved_lsb;
    UINT32 rx_reserved_msb;
    UINT32 rx_symbol_err_lsb;
    UINT32 rx_symbol_err_msb;
    UINT32 rx_layer1_err_lsb;
    UINT32 rx_layer1_err_msb;
    UINT32 rx_in_range_length_err_lsb;
    UINT32 rx_in_range_length_err_msb;
    UINT32 rx_long_length_err_lsb;
    UINT32 rx_long_length_err_msb;
    UINT32 rx_long_length_crc_err_lsb;
    UINT32 rx_long_length_crc_err_msb;
    UINT32 rx_short_length_error_lsb;
    UINT32 rx_short_length_error_msb;
    UINT32 rx_short_length_crc_error_lsb;
    UINT32 rx_short_length_crc_error_msb;
    UINT32 rx_frms_64_octs_lsb;
    UINT32 rx_frms_64_octs_msb;
    UINT32 rx_frms_65_to_127_octs_lsb;
    UINT32 rx_frms_65_to_127_octs_msb;
    UINT32 rx_frms_128_to_255_octs_lsb;
    UINT32 rx_frms_128_to_255_octs_msb;
    UINT32 rx_frms_256_to_511_octs_lsb;
    UINT32 rx_frms_256_to_511_octs_msb;
    UINT32 rx_frms_512_to_1023_octs_lsb;
    UINT32 rx_frms_512_to_1023_octs_msb;
    UINT32 rx_frms_1024_to_1518_octs_lsb;
    UINT32 rx_frms_1024_to_1518_octs_msb;
    UINT32 rx_frms_1519_to_max_lsb;
    UINT32 rx_frms_1519_to_max_msb;
    UINT32 rx_station_addr_fltr_lsb;
    UINT32 rx_station_addr_fltr_msb;
    UINT32 rx_variable_lsb;
    UINT32 rx_variable_msb;
    UINT32 rx_frms_1519_to_1522_octs_lsb;
    UINT32 rx_frms_1519_to_1522_octs_msb;
    UINT32 rx_frms_1523_to_1548_octs_lsb;
    UINT32 rx_frms_1523_to_1548_octs_msb;
    UINT32 rx_frms_1549_to_2000_octs_lsb;
    UINT32 rx_frms_1549_to_2000_octs_msb;
    UINT32 rx_frms_2001_to_2500_octs_lsb;
    UINT32 rx_frms_2001_to_2500_octs_msb;
    UINT32 rx_frms_2501_to_max_lsb;
    UINT32 rx_frms_2501_to_max_msb;
    UINT32 tx_frames_ok_lsb;
    UINT32 tx_frames_ok_msb;
    UINT32 tx_octets_ok_lsb;
    UINT32 tx_octets_ok_msb;
    UINT32 tx_octets_lsb;
    UINT32 tx_octets_msb;
    UINT32 tx_tagged_frms_ok_lsb;
    UINT32 tx_tagged_frms_ok_msb;
    UINT32 tx_mac_pause_ctrl_frms_lsb;
    UINT32 tx_mac_pause_ctrl_frms_msb;
    UINT32 tx_fcs_err_lsb;
    UINT32 tx_fcs_err_msb;
    UINT32 tx_short_length_err_lsb;
    UINT32 tx_short_length_err_msb;
    UINT32 tx_long_length_err_lsb;
    UINT32 tx_long_length_err_msb;
    UINT32 tx_system_err_lsb;
    UINT32 tx_system_err_msb;
    UINT32 tx_mac_err_lsb;
    UINT32 tx_mac_err_msb;
    UINT32 tx_jumbo_frms_lsb;
    UINT32 tx_jumbo_frms_msb;
    UINT32 tx_mac_ctrl_frms_ok_lsb;
    UINT32 tx_mac_ctrl_frms_ok_msb;
    UINT32 tx_unicast_frms_ok_lsb;
    UINT32 tx_unicast_frms_ok_msb;
    UINT32 tx_broadcast_frms_ok_lsb;
    UINT32 tx_broadcast_frms_ok_msb;
    UINT32 tx_multicast_frms_ok_lsb;
    UINT32 tx_multicast_frms_ok_msb;
    UINT32 tx_unicast_frms_attempted_lsb;
    UINT32 tx_unicast_frms_attempted_msb;
    UINT32 tx_broadcast_frms_attempted_lsb;
    UINT32 tx_broadcast_frms_attempted_msb;
    UINT32 tx_multicast_frms_attempted_lsb;
    UINT32 tx_multicast_frms_attempted_msb;
    UINT32 tx_frms_64_octs_lsb;
    UINT32 tx_frms_64_octs_msb;
    UINT32 tx_frms_65_to_127_octs_lsb;
    UINT32 tx_frms_65_to_127_octs_msb;
    UINT32 tx_frms_128_to_255_octs_lsb;
    UINT32 tx_frms_128_to_255_octs_msb;
    UINT32 tx_frms_256_to_511_octs_lsb;
    UINT32 tx_frms_256_to_511_octs_msb;
    UINT32 tx_frms_512_to_1023_octs_lsb;
    UINT32 tx_frms_512_to_1023_octs_msb;
    UINT32 tx_frms_1024_to_1518_octs_lsb;
    UINT32 tx_frms_1024_to_1518_octs_msb;
    UINT32 tx_frms_1519_to_max_lsb;
    UINT32 tx_frms_1519_to_max_msb;
    UINT32 tx_variable_lsb;
    UINT32 tx_variable_msb;
    UINT32 rx_jumbo_frms_ok_lsb;
    UINT32 rx_jumbo_frms_ok_msb;
    UINT32 tx_frms_1519_to_1522_octs_lsb;
    UINT32 tx_frms_1519_to_1522_octs_msb;
    UINT32 tx_frms_1523_to_1548_octs_lsb;
    UINT32 tx_frms_1523_to_1548_octs_msb;
    UINT32 tx_frms_1549_to_2000_octs_lsb;
    UINT32 tx_frms_1549_to_2000_octs_msb;
    UINT32 tx_frms_2001_to_2500_octs_lsb;
    UINT32 tx_frms_2001_to_2500_octs_msb;
    UINT32 tx_frms_2501_to_max_lsb;
    UINT32 tx_frms_2501_to_max_msb;
} digi_pmon_coreotn_fege_mstat_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_cbrc_sdh_pmg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining SDH PMG line and section error counts.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*
*    rx_section_bip_err_count       - SBIPE[15:0] 
*    rx_line_bip_err_count_lsb      - LBIPE[15:0] 
*    rx_line_bip_err_count_msb      - LBIPE[25:16] 
*    rx_line_rei_err_count_lsb      - LREIE[15:0] 
*    rx_line_rei_err_count_msb      - LREIE[25:16] 
*
*    tx_section_bip_err_count       - SBIPE[15:0] 
*    tx_line_bip_err_count_lsb      - LBIPE[15:0] 
*    tx_line_bip_err_count_msb      - LBIPE[25:16]
*    tx_line_rei_err_count_lsb      - LREIE[15:0] 
*    tx_line_rei_err_count_msb      - LREIE[25:16] 
* 
*    cpb_dcpb_slv_dpi_packet_drop_count   - CH_DROP_CNT_VAL 
*******************************************************************************/
typedef struct digi_pmon_cbrc_sdh_pmg_t
 {
    UINT32 rx_section_bip_err_count;
    UINT32 rx_line_bip_err_count_lsb;
    UINT32 rx_line_bip_err_count_msb;
    UINT32 rx_line_rei_err_count_lsb;
    UINT32 rx_line_rei_err_count_msb;
    
    UINT32 tx_section_bip_err_count;
    UINT32 tx_line_bip_err_count_lsb;
    UINT32 tx_line_bip_err_count_msb;
    UINT32 tx_line_rei_err_count_lsb;
    UINT32 tx_line_rei_err_count_msb;

    UINT32 cpb_dcpb_slv_dpi_packet_drop_count;
 } digi_pmon_cbrc_sdh_pmg_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_cbrc_fc_cbr_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining FC 800 counts.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*   src_rx_good_frame_count      - RX_GF_DWRD
*   src_rx_bad_frame_count       - RX_BF_DWRD
*   src_rx_byte_count            - RX_WRD_CNT
*   src_rx_bad_crc_count            - RX_BAD_CRC 
*   src_rx_total_packet_count    - RX_TOT_PKTS      
*   src_tx_good_frame_count      - TX_GF_DWRD
*   src_tx_bad_frame_count       - TX_BF_DWRD
*   src_tx_byte_count            - TX_WRD_CNT
*   src_tx_bad_crc_count            - TX_BAD_CRC 
*   src_tx_total_packet_count    - TX_TOT_PKTS 
*   snk_rx_good_frame_count      - RX_GF_DWRD
*   snk_rx_bad_frame_count       - RX_BF_DWRD
*   snk_rx_byte_count            - RX_WRD_CNT
*   snk_rx_bad_crc_count            - RX_BAD_CRC 
*   snk_rx_total_packet_count    - RX_TOT_PKTS      
*   snk_tx_good_frame_count      - TX_GF_DWRD
*   snk_tx_bad_frame_count       - TX_BF_DWRD
*   snk_tx_byte_count            - TX_WRD_CNT
*   snk_tx_bad_crc_count            - TX_BAD_CRC 
*   snk_tx_total_packet_count    - TX_TOT_PKTS
*******************************************************************************/
typedef struct digi_pmon_cbrc_fc_cbr_t 
{
    /*FC-800 Src PMON CBRC_FC_ILVR*/
    UINT32 src_rx_good_frame_count;
    UINT32 src_rx_bad_frame_count;
    UINT32 src_rx_byte_count;
    UINT32 src_rx_bad_crc_count;
    UINT32 src_rx_total_packet_count;
    UINT32 src_tx_good_frame_count;
    UINT32 src_tx_bad_frame_count;
    UINT32 src_tx_byte_count;
    UINT32 src_tx_bad_crc_count;
    UINT32 src_tx_total_packet_count;
    UINT32 src_lcv_cnt;
    
    /*FC-800 Snk PMON CBRC_FC_DLVR*/
    UINT32 snk_rx_good_frame_count;
    UINT32 snk_rx_bad_frame_count;
    UINT32 snk_rx_byte_count;
    UINT32 snk_rx_bad_crc_count;
    UINT32 snk_rx_total_packet_count;  
    UINT32 snk_tx_good_frame_count;
    UINT32 snk_tx_bad_frame_count;
    UINT32 snk_tx_byte_count;
    UINT32 snk_tx_bad_crc_count;
    UINT32 snk_tx_total_packet_count;
    UINT32 snk_lcv_cnt;
    
} digi_pmon_cbrc_fc_cbr_t; 

/*******************************************************************************
* STRUCTURE: digi_pmon_cbrc_fc1200_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining FC 1200 counts.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*   src_rx_err_count                  - ERR_CNTS: bits 23:16 HIGH_BER_ERR_CNT, 
*                                                 bits 15:0  PRBS31_ERR_CNT
*   src_rx_block_err_count            - BLOCK_ERR_CNTS: bits 31:16 BLK_TYPE_ERR_CNT,
*                                                       bits 15:0  INVLD_BLK_CNT
*   src_rx_data_byte_count_lsb        - RXD_DATA_BYTE_CNT_LSB (LSB 32 bits)
*   src_rx_data_byte_count_msb        - RXD_DATA_BYTE_CNT_MSB (MSB 8 bits)
*   src_rx_ordered_set_count          - RXD_ORDERED_SET_CNT
*   src_rx_valid_frame_count          - VALID_FRM_COUNT
*   src_rx_invalid_frame_count        - INVLD_FRM_COUNT
*   src_rx_valid_frame_byte_count     - VALID_FRM_BYTE_COUNT
*   snk_tx_byte_count_lsb             - TX_DATA_BYTE_CNT_LSB (LSB 32 bits)
*   snk_tx_byte_count_msb             - TX_DATA_BYTE_CNT_MSB (MSB 8 bits)
*   snk_tx_ordered_set_count          - TX_ORDERED_SET_CNT
*   snk_err_ctrl_block_count          - ERROR_BLK_CNT
*   snk_local_fault_ordered_set_count - LF_OS_CNT
*   snk_rx_valid_frame_count          - VALID_FRM_COUNT
*   snk_rx_invalid_frame_count        - INVLD_FRM_COUNT
*   snk_rx_valid_frame_byte_count     - VALID_FRM_BYTE_COUNT
*   tx_gfp_frame_count                - GFPCNT [23:0]
*   tx_gfp_errored_frame_count        - GFPERRCNT [23:0]
*   crc_24_err_count                  - CRCERRCNT
*   invalid_blk_hdr_count             - INVALBLKHDRCNT
*******************************************************************************/
typedef struct digi_pmon_cbrc_fc1200_t 
{    
    /*FC-1200 Src PMON: FC1200_PCS_RX*/
    UINT32 src_rx_err_count;            
    UINT32 src_rx_block_err_count;
    UINT32 src_rx_data_byte_count_lsb;
    UINT32 src_rx_data_byte_count_msb;
    UINT32 src_rx_ordered_set_count;
 
    /*FC-1200 Src PMON: FC1200_PMON*/
    UINT32 src_rx_valid_frame_count;
    UINT32 src_rx_invalid_frame_count;
    UINT32 src_rx_valid_frame_byte_count;
    
    /*FC-1200 Snk PMON: FC1200_PCS_TX*/
    UINT32 snk_tx_byte_count_lsb;
    UINT32 snk_tx_byte_count_msb;
    UINT32 snk_tx_ordered_set_count;
    UINT32 snk_err_ctrl_block_count;
    UINT32 snk_local_fault_ordered_set_count;

    /*FC-1200 Snk PMON: FC1200_PMON Snk*/
    UINT32 snk_rx_valid_frame_count;
    UINT32 snk_rx_invalid_frame_count;
    UINT32 snk_rx_valid_frame_byte_count;

    /*FC-1200 TTT Mapper: FC_TTM*/
    UINT32 tx_gfp_frame_count;
    UINT32 tx_gfp_errored_frame_count;

    /*FC_TTTD */
    UINT32 crc_24_err_count;
    UINT32 invalid_blk_hdr_count;
} digi_pmon_cbrc_fc1200_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_cpb_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining cpb counts.
*   The description of each counter is the register field name.
*
* ELEMENTS:
*    packet_drop_count[150] - CH_DROP_CNT_VAL [31:0]
*******************************************************************************/
typedef struct digi_pmon_cpb_t 
{
    UINT32 packet_drop_count[150];
} digi_pmon_cpb_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_sifd_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining SIFD error counts.
*   The description of each counter is the register field name. 
* 
*   For register TX_ERR_CNT, TX_DIAG_CNT, and RX_DIAG_CNT, N=0 is for CPB, and N=1 
*   is for ODUK 
*   For register BYTE_CNT_HIGH_READ and BYTE_CNT_LOW_READ, N=0 is for TX, and N=1 
*   is for RX. And also, MSB_BC_RDATA is the lower 16-bit of BYTE_CNT_HIGH_READ. 
*
*
* ELEMENTS:
*
*    tx_error_count_cpb                   - TX_ERR_CNT(N=0) [31:0]
*    tx_error_count_oduk                  - TX_ERR_CNT(N=1) [31:0]
*    rx_error_count                       - RX_ERR_CNT [31:0] 
* 
*    tx_diag_count_cpb                    - TX_DIAG_CNT(N=0)[31:0]
*    tx_diag_count_oduk                   - TX_DIAG_CNT(N=1)[31:0]
*    rx_diag_count_cpb                    - RX_DIAG_CNT(N=0)[31:0] 
*    rx_diag_count_oduk                   - RX_DIAG_CNT(N=1)[31:0]  
* 
*    cpb_dcpb_slv_dpi_packet_drop_count   - CH_DROP_CNT_VAL (for DCPB)
*    cpb_ocpb_slv_dpi_packet_drop_count   - CH_DROP_CNT_VAL (for ODUK_SW)   
*******************************************************************************/
typedef struct digi_pmon_sifd_t
{
     UINT32  tx_error_count_cpb;
     UINT32  tx_error_count_oduk;
     UINT32  rx_error_count;

     UINT32  tx_diag_count_cpb ;
     UINT32  tx_diag_count_oduk;
     UINT32  rx_diag_count_cpb;
     UINT32  rx_diag_count_oduk;

     UINT32  cpb_dcpb_slv_dpi_packet_drop_count[DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX];
     UINT32  cpb_ocpb_slv_dpi_packet_drop_count[DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX];
} digi_pmon_sifd_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_enet_eclass_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining PMON counts from the ENET ECLASS
*
* ELEMENTS:
*   rx_lldp_pkt_cnt - Packets filtered in ingress by the ECLASS LLDP filter.
*   tx_lldp_pkt_cnt - Packets filtered in egress by the ECLASS LLDP filter. 
*
*******************************************************************************/
typedef struct digi_pmon_enet_eclass_t
{
    UINT32 rx_lldp_pkt_cnt;
    UINT32 tx_lldp_pkt_cnt;
    UINT32 rx_lldp_pkt_missed_cnt;
    UINT32 tx_lldp_pkt_missed_cnt;
} digi_pmon_enet_eclass_t;

/*******************************************************************************
* STRUCTURE: pmon_dma_hdr_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining PMON headers
*
* ELEMENTS:
*
*    lclk_stamp        - LCLK timestamp. Incremented with every PMON
*                        collection cycle
*    work_order        - Work order number for the hardware block
*    seq_num           - Sequence number of host message that enabled PMON
*                        collection for this hw_block
*
*******************************************************************************/
typedef struct pmon_dma_hdr_t
{
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 lclk_stamp   :16;
    UINT32 work_order   :8;
    UINT32 seq_num      :8;
#else
    UINT32 seq_num      :8;
    UINT32 work_order   :8;
    UINT32 lclk_stamp   :16;
#endif
} pmon_dma_hdr_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for PMON interrupts.
*
* ELEMENTS:
*   otn_i   - Indicates that OTN PMON is available (including Mapper FO2)
*             When this interrupt is active, the following PMON retrieval
*             APIs can be used:
*             digi_otn_pmon_retrieve()
*             digi_mapper_pmon_fo2_retrieve()
*
*   misc_i  - All other PMON
*             When this interrupt is active, the following PMON retrieval
*             APIs can be used:
*             digi_cbr_pmon_retrieve()
*             digi_enet_line_pmon_retrieve()
*             digi_enet_sys_pmon_retrieve()
*             digi_mapper_pmon_retrieve()
*             digi_sifd_pmon_retrieve()
*
*******************************************************************************/
typedef struct digi_pmon_int_t 
{
    BOOL    otn_i;
    BOOL    misc_i;
} digi_pmon_int_t;

/*
** Function Prototypes
*/
/* FW related */
PUBLIC PMC_ERROR digi_pmon_collection_mode_cfg(digi_handle_t *digi_handle,
                                               UINT32 pmon_trig_mode,
                                               UINT32 pmon_sync_en,
                                               UINT32 trig_period,
                                               UINT64 data_store_phys_start_addr,
                                               void  *data_store_vir_start_addr,
                                               UINT32 data_buf_size,
                                               UINT32 num_data_buf);
PUBLIC PMC_ERROR digi_pmon_work_order_cfg(digi_handle_t *digi_handle,
                                          digi_pmon_work_order_msg_t *work_order);
PUBLIC PMC_ERROR digi_pmon_ddeg_coreotn_seg_cfg(digi_handle_t *digi_handle,
                                                BOOL mon_coreotn_stg_4);
PUBLIC PMC_ERROR digi_pmon_ddeg_param_cfg(digi_handle_t *digi_handle,
                                          digi_pmon_ddeg_mode_t ddeg_mode,
                                          digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                          digi_pmon_ddeg_chnl_bitmsk_t ddeg_chnl,
                                          UINT8 mon_intvl, UINT32 set_threshold,
                                          UINT32 clear_threshold,
                                          BOOL8 mon_intvl_valid,
                                          BOOL8 set_threshold_valid,
                                          BOOL8 clear_threshold_valid);
PUBLIC PMC_ERROR digi_pmon_ddeg_en_cfg(digi_handle_t *digi_handle,
                                       digi_pmon_ddeg_mode_t ddeg_mode,
                                       digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                       digi_pmon_ddeg_chnl_bitmsk_t ddeg_chnl,
                                       BOOL enable);
PUBLIC PMC_ERROR digi_pmon_ddeg_int_en_cfg(digi_handle_t *digi_handle,
                                           digi_pmon_ddeg_mode_t ddeg_mode,
                                           digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                           digi_pmon_ddeg_chnl_bitmsk_t ddeg_chnl,
                                           BOOL int_enable);
PUBLIC PMC_ERROR digi_pmon_ddeg_int_get(digi_handle_t *digi_handle,
                                        digi_pmon_ddeg_mode_t ddeg_mode,
                                        UINT8 int_type,
                                        digi_pmon_ddeg_ints_t *digi_pmon_ddeg_ints_ptr);
PUBLIC PMC_ERROR digi_pmon_ddeg_param_get(digi_handle_t *digi_handle,
                                          digi_pmon_ddeg_mode_t ddeg_mode,
                                          digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                          UINT8 chnl,
                                          digi_pmon_ddeg_param_t *digi_pmon_ddeg_param_ptr);
PUBLIC PMC_ERROR digi_pmon_ddeg_state_get(digi_handle_t *digi_handle,
                                          digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                          UINT8 chnl,
                                          digi_pmon_ddeg_state_t *digi_pmon_ddeg_state_ptr);
PUBLIC PMC_ERROR digi_pmon_ddeg_enable_status_get(digi_handle_t *digi_handle,
                                                  digi_pmon_ddeg_mode_t ddeg_mode,
                                                  digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                                  digi_pmon_ddeg_enable_status_t *digi_pmon_ddeg_enable_status_ptr);
PUBLIC PMC_ERROR digi_pmon_ddeg_cfg_hw_ddeg_lclk(digi_handle_t *digi_handle,
                                                 BOOL enable);
PUBLIC PMC_ERROR digi_pmon_fw_fec_manual_update(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_pmon_manual_trigger_cfg(digi_handle_t *digi_handle, UINT32 manual_trig_msk);
PUBLIC PMC_ERROR digi_pmon_fw_sm_dma_status_get(digi_handle_t *digi_handle,
                                                digi_pmon_fw_sm_dma_status_t *digi_pmon_status_ptr);
PUBLIC PMC_ERROR digi_pmon_set_rcp_los_cfg(digi_handle_t *digi_handle, 
                                           UINT16 replace_sdh_di_rcp_enet_los_bit_mask);
PUBLIC PMC_ERROR digi_pmon_prbs_collection_mode_cfg(digi_handle_t *digi_handle, 
                                                    UINT32 prbs_collection_mode);

/* Interrupt Functions */
PUBLIC PMC_ERROR digi_pmon_int_register_cb(digi_handle_t      *digi_handle,
                                           void               *token,
                                           UINT32              token_size,
                                           digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_pmon_int_enable(digi_handle_t            *digi_handle,
                                      digi_pmon_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_pmon_int_disable(digi_handle_t            *digi_handle,
                                       digi_pmon_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_pmon_int_clear(digi_handle_t            *digi_handle,
                                     digi_pmon_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_pmon_int_retrieve(digi_handle_t            *digi_handle,
                                        digi_pmon_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_pmon_int_enabled_check(digi_handle_t            *digi_handle);

PUBLIC PMC_ERROR digi_pmon_fw_fec_cfg(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_pmon_shared_mem_size_get(digi_handle_t    *digi_handle,
                                               UINT32           *size_ptr);
#ifdef __cplusplus
}
#endif

#endif /* _DIGI_PMON_API_H */
