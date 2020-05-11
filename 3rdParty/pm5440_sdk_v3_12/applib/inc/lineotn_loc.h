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
#ifndef _LINEOTN_LOC_H
#define _LINEOTN_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include <math.h>
#include "pmc_sys.h"
#include "pmc_log.h"
#include "otu_frm.h"
#include "lineotn.h"   
#include "lineotn_ss_mtsb_map.h"

#include "hostmsg.h"
 



/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: lineotn_chnl_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   LINEOTN channel rate: 10G, 40G, or 100G.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{                       
    LINE_OTN_RATE_10G   = 0, 
    LINE_OTN_RATE_40G, 
    LINE_OTN_RATE_100G, 
    LAST_LINE_OTN_RATE

} lineotn_chnl_rate_t;


/*******************************************************************************
* ENUM: lineotn_lof_mon_hostmsg_intf_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   LINEOTN LOF monitoring interface for the firmware host message.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{                       
    LINE_OTN_LOF_MON_INTF_OTU4   = 0, 
    LINE_OTN_LOF_MON_INTF_OTU3_0 = 1, 
    LINE_OTN_LOF_MON_INTF_OTU3_1 = 2, 
    LINE_OTN_LOF_MON_INTF_OTU3_2 = 3, 
    LINE_OTN_LOF_MON_INTF_LAST_CFG   /* this is for out of range values error handling                                   */

} lineotn_lof_mon_hostmsg_intf_type_t;

/*******************************************************************************
* STRUCTURE: lineotn_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining LINEOTN energy state
*
* ELEMENTS: 
*   See typedef.
*
*******************************************************************************/    
typedef struct lineotn_energy_state_struct_t 
{     
    pmc_energy_state_t otu4_framer;   
    pmc_energy_state_t mld_100g;   
    pmc_energy_state_t otu23_framer[3];   
    pmc_energy_state_t mld_10_40g[3];   
    
} lineotn_energy_state_t;

/*
** Constants
*/
#define LINEOTN_FEC_STAT_COLLECTION_PER_MAX_MS     1000
#define LINEOTN_FEC_STAT_COLLECTION_MS_200MHZ_TICK 0x30D40
#define LINEOTN_FEC_STAT_MAX_FSL_TBL_ENTRIES       48
#define LINEOTN_FEC_STAT_1_FEC_SOURCE              0x1
#define LINEOTN_FEC_STAT_3_FEC_SOURCE              0x7
#define LINEOTN_FEC_STAT_NULL_ENTRY                0xFF

/* Constants for Swizzle FEC Frame Slip Monitor */
#define LINEOTN_SWIZZLE_FEC_FRAME_SLIP_MAX_NUM_RETRY   10

/*
** Macro Definitions
*/

#define LINEOTN_IS_LINE_MODE(lineotn_handle)  (TRUE == lineotn_handle->var.lineotn_mode)

#define LINEOTN_SFIS_INT_FIELD10_ARRAY_ENABLE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            switch(link)                                                        \
            {                                                                   \
                case 0:  tsb_name##_field_##tsb_field##0##_E_set(NULL, handle, i, enable); break;  \
                case 1:  tsb_name##_field_##tsb_field##1##_E_set(NULL, handle, i, enable); break;  \
                case 2:  tsb_name##_field_##tsb_field##2##_E_set(NULL, handle, i, enable); break;  \
                case 3:  tsb_name##_field_##tsb_field##3##_E_set(NULL, handle, i, enable); break;  \
                case 4:  tsb_name##_field_##tsb_field##4##_E_set(NULL, handle, i, enable); break;  \
                case 5:  tsb_name##_field_##tsb_field##5##_E_set(NULL, handle, i, enable); break;  \
                case 6:  tsb_name##_field_##tsb_field##6##_E_set(NULL, handle, i, enable); break;  \
                case 7:  tsb_name##_field_##tsb_field##7##_E_set(NULL, handle, i, enable); break;  \
                case 8:  tsb_name##_field_##tsb_field##8##_E_set(NULL, handle, i, enable); break;  \
                case 9:  tsb_name##_field_##tsb_field##9##_E_set(NULL, handle, i, enable); break;  \
                default: break;                                                 \
            }                                                                   \
            int_en_table_ptr->struct_field[i] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)


#define LINEOTN_SFIS_INT_FIELD10_ARRAY_CLEAR(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            switch(link)                                                        \
            {                                                                   \
                case 0:  tsb_name##_field_##tsb_field##0##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 1:  tsb_name##_field_##tsb_field##1##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 2:  tsb_name##_field_##tsb_field##2##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 3:  tsb_name##_field_##tsb_field##3##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 4:  tsb_name##_field_##tsb_field##4##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 5:  tsb_name##_field_##tsb_field##5##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 6:  tsb_name##_field_##tsb_field##6##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 7:  tsb_name##_field_##tsb_field##7##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 8:  tsb_name##_field_##tsb_field##8##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 9:  tsb_name##_field_##tsb_field##9##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                default: break;                                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_SFIS_INT_FIELD10_ARRAY_RETRIEVE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            switch(link)                                                        \
            {                                                                   \
                case 0:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##0##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 1:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##1##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 2:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##2##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 3:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##3##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 4:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##4##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 5:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##5##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 6:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##6##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 7:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##7##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 8:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##8##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 9:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##9##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                default: break;                                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_SFIS_STATUS_FIELD10_ARRAY_RETRIEVE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            switch(link)                                                        \
            {                                                                   \
                case 0:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##0##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 1:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##1##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 2:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##2##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 3:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##3##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 4:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##4##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 5:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##5##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 6:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##6##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 7:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##7##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 8:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##8##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 9:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##9##_V_get(tsb_name##_buffer_ptr, handle, i); break;  \
                default: break;                                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_SFIS_INT_FIELD10_ARRAY_CHECK(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((FALSE == *int_found_ptr) &&                                        \
            (TRUE == int_en_table_ptr->struct_field[i])) {                      \
            switch(link)                                                        \
            {                                                                   \
                case 0:  *int_found_ptr = tsb_name##_field_##tsb_field##0##_I_get(NULL, handle, i); break;  \
                case 1:  *int_found_ptr = tsb_name##_field_##tsb_field##1##_I_get(NULL, handle, i); break;  \
                case 2:  *int_found_ptr = tsb_name##_field_##tsb_field##2##_I_get(NULL, handle, i); break;  \
                case 3:  *int_found_ptr = tsb_name##_field_##tsb_field##3##_I_get(NULL, handle, i); break;  \
                case 4:  *int_found_ptr = tsb_name##_field_##tsb_field##4##_I_get(NULL, handle, i); break;  \
                case 5:  *int_found_ptr = tsb_name##_field_##tsb_field##5##_I_get(NULL, handle, i); break;  \
                case 6:  *int_found_ptr = tsb_name##_field_##tsb_field##6##_I_get(NULL, handle, i); break;  \
                case 7:  *int_found_ptr = tsb_name##_field_##tsb_field##7##_I_get(NULL, handle, i); break;  \
                case 8:  *int_found_ptr = tsb_name##_field_##tsb_field##8##_I_get(NULL, handle, i); break;  \
                case 9:  *int_found_ptr = tsb_name##_field_##tsb_field##9##_I_get(NULL, handle, i); break;  \
                default: break;                                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

/* Array interrupt accessors */
#define LINEOTN_INT_ENABLE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = start_id, j = 0; i < end_id; i++, j++) {                       \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[j])) {                         \
            tsb_name##_field_##tsb_field##_E_set(NULL, handle, link, i, enable); \
            int_en_table_ptr->struct_field[j] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_INT_RETRIEVE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = start_id, j = 0; i < end_id; i++, j++) {                       \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[j])) {                        \
            int_table_ptr->struct_field[j] = tsb_name##_field_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, link, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_STATUS_RETRIEVE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = start_id, j = 0; i < end_id; i++, j++) {                       \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[j])) {                        \
            int_table_ptr->struct_field[j] = tsb_name##_field_##tsb_field##_get(tsb_name##_buffer_ptr, handle, link, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_INT_CLEAR_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = start_id, j = 0; i < end_id; i++, j++) {                   \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[j])) {                 \
            tsb_name##_field_##tsb_field##_I_set_to_clear(NULL, handle, link, i, TRUE); \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_INT_CHECK_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = start_id, j = 0; i < end_id && FALSE == *int_found_ptr; i++, j++) {                   \
        if (TRUE == int_en_table_ptr->struct_field[j]) {                        \
            *int_found_ptr = tsb_name##_field_##tsb_field##_I_get(NULL, handle, link, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_INT_ENABLE_ARRAY_RANGE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = start_id, j = 0; i < end_id; i++, j++) {                   \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[j])) {                         \
            tsb_name##_field_range_##tsb_field##_E_set(NULL, handle, link, i, i, enable); \
            int_en_table_ptr->struct_field[j] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_INT_CLEAR_ARRAY_RANGE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                     \
    for (i = start_id, j = 0; i < end_id; i++, j++) {                   \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[j])) {                         \
            tsb_name##_field_range_##tsb_field##_I_set_to_clear(NULL, handle, link, i, i, TRUE); \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_INT_RETRIEVE_ARRAY_RANGE(handle, tsb_name, array_len, tsb_field, struct_field) \
    do {                                                                \
    for (i = start_id, j = 0; i < end_id; i++, j++) {                   \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[j])) {                        \
            int_table_ptr->struct_field[j] = tsb_name##_field_range_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, link, i, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_STATUS_RETRIEVE_ARRAY_RANGE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = start_id, j = 0; i < end_id; i++, j++) {                   \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[j])) {                        \
            int_table_ptr->struct_field[j] = tsb_name##_field_range_##tsb_field##_get(tsb_name##_buffer_ptr, handle, link, i, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define LINEOTN_INT_CHECK_ARRAY_RANGE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = start_id, j = 0; (i < end_id) && (FALSE == *int_found_ptr); i++, j++) { \
        if (TRUE == int_en_table_ptr->struct_field[j]) {                        \
            *int_found_ptr = tsb_name##_field_range_##tsb_field##_I_get(NULL, handle, link, i, i); \
        }                                                                       \
    }                                                                           \
} while(0)


/*
** Global variables
*/

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: lineotn_dsi_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for a DSI Stream.
*
* ELEMENTS:
*   dsi_rate                - DSI rate              
*   dsi_provisioned         - Indicates whether the stream is provisioned
*   dsi_activated_rx        - Indicates whether the RX path is activated
*   dsi_activated_tx        - Indicates whether the TX path is activated
*   dsi_fec_type            - Indicates the selected FEC type for the DSI stream
*   dsi_fec_pwr             - Indicates the power mode for the subsystem
*   fec_bypassed            - Indicates whether the FEC is bypassed
*   fec_stat_enabled        - Indicates whether the FEC statistics interface is enabled
*   scramble_in_opsm        - Indicates whether scrambling is enabled in the OPSM or OTU_FRM
*   dsi_hw_ifc              - Indicates the HW 
*   deskew_type;            - Indicates the selected deskew mode
*   dci_channel;            - Indicates the DCI channel number for this DSI stream
*   fec_dec_passthru;       - Indicates whether the FEC decoder has it's internal passthru mode enabled
*   bergen_enabled          - Indicates whether the BERGEN in the TX path is enabled
* 
*******************************************************************************/
typedef struct lineotn_dsi_cfg_t
{
    lineotn_frame_rate_t    dsi_rate;
    BOOL                    dsi_provisioned;
    BOOL                    dsi_activated_rx;
    BOOL                    dsi_activated_tx;
    lineotn_fec_t           dsi_fec_type;
    lineotn_power_mode_t    dsi_fec_pwr;
    BOOL                    fec_bypassed;
    BOOL8                   fec_stat_enabled;
    BOOL                    scramble_in_opsm;
    mld_ifc_type_t          dsi_hw_ifc;
    lineotn_deskew_type_t   deskew_type;
    UINT8                   dci_channel;
    BOOL                    fec_dec_passthru;
    BOOL8                   bergen_enabled;
} lineotn_dsi_cfg_t;


/*******************************************************************************
* STRUCTURE: lineotn_fec_stat_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration Structure for the FEC Statistics table 
*
* ELEMENTS:
*    fsl_table   - Table indicating which FEC statistics are exported over the external
*                  four wire I/F
*                  
*    enable      - Indicates whether the FEC statistics interface is enabled
*
*******************************************************************************/
typedef struct lineotn_fec_stat_cfg_t
{    
    UINT8  fsl_table[LINEOTN_FEC_STAT_MAX_FSL_TBL_ENTRIES];
    BOOL8  enable;
} lineotn_fec_stat_cfg_t;


/*******************************************************************************
* STRUCTURE: lineotn_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration Structure for LINEOTN Initial values
*
* ELEMENTS:
*    max_dsi_streams    - Indicates the maximum number of DSI streams the subsystem
*                         can process; used to differentiate between LINEOTN and
*                         SYSOTN mode
* 
*******************************************************************************/
typedef struct lineotn_cfg_t
{
    UINT32 max_dsi_streams;
} lineotn_cfg_t;


/*******************************************************************************
* STRUCTURE: lineotn_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration Structure for LINEOTN dynamic variables
*
* ELEMENTS:
*   mld_curr_num_otu_configured     - Bit 0-11:  Indicates the number of OTU streams which have
*                                                been configured
*                                     Bit 12-29: Reservred
*                                     Bit 30-31: Bit Indicates for TOP_INTERRUPT_ENABLE: TOP_INT_EN(0xA00004) 
*                                                on sysotn subsystem. 
                                                 No meaning for lineotn subsystem.
*   lineotn_dsi_dist                - Holds the configuration data for the DSI streams
*   lineotn_sfi51_0_frame_rate      - Indicates the frame rate for the SFI-5.1 #0 interface
*   lineotn_sfi51_1_frame_rate      - Indicates the frame rate for the SFI-5.1 #1 interface
*   current_lineotn_cfg             - Indicates which of the 19 configuration modes is used
*   fec_stat_cfg;                   - Holds the FEC statistics configuration data
*   lineotn_mode;                   - If TRUE: LINEOTN mode is enabled; If FALSE: SYSOTN mode is enabled
*   lineotn_start_state;            - Indicates whether the subsystem is in the start state
*   fw_ddeg;                        - Indicates whether FW dDEG is enabled
* 
*******************************************************************************/
typedef struct lineotn_var_t
{
    UINT32                  mld_curr_num_otu_configured;
    lineotn_dsi_cfg_t       lineotn_dsi_dist[LINE_OTN_MAX_DSI_STREAMS];
    lineotn_frame_rate_t    lineotn_sfi51_0_frame_rate;
    lineotn_frame_rate_t    lineotn_sfi51_1_frame_rate;
    lineotn_cfg_type_t      current_lineotn_cfg;
    lineotn_fec_stat_cfg_t  fec_stat_cfg;
    BOOL8                   lineotn_mode;
    BOOL8                   lineotn_start_state;
    BOOL8                   fw_ddeg;
} lineotn_var_t;


/*******************************************************************************
* STRUCTURE: lineotn_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Stores the recovered context for LINEOTN
*
* ELEMENTS:
*   var                 - configuration structure for the dynamic variables
*   fec_type_fuzzy      - Fuzzy state
*   oduk_type_fuzzy     - Fuzzy state 
*   oduksc_scbs3_page   - SCBS3 page configuration parameters  
* 
*******************************************************************************/
typedef struct lineotn_recover_var_t
{
    lineotn_var_t   var;
    BOOL8           fec_type_fuzzy[LINE_OTN_MAX_DSI_STREAMS];
    BOOL8           oduk_type_fuzzy[LINE_OTN_MAX_DSI_STREAMS];
    scbs3_page_t    oduksc_scbs3_page;
} lineotn_recover_var_t;


/*******************************************************************************
* STRUCTURE: lineotn_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   LINEOTN Handle Structure - contains static and dynamic variables and handles for sub-blocks. 
*
* ELEMENTS:
*   base - Holds the base address
*   cfg  - configuration structure for static variables
*   var  - configuration structure for dynamic variables
*   otu_frm_handle - Handle for the OTU-FRM sub-block.
* 
*******************************************************************************/
struct lineotn_handle_struct_t {
    pmc_handle_t  base;
    
    lineotn_cfg_t cfg;
    lineotn_var_t var;

    otu_frm_handle_t *otu_frm_handle;       
};

/*
** Function Prototypes
*/

PUBLIC void mld_var_init(lineotn_var_t *var_ptr);
PUBLIC void fec_var_init             ( lineotn_var_t *var_ptr);

PUBLIC PMC_ERROR fec_config_get(lineotn_handle_t       *lineotn_handle,
                                BOOL8                   is_lineotn_ss,
                                lineotn_fec_t          *fec_type,
                                lineotn_power_mode_t   *fec_pwr,
                                BOOL8                  *fec_bypassed,
                                BOOL8                  *fec_stat_enabled,
                                BOOL8                  *fec_dec_passthru,
                                BOOL8                  *fec_type_fuzzy);

PUBLIC void fec_stat_cfg_get(lineotn_handle_t          *lineotn_handle,
                             lineotn_fec_stat_cfg_t    *fec_stat_cfg_ptr);

PUBLIC void lineotn_mld_ctxt_reg_recover(lineotn_handle_t        *lineotn_handle,
                                         pmc_energy_state_t       top_energy_state_reg,         
                                         lineotn_recover_var_t   *lineotn_var_reg,                
                                         lineotn_energy_state_t  *lineotn_energy_state_reg);

PUBLIC void lineotn_mld_reg_cleanup(lineotn_handle_t    *lineotn_handle,
                                    UINT32               num_dsi_streams, 
                                    UINT32              *dsi_streams);

PUBLIC void lineotn_fec_reg_cleanup(lineotn_handle_t    *lineotn_handle,
                                    UINT32               num_dsi_streams, 
                                    UINT32              *dsi_streams);
    

PUBLIC BOOL8 fec_dec_status_get(lineotn_handle_t  *lineotn_handle,
                                UINT32             dsi_stream,
                                lineotn_fec_t      fec_mode);

PUBLIC PMC_ERROR lineotn_swizzle_fec_frame_slip_monitor_cfg(lineotn_handle_t *h, BOOL enable);

PUBLIC lineotn_chnl_rate_t lineotn_stream_rate_get(lineotn_frame_rate_t dsi_rate);

#ifdef __cplusplus
}
#endif

#endif /* _LINEOTN_LOC_H */

/* 
** end of file 
*/

