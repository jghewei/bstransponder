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
#ifndef _SDH_PMG_LOC_H
#define _SDH_PMG_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"

#include "sdh_pmg.h"    
#include "sdh_pmg_ind.h"
#include "util_global.h"

#include "sdh_pmg_io.h"
#include "rrmp_io.h"
#include "srli_768_io.h"
#include "rttp_io.h"
#include "sber_io.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#define SDH_PMG_MAX_INDEX                   4
#define SDH_PMG_RRMP_SLICES_PER_INDEX       4
#define SDH_PMG_RRMP_MAX_INDEX              16
#define LAST_SDH_PMG_SBER_STS_MODES         2
#define SDH_PMG_SBER_STS_192                0
#define SDH_PMG_SBER_STS_768                1

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: sdh_pmg_sber_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure to store SBER config values.
*
* ELEMENTS:
*   set_threshold   - Register value for desired set threshold in sber_tbl
*   clr_threshold   - Register value for desired clear threshold in sber_tbl
*   int_per_2       - Integration period register value
*   int_per_1       - Integration period register value
*
*******************************************************************************/
typedef struct {
    UINT32      set_threshold;
    UINT32      clr_threshold;
    UINT32      int_per_2;
    UINT32      int_per_1;
} sdh_pmg_sber_info_t;



/*******************************************************************************
* STRUCTURE: sdh_pmg_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for inital values.
*
* ELEMENTS:
*   None.
*
*******************************************************************************/
typedef struct {
    
} sdh_pmg_cfg_t;



/*******************************************************************************
* STRUCTURE: sdh_pmg_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context.
*
* ELEMENTS:
*   mode        - Configured mode for the link (e.g. OC-192, OC-768)
*   rttp_algo   - Configured algorithm for tail trace matching
*   rttp_len    - Configured length of tail trace message
*   sber_byte   - B1 or B2 byte to monitor for BER thresholds
*   sber_ber    - Configured thresholds for SBER to declare SD/SF.
*   
*
*******************************************************************************/
typedef struct {
    
    sdh_pmg_sts_mode_t      mode[SDH_PMG_MAX_INDEX];
    sdh_pmg_rttp_algo_t     rttp_algo[SDH_PMG_MAX_INDEX];
    sdh_pmg_rttp_length_t   rttp_len[SDH_PMG_MAX_INDEX];
    sdh_pmg_sber_byte_t     sber_byte;
    sdh_pmg_sber_ber_t      sber_ber[LAST_SDH_PMG_SBER_ALARM][SDH_PMG_MAX_INDEX];
      
} sdh_pmg_var_t;



/*******************************************************************************
* STRUCTURE: sdh_pmg_handle_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining SDH PMG handle information.
*
* ELEMENTS: 
*   base                - Handle base information
*   cfg                 - Static config structure
*   var                 - Variable config structure
*
*******************************************************************************/    
struct sdh_pmg_handle_struct_t {
    pmc_handle_t base;
    
    sdh_pmg_cfg_t cfg;
    sdh_pmg_var_t var;
       
};

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR sdh_pmg_prov_reset(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                    UINT32               index,
                                    BOOL                 reset);

#ifdef __cplusplus
}
#endif

#endif /* _SDH_PMG_LOC_H */

/* 
** end of file 
*/

