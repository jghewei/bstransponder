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
*   pv_accessors.c implements a set of functions that retrieve  
*   data from opaque structures.
*
*   NOTES:
*
*******************************************************************************/
#include "digi_loc.h"
#include "util_global.h"
#include "pv_accessors.h"


#define ERROR_RETURN_CODE   (0xffffffff)

/* util global requested fields */
#define    UTIL_GLOBAL_CPB_IS_INPUT_MAPPED     0
#define    UTIL_GLOBAL_CPB_IS_OUTPUT_MAPPED    1
#define    UTIL_GLOBAL_CPB_IS_ACTIVE           2
#define    UTIL_GLOBAL_CPB_CHAN                3
#define    UTIL_GLOBAL_ODUK_CHAN               4
#define    UTIL_GLOBAL_ODUK_IS_INPUT_MAPPED    5
#define    UTIL_GLOBAL_ODUK_IS_OUTPUT_MAPPED   6
#define    UTIL_GLOBAL_ODUK_IS_ACTIVE          7

/* otn server requested fields */
#define    OTN_SRVR_DCI_CHAN               0

/*******************************************************************************
* access_util_global_switch_data_def
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to dereference specific util_global_switch_data 
*   structure fields from a void pointer.
*
* INPUTS:
*   util_global_ptr        - void pointer to util_global_switch_data_def_t instance
*
*   req_field             - unsigned, code for a requested field value out of this structure:
*                           UTIL_GLOBAL_CPB_IS_INPUT_MAPPED     0
*                           UTIL_GLOBAL_CPB_IS_OUTPUT_MAPPED    1
*                           UTIL_GLOBAL_CPB_IS_ACTIVE           2
*                           UTIL_GLOBAL_CPB_CHAN                3
*                           UTIL_GLOBAL_ODUK_CHAN               4
*                           UTIL_GLOBAL_ODUK_IS_INPUT_MAPPED    5
*                           UTIL_GLOBAL_ODUK_IS_OUTPUT_MAPPED   6
*                           UTIL_GLOBAL_ODUK_IS_ACTIVE          7
*
* OUTPUTS:
*   None
* RETURNS:
*   unsigned, requested field structure, or,
*   ERROR_RETURN_CODE (0xffffffff)
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 access_util_global_switch_data_def(void *util_global_ptr,
                                                 UINT8 req_field)
{
    UINT32 ret_val;
    util_global_switch_data_def_t *tmp_ptr =(util_global_switch_data_def_t *)util_global_ptr;
   
    PMC_ENTRY();

    if( tmp_ptr == NULL )
        PMC_RETURN(ERROR_RETURN_CODE);

    switch(req_field)
    {
        case UTIL_GLOBAL_CPB_IS_INPUT_MAPPED:
            ret_val=(UINT32)tmp_ptr->cpb_data.op_state.map_state.is_input_mapped;
            break;
    
        case UTIL_GLOBAL_CPB_IS_OUTPUT_MAPPED:
            ret_val=(UINT32)tmp_ptr->cpb_data.op_state.map_state.is_output_mapped;
            break;

        case UTIL_GLOBAL_CPB_IS_ACTIVE:
            ret_val=(UINT32)tmp_ptr->cpb_data.op_state.active_state;
            break;

        case UTIL_GLOBAL_CPB_CHAN:
            ret_val=tmp_ptr->cpb_data.channel;
            break;

        case UTIL_GLOBAL_ODUK_CHAN:
            ret_val=tmp_ptr->oduk_data.channel;
            break;

        case UTIL_GLOBAL_ODUK_IS_INPUT_MAPPED:
            ret_val=tmp_ptr->oduk_data.op_state.map_state.is_input_mapped;
            break;

        case UTIL_GLOBAL_ODUK_IS_OUTPUT_MAPPED:
            ret_val=tmp_ptr->oduk_data.op_state.map_state.is_output_mapped;
            break;

        case UTIL_GLOBAL_ODUK_IS_ACTIVE:
            ret_val=tmp_ptr->oduk_data.op_state.active_state;
            break;

        default:
            ret_val = ERROR_RETURN_CODE;
            break;
    }
    
    PMC_RETURN(ret_val);
}

/*******************************************************************************
* access_otn_server_chnl_def
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to dereference specific otn_server_chnl_def 
*   structure fields from a void pointer.
*
* INPUTS:
*   otn_srvr_ptr          - void pointer to otn_server_chnl_def_t instance
*
*   req_field             - unsigned, code for a requested field value out of this structure:
*                           OTN_SRVR_DCI_CHAN                  0
*
* OUTPUTS:
*   None
* RETURNS:
*   unsigned, requested field structure, or,
*   ERROR_RETURN_CODE (0xffffffff)
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 access_otn_server_chnl_def (void *otn_srvr_ptr,
                                          UINT8 req_field)
{
    UINT32 ret_val;
    digi_otn_server_chnl_def_t *tmp_ptr =(digi_otn_server_chnl_def_t *)otn_srvr_ptr;
   
    PMC_ENTRY();

    if( tmp_ptr == NULL )
        PMC_RETURN(ERROR_RETURN_CODE);

    switch(req_field)
    {
        case OTN_SRVR_DCI_CHAN:
            ret_val=tmp_ptr->dci_chnl;
            break;
    
        default:
            ret_val = ERROR_RETURN_CODE;
            break;
    }

    PMC_RETURN(ret_val);

}


