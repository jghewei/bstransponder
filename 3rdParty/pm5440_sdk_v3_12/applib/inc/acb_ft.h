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
*     ACB FT block structs, enumeration, constants, and functions 
*     visible to its parent.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ACB_FT_H
#define _ACB_FT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_global.h"
#include "acb_ft_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: acb_ft_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify ACB_FT state 
*
* ELEMENTS:
*   ACB_FT_START         - start state
*   ACB_FT_EQUIPPED      - provisioned state
*   ACB_FT_OPERATIONAL   - activated and operational state
*           
*******************************************************************************/
typedef enum {
    ACB_FT_START        = 0,
    ACB_FT_EQUIPPED     = 1,
    ACB_FT_OPERATIONAL  = 2,
} acb_ft_state_t;


/*******************************************************************************
* ENUM: acb_ft_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining serdes ACB mode of operation\n\n
*
*
* ELEMENTS:
*    ACB_FT_MODE_NOMINAL     - ACB does not influence CSU offset
*    ACB_FT_MODE_B_ON_C      - ACB B input used to adjust CSU offset
*    ACB_FT_MODE_B_AND_C     - ACB B and C input used to adjust CSU
*                              offset
*    ACB_FT_MODE_C           - ACB C input used to adjust CSU offset
*    ACB_FT_MODE_LOOP_TIMING  - Loopback from RX side to TX side
*    ACB_FT_MODE_LINE_TIMING_125MHz - PTP Line timing PMM_LITE 125MHz reference
*    ACB_FT_MODE_LINE_TIMING_PGMRCLK - PGMRCLK Line timing PMM_LITE
*
*******************************************************************************/
typedef enum {
    ACB_FT_MODE_NOMINAL = 0,
    ACB_FT_MODE_B_ON_C,
    ACB_FT_MODE_B_AND_C,
    ACB_FT_MODE_C,
    ACB_FT_MODE_LOOP_TIMING,
    ACB_FT_MODE_LINE_TIMING_125MHz,
    ACB_FT_MODE_LINE_TIMING_PGMRCLK,
    LAST_ACB_FT_MODE      /* this is for out of range values error handling */

} acb_ft_mode_t;

/*******************************************************************************
* ENUM: acb_ft_client_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Structure for enumerated type for defining client type
*
*
* ELEMENTS:
*    ACB_FT_OTN           - OTN client 
*    ACB_FT_OTN_ODU4_ODU3 - ODU4/ODU3 OTN client
*    ACB_FT_DEMAPPED      - Demapped client 
*    ACB_FT_VIDEO         - Video client
*    ACB_FT_100GE_CBR     - 100GE CBR client
*
*******************************************************************************/
typedef enum {
    ACB_FT_OTN             = 300,
    ACB_FT_OTN_ODU4_ODU3   = 50,
    ACB_FT_DEMAPPED        = 30,
    ACB_FT_VIDEO           = 3,
    ACB_FT_100GE_CBR       = 100,
    LAST_ACB_FT_CLIENT_TYPE   /* this is for out of range values error handling */
} acb_ft_client_type_t;


/*******************************************************************************
* ENUM: acb_ft_serdes_if_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Structure for enumerated type for defining serdes ACB parent\n\n
*
*
* ELEMENTS:
*    ACB_FT_SERDES_32b   - ACB parent is a 32b serdes
*    ACB_FT_SERDES_20b   - ACB parent is a 20b subsystem
*    ACB_FT_SERDES_8b    - ACB parent is a 8b subsystem
*
*******************************************************************************/
typedef enum {
    ACB_FT_SERDES_32b = 4,
    ACB_FT_SERDES_20b = 2,
    ACB_FT_SERDES_8b  = 1,
    LAST_ACB_FT_SERDES_IF      /* this is for out of range values error handling */

} acb_ft_serdes_if_t;

/*******************************************************************************
* ENUM: acb_ft_parent_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Structure for enumerated type for defining serdes ACB parent\n\n
*
*
* ELEMENTS:
*    ACB_FT_S16     - ACB parent is S16 serdes
*    ACB_FT_SFI51   - ACB parent is SFI51 subsystem
*
*******************************************************************************/
typedef enum {
    ACB_FT_S16 = 0,
    ACB_FT_SFI51,
    LAST_ACB_FT_PARENT      /* this is for out of range values error handling */
} acb_ft_parent_t;

/*
** Constants
*/
/* Maximum number of ACB FT context that can be saved */
#define ACB_FT_MAX_NUM_CTXT 2

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: acb_ft_prov_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure that define parameters used to provision the ACB_FT. 
*
* ELEMENTS:
*   mode             - operation mode 
*   num_lane         - how many serdes lanes are used for the stream
*   serdes_if        - the size of the serdes interface
*   client_type      - client type for the stream
*   packet_per_int_C - Packet period from source server (also used for loopback mode)
*   packet_per_n_C   - Packet period from source server (also used for loopback mode)
*   packet_per_d_C   - Packet period from source server (also used for loopback mode)
*   packet_size_C    - Packet size from source server (also used for loopback mode)
*   packet_per_int_B - Packet period from source client
*   packet_per_n_B   - Packet period from source client
*   packet_per_d_B   - Packet period from source client
*   packet_size_B    - Packet size from source client
*   csu_div          - CSU divider in DOUBLE notation
*   csu_denomin      - CSU denominator value
*   ref_clk          - reference clock
*   max_ppm          - maximum PPM
*   master_serdes_if - ACB master serdes interface type
*******************************************************************************/
typedef struct acb_ft_prov_cfg_t
{
    acb_ft_mode_t           mode;    
    UINT8                   num_lane;
    acb_ft_serdes_if_t      serdes_if;
    acb_ft_client_type_t    client_type;
    UINT32                  packet_per_int_C;
    UINT32                  packet_per_n_C;
    UINT32                  packet_per_d_C;
    UINT32                  packet_size_C;
    UINT32                  packet_per_int_B;
    UINT32                  packet_per_n_B;
    UINT32                  packet_per_d_B;
    UINT32                  packet_size_B;
    DOUBLE                  capture_rate;
    DOUBLE                  transmit_rate;
    DOUBLE                  server_rate;
    DOUBLE                  csu_div;
    UINT64                  csu_denomin;  
    DOUBLE                  ref_clk;
    UINT32                  max_ppm; 
    acb_ft_serdes_if_t      master_serdes_if; 
} acb_ft_prov_cfg_t;

/*******************************************************************************
* STRUCTURE: acb_ft_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure that enumerates ACB FT variables saved in its context.
*
* ELEMENTS:
*   active            - TRUE when entry is valid, otherwise FALSE
*   mode              - operation mode 
*   cnt_n             - feedback clock counter DSM numerator 
*   cnt_d             - feedback clock counter DSM denominator 
*   pkt_size          - packet size
*   pkt_size_var      - packet size variation 
*   fb_wordsize       - feedback clock word size
*   int_gain          - the gain of the integral path through the ACB_FT 
*                        control loop's loop filter..
*   filt_gain         - the bandwidth of the low pass filter in the phase adaptation circuit
*   final_shift       - the value of the shift at the output of the control loop
*   pkt_period_int    - pkt period (interger part)
*   pkt_period_d_msb  - pkt period (denominator MSB)
*   pkt_period_d_lsb  - pkt period (denominator LSB)
*   pkt_period_n_msb  - pkt period (numerator MSB)
*   pkt_period_n_lsb  - pkt period (numerator LSB)
*   thermometer_int   - thermometer integer value
*   thermometer_d     - thermometer denominator value
*   thermometer_n     - thermometer numerator value
*   pll_s_max         - maximum value of the ACB_FT output to the CSU
*
*******************************************************************************/
typedef struct acb_ft_ctxt_struct_t {
    BOOL8 active;
    acb_ft_mode_t  mode;
    INT32 cnt_n;
    UINT32 cnt_d;
    UINT32 pkt_size:9;
    UINT32 pkt_size_var:1;
    UINT32 fb_wordsize:6;
    UINT32 int_gain:4;
    UINT32 filt_gain:4;
    UINT32 final_shift:4;
    UINT32 pkt_period_int:16;
    UINT32 pkt_period_d_msb:9; 
    UINT32 pkt_period_d_lsb; 
    UINT32 pkt_period_n_msb:9; 
    UINT32 pkt_period_n_lsb;
    UINT32 thermometer_int:4;
    UINT32 thermometer_d; 
    UINT32 thermometer_n; 
    UINT32 pll_s_max:5;
} acb_ft_ctxt_t;

/*******************************************************************************
* STRUCTURE: acb_ft_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This struct is to illustrate the dynamically changing context.
*
* ELEMENTS:
*  state         - block state
*  mode          - block current mode of operation
*  ctxt          - array of contexts. Only one context is used at the same time
*                  in runtime.
*
*******************************************************************************/
typedef struct acb_ft_var_struct_t {
    acb_ft_state_t state;
    acb_ft_mode_t  mode;
    acb_ft_ctxt_t  *ctxt;
} acb_ft_var_t;

/*******************************************************************************
* STRUCTURE: acb_ft_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure that enumerates ACB FT handle fields.
*
* ELEMENTS:
*   See typedef
*
*******************************************************************************/
typedef struct acb_ft_handle_struct_t {
    pmc_handle_t base;
    acb_ft_var_t var;    
} acb_ft_handle_struct_t;

/*! ACB FT handle used by its parent */
typedef struct acb_ft_handle_struct_t acb_ft_handle_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC acb_ft_handle_t *acb_ft_ctxt_create(pmc_handle_t      *parent, 
                                           UINT32             base_address, 
                                           pmc_sys_handle_t  *sys_handle, 
                                           const char        *tsb_name);

PUBLIC void acb_ft_ctxt_destroy(acb_ft_handle_t *acb_ft_handle);

PUBLIC void acb_ft_handle_init(acb_ft_handle_t  *acb_ft_handle, 
                               acb_ft_parent_t   parent);        
                    
PUBLIC PMC_ERROR acb_ft_handle_restart_init(acb_ft_handle_t  *acb_ft_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                            pmc_energy_state_t energy_state);                                        

PUBLIC UINT32 acb_ft_base_address_get(acb_ft_handle_t *acb_ft_handle);

PUBLIC PMC_ERROR acb_ft_prov(acb_ft_handle_t    *acb_ft_handle, 
                             acb_ft_prov_cfg_t  *cfg);

PUBLIC PMC_ERROR acb_ft_deprov(acb_ft_handle_t *acb_ft_handle);

PUBLIC void acb_ft_cleanup(acb_ft_handle_t *acb_ft_handle);
    
PUBLIC PMC_ERROR acb_ft_activate(acb_ft_handle_t *acb_ft_handle);

PUBLIC PMC_ERROR acb_ft_deactivate(acb_ft_handle_t *acb_ft_handle);

PUBLIC PMC_ERROR acb_ft_context_push(acb_ft_handle_t *acb_ft_handle,
                                     UINT8            ctxt_idx);

PUBLIC PMC_ERROR acb_ft_context_pop(acb_ft_handle_t *acb_ft_handle,
                                     UINT8           ctxt_idx);

PUBLIC BOOL8 acb_ft_provisionned(acb_ft_handle_t *acb_ft_handle);

PUBLIC PMC_ERROR acb_ft_sfi51_loop_timing_cfg(acb_ft_handle_t *acb_ft_handle, 
                                              UINT32           pkt_period_int, 
                                              UINT32           pkt_period_size);

PUBLIC PMC_ERROR acb_ft_s16_loop_timing_activate(acb_ft_handle_t *acb_ft_handle);

PUBLIC PMC_ERROR acb_ft_s16_loop_timing_deactivate(acb_ft_handle_t *acb_ft_handle);

PUBLIC void acb_ft_context_copy(acb_ft_handle_t *acb_ft_handle, 
                                UINT8            from_idx, 
                                UINT8            to_idx);

PUBLIC void acb_ft_pll_s_max_cfg(acb_ft_handle_t *acb_ft_handle,
                                 UINT32           pll_s_max,
                                 UINT64           csu_n,
                                 UINT64           csu_d,
                                 UINT32           csu_r);

PUBLIC void acb_ft_flush_context(acb_ft_handle_t  *acb_ft_handle,
                                 UINT8          ctxt_idx);

PUBLIC PMC_ERROR  acb_ft_reprov_pkt_size_configs(acb_ft_handle_t    *acb_ft_handle, 
                                                 acb_ft_prov_cfg_t  *cfg_ptr);

PUBLIC void acb_ft_restart(acb_ft_handle_t *acb_ft_handle,
                           BOOL8            is_s16_parent);


PUBLIC void acb_ft_cntn_cntd_recalculate_and_cfg(acb_ft_handle_t    *acb_ft_handle, 
                                                 UINT8               master_lane_bus_width,
                                                 UINT8               lane_bus_width);
#ifdef __cplusplus
}
#endif

#endif /* _ACB_FT_H */

/*
** end of file
*/
