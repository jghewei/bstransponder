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
#ifndef _ECLASS_LOC_H
#define _ECLASS_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "stdlib.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "eclass.h"
#include "eclass_io.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define ECLASS_COMB_AND_ARRAY_MAX_SIZE 40
#define ECLASS_FOUT_COMB_OROUT_1_ARRAY_MAX_SIZE 26
#define ECLASS_COMB_OROUT_FILTAND_EN_MSB_ARRAY_MAX_SIZE 26
#define ECLASS_COMB_ORIN_OUT_INV_ARRAY_MAX_SIZE 16
#define ECLASS_F16_ARRAY_MAX_SIZE 24
#define ECLASS_FANDS_ARRAY_MAX_SIZE 12
#define ECLASS_FOUT_X_FAND_ARRAY_MAX_SIZE 12

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: eclass_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Contains ECLASS context parameters.
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef struct eclass_ctxt_struct_t
{    
    eclass_pkt_extract_cfg_t  extract_cfg_tbl[ECLASS_NUM_LINKS][ECLASS_NUM_SUPPORTED_PKT_PER_LINK];    
} eclass_ctxt_t;

/*******************************************************************************
* STRUCTURE: eclass_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef struct {
/*    
    UINT32 num_eclass_chans;
*/    
} eclass_cfg_t;


/*******************************************************************************
* STRUCTURE: eclass_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context. 
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef struct {
   
    BOOL eclass_link_en[ECLASS_NUM_LINKS];
    eclass_ctxt_t      eclass_ctxt;
} eclass_var_t;


/*******************************************************************************
* STRUCTURE: eclass_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct defines ECLASS module properties. 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context
*
*******************************************************************************/
typedef struct eclass_handle_struct_t {
    pmc_handle_t base;    
    eclass_cfg_t cfg;
    eclass_var_t var;    
} eclass_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
          

#ifdef __cplusplus
}
#endif

#endif /* _ECLASS_LOC_H */

/* 
** end of file 
*/

