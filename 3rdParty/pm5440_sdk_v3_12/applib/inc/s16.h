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
*   DESCRIPTION :
*
*   NOTES:
*
*******************************************************************************/

#ifndef _S16_H
#define _S16_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "s16_api.h"
#include "acb_ft.h"
#include "acb_ft_api.h"
#include "util_global_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: s16_port_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify S16 port state 
*
* ELEMENTS:
*   S16_PORT                                        - in reset state
*   S16_PORT_START                                  - start state
*   S16_PORT_EQUIPPED                               - provisioned state
*   S16_PORT_OPERATIONAL                            - activated and operational state
*   S16_PORT_OPERATIONAL_LOOPBACK                   - activated and operational state (in loopback mode)
*   S16_PORT_OPERATIONAL_FORCE_NOMINAL              - activated and operational state (in loopback mode)
*   S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL - activated and operational state (in loopback mode)
*           
*******************************************************************************/
typedef enum
{
    S16_PORT_RESET        = 0,
    S16_PORT_START        = 1,
    S16_PORT_EQUIPPED     = 2,
    S16_PORT_OPERATIONAL  = 3,
    S16_PORT_OPERATIONAL_LOOPBACK  = 4,
    S16_PORT_OPERATIONAL_FORCE_NOMINAL  = 5,
    S16_PORT_OPERATIONAL_LOOPBACK_AND_FORCE_NOMINAL  = 6,
    LAST_S16_PORT_STATE
} s16_port_state_t;

/*******************************************************************************
* ENUM: s16_loopback_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying S16 loopback mode.
*
* ELEMENTS:
*           S16_FACILITY_LOOPBACK             - S16 facility loopback
*           S16_DIAGNOSTIC_LOOPBACK           - S16 diagnostic loopback
*           S16_DIAGNOSTIC_WITH_TX_LOOPBACK   - S16 diagnostic loopback with data
*                                               transmitted through TX interface
*           S16_ACB_ONLY_LOOPBACK             - S16 acb only loopback
*
*******************************************************************************/
typedef enum
{
    S16_FACILITY_LOOPBACK    = 0,
    S16_DIAGNOSTIC_LOOPBACK,
    S16_ACB_ONLY_LOOPBACK,
    S16_LOOPBACK_MODE_UNUSED,
    S16_DIAGNOSTIC_WITH_TX_LOOPBACK
} s16_loopback_mode_t;

/*******************************************************************************
* ENUM: s16_port_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   Enumeration of the S16 serdes port major modes of operation. \n\n
*
*   Terms take on the following scheme: \n
*   S16_PORT_MODE_[serdes_type]_[mode] \n
*   Where:\n
*   [serdes_type]: specifies the serdes instatiation type\n
*   [mode]: pecifies the desired rate of operation or the unused lowest power 
*   consumption state of a serdes port.\n\n
*
*
* ELEMENTS:
*   See Typedef.
*******************************************************************************/
typedef enum
{
    S16_PORT_MODE_2G488    = 1,
    S16_PORT_MODE_3G125    = 2,
    S16_PORT_MODE_4G250    = 3,
    S16_PORT_MODE_4G915    = 4,
    S16_PORT_MODE_5G000    = 5,
    S16_PORT_MODE_6G144    = 6,
    S16_PORT_MODE_6G250    = 7,
    S16_PORT_MODE_8G500    = 8,
    S16_PORT_MODE_9G830    = 9,
    S16_PORT_MODE_9G953    = 10,

    S16_PORT_MODE_10G000   = 11,
    S16_PORT_MODE_10G037   = 12,
    S16_PORT_MODE_10G312   = 13,
    S16_PORT_MODE_10G312_DIC = 14, 
    S16_PORT_MODE_10G355   = 15,
    S16_PORT_MODE_10G399   = 16,
    S16_PORT_MODE_10G518   = 17,
    S16_PORT_MODE_10G562   = 18,
    S16_PORT_MODE_10G607   = 19,
    S16_PORT_MODE_10G709   = 20,
    S16_PORT_MODE_10G754   = 21,
    S16_PORT_MODE_10G925   = 22,

    S16_PORT_MODE_11G049   = 23,
    S16_PORT_MODE_11G095   = 24,
    S16_PORT_MODE_11G142   = 25,
    S16_PORT_MODE_11G145   = 26,
    S16_PORT_MODE_11G180   = 27,
    S16_PORT_MODE_11G182   = 28,
    S16_PORT_MODE_11G200   = 29,
    S16_PORT_MODE_11G270   = 30,
    S16_PORT_MODE_11G317   = 31,
    S16_PORT_MODE_11G319   = 32,
    S16_PORT_MODE_11G320   = 33,
    S16_PORT_MODE_11G32252 = 34,
    S16_PORT_MODE_11G32259 = 35,
    S16_PORT_MODE_11G32275 = 36,
    S16_PORT_MODE_11G500   = 37,
    S16_PORT_MODE_12G500   = 38,
    LAST_S16_PORT_MODE      /* this is for out of range values error handling */
} s16_port_mode_t;

/*******************************************************************************
* ENUM: s16_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumeration of the S16 Interfaces.
*
* ELEMENTS:
*   S16_LINE               - S16 line side
*   S16_SYSTEM             - S16 system side 
*
*******************************************************************************/
typedef enum
{
    S16_LINE    = 0,
    S16_SYSTEM  = 1,
} s16_type_t;

/*******************************************************************************
* ENUM: s16_mld_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumeration of the S16 MLD modes.
*
* ELEMENTS:
*           S16_MASTER             - S16 master
*           S16_SLAVE              - S16 slave
*
*******************************************************************************/
typedef enum
{
    S16_MASTER = 0,
    S16_SLAVE,
    LAST_S16_MLD_MODE      /* this is for out of range values error handling */
} s16_mld_mode_t;

/*******************************************************************************
* ENUM: s16_clk_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumeration of the S16 clock type.
*
* ELEMENTS:
*           S16_CLK_CFG_NOT_CLK_LANE  - S16 is not used as a clock lane
*           S16_CLK_CFG_AS_SLAVE      - S16 used as a clock will be configured as 
*                                       a slave lane
*           S16_CLK_CFG_AS_MASTER     - S16 used as a clock will be configured as
*                                        a master lane
*
*******************************************************************************/
typedef enum
{
    S16_CLK_CFG_NOT_CLK_LANE = 0,    
    S16_CLK_CFG_AS_SLAVE = 1,
    S16_CLK_CFG_AS_MASTER = 2,
    LAST_S16_CLK_CFG /* this is for out of range values error handling */
} s16_clk_cfg_t;


/*
** Constants
*/
#define S16_LPBK_ACB_FT_CTXT_IDX 0
#define S16_FORCE_NOMINAL_ACB_FT_CTXT_IDX 1
#define S16_ACB_FT_CTXT_DEFAULT_IDX S16_LPBK_ACB_FT_CTXT_IDX

/*
** Macro Definitions
*/
#define S16_INT_TABLE_DEFINE() \
    S16_INT(acb_ft_handle, acb_ft,           ACB_FAIL    , acb_fail_i    ); \

#define S16_STATUS_TABLE_DEFINE() \
    S16_STATUS(acb_ft_handle, acb_ft,        ACB_FAIL_V    , acb_fail_v    ); \

/* Maximum number of ACB FT context backups */
#define S16_ACB_FT_CTXT_MAX_NUM_BACKUP       2
/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: s16_loopback_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   S16 loopback configuration
*
*   
* ELEMENTS:
*     loopback_mode - the mode
*     serdes_rate   - serdes rate (only for FACILITY LOOPBACK)
*     is_otu_rate   - indicate if we have a OTU rate (only for FACILITY LOOPBACK)
*     serdes_if     - serdes interface type: OTN or ENET
*       
*******************************************************************************/
typedef struct s16_loopback_cfg_t
{
    s16_loopback_mode_t loopback_mode;
    DOUBLE serdes_rate;
    BOOL8 is_otu_rate;    
    acb_ft_serdes_if_t serdes_if;
} s16_loopback_cfg_t;

/*******************************************************************************
* STRUCTURE: s16_ctxt_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    S16 context variables. 
*
* ELEMENTS:
*  active          - S16 activation status
*  tx_source       - transmit data source selector    
*  t_piso_ctrl     - transmit piso controler value
*  mtsb_bus_width  - MTSB BUS width: 20b, 32b or 8b
*  polarity_inv    - serdes polarity inversion
*  lpbk_cfg_valid  - Valid indicator
*  lpbk_cfg        - Loopback configuration
*
*******************************************************************************/
typedef struct s16_ctxt_struct_t
{
    BOOL8 active;
    UINT8 tx_source;
    UINT8 t_piso_ctrl;
    UINT8 mtsb_bus_width;
    util_global_direction_t polarity_inv;
    BOOL8 lpbk_cfg_valid;
    s16_loopback_cfg_t lpbk_cfg;
} s16_ctxt_t;

/*******************************************************************************
* STRUCTURE: s16_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure defines S16 context variables that are only set during handle 
*    initialization. 
*
* ELEMENTS:
*    s16_type            - System or Line serdes
*    num_serdes_instance - How many serdes instance in the group of serdes
*
*******************************************************************************/
typedef struct
{
    s16_type_t s16_type;
    UINT8 num_serdes_instance;
} s16_cfg_t;


/*******************************************************************************
* STRUCTURE: s16_var_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This struct is for dynamically changing context
*
* ELEMENTS:
*    mld_mode                        - It is a slave or a master in a MLD configuration    
*    has_acb                         - It use ACB 
*    state                           - Current activation state 
*    patt_gen_enabled                - Pattern generator is enabled
*    loopback_mode                   - The enabled loopback
*    prev_prev_state                 - The state before previous state
*    prev_state                      - The previous state
*    has_lpbk_acb_ft_backup          - ACB configuration has been backup for loopback cfg
*    has_force_nominal_acb_ft_backup - ACB configuration has been backup for loopback cfg
*    s16_ctxt                        - An stored s16 context (used to restore state 
*                                      when we enable the pattern generator)
*    clk_cfg                         - Indicate that the serdes insatcne is used as a \n
*                                      a clock
*******************************************************************************/
typedef struct
{
    s16_mld_mode_t      mld_mode;          
    BOOL8               has_acb;           
    s16_port_state_t    state;             
    BOOL8               patt_gen_enabled;  
    s16_loopback_mode_t loopback_mode;     
    s16_port_state_t    prev_prev_state;     
    s16_port_state_t    prev_state;        
    BOOL8               has_lpbk_acb_ft_backup; 
    BOOL8               has_force_nominal_acb_ft_backup; 
    s16_ctxt_t          s16_ctxt;      
    UINT8               clk_cfg;
} s16_var_t;


/*******************************************************************************
* STRUCTURE: s16_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   S16 handle structure. A S16 block is the set of S16 serdes on either the 
*   LINE or SYSTEM side.
*
* ELEMENTS:
*   See typedef    
*******************************************************************************/
typedef struct s16_handle_struct_t
{ 
    pmc_handle_t base;
    
    s16_cfg_t cfg;
    s16_var_t *var; 

    /* ACB_FT handles */
    acb_ft_handle_t **acb_ft_handle; 
    
} s16_handle_struct_t;

/*******************************************************************************
* STRUCTURE: s16_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generic naming of S16 handle struct.
*
* ELEMENTS:
*   NONE.    
*******************************************************************************/
typedef struct s16_handle_struct_t s16_handle_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC s16_handle_t *s16_ctxt_create(pmc_handle_t *parent, 
                                     UINT32 base_address, 
                                     pmc_sys_handle_t *sys_handle, 
                                     const char *tsb_name, 
                                     s16_type_t s16_type);
PUBLIC void s16_ctxt_destroy(s16_handle_t *s16_handle);

PUBLIC void s16_handle_init(s16_handle_t *s16_handle);

PUBLIC PMC_ERROR s16_prov(s16_handle_t *s16_handle, 
                          UINT8 port, 
                          UINT8 master_port, 
                          acb_ft_prov_cfg_t *cfg_ptr,
                          s16_clk_cfg_t clk_cfg);

PUBLIC PMC_ERROR s16_deprov(s16_handle_t *s16_handle, UINT8 port);

PUBLIC PMC_ERROR s16_activate(s16_handle_t *s16_handle, 
                              UINT8 port,
                              BOOL8 activate_acb);

PUBLIC PMC_ERROR s16_deactivate(s16_handle_t *s16_handle, UINT8 port);   

PUBLIC void s16_cleanup(s16_handle_t *s16_handle, UINT8 lane);             
            
PUBLIC PMC_ERROR s16_patt_mon_cfg(s16_handle_t *handle,
                                  UINT8 serdes_id,
                                  s16_port_mode_t s16_mode,
                                  util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);

PUBLIC PMC_ERROR s16_all_ones_zeroes_detection_cfg(s16_handle_t *handle,
                                                   UINT8         serdes_id,
                                                   BOOL8         enable,
                                                   BOOL8         detect_all_ones);

PUBLIC PMC_ERROR s16_patt_mon_resync(s16_handle_t *handle,
                                     UINT8 serdes_id);

PUBLIC PMC_ERROR s16_patt_mon_err_count_get(s16_handle_t          *handle,
                                            UINT8                  serdes_id,
                                            util_patt_status_t    *status,
                                            UINT32                *err_count);

PUBLIC PMC_ERROR s16_patt_gen_cfg(s16_handle_t *handle,
                                  UINT8 serdes_id,
                                  s16_port_mode_t port_mode,
                                  util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);

PUBLIC PMC_ERROR s16_patt_gen_err_insert_cfg(s16_handle_t *handle,
                                             UINT8 serdes_id);

PUBLIC PMC_ERROR s16_loopback_activate(s16_handle_t *handle,
                                       UINT8 serdes_id,
                                       s16_loopback_cfg_t *cfg_ptr);

PUBLIC PMC_ERROR s16_loopback_deactivate(s16_handle_t        *handle,
                                         UINT8                serdes_id);

PUBLIC PMC_ERROR s16_loopback_status_get(s16_handle_t *handle,
                                         UINT8 serdes_id,
                                         s16_loopback_mode_t *loopback_mode_ptr);

PUBLIC void s16_polarity_cfg(s16_handle_t             *handle,
                             UINT8                     serdes_id,
                             util_global_direction_t   dir,
                             BOOL8                     enable);

PUBLIC PMC_ERROR s16_polarity_get(s16_handle_t             *handle,
                                  UINT8                     serdes_id,
                                  util_global_direction_t   dir,
                                  BOOL8                    *polarity_inv);

PUBLIC PMC_ERROR s16_tx_swing_emphasis_levels_set(s16_handle_t        *handle,
                                                  UINT8                serdes_id,
                                                  s16_tx_swing_levels_t  *levels_cfg_ptr);

PUBLIC PMC_ERROR s16_dc_coupling_set(s16_handle_t           *handle,
                                     UINT8                  serdes_id,
                                     BOOL8                  dc_coupling_enabled);


PUBLIC void s16_tx_refclk_cfg(s16_handle_t *handle,
                              UINT8         serdes_id,
                              BOOL8         div16_enable);


PUBLIC void s16_tx_squelch_enable(s16_handle_t *handle,
                                  UINT8 serdes_id);

PUBLIC void s16_tx_squelch_disable(s16_handle_t *handle,
                                   UINT8 serdes_id);

PUBLIC PMC_ERROR s16_int_chnl_enable(s16_handle_t    *s16_handle,
                                     UINT32           serdes_id,
                                     s16_int_pin_t   *int_table_ptr,
                                     s16_int_pin_t   *int_en_table_ptr,
                                     BOOL             enable);

PUBLIC PMC_ERROR s16_int_chnl_clear(s16_handle_t   *s16_handle,
                                    UINT32          serdes_id,
                                    s16_int_pin_t  *int_table_ptr,
                                    s16_int_pin_t  *int_en_table_ptr);

PUBLIC PMC_ERROR s16_int_chnl_retrieve(s16_handle_t   *s16_handle,
                                       UINT32          serdes_id,
                                       s16_int_pin_t  *filt_table_ptr,
                                       s16_int_pin_t  *int_table_ptr);

PUBLIC PMC_ERROR s16_int_chnl_enabled_check(s16_handle_t     *s16_handle,
                                             UINT32           serdes_id,
                                             s16_int_pin_t   *int_en_table_ptr,
                                             BOOL            *int_found_ptr);

PUBLIC void s16_max_ppm_set(s16_handle_t   *s16_handle,
                            UINT8           serdes_id,
                            UINT32          max_ppm);

PUBLIC void s16_csu_div_val_get(s16_handle_t *s16_handle, 
                                 UINT8 lane, 
                                 DOUBLE *csu_div, 
                                 UINT64 *csu_n, 
                                 UINT64 *csu_denomin, 
                                 UINT32 *csur);

PUBLIC PMC_ERROR s16_handle_restart_init(s16_handle_t                   *s16_handle,
                                         util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                         UINT32                          s16_reset_msk,
                                         UINT32                          skip_msk);

PUBLIC PMC_ERROR s16_ext_ctxt_state_set(s16_handle_t      *s16_handle, 
                                        UINT16             lane_msk,
                                        BOOL8              activate);

PUBLIC BOOL8 s16_reset_state_get(s16_handle_t      *s16_handle, 
                                 UINT16             lane_msk) ;

PUBLIC PMC_ERROR s16_restart(s16_handle_t *s16_handle, 
                             UINT8         lane);

PUBLIC PMC_ERROR  s16_vert_hist_get(s16_handle_t     *s16_handle,
                                    UINT32            lane_0_19_msk,
                                    util_vhist_test_length_t  test_length,
                                    UINT64            userdef_length,
                                    UINT64            bin_buffer[][64]);

PUBLIC BOOL8 s16_tx_squelch_enable_status_get(s16_handle_t *handle,
                                              UINT8 serdes_id);

PUBLIC s16_loopback_mode_t s16_loopback_cfg_get(s16_handle_t *handle, 
                                                UINT8         serdes_id);

PUBLIC PMC_ERROR s16_as_a_clock_facilty_loopback_cfg(s16_handle_t       *handle,
                                                     UINT8               serdes_id,
                                                     UINT8               master_lane,
                                                     BOOL8               enable);


#ifdef __cplusplus
}
#endif

#endif /* _S16_H */

/*
** end of file
*/
