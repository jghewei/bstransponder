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
#ifndef _ENET_LOC_H
#define _ENET_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "enet.h"
#include "enet_io.h"
#include "enet_mtsb_map.h"

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
* STRUCTURE: enet_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining ENET energy state
*
* ELEMENTS: 
*   epmm_mpmo   - EPMM MPMO energy state
*   epmm_mpma   - EPMM MPMA energy state
*
*******************************************************************************/    
typedef struct enet_energy_state_struct_t {     
    pmc_energy_state_t etrans;   
    pmc_energy_state_t emac;   
    pmc_energy_state_t epmm;   
    pmc_energy_state_t empei_i;   
    pmc_energy_state_t empei_e;
    
} enet_energy_state_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/  
PUBLIC enet_data_rate_t enet_convert_datapath_to_enet_data_rate(enet_datapath_t datapath);
   
#ifdef __cplusplus
}
#endif

#endif /* _ENET_LOC_H */

/* 
** end of file 
*/

