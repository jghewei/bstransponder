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

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

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
* STRUCTURE: digi_pgmrclk_div_params_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining DIGI PGMRCLK divider parameters.
*
* ELEMENTS: 
*   div_integer               - divider fractional division ration integer value
*   div_n                     - divider numerator
*   div_d                     - divider denominator
*  
* NOTE
*   For example to select source clock SFI5.1 TX[2] of clock divider 3: 
*   div_id = 3, clk_source =  DIGI_PGMRCLK_SFI_1_TX and clk_source_idx = 2
*******************************************************************************/
typedef struct digi_pgmrclk_div_params_t
{
    UINT32 div_integer;
    UINT32 div_n;
    UINT32 div_d;    
} digi_pgmrclk_div_params_t;


/*******************************************************************************
* STRUCTURE: digi_pgmrclk_div_cfg_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining DIGI PGMRCLK context.
*
* ELEMENTS:
*   valid          - array of boolean that specifies PRMGCLK configuration status.
*   pgmrclk_cfg    - array of PGMRCLK configurations.
*
* NOTE
*******************************************************************************/
typedef struct
{
    BOOL8                    valid[LAST_DIGI_PGMRCLK_DIV];
    digi_pgmrclk_div_cfg_t   pgmrclk_cfg[LAST_DIGI_PGMRCLK_DIV];
} digi_pgmrclk_ctxt_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
    
PUBLIC PMC_ERROR digi_pgmrclk_restart_init(digi_handle_t *digi_handle,
                                           util_global_restart_init_cfg_t *restart_init_cfg_ptr);

#ifdef __cplusplus
}
#endif

/* 
** end of file 
*/

