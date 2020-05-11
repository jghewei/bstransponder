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
*   DESCRIPTION:
*    This file contains PMC system access functions that map to the system level
*    calls. This file is setup to support customer systems as well as the PMC
*    EVBD, emulation, simulation, and verification environment. There is no
*    customer porting required in this file that occurs in pmc_customer_os.h and
*    pmc_customer_os.c.
*
*   NOTES:
*
*
******************************************************************************/

/*---- Compile Options -----------------------------------------------------*/

/*---- Include Files -------------------------------------------------------*/
#include "pmc_sys.h"


/*---- Extern Declarations -------------------------------------------------*/

/*---- Private Constant and Type Declarations ------------------------------*/
/*---- glocal variables -------------------------------------------------*/

/*! Flag to indicate is stats have been initialized */
static int pmc_os_stats_initialized = FALSE; 

#ifdef PMC_TESTS
/*! Crash event */
pmc_sys_crash_event_t g_crash_event = {0};
#endif

/*! Stats */
pmc_os_stats_t digi120_pmc_os_stats; 

/*! Stats as of last call to pmc_sys_stat_show_all */
PRIVATE pmc_os_stats_t pmc_os_stats_show_all_snapshot = {0}; 

/*! Stats pointer */
PRIVATE pmc_os_stats_t * pmc_os_stats_ptr; 

#ifndef PMC_NO_ATOMIC_FUNCTION_STATS
/*! Stats for atomic regions */
static pmc_sys_stat_t pmc_atomic_stat[1] = {{0,0,"pmc_atomic",""}};
#endif

/*! Flag to indicate if tracking atomic region */
static int pmc_atomic_attach = TRUE; 

/*! linked list of all pmc_sys_stat_t */
static pmc_sys_stat_t *gbl_all_stats=NULL;

/*! Flag to indicate if to use 32 bit floats where possible */
/*! On certain customer platforms, in case 'double' calcuation is inefficient, then try best to use 'float' instead. */
PRIVATE BOOL8 digi_use_float32_mode = FALSE;

#ifndef DOXYGEN_PUBLIC_ONLY
void pmc_sys_stat_atomic_mark_end(pmc_sys_stat_t *s);
#endif /* DOXYGEN_PUBLIC_ONLY */

/****************************************************************************
*
*   pmc_lof_monitoring_in_fw
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 LOF related FW calls.
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_lof_monitoring_in_fw(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}


/****************************************************************************
*  pmc_enable_otu4_clock_cfg_in_fw
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 OTU4 Clock configuration in firmware
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_enable_otu4_clock_cfg_in_fw(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}


/****************************************************************************
*  pmc_coreotn_odu_rcp_enable_all_chnls
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 enabling all RCP channels
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_odu_rcp_enable_all_chnls(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}


/****************************************************************************
*
*   pmc_coreotn_mux_empty_ho_order_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 empty High Order workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_mux_empty_ho_order_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}


/****************************************************************************
*
*   pmc_coreotn_dmx_empty_ho_order_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 empty High Order workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_dmx_empty_ho_order_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}

/****************************************************************************
*
*   pmc_coreotn_schd_extra_channel_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 extra scheduler channel workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_schd_extra_channel_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}

/****************************************************************************
*
*   pmc_asymmetric_msi_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 asymetric smi workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_asymmetric_msi_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}


/****************************************************************************
*
*   pmc_coreotn_enable_odujat_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 ODUJAT enable workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_enable_odujat_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;
    
    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}


/****************************************************************************
*
*   pmc_lineotn_remove_null_chnl_id_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 LineOTN NULL Channel ID workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_lineotn_remove_null_chnl_id_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }    

    PMC_RETURN(result);
}


/****************************************************************************
*
*   pmc_dcpb_one_trib_slot_min_buffer_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 CPB Trib Slot workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_dcpb_one_trib_slot_min_buffer_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }    

    PMC_RETURN(result);
}

/****************************************************************************
*
*   pmc_mpmo_mpma_reg_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 MPMO revision to ensure the correct register is set.
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_mpmo_mpma_reg_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}


/****************************************************************************
*
*   pmc_is_digi_rev_a_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 REV A only purposes
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_is_digi_rev_a_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }    

    PMC_RETURN(result);
}

/****************************************************************************
*
*   pmc_cpb_reset_pointer_access_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 synchronizing CPB reset pointer. 
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV B
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_cpb_reset_pointer_access_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_B)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
}


/****************************************************************************
*   pmc_cpb_pmpm_block_remap
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 CPB page swap feature.
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV A
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_cpb_pmpm_block_remap(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code != DIGI_REVISION_CODE_REV_A)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* pmc_cpb_pmpm_block_remap */


/****************************************************************************
*
*   pmc_coreotn_mux_empty_ho_keep_in_sync_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 COREOTN MUX empty High Order workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV B
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_mux_empty_ho_keep_in_sync_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_B)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* pmc_coreotn_mux_empty_ho_keep_in_sync_revision */


/****************************************************************************
*
*   pmc_coreotn_dmx_mgen_ipt_valid_toggle
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 COREOTN MUX empty High Order workaround
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV B
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_dmx_mgen_ipt_valid_toggle(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_B)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* pmc_coreotn_dmx_mgen_ipt_valid_toggle */


/****************************************************************************
*
*   pmc_coreotn_dmx_two_stage_proxy_mo_chnl_prov_revision
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that checks that the correct revision is used for
*                 proxy MO channel provisioning to avoid channel id collision
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV B
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_dmx_two_stage_proxy_mo_chnl_prov_revision(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_B)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* pmc_coreotn_dmx_two_stage_proxy_mo_chnl_prov_revision */

/****************************************************************************
*
*   pmc_prov_all_prov_mode_available
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that determines whether bulk (de)provisioning is
*                 available.
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    FALSE - REV A
*    TRUE - Otherwise
*
**********************************************************************/
BOOL8 pmc_prov_all_prov_mode_available(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = TRUE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_A)
    {
        result = FALSE;
    }

    PMC_RETURN(result);
} /* pmc_prov_all_prov_mode_available */

/****************************************************************************
*
*   pmc_coreotn_odujat_enable_for_128B_pkt
*  ___________________________________________________________________________
*
*  DESCRIPTION:   Function that is used to enable ODUJAT when 128 pkt size is
*                 used on ODUflex clients
*
*  INPUTS:
*    *pmc_handle_ptr - Pointer to the pmc_handle
*
*  RETURNS:
*    TRUE: REV B
*    FALSE: Otherwise
*
**********************************************************************/
BOOL8 pmc_coreotn_odujat_enable_for_128B_pkt(pmc_handle_t *pmc_handle_ptr)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (pmc_handle_ptr->common_info->device_revision_code == DIGI_REVISION_CODE_REV_B)
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* pmc_coreotn_odujat_enable_for_128B_pkt */

/******************************************************************************
*  pmc_sys_handle_create
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function creates a pmc_sys_handle_t.
* 
*  INPUTS:
*   None
* 
*  OUTPUTS:
*   None
*
*  RETURNS:
*   pmc_sys_handle_t - pointer to pmc_sys_handle_t
* 
*  NOTES:
*     The first field of the returned structure is used by pmc and must
*     be reserved.\n\n
*
*     e.g: typedef struct {\n
*              void *reserved_for_pmc_use;\n
*              ...platform specific_variables...\n
*           } *_sys_handle_t;
**********************************************************************/
pmc_sys_handle_t *pmc_sys_handle_create()
{
    pmc_sys_handle_t *pmc_sys_handle;

    PMC_ENTRY();

#ifdef PMC_SW_SIMULATION
    pmc_sys_handle = (pmc_sys_handle_t *)simulation_sys_create();
#else
#ifdef PMC_CUSTOMER_OS
    pmc_sys_handle = (pmc_sys_handle_t *)customer_sys_handle_create();
#else
#ifdef PMC_COMEXPRESS
    pmc_sys_handle = (pmc_sys_handle_t *) emulation_sys_handle_create();
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif
    /* allocate and attach the internal pmc handle within the handle */
    l1sys_allocate( pmc_sys_handle );
    PMC_RETURN(pmc_sys_handle);
}


/******************************************************************************
*  pmc_sys_handle_destroy
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function deletes a pmc_sys_handle_t.
*
*  INPUTS:
*   pmc_sys_handle - pointer to pmc_sys_handle_t
* 
*  OUTPUTS:
*   pmc_sys_handle - pointer set to NULL
* 
*  RETURNS:
*   None
* 
*  NOTES:
******************************************************************************/
void pmc_sys_handle_destroy(pmc_sys_handle_t **pmc_sys_handle)
{
    PMC_ENTRY();

    /* free the memory associate with the l1sys handle */
    l1sys_deallocate(*pmc_sys_handle);
#ifdef PMC_SW_SIMULATION
    simulation_sys_destroy(pmc_sys_handle);
#else
#ifdef PMC_CUSTOMER_OS
    pmc_mem_pool_destroy_all(pmc_sys_handle);
    customer_sys_handle_destroy(pmc_sys_handle);
#else
#ifdef PMC_COMEXPRESS
    pmc_mem_pool_destroy_all(pmc_sys_handle);
    emulation_sys_handle_destroy(pmc_sys_handle);
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif

    PMC_RETURN();
}

/******************************************************************************
*
*  pmc_sys_init
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*    This function is used to initialize the particular device specified by
*    the parameters base_addr_ptr and name_ptr. Depending on the system these
*    pointers may be initialized as alternative id methods (e.g. slot number,
*    structure of inforamtion, etc.) that can be used to uniquely identify
*    the device. If there is only one device to open in the system then this
*    information may be NULL and the implementation can simply open the device
*    based on hard coded values.
*
*   INPUTS:
*    *pmc_sys_handle - handle created from call to pmc_sys_handle_create
*    *base_addr_ptr - base address or other information that can be used to
*                     uniquely identify the device in the system
*    *name_ptr - string or other information that can be used to uniquely
*                identify the device in the system.
*
*  RETURNS:
*     PMC_SUCCESS
*     PMC_SHARED_ERR_DEVICE_NOT_INIT - device cannot be found or initialized
*
******************************************************************************/
PUBLIC PMC_ERROR pmc_sys_init(pmc_sys_handle_t *pmc_sys_handle, void *base_addr_ptr, const char *name_ptr )
{
    PMC_ERROR result;
    PMC_ENTRY();

#ifdef PMC_SW_SIMULATION
    /* currently the simulation environment does not need an init operation */
    result = PMC_SUCCESS;   
#else
#ifdef PMC_CUSTOMER_OS
    result = customer_sys_init(pmc_sys_handle, base_addr_ptr, name_ptr);
    if (PMC_SUCCESS == result)
    {
        pmc_mem_init(pmc_sys_handle);
    }   
#else
#ifdef PMC_COMEXPRESS
    result = emulation_sys_init(pmc_sys_handle, base_addr_ptr, name_ptr);
    if (PMC_SUCCESS == result)
    {
        pmc_mem_init(pmc_sys_handle);
    }
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif
    if(PMC_SUCCESS != result)
    {
        result = PMC_SHARED_ERR_DEVICE_NOT_INIT;
    }
    
    PMC_RETURN(result);
}

/*******************************************************************************
*  pmc_sys_file_open
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function opens a file. Content must be updated by user for their 
*    specific system requirements.
*
*  INPUTS:        
*    *path_ptr      - path to file 
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:
*   FILE - pointer to stream, NULL on errror
*
*******************************************************************************/
PUBLIC FILE* pmc_sys_file_open(const char* path_ptr)
{
    FILE* file = NULL;

    PMC_ENTRY();

#ifdef PMC_SW_SIMULATION
    file = simulation_sys_file_open(path_ptr);
#else
#ifdef PMC_CUSTOMER_OS
    file = customer_sys_file_open(path_ptr);
#else
#ifdef PMC_COMEXPRESS
    file = emulation_sys_file_open(path_ptr);
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif
    PMC_RETURN(file);

} /* pmc_sys_file_open */

/*******************************************************************************
*
*  pmc_sys_output_file_open
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function opens a file in write mode. 
*    Content must be updated by user for their specific system requirements.
*
*  INPUTS:        
*    *path_ptr      - path to file 
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:
*   FILE - pointer to stream, NULL on errror
*
*******************************************************************************/
PUBLIC FILE* pmc_sys_output_file_open(const char* path_ptr)
{
    FILE* file = NULL;

    PMC_ENTRY();

#ifdef PMC_SW_SIMULATION
    file = simulation_sys_output_file_open(path_ptr);
#else
#ifdef PMC_CUSTOMER_OS
    file = customer_sys_output_file_open(path_ptr);
#else
#ifdef PMC_COMEXPRESS
    file = emulation_sys_output_file_open(path_ptr);
#else
    file = NULL;
#endif /* PMC_COMEXPRESS */
#endif /* PMC_CUSTOMER_OS */
#endif /* PMC_SW_SIMULATION */

    PMC_RETURN(file);
} /* pmc_sys_output_file_open */

/*******************************************************************************
*  pmc_sys_file_close
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function closes a file. Content must be updated by user for their 
*    specific system requirements.
*
*  INPUTS:        
*     *stream_ptr  - file stream pointer
*
*  OUTPUTS:       
*     None.
*
*  RETURNS:
*   PMC_SUCCESS - on SUCCESS.
*   PMC_ERR_FAIL
*   Values from sub-fuctions 
*******************************************************************************/
PUBLIC PMC_ERROR pmc_sys_file_close(FILE* stream_ptr)
{
    PMC_ERROR result = PMC_ERR_FAIL;

    PMC_ENTRY();

#ifdef PMC_SW_SIMULATION
    result = simulation_sys_file_close(stream_ptr);
#else
#ifdef PMC_CUSTOMER_OS
    result = customer_sys_file_close(stream_ptr);
#else
#ifdef PMC_COMEXPRESS
    result = emulation_sys_file_close(stream_ptr);
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif

    PMC_RETURN(result);
} /* pmc_sys_file_close */

/*******************************************************************************
*
*  pmc_sys_file_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function reads 4-bytes from the file at the specified pointer 
*    location. Content must be updated by user for their specific system 
*    requirements.
*

*  INPUTS:        
*    *f_ptr       - file stream pointer
*    *buffer_ptr  - buffer pointer
*    *offset_ptr  - offset into the buffer pointer
*    *rd_len_ptr  - Read length pointer
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:        
*    PMC_SUCCESS on Success, failure otherwise.
*
*  NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
******************************************************************************/
PUBLIC PMC_ERROR pmc_sys_file_read(FILE *f_ptr, UINT8* buffer_ptr, UINT32 *offset_ptr, UINT8 *rd_len_ptr)
{
    PMC_ERROR result = PMC_ERR_FAIL;


#ifdef PMC_SW_SIMULATION
    result = simulation_sys_file_read(f_ptr, buffer_ptr, offset_ptr, rd_len_ptr);
#else
#ifdef PMC_CUSTOMER_OS
    *rd_len_ptr = 4;
    result = customer_sys_file_read(f_ptr, buffer_ptr, offset_ptr, rd_len_ptr);
#else
#ifdef PMC_COMEXPRESS
    *rd_len_ptr = 4;
    result = emulation_sys_file_read(f_ptr, buffer_ptr, offset_ptr, rd_len_ptr);
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif
    return(result);
} /* pmc_sys_file_read */

/*******************************************************************************
*
*  pmc_sys_event_init
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    Initialize event subsystem.
*
*  INPUTS:        
*    sys_handle - system handle
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:        
*    None.
*
******************************************************************************/
PUBLIC void pmc_sys_event_init(pmc_sys_handle_t *sys_handle)
{
    PMC_ENTRY();
#ifdef PMC_SW_SIMULATION
    /* Placeholder. */
#else
#ifdef PMC_CUSTOMER_OS
    customer_event_init(sys_handle);
#else
#ifdef PMC_COMEXPRESS
    emulation_event_init(sys_handle);
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif
    PMC_RETURN();
}

/*******************************************************************************
*
*  pmc_sys_event_destroy
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   Destroy event subsystem.
*
*  INPUTS:        
*    sys_handle - system handle
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:        
*    None.
*
******************************************************************************/
PUBLIC void pmc_sys_event_destroy(pmc_sys_handle_t *sys_handle)
{
    PMC_ENTRY();
#ifdef PMC_SW_SIMULATION
    /* Placeholder. */
#else
#ifdef PMC_CUSTOMER_OS
    customer_event_destroy(sys_handle);
#else
#ifdef PMC_COMEXPRESS
    emulation_event_destroy(sys_handle);
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif
    PMC_RETURN();
}

/*******************************************************************************
*
*  pmc_sys_event_register
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   Register system event.
*
*  INPUTS:
*    sys_handle     - system handle
*    event_cfg_ptr  - Event configuration structure
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*    None.
* 
*  NOTES:
*    Some events are really important and are hard coded (such as
*    PMC_EVENT_ID_OB_PCIE_IRQ_MON event).
*
*******************************************************************************/
PUBLIC void pmc_sys_event_register(pmc_sys_handle_t * sys_handle, pmc_sys_event_cfg_t *event_cfg_ptr)
{
    PMC_ENTRY();
#ifdef PMC_SW_SIMULATION
    /* Placeholder. */
#else
#ifdef PMC_CUSTOMER_OS
    customer_event_register(sys_handle, PMC_EVENT_ID_OB_PCIE_IRQ_MON, event_cfg_ptr);
#else
#ifdef PMC_COMEXPRESS
    emulation_event_register(sys_handle, PMC_EVENT_ID_OB_PCIE_IRQ_MON, event_cfg_ptr);
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif
    PMC_RETURN();
}

/*******************************************************************************
* pmc_atomic_internal_start
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   This function is wrapper that can user implementation of pmc_atomic_start.
*   It conditionnaly call _end function if keep_lock flag is not set
*
*  INPUTS:
*   *sys_handle - system handle
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*   PMC_SHARED_ATOMIC_KEEP_LOCK_MODE_ALREADY_SET when this function has been \n
*   already called 
* 
*  NOTES:
*
*******************************************************************************/
PUBLIC void pmc_atomic_internal_start(pmc_sys_handle_t *sys_handle)
{
    pmc_handle_t *pmc_handle_ptr;
    UINT32 thread_id;
    PMC_ENTRY();

    if (sys_handle != NULL)
    {   
        if (FALSE == pmc_atomic_continue((pmc_handle_t *)sys_handle))
        {     
           PMC_RETURN();
        }
        thread_id = pmc_thread_id_get(sys_handle);
        pmc_handle_ptr = (pmc_handle_t *) sys_handle; 

        /* Check if the mutex is currently held. If a thread switch was
           to occur just after this check and the other thread calls
           pmc_atomic_start() then this thread will just block while
           that thread completes. */
        if (0 == pmc_handle_ptr->mutex_ctxt.recursive_level)
        {
            /* No recursive level - get mutex */
            pmc_atomic_start(pmc_handle_ptr->mutex_ctxt.user_ctxt);

            /* switch the queue mode to buffered io */
            l5sys_buffered(pmc_handle_ptr);

#ifndef PMC_NO_ATOMIC_FUNCTION_STATS
            /* start performance monitor */
            pmc_sys_stat_mark_start(pmc_atomic_stat);
#endif

            /* Got the mutex, now set current thread_id as owner */
            pmc_handle_ptr->mutex_ctxt.current_thread_id = thread_id;

            /* increment the recursive_level */
            pmc_handle_ptr->mutex_ctxt.recursive_level +=1;
        } 
        else /* this is a recursive case */
        {
            /* Check if still in the same thread */
            if (thread_id == pmc_handle_ptr->mutex_ctxt.current_thread_id) 
            {
                /* In the same thread which owns mutex so continue operations incrementing count */
                /* PMC_LOG(PMC_LOG_SEV_MEDIUM, PMC_SHARED_LOG_TRACE,0,0,"Recursive PUBLIC API call: %s\n",__FUNCTION__); */
                pmc_handle_ptr->mutex_ctxt.recursive_level +=1;
            }
            else
            {   
                /* Not in the same thread so block to get mutex */
                pmc_atomic_start(pmc_handle_ptr->mutex_ctxt.user_ctxt);

#ifndef PMC_NO_ATOMIC_FUNCTION_STATS
                /* start performance monitor */
                pmc_sys_stat_mark_start(pmc_atomic_stat);
#endif
                /* Got the mutex, now set current thread_id as owner */
                pmc_handle_ptr->mutex_ctxt.current_thread_id = thread_id;

                /* As a sanity check, ensure that recursive_level is 0, should be as the
                   thread that released control should have unwound all its layers */
                PMC_ASSERT(pmc_handle_ptr->mutex_ctxt.recursive_level == 0, PMC_SHARED_LOG_TRACE, 0, 0);
                 
                /* increment the recursive_level */
                pmc_handle_ptr->mutex_ctxt.recursive_level +=1;
            }
        }
    }
    PMC_RETURN();
}  /*pmc_atomic_internal_start */

/*******************************************************************************
*  pmc_atomic_internal_end
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   This function is wrapper that can user implementation of pmc_atomic_end.
*   It conditionnaly call _end function if keep_lock flag is not set
*
*  INPUTS:
*   *sys_handle - system handle
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*   PMC_SHARED_ATOMIC_KEEP_LOCK_MODE_ALREADY_SET when this function has been \n
*   already called 
* 
*  NOTES:
*
*******************************************************************************/
PUBLIC void pmc_atomic_internal_end(pmc_sys_handle_t *sys_handle)
{
    pmc_handle_t *pmc_handle_ptr;
    PMC_ENTRY();
    if (FALSE == pmc_atomic_continue((pmc_handle_t *)sys_handle))
    {     
        PMC_RETURN();
    }
    
    pmc_handle_ptr = (pmc_handle_t *) sys_handle;

    /* As a sanity check, ensure that recursive_level is > 0, should be as the
       thread that took control would have incremented */
    PMC_ASSERT(pmc_handle_ptr->mutex_ctxt.recursive_level > 0, PMC_SHARED_LOG_TRACE, 0, 0);

    /* Check if recursive level to determine whether to release lock */
    if (1 == pmc_handle_ptr->mutex_ctxt.recursive_level)
    {
        /* 1 level deep so unless the keep_lock flag is set the lock is removed */
        if (FALSE == pmc_handle_ptr->mutex_ctxt.keep_lock) 
        { 
            /* Set the counter back to 0 */
            pmc_handle_ptr->mutex_ctxt.recursive_level = 0;

#ifndef PMC_NO_ATOMIC_FUNCTION_STATS
            /* stop performance monitor */
            pmc_sys_stat_atomic_mark_end(pmc_atomic_stat);
#endif

            /* change the firmware queue mode to always flush every message */
            l5sys_unbuffered(pmc_handle_ptr);

            /* Release the mutex */
            pmc_atomic_end(pmc_handle_ptr->mutex_ctxt.user_ctxt); 
        }                
    }
    else
    {
        /* There have been recursive calls (recursive_level > 0) so keep the lock
           and decrement by one */
        pmc_handle_ptr->mutex_ctxt.recursive_level -= 1;
    }

    PMC_RETURN();
} /* pmc_atomic_internal_end */

/*******************************************************************************
*  pmc_atomic_is_nested
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   checks we are currently nested inside multiple PMC_ATOMIC_ENTRY functions.
*
*  INPUTS:
*   *sys_handle - system handle
*
*  OUTPUTS:
*    none
*
*  RETURNS:        
*    TRUE if inside multiple PMC_ATOMIC_ENTRY functions.
* 
*  NOTES:
*
*******************************************************************************/
PUBLIC BOOL pmc_atomic_is_nested(pmc_sys_handle_t *sys_handle)
{
    pmc_handle_t *pmc_handle_ptr;
    PMC_ENTRY();
    
    pmc_handle_ptr = (pmc_handle_t *) sys_handle;

    /* Check if recursive level to determine whether to release lock */
    if (pmc_handle_ptr->mutex_ctxt.recursive_level > 1)
    {
        PMC_RETURN( TRUE );
    }
    else
    {
        PMC_RETURN( FALSE );
    }
} /* pmc_atomic_is_nested */


/*******************************************************************************
* pmc_atomic_continue
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*  Function checks the state of the recursive_level to determine whether
*  PMC_ATOMIC_ENTRY or PMC_ATOMIC_RETURN are being called from within a 
*  crash or warm restart. If true then ATOMIC_ENTRY and ATOMIC_RETURN are
*  suspended until the operation completes.
*
*  INPUTS:
*   *digi_handle - PMC HANDLE that is passed in from the digi_handle
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*    TRUE:  This is not being called in a restart and ATOMIC operation can
*           continue:
*    FALSE: Restart is in progress and ATOMIC operations should be suspended
* 
*  NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 pmc_atomic_continue(pmc_handle_t *digi_handle)
{
    BOOL8 result = TRUE;

    PMC_ENTRY();
    if (0x1 == digi_handle->mutex_ctxt.recursive_level >> 31)
    {
        result = FALSE;
    }

    PMC_RETURN(result);
} /* pmc_atomic_continue */


#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* pmc_atomic_suspended_ctxt_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   This function is used to get the suspended context of the current thread. 
*   The context is allocated if it not found
*
*  INPUTS:
*   *sys_handle - sys handle
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*   pmc_mutex_suspended_ctxt_t - The suspended context of the current thread
* 
*  NOTES:
*
*******************************************************************************/
PRIVATE pmc_mutex_suspended_ctxt_t * pmc_atomic_suspended_ctxt_get(pmc_sys_handle_t *sys_handle)
{
    pmc_handle_t *pmc_handle_ptr;    
    pmc_mutex_suspended_ctxt_t *ret_ctxt = NULL, *last_ctxt;
    pmc_mutex_suspended_ctxt_t *head_ctxt;
    UINT32 thread_id;
    PMC_ENTRY();

    PMC_ASSERT(sys_handle != NULL, PMC_SHARED_LOG_TRACE, 0, 0);

    thread_id = pmc_thread_id_get(sys_handle);
    pmc_handle_ptr = (pmc_handle_t*) sys_handle;
    head_ctxt = PMC_MUTEX_SUSPENDED_CTXT_CONV_PTR(pmc_handle_ptr->mutex_ctxt.suspended_thread_id,
                                                  pmc_handle_ptr->mutex_ctxt.suspended_recursive_level);
    
    /* this condition */
    if (head_ctxt == NULL ||
        pmc_handle_ptr->mutex_ctxt.suspended_thread_id == PMC_SYS_THREAD_ID_UNSET)
    { 
        head_ctxt = (pmc_mutex_suspended_ctxt_t*)PMC_CALLOC(sizeof(pmc_mutex_suspended_ctxt_t));
        head_ctxt->thread_id = thread_id;
        ret_ctxt = head_ctxt;
        pmc_handle_ptr->mutex_ctxt.suspended_thread_id = PMC_MUTEX_SUSPENDED_CTXT_CONV_UINT32_A(head_ctxt);
        pmc_handle_ptr->mutex_ctxt.suspended_recursive_level = PMC_MUTEX_SUSPENDED_CTXT_CONV_UINT32_B(head_ctxt);
        
    } else 
    {
        ret_ctxt = head_ctxt;
        last_ctxt = ret_ctxt;
        while (ret_ctxt != NULL && ret_ctxt->thread_id != thread_id)
        {
            last_ctxt = ret_ctxt;
            ret_ctxt = ret_ctxt->next;
        }
        if (NULL == ret_ctxt) 
        {
            last_ctxt->next = (pmc_mutex_suspended_ctxt_t*)PMC_CALLOC(sizeof(pmc_mutex_suspended_ctxt_t));
            last_ctxt->next->thread_id = thread_id;
            ret_ctxt = last_ctxt->next;         
        }        
    }    
    
    PMC_RETURN(ret_ctxt);
} /* pmc_atomic_suspended_ctxt_get*/
#endif /* DOXYGEN_PUBLIC_ONLY */

/*********************************************************************
* pmc_atomic_internal_delete
* ____________________________________________________________________
*
* DESCRIPTION:
*   This function destroy mutex context 
*
* INPUTS:
*   *mutex_ctxt  -  A pointer to the mutex context 
*
* OUTPUTS:
*   None
*
* RETURNS:
*    None
*
* NOTES:
* 
**********************************************************************/
PUBLIC void pmc_atomic_internal_delete(pmc_mutex_ctxt_t *mutex_ctxt)
{
    pmc_mutex_suspended_ctxt_t *cur_ctxt, *next_ctxt;    
    pmc_mutex_suspended_ctxt_t *head_ctxt = PMC_MUTEX_SUSPENDED_CTXT_CONV_PTR(mutex_ctxt->suspended_thread_id,
                                                                              mutex_ctxt->suspended_recursive_level);
    PMC_ENTRY();

    if ( !(head_ctxt == NULL ||
           mutex_ctxt->suspended_thread_id == PMC_SYS_THREAD_ID_UNSET))
    {
        cur_ctxt = head_ctxt;
        while(cur_ctxt != NULL)
        {
            next_ctxt = cur_ctxt->next;
            PMC_FREE(&cur_ctxt);
            cur_ctxt  = next_ctxt;
        }
    }
    mutex_ctxt->suspended_thread_id       = PMC_MUTEX_SUSPENDED_CTXT_CONV_UINT32_A(NULL);
    mutex_ctxt->suspended_recursive_level = PMC_MUTEX_SUSPENDED_CTXT_CONV_UINT32_B(NULL);
    pmc_atomic_delete(&mutex_ctxt->user_ctxt);
    PMC_RETURN();
} /* pmc_atomic_internal_delete */

/*******************************************************************************
* pmc_atomic_internal_force
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   This function is used to force release the mutex even if call are nested.
*
*  INPUTS:
*   *sys_handle - system handle
*   lock        - TRUE locks the mutex, FALSE releases the mutex
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*   
* 
*  NOTES:
*
*******************************************************************************/
PUBLIC void pmc_atomic_internal_force (pmc_sys_handle_t *sys_handle, BOOL8 lock)
{
    pmc_handle_t *pmc_handle_ptr;
    pmc_mutex_suspended_ctxt_t *cur_suspended_ctxt = NULL;
    PMC_ENTRY();

    if (sys_handle != NULL)
    {        
        pmc_handle_ptr = (pmc_handle_t *) sys_handle;                             
        cur_suspended_ctxt = pmc_atomic_suspended_ctxt_get(sys_handle);
            
        if (TRUE == lock) 
        {
            pmc_atomic_start(pmc_handle_ptr->mutex_ctxt.user_ctxt);
            pmc_handle_ptr->mutex_ctxt.current_thread_id = cur_suspended_ctxt->thread_id;
            pmc_handle_ptr->mutex_ctxt.recursive_level = cur_suspended_ctxt->recursive_level;
            /* start performance monitor */
#ifndef PMC_NO_ATOMIC_FUNCTION_STATS
            pmc_sys_stat_mark_start(pmc_atomic_stat);
#endif
        } else 
        {
            cur_suspended_ctxt->recursive_level = pmc_handle_ptr->mutex_ctxt.recursive_level;
            /* stop performance monitor */
#ifndef PMC_NO_ATOMIC_FUNCTION_STATS
            pmc_sys_stat_atomic_mark_end(pmc_atomic_stat);            
#endif
            pmc_handle_ptr->mutex_ctxt.recursive_level = 0;
            pmc_atomic_end(pmc_handle_ptr->mutex_ctxt.user_ctxt);
        }
    }

    PMC_RETURN();
}  /*pmc_atomic_internal_force */

/*******************************************************************************
*  pmc_sys_event_monitor
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   Monitor system events.
*
*  INPUTS:
*    sys_handle - system handle
*    event_cfg  - Event monitor configuration
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*    None.
* 
*  NOTES:
*
*******************************************************************************/
PUBLIC void pmc_sys_event_monitor(pmc_sys_handle_t *sys_handle, pmc_sys_event_cfg_t *event_cfg)
{
    PMC_ENTRY();
#ifdef PMC_SW_SIMULATION
    /* Placeholder. */
#else
#ifdef PMC_CUSTOMER_OS
    customer_event_monitor(sys_handle, event_cfg);
#else
#ifdef PMC_COMEXPRESS
    emulation_event_monitor(sys_handle, event_cfg);
#else
#error BUILD ERROR - Please define BUILD_TARGET
#endif
#endif
#endif
    PMC_RETURN();
}

/*******************************************************************************
*  pmc_os_stats_init
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   Initialize the stats operations global structures
*
*  INPUTS:
*    None
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*    None.
* 
*  NOTES:
*
*******************************************************************************/
void pmc_os_stats_init()
{
    PMC_ENTRY();

    pmc_os_stats_initialized = TRUE;
    
    pmc_os_stats_ptr = &digi120_pmc_os_stats;
    PMC_MEMSET( pmc_os_stats_ptr, 0, sizeof(digi120_pmc_os_stats));

    PMC_RETURN();
}

/*******************************************************************************
*  pmc_os_stats_snapshot
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   Capture a snapshot of the current global stats
*
*  INPUTS:
*    None
*
*  OUTPUTS:
*    snapshot - values captured in snapshot
*
*  RETURNS:        
*    None.
* 
*  NOTES:
*
*******************************************************************************/
void pmc_os_stats_snapshot( pmc_os_stats_t *snapshot )
{
    PMC_ENTRY();

    if (!pmc_os_stats_initialized)
    {
        pmc_os_stats_init();
    }
    pmc_os_stats_ptr = &digi120_pmc_os_stats;
    PMC_MEMCPY( snapshot, pmc_os_stats_ptr, sizeof(digi120_pmc_os_stats) );

    PMC_RETURN();
}

#ifdef PMC_TESTS
/*******************************************************************************
*  pmc_sys_schedule_crash_event
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   This function is used to schedule a crash event 
*
*  INPUTS:
*    crash_event - specify at which  event a crash will occur 
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*    None.
* 
*  NOTES:
*
*******************************************************************************/
void pmc_sys_schedule_crash_event(UINT32 crash_event)
{
    PMC_ENTRY();
    g_crash_event.is_setup = FALSE;
    g_crash_event.is_scheduled = TRUE;
    g_crash_event.crash_at_write = crash_event;
    PMC_RETURN();
}
/*******************************************************************************
*  pmc_sys_crash_event_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   This function return the current crash event 
*
*  INPUTS:
*    None
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*    UINT32  - The current crash event
* 
*  NOTES:
*
*******************************************************************************/
UINT32 pmc_sys_crash_event_get(void)
{
    PMC_ENTRY();
    PMC_RETURN(digi120_pmc_os_stats.num_reg_write);
}

/*******************************************************************************
*  pmc_sys_crash_event_required
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   This function is used to determine that a crash event is required
*   Now we only support crash on digi120_pmc_os_stats:
*     num_reg_read
*     num_reg_write
*     num_queued_write
*     num_queued_read 
*  INPUTS:
*    None
*
*  OUTPUTS:
*    None.
*
*  RETURNS:        
*    TRUE when a crash event is required, otherwise FALSE
* 
*  NOTES:
*
*******************************************************************************/
BOOL8 pmc_sys_crash_event_required()
{
    BOOL8 result = FALSE;
    if (g_crash_event.is_setup == TRUE && 
        g_crash_event.is_scheduled == TRUE)
    {
        if (g_crash_event.crash_at_write    == digi120_pmc_os_stats.num_reg_write)
        {
            result = TRUE;
        }
    }       
    return result;
}
#endif

#ifdef PRIu64
#undef PRIu64
/* PRIu64 seems to be defined wrong */
#endif

#ifdef WIN32
#define PRIu64 "I64u"
#else
#define PRIu64 "llu"
#endif

#define LOG_STAT( x, factor ) {UINT32 x = digi120_pmc_os_stats.x - snapshot->x; total_time += factor * x; if (x > 0) {PMC_PRINT( "%s = %u\n", #x, x );} }
#define LOG_STAT64( x, factor ) {UINT64 x = digi120_pmc_os_stats.x - snapshot->x; total_time += factor * x; if (x > 0) {PMC_PRINT( "%s = %"PRIu64"\n", #x, x );} }

/*******************************************************************************
*  pmc_os_stats_show
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*   Display a snapshot captured using LOG_STAT
*
*  INPUTS:
*    snapshot - captures snapshot
*
*  OUTPUTS:
*    None
*
*  RETURNS:        
*    None.
* 
*  NOTES:
*
*******************************************************************************/
void pmc_os_stats_show( pmc_os_stats_t *snapshot )
{
    DOUBLE total_time = 0.0;
    PMC_ENTRY();
    
    LOG_STAT( num_reg_read, 2.218 );
    LOG_STAT( num_field_read, 0 );
    LOG_STAT( num_reg_write, 0.260 );
    LOG_STAT( num_reg_read_modify_write, 2.271 );
    LOG_STAT( num_field_read_modify_write, 0 );
    LOG_STAT( num_reg_read_modify_write_duplicate, 0 );
    LOG_STAT( num_reg_read_modify_write_no_effect, 0 );
    LOG_STAT( num_poll_bit, 2.218 );
    LOG_STAT( num_reg_burst_read, 0 );
    LOG_STAT( num_reg_burst_write, 0 );
    LOG_STAT( num_reg_burst_read_words, 2.218 );
    LOG_STAT( num_reg_burst_write_words, 0.260 );
    LOG_STAT( num_config_cache_read, 0 );
    LOG_STAT( num_config_cache_write, 0 );
    LOG_STAT( num_config_cache_read_modify_write, 0 );
    LOG_STAT( num_config_cache_burst_read, 0 );
    LOG_STAT( num_config_cache_burst_write, 0 );
    LOG_STAT( num_config_cache_burst_read_words, 0 );
    LOG_STAT( num_config_cache_burst_write_words, 0 );
    LOG_STAT( num_sleep_calls, 0 );
    LOG_STAT( num_msleep_calls, 0 );
    LOG_STAT( num_usleep_calls, (-0.5) ); /* yes,... every usleep will deduct have a microsecond from the total time (since the microseconds slept will be off by a fraction of 1 microsecond) */
    LOG_STAT( num_nanosleep_calls, 0 );
    LOG_STAT64( num_seconds_slept, 1000000.0 );
    LOG_STAT64( num_milliseconds_slept, 1000.0 );
    LOG_STAT64( num_microseconds_slept, 1.0 );
    LOG_STAT64( num_nanoseconds_slept, 0.001 );
    LOG_STAT( num_queued_write, 0 );
    LOG_STAT( num_queued_rmw_short, -2.271 + 0.260 );
    LOG_STAT( num_queued_rmw_long, -2.271 + 0.260 + 0.260 );
    LOG_STAT( num_queued_read, +1.5 );
    LOG_STAT( num_queued_usleep, 0.260 );
    LOG_STAT64( num_microseconds_queued, 0 );
    LOG_STAT( num_queued_poll, 0.260 + 0.260 + 0.260 ); /* <-- could squish poll into 4 words instead of the current 6 to save a few nanoseconds. */
    
    if (total_time > 0.0) {
        const char *units;
        DOUBLE mult;
        char total_str[20];
        units = pmc_sys_get_units(total_time,&mult);
        pmc_sys_print_time(total_time,mult,total_str);
        PMC_PRINT("system time: %s %s\n",total_str,units);
    }
    
    PMC_RETURN();
}

/*******************************************************************************
*  count_trailing_zeros
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*  The function to count the number of zeros starting at the least significant
*  bit position
*
*  INPUTS:
*    x - value to count the trailing 0's in
*
*  OUTPUTS:
*    NONE.
*
*  RETURNS:        
*    UINT8 - the number of trailing 0's
* 
*  NOTES:
*******************************************************************************/
PUBLIC UINT8 count_trailing_zeros(UINT16 x)
{
    UINT8 c = 0;
    UINT16 y = 0;

    PMC_ENTRY();

    y = x - 1;
    x = ~x;    
    x = x & y;

    if(x) 
    {
        for(c = 0; x ; c++)
        {
            x = x >> 1;
        }
    }
    
    PMC_RETURN (c);
}



/*******************************************************************************
* pmc_sys_poll_bit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Poll a set of bits in a register for a set number of times with a delay
*   between each poll.  The simulation environment typically returns an
*   immediate success
*
* INPUTS:
*   sys - handle to the device
*   addr - address/offset of the location to poll 
*   mask - mask of the bits to poll
*   value - value for those bits to change to
*   max_count - maximum number of polls
*   delay_between_polls_in_microseconds - microseconds between polls
* 
* OUTPUTS:
*   num_failed_polls - number of times it failed the poll before passing or
*                      timing out
*
* RETURNS:
*   PMC_SUCCESS - if poll passed, error otherwise
* 
* NOTES:
*
*******************************************************************************/
PMC_ERROR pmc_sys_poll_bit(pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds)
{
    UINT32 i;
    UINT32 regval;
    /*! Declare statistics structure */
    PMC_SYS_STAT_DECLARE( poll_delays1, "poll delays1" );

    PMC_ENTRY();

    for (i=0;i<max_count;i++)
    {
        regval = pmc_sys_reg_read(sys,addr);
        if ((regval & mask) == value)
        {
            if (num_failed_polls != NULL)
            {
                *num_failed_polls = i;
            }
            if (i!=0) {
                pmc_sys_stat_mark_end(poll_delays1);
            }
            PMC_RETURN(PMC_SUCCESS);
        }
        if (i==0) {
            pmc_sys_stat_mark_start(poll_delays1);
        }
        if (delay_between_polls_in_microseconds > 0) 
        {
            PMC_OS_USLEEP_NO_LOG(delay_between_polls_in_microseconds);
        }
    }
    if (num_failed_polls != NULL)
    {
        *num_failed_polls = i;
    }
    if (i==0) {
        pmc_sys_stat_mark_end(poll_delays1);
    }
#ifdef PMC_SW_SIMULATION
    PMC_RETURN(PMC_SUCCESS); /* always return SUCCESS in simulation */
#else
    PMC_RETURN(PMC_SHARED_ERR_CODE_POLL_TIMEOUT);
#endif
}

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
*  tv_to_microseconds
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Convert a timeval structure to microseconds
*
*  INPUTS:
*   tv - pointer to timeval structure
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   UINT64 - number of microseconds since epoch as a UINT64.
*
******************************************************************************/
static UINT64 tv_to_microseconds(pmc_sys_timeval_t *tv)
{
    UINT64 microseconds = tv->tv_sec;
    microseconds *= 1000000;
    microseconds += tv->tv_usec;
    return (microseconds);
}
#endif

/*******************************************************************************
* pmc_sys_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Poll a set of bits in a register for a set number of times with a delay
*   between each poll.  The simulation environment typically returns an
*   immediate success
*
* INPUTS:
*   sys - handle to the device
*   addr - address/offset of the location to poll 
*   mask - mask of the bits to poll
*   value - value for those bits to change to
*   cmp - type of comparison to perform (= != > < >= <=)
*   max_count - maximum number of polls
*   delay_between_polls_in_microseconds - microseconds between polls
* 
* OUTPUTS:
*   num_failed_polls - number of times it failed the poll before passing or
*                      timing out
*
* RETURNS:
*   PMC_SUCCESS - if poll passed, error otherwise
* 
* NOTES:
*
*******************************************************************************/
PMC_POLL_RETURN_TYPE pmc_sys_poll( pmc_sys_handle_t *sys, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    UINT32 i;
    UINT32 regval;
    UINT64 microseconds_now;
    UINT64 microseconds_start;
    UINT64 dt;
    UINT64 total_timeout;
    pmc_sys_timeval_t tv_now;
    pmc_sys_timeval_t tv_start;
    BOOL8 last = FALSE;

    /*! Declare statistics structure */
    PMC_SYS_STAT_DECLARE( poll_delays2, "poll delays2" );
    
    PMC_ENTRY();
    PMC_TIMEVAL_READ(&tv_start);
    microseconds_start = tv_to_microseconds(&tv_start);
    i = 0;
    total_timeout = max_count * delay_between_polls_in_microseconds;

    while (!last)
    {
        BOOL8 passed = FALSE;

        /* read current time */
        PMC_TIMEVAL_READ(&tv_now);
        microseconds_now = tv_to_microseconds(&tv_now);
        /* if we are beyond the maximum time,... make this the last read */
        dt = microseconds_now - microseconds_start;
        if (dt > total_timeout) {
            last = TRUE;
        }

        regval = pmc_sys_reg_read(sys,addr);
        switch (cmp)
        {
        case PMC_POLL_EQUAL            : passed = (regval & mask) == value; break;
        case PMC_POLL_NOT_EQUAL        : passed = (regval & mask) != value; break;
        case PMC_POLL_GREATER          : passed = (regval & mask) >  value; break;
        case PMC_POLL_LESS             : passed = (regval & mask) <  value; break;
        case PMC_POLL_GREATER_OR_EQUAL : passed = (regval & mask) >= value; break;
        case PMC_POLL_LESS_OR_EQUAL    : passed = (regval & mask) <= value; break;
        }
        if (passed)
        {
            if (num_failed_polls != NULL)
            {
                *num_failed_polls = i;
            }
            if (i!=0) {
                pmc_sys_stat_mark_end(poll_delays2);
            }
            PMC_RETURN(PMC_SUCCESS);
        }
        if (i==0) {
            pmc_sys_stat_mark_start(poll_delays2);
        }
        if (delay_between_polls_in_microseconds > 0) 
        {
#ifndef PMC_SW_SIMULATION
            PMC_OS_USLEEP_NO_LOG(delay_between_polls_in_microseconds);
#endif
        }
        i++;
    }
    if (num_failed_polls != NULL)
    {
        *num_failed_polls = i;
    }
    pmc_sys_stat_mark_end(poll_delays2);
#ifdef PMC_SW_SIMULATION
    PMC_RETURN(PMC_SUCCESS); /* always return SUCCESS in simulation */
#else
    PMC_RETURN(PMC_SHARED_ERR_CODE_POLL_TIMEOUT);
#endif
}



#ifndef DOXYGEN_PUBLIC_ONLY


#ifdef PMC_FUNCTION_PROFILER
static FILE *fp_trace;
static char profile_filename[128];
typedef struct {
    UINT32      entry_exit;
    void*       func;
    void*       caller;
    UINT32      tv_sec;
    UINT32      tv_usec;
} function_profile_struct_t;


void enable_function_profiling(char *filename)
{
    fp_trace = fopen(filename, "wb");
    strcpy(profile_filename, filename);
}
 
 
void disable_function_profiling()
{
    if (fp_trace != NULL)
    {
        function_profile_struct_t profile_output;
        FILE *txt_file;
        fflush(fp_trace);
        fclose(fp_trace);
        fp_trace = fopen(profile_filename, "rb");
        strcat(profile_filename, ".txt");
        txt_file = fopen(profile_filename, "w");
        while (!feof(fp_trace) && fread(&profile_output,  sizeof(profile_output), 1, fp_trace) == 1)
        {
            if (profile_output.entry_exit == 1)
            {
                fprintf(txt_file, "e %p %p %u %u\n", profile_output.func, profile_output.caller, profile_output.tv_sec, profile_output.tv_usec );
            }
            else
            {
                fprintf(txt_file, "x %p %p %u %u\n", profile_output.func, profile_output.caller, profile_output.tv_sec, profile_output.tv_usec );
            }
        }
        fflush(fp_trace);
        fclose(fp_trace);
        fflush(txt_file);
        fclose(txt_file);
        fp_trace = NULL;
    }
}
 
void __cyg_profile_func_enter (void *func,  void *caller)
{
    if (fp_trace != NULL)
    {
        pmc_sys_timeval_t now;
        PMC_TIMEVAL_READ(&now);
        function_profile_struct_t profile_output;
        profile_output.entry_exit = 1;
        profile_output.func = func;
        profile_output.caller = caller;
        profile_output.tv_sec = now.tv_sec;
        profile_output.tv_usec = now.tv_usec;
        fwrite(&profile_output, sizeof(profile_output), 1, fp_trace);
    }
}

void __cyg_profile_func_exit (void *func, void *caller)
{
    if(fp_trace != NULL)
    {
        pmc_sys_timeval_t now;
        PMC_TIMEVAL_READ(&now);
        function_profile_struct_t profile_output;
        profile_output.entry_exit = 0;
        profile_output.func = func;
        profile_output.caller = caller;
        profile_output.tv_sec = now.tv_sec;
        profile_output.tv_usec = now.tv_usec;
        fwrite(&profile_output, sizeof(profile_output), 1, fp_trace);
    }
}
#endif /* PMC_FUNCTION_PROFILER */

/*******************************************************************************
*
*  record_duration
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   records the duration of a period. This increments the number of samples, updates
*   min & max if needed, and updates the total time.
*
*  INPUTS:
*   s - pointer to stats structure
*   dt - delta time in microseconds
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
******************************************************************************/
PRIVATE void record_duration(pmc_sys_dstat_t *s,double dt)
{
    if (s->num == 0)
    {
        s->min = dt;
        s->max = dt;
        s->total = 0;
    }
    if (dt > s->max)
    {
        s->max = dt;
    }
    if (dt < s->min)
    {
        s->min = dt;
    }
    s->total += dt;
    s->num++;
    s->mark_started = 0;
} /* record_duration() */

/*******************************************************************************
*
*  mark_end_time
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Mark the end of a period. This increments the number of samples, updates
*   min & max if needed, and updates the total time.
*
*  INPUTS:
*   s - pointer to stats structure
*   now - timeval structure
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
******************************************************************************/
static void mark_end_time(pmc_sys_stat_t *s,pmc_sys_timeval_t now)
{
    UINT64 microseconds_now = tv_to_microseconds(&now);
    UINT64 microseconds_mark = tv_to_microseconds(&s->mark);
    UINT64 dt = microseconds_now - microseconds_mark;
    if (s->num == 0)
    {
        s->min = dt;
        s->max = dt;
        s->total = 0;
    }
    if (dt > s->max)
    {
        s->max = dt;
    }
    if (dt < s->min)
    {
        s->min = dt;
    }
    s->total += dt;
    s->num++;
    s->mark_started = 0;
} /* mark_end_time() */

/*******************************************************************************
*  mark_atomic_end_time
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Mark the end of a period. This increments the number of samples, updates
*   min & max if needed, and updates the total time.
*
*  INPUTS:
*   s - pointer to stats structure
*   dt -  timeval
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
******************************************************************************/
static void mark_atomic_end_time(pmc_sys_stat_t *s, UINT64 dt)
{
    if (s->atomic_num == 0)
    {
        s->atomic_min = dt;
        s->atomic_max = dt;
        s->atomic_total = 0;
    }
    if (dt > s->atomic_max)
    {
        s->atomic_max = dt;
    }
    if (dt < s->atomic_min)
    {
        s->atomic_min = dt;
    }
    s->atomic_total += dt;
    s->atomic_num++;

} /* mark_atomic_end_time() */

/*******************************************************************************
*  pmc_sys_stat_merge
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   add the contents of one statistic into another: s += x
*
*  INPUTS:
*   s - pointer to stats structure (total)
*   x - pointer to stats structure (increment)
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
******************************************************************************/
PUBLIC void pmc_sys_stat_merge(pmc_sys_stat_t *s, pmc_sys_stat_t *x)
{
    PMC_ENTRY();
    if (!s->initialized) {

        s->num = 0;
        s->total = 0;
        s->mark_started = 0;
        s->display_average = 0;

        s->initialized = 1;
    }
    if (x->num > 0) {
        if (s->num == 0) {
            s->min = x->min;
            s->max = x->max;
            s->total = x->total;
        } else {
            s->total += x->total;
            if (x->min < s->min) {
                s->min = x->min; 
            }
            if (x->max > s->max) {
                s->max = x->max; 
            }
        }
        s->num += x->num;
    }
    PMC_RETURN();
} /* pmc_sys_stat_merge() */

/*******************************************************************************
*  pmc_sys_dstat_merge
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   add the contents of one statistic into another: s += x
*
*  INPUTS:
*   s - pointer to stats structure (total)
*   x - pointer to stats structure (increment)
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
******************************************************************************/
PUBLIC void pmc_sys_dstat_merge(pmc_sys_dstat_t *s, pmc_sys_dstat_t *x)
{
    PMC_ENTRY();
    if (!s->initialized) {
        s->num = 0;
        s->total = 0;
        s->mark_started = 0;
        s->display_average = 0;

        s->initialized = 1;
    }
    if (x->num > 0) {
        if (s->num == 0) {
            s->min = x->min;
            s->max = x->max;
            s->total = x->total;
        } else {
            s->total += x->total;
            if (x->min < s->min) {
                s->min = x->min; 
            }
            if (x->max > s->max) {
                s->max = x->max; 
            }
        }
        s->num += x->num;
    }
    PMC_RETURN();
} /* pmc_sys_dstat_merge() */

/*******************************************************************************
*  pmc_sys_dstat_record_duration
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Records the duration of an activity.
*
*  INPUTS:
*   s - pointer to stats structure
*   dt - duration of the variable.
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
******************************************************************************/
PUBLIC void pmc_sys_dstat_record_duration(pmc_sys_dstat_t *s, double dt)
{
    PMC_ENTRY();
    if (!s->initialized) {

        s->num = 0;
        s->total = 0;
        s->mark_started = 0;
        s->display_average = 0;

        s->initialized = 1;
    }
    record_duration(s,dt);
    PMC_RETURN();
} /* pmc_sys_dstat_record_duration() */

/*******************************************************************************
*  pmc_sys_stat_mark_start
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Mark the beginning of a block of code.
*
*   If back to back calls to pmc_sys_stat_mark_start are made, then
*   this actually calls pmc_sys_stat_mark_end() and then calls
*   pmc_sys_stat_mark_start(),... but it uses the same call to 
*   gettimeofday() for efficiency.
*
*  INPUTS:
*   s - pointer to stats structure
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
******************************************************************************/
void pmc_sys_stat_mark_start(pmc_sys_stat_t *s)
{
    pmc_sys_timeval_t now;
    PMC_TIMEVAL_READ(&now);
    if (!s->initialized) {

#if defined(PMC_DEBUG_DIGI_DUMP_STATS)
        if (gbl_all_stats == NULL) {
            atexit(pmc_sys_stat_show_all);
        }
#endif

        s->next = gbl_all_stats;
        gbl_all_stats = s;
        s->num = 0;
        s->total = 0;
        s->mark_started = 0;
        s->display_average = 0;
        s->atomic_num = 0;
        s->atomic_total = 0;

        s->initialized = 1;
    }
    if (s->mark_started) {
        mark_end_time(s,now);
    }
    s->mark = now;
    s->mark_started = 1;
} /* pmc_sys_stat_mark_start() */

/*******************************************************************************
*  pmc_sys_stat_mark_end
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Mark the end of a block of code. This increments the number of samples,
*   updates min & max if needed, and updates the total time.
*
*  INPUTS:
*   s - pointer to stats structure
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
******************************************************************************/
void pmc_sys_stat_mark_end(pmc_sys_stat_t *s)
{
    pmc_sys_timeval_t now;
    PMC_TIMEVAL_READ(&now);
    mark_end_time(s,now);
} /* pmc_sys_stat_mark_end */

/*******************************************************************************
*  pmc_sys_stat_mark_cancel
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Cancel collection of status of a block of code 
*
*  INPUTS:
*   s - pointer to stats structure
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
******************************************************************************/
void pmc_sys_stat_mark_cancel(pmc_sys_stat_t *s)
{
    s->mark_started = 0; 
} /* pmc_sys_stat_mark_cancel */

/*******************************************************************************
*  pmc_sys_stat_atomic_mark_end
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Mark the end of a block of code and annotate parent (or open)
*   block code. This function is intended to be used with pmc_atomic block code.
*   This increments the number of samples, updates min & max if needed, and
*   updates the total time.
*
*  INPUTS:
*   s - pointer to stats structure
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
******************************************************************************/
void pmc_sys_stat_atomic_mark_end(pmc_sys_stat_t *s)
{
    pmc_sys_stat_t *iterator = NULL;
    pmc_sys_timeval_t now;
    UINT64 microseconds_now;
    UINT64 microseconds_mark;
    UINT64 dt;
    PMC_TIMEVAL_READ(&now);

    /* mark current bloc */
    mark_end_time(s,now);

    if (pmc_atomic_attach == TRUE)
    {
        microseconds_now = tv_to_microseconds(&now);
        microseconds_mark = tv_to_microseconds(&s->mark);
        dt = microseconds_now - microseconds_mark;
  
        iterator = gbl_all_stats;
        while (iterator != NULL)
        {
            if (iterator->mark_started == 1)
            {            
                mark_atomic_end_time(iterator,dt);
            }
            iterator = iterator->next;
        }   
    }
} /* pmc_sys_stat_atomic_mark_end */


/*******************************************************************************
*  pmc_sys_stat_set_num
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Set the number of samples that the were actually performed.
*
*   If pmc_sys_stat_mark_start() and pmc_sys_stat_mark_end() surround a loop
*   then call pmc_sys_stat_set_num to indicate the number of iterations performed.
*
*   pmc_sys_stat_show_all() will only show average times and not min/max times.
*
*  INPUTS:
*   s - pointer to stats structure
*   num - number of iterations performed.
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
******************************************************************************/
PUBLIC void pmc_sys_stat_set_num(pmc_sys_stat_t *s, UINT32 num)
{
    s->num = num;
    s->display_average = 1;
}

/*******************************************************************************
*  pmc_sys_stat_running_time
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Returns the number of microseconds since the pmc_sys_stat_mark_start() was called.
*
*  INPUTS:
*   s - pointer to stats structure
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   UINT64 - time in microseconds
*
******************************************************************************/
PUBLIC UINT64 pmc_sys_stat_running_time(pmc_sys_stat_t *s)
{
    pmc_sys_timeval_t now;
    PMC_TIMEVAL_READ(&now);
    
    UINT64 microseconds_now = tv_to_microseconds(&now);
    UINT64 microseconds_start = tv_to_microseconds(&s->mark);
    UINT64 dt = microseconds_now - microseconds_start;
    return dt;
}

/*******************************************************************************
*  pmc_sys_get_units
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Returns units suitable for displaying the microsecond time
*   and a multiplication factor to convert from microseconds to those units.\n
*         "nanoseconds", 1000\n
*         "usec", 1\n
*         "milliseconds", 0.0001\n
*         "seconds", 0.0000001\n
*         "minutes", 0.0000000166\n
*         "hours",   0.000000000277
*
*  INPUTS:
*    dt - value in microseconds
*
*  OUTPUTS:
*    mult - factor to use to convert to the indicated units.
* 
*  RETURNS:
*    string - a string like "nanoseconds","usec",....
*
******************************************************************************/
PUBLIC const char *pmc_sys_get_units(DOUBLE dt,DOUBLE *mult)
{
    const char *units;
    
    if (dt < 1) {
        *mult  = 1000.0;
        units = "nanoseconds";
    } else if (dt < 1000) {
        *mult  = 1.0;
        units = "usec";
    } else if (dt < 1000*1000) {
        *mult  = 1.0/1000.0;
        units = "milliseconds";
    } else if (dt < 1000*1000*60) {
        *mult  = 1.0/(1000.0*1000.0);
        units = "seconds";
    } else if (dt < 1000.0*1000*60*60) {
        *mult  = 1.0/(60.0*1000.0*1000.0);
        units = "minutes";
    } else {
        *mult  = 1.0/(60.0*60.0*1000.0*1000.0);
        units = "hours";
    }
    return units;
}

/****************************************************************************
*   pmc_sys_print_time
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Prints a DOUBLE (dt * factor) using 4 significant figures
*
*  INPUTS:
*    dt   - delta time in microseconds.
*    mult - multiplication factor.
*    buff - char * buffer of at least char[10].
*
*  RETURNS:
*    char - Returns a pointer to buff for convenience.
*
**********************************************************************/
PUBLIC char *pmc_sys_print_time(DOUBLE dt,DOUBLE mult,char *buff)
{
    DOUBLE x = dt * mult;
    /* check for negative numbers */
    if (x<0) {
        *buff++ = '-';
        x = -x;
    }
    /* print very large numbers in exponential notation */
    if (x >= 1E6) {
        sprintf(buff,"%0.3e",x);
    } if (x >= 1000) {
        sprintf(buff,"%0.0f",x);
    } else if (x >= 100) {
        sprintf(buff,"%0.1f",x);
    } else if (x >= 10) {
        sprintf(buff,"%0.2f",x);
    } else if (x >= 1) {
        sprintf(buff,"%0.3f",x);
    } else {
        sprintf(buff,"%0.4f",x);
    }
    return buff;
}

/*******************************************************************************
*  pmc_sys_stat_calc_avg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Calculate the average time
*
*  INPUTS:
*   s - pointer to statistics structure
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   DOUBLE - average time in microseconds.
*
******************************************************************************/
PUBLIC DOUBLE pmc_sys_stat_calc_avg(pmc_sys_stat_t *s)
{
    DOUBLE avg;
    PMC_ENTRY();
    if (s->num == 0) {
        avg = 0;
    } else {
        avg = (s->total * 1.0)/s->num;
    }
    PMC_RETURN(avg);
}

/*******************************************************************************
*  pmc_sys_stat_show_avg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Displays the statistics average and total times.
*
*  INPUTS:
*   s - pointer to statistics structure
*   prefix - string to pre-pend to the output.
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   DOUBLE - average time in microseconds.
*
*  NOTES:   This function works with PMC_SYS_STAT_DECLARE and pmc_sys_mark_start()
*     and pmc_sys_mark_end().
*
*  A sample output is:
*       fw_bench_test_stats: n=12000000, total=985.3 milliseconds, avg=82.11 nanoseconds
*
******************************************************************************/
PUBLIC DOUBLE pmc_sys_stat_show_avg(pmc_sys_stat_t *s,const char*prefix)
{
    DOUBLE total_mult;
    const char *total_units;
    char total_str[20];
    char avg_str[20];
    DOUBLE avg = (s->total * 1.0)/s->num;
    DOUBLE avg_mult;
    const char *avg_units = pmc_sys_get_units(avg,&avg_mult);
    
    total_units = pmc_sys_get_units(s->total,&total_mult);
    pmc_sys_print_time(s->total,total_mult,total_str);
    
    pmc_sys_print_time(avg,avg_mult,avg_str);
    PMC_PRINT("%s%s%s: n=%u, total=%s %s, avg=%s %s\n",prefix,s->description,s->description2,s->num,total_str,total_units,avg_str,avg_units );
    return avg;
}

/*******************************************************************************
*  pmc_sys_stat_show_to_buffer
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Displays the statistics showing min/max and average times.
*
*  INPUTS:
*   s - pointer to statistics structure
*   prefix - string to pre-pend to the output.
*   buffer - storage space for the message
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
*  NOTES:
*   This function works with PMC_SYS_DSTAT_DECLARE and pmc_sys_dstat_record_duration()
*
*  A sample output is:
*       driver doorbell waits: n=1566479, total=12.84 seconds, min=2.000, max=1054, avg=8.194 usec
*
******************************************************************************/
PUBLIC void pmc_sys_dstat_show_to_buffer(pmc_sys_dstat_t *s,const char *prefix,char *buffer)
{
    DOUBLE total_mult;
    const char *total_units;
    char total_str[20];
    char in_progress_str[70];
    char *tail = buffer;
    
    PMC_ENTRY();
    if (s->mark_started) {
        const char *in_progress_units;
        DOUBLE in_progress_mult;
        pmc_sys_timeval_t now;
        PMC_TIMEVAL_READ(&now);
        UINT64 microseconds_now = tv_to_microseconds(&now);
        UINT64 microseconds_mark = tv_to_microseconds(&s->mark);
        UINT64 dt = microseconds_now - microseconds_mark;
        in_progress_units = pmc_sys_get_units(dt,&in_progress_mult);
        strcpy(in_progress_str," and one started ");
        pmc_sys_print_time(dt,in_progress_mult,&(in_progress_str[strlen(in_progress_str)]));
        sprintf(&(in_progress_str[strlen(in_progress_str)])," %s ago",in_progress_units);
    } else {
        strcpy(in_progress_str,"");
    }
    
    total_units = pmc_sys_get_units(s->total,&total_mult);
    pmc_sys_print_time(s->total,total_mult,total_str);
    if (s->num == 0) {
        PMC_SPRINTF(tail,"%s%s%s: no stats collected%s\n",prefix,s->description,s->description2,in_progress_str);
        tail += strlen(tail);
    } else if (s->total == 0.0) {
        PMC_SPRINTF(tail,"%s%s%s: %u%s\n",prefix,s->description,s->description2,s->num,in_progress_str );
        tail += strlen(tail);
    } else if (s->num == 1) {
        PMC_SPRINTF(tail,"%s%s%s: %s %s%s\n",prefix,s->description,s->description2,total_str,total_units,in_progress_str );
        tail += strlen(tail);
    } else {
        char min_str[20];
        char max_str[20];
        char avg_str[20];
        DOUBLE avg_mult;
        DOUBLE avg = (s->total * 1.0)/s->num;
        const char *avg_units = pmc_sys_get_units(avg,&avg_mult);

        pmc_sys_print_time(avg,avg_mult,avg_str);
        pmc_sys_print_time(s->min,avg_mult,min_str);
        pmc_sys_print_time(s->max,avg_mult,max_str);
        PMC_SPRINTF(tail,"%s%s%s: n=%u, total=%s %s, min=%s, max=%s, avg=%s %s%s\n",prefix,s->description,s->description2,s->num,total_str,total_units,min_str,max_str,avg_str,avg_units,in_progress_str );
        tail += strlen(tail);
    }
    PMC_RETURN();
}     

/*******************************************************************************
*  pmc_sys_stat_show
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Displays the statistics showing min/max and average times.
*
*  INPUTS:
*   s - pointer to statistics structure
*   prefix - string to pre-pend to the output.
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
*  NOTES:
*   This function works with PMC_SYS_STAT_DECLARE and pmc_sys_mark_start()
*   and pmc_sys_mark_end().
*
*  A sample output is:
*       driver doorbell waits: n=1566479, total=12.84 seconds, min=2.000, max=1054, avg=8.194 usec
*
******************************************************************************/
PUBLIC void pmc_sys_stat_show(pmc_sys_stat_t *s,const char *prefix)
{
    DOUBLE total_mult;
    const char *total_units;
    char total_str[20];
    char in_progress_str[70];
    
    if (s->mark_started) {
        const char *in_progress_units;
        DOUBLE in_progress_mult;
        pmc_sys_timeval_t now;
        PMC_TIMEVAL_READ(&now);
        UINT64 microseconds_now = tv_to_microseconds(&now);
        UINT64 microseconds_mark = tv_to_microseconds(&s->mark);
        UINT64 dt = microseconds_now - microseconds_mark;
        in_progress_units = pmc_sys_get_units(dt,&in_progress_mult);
        strcpy(in_progress_str," and one started ");
        pmc_sys_print_time(dt,in_progress_mult,&(in_progress_str[strlen(in_progress_str)]));
        sprintf(&(in_progress_str[strlen(in_progress_str)])," %s ago",in_progress_units);
    } else {
        strcpy(in_progress_str,"");
    }
    
    total_units = pmc_sys_get_units(s->total,&total_mult);
    pmc_sys_print_time(s->total,total_mult,total_str);
    if (s->num == 0) {
        PMC_PRINT("%s%s%s: no stats collected%s\n",prefix,s->description,s->description2,in_progress_str);
    } else if (s->num == 1) {
        PMC_PRINT("%s%s%s: %s %s%s\n",prefix,s->description,s->description2,total_str,total_units,in_progress_str );
    } else {
        char min_str[20];
        char max_str[20];
        char avg_str[20];
        DOUBLE avg_mult;
        DOUBLE avg = (s->total * 1.0)/s->num;
        const char *avg_units = pmc_sys_get_units(avg,&avg_mult);

        pmc_sys_print_time(avg,avg_mult,avg_str);
        pmc_sys_print_time(s->min,avg_mult,min_str);
        pmc_sys_print_time(s->max,avg_mult,max_str);
        PMC_PRINT("%s%s%s: n=%u, total=%s %s, min=%s, max=%s, avg=%s %s%s\n",prefix,s->description,s->description2,s->num,total_str,total_units,min_str,max_str,avg_str,avg_units,in_progress_str );
        if (TRUE == pmc_atomic_attach && s->atomic_num != 0)
        {
            avg = (s->atomic_total * 1.0)/s->atomic_num;
            avg_units = pmc_sys_get_units(avg,&avg_mult);

            pmc_sys_print_time(avg,avg_mult,avg_str);
            pmc_sys_print_time(s->atomic_min,avg_mult,min_str);
            pmc_sys_print_time(s->atomic_max,avg_mult,max_str);

            PMC_PRINT("%s%s%s: n=%u, total=%s %s, min=%s, max=%s, avg=%s %s\n",s->description,s->description2,"::pmc_atomic",s->atomic_num,total_str,total_units,min_str,max_str,avg_str,avg_units );
        }
    }
}                                                                                                             

/*******************************************************************************
*  pmc_sys_fw_stat_show
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Displays percent active - (average duration/average period)*100.
*   Displays min/max and average times (duration:)
*   Displays min/max and average times (period: time between successive 'starts')
*
*  INPUTS:
*   s - pointer to statistics structure
*   prefix - string to pre-pend to the output.
*
*  OUTPUTS:
*   None.
*
*  RETURNS:
*   None.
*
*  NOTES:
*  A sample output is:
*    FIRMWARE: DIGI_1 KERN_BG_TASK duration: 19.98% n=1421510, total=520.4 milliseconds, min=336.7, max=1517617, avg=366.1 nanoseconds and one in progress
*                                  user:     19.25% n=1421510, total=501.3 milliseconds, min=0.0000, max=1446443, avg=352.7 nanoseconds and one in progress
*                                  period:   n=1421509, total=2.604 seconds, min=1.720, max=1519, avg=1.832 usec
*    FIRMWARE: DIGI_1 WDOG_KICK 0.0002% n=21, total=4.973 usec, min=226.7, max=333.3, avg=236.8 nanoseconds
*                               period: n=20, total=2.500 seconds, min=125.0, max=125.0, avg=125.0 milliseconds
*
*  If the "duration:" and "user:" are specified, then
*   "duration:" is the wall clock from start to finish, and
*   "user:" excludes any time spent in periodic timer events
*
******************************************************************************/
PUBLIC void pmc_sys_fw_stat_show(pmc_sys_fw_stats_t *s,const char *prefix)                                    
{                                                                                                 
    DOUBLE total_mult;                                                                            
    const char *total_units;                                                                      
    char total_str[20];                                                                           
    char in_progress_str[70];  
    UINT64 num_periods;    
    BOOL just_value = (s->description[0] == 'i')?TRUE:FALSE;  /**!< if the statistic starts with 'i' just display numbers, no times */
    double convert_period_to_microseconds = 1/300.0;
    double convert_value_to_microseconds;
         
    if (s->mark_started) {                                                                        
        strcpy(in_progress_str," AND ONE IN PROGRESS <-- "); 
        num_periods = s->num;                                         
    } else {                                                                                      
        strcpy(in_progress_str,"");                                                               
        num_periods = s->num - 1;
    }                                                                                             
     
    if (just_value) {
        convert_value_to_microseconds = 1;  
        total_units = "";                         
        total_mult = 1;
    } else {                                
        convert_value_to_microseconds= 1.0/300.0;   
        total_units = pmc_sys_get_units(s->duration_sum*convert_value_to_microseconds,&total_mult);                                   
    }
    pmc_sys_print_time(s->duration_sum*convert_value_to_microseconds,total_mult,total_str);                                        
    if (s->num == 0) {                                                                            
        FW_LOG("FIRMWARE: %s %s: no stats collected%s\n",prefix,s->description,in_progress_str);             
    } else if (s->num == 1) {                                                                     
        FW_LOG("FIRMWARE: %s %s: %s %s%s\n",prefix,s->description,total_str,total_units,in_progress_str );   
    } else {                                                                                      
        char min_str[20];                                                                         
        char max_str[20];                                                                         
        char avg_str[20];                                                                         
        DOUBLE avg_mult;                                                                          
        DOUBLE avg_duration;                                                                       
        DOUBLE avg_user;                                                                      
        DOUBLE avg_period;                                                                        
        const char *avg_units;
        DOUBLE ratio_duration_over_period; /* avg_duration / avg_period */
        DOUBLE ratio_user_over_period;     /* avg_user / avg_period */
        char percent_str[20];  
        const char *period_label;
        const char *duration_label;
        const char *user_label;
        BOOL print_duration = FALSE;
        BOOL print_user = FALSE;
        UINT32 indent;
        BOOL first_line_printed = FALSE;


        avg_duration = (s->duration_sum * convert_value_to_microseconds)/s->num;
        avg_user = (s->user_sum * convert_value_to_microseconds)/s->num;
        avg_period = (s->period_sum * convert_period_to_microseconds)/(s->num-1);

        if (s->user_sum != s->duration_sum) {
            print_duration = TRUE;
        }
        if (s->user_sum != 0) {
            print_user = TRUE;
        }
        if (just_value) {
            duration_label = " x:      ";
            period_label   = " period: ";
            user_label     = " y:      ";
        } else if (print_duration && print_user) {
            duration_label = " duration:";
            period_label   = " period:  ";
            user_label     = " user:    ";
        } else {
            duration_label = "";
            period_label   = " period:";
            user_label     = "";
        }
        indent = strlen("FIRMWARE:  ")+strlen(prefix)+strlen(s->description);
        
        FW_LOG("FIRMWARE: %s %s",prefix,s->description);
        if (s->user_sum != s->duration_sum) {
            if (just_value) {
                *percent_str = 0;
            } else {
                ratio_duration_over_period = (avg_duration) / (avg_period);
                pmc_sys_print_time(ratio_duration_over_period,100,percent_str);
                strncat(percent_str,"% ",sizeof(percent_str)-1);
                percent_str[sizeof(percent_str)-1] = '\0'; /* ensure that there is still a null terminator */
            }
            if (just_value) {
                avg_units = "";
                avg_mult = 1;
            } else {
                avg_units = pmc_sys_get_units(avg_duration,&avg_mult);
            }

            pmc_sys_print_time(avg_duration,avg_mult,avg_str);
            pmc_sys_print_time(s->duration_min * convert_value_to_microseconds,avg_mult,min_str);
            pmc_sys_print_time(s->duration_max * convert_value_to_microseconds,avg_mult,max_str);
            first_line_printed = TRUE;
            FW_LOG("%s %sn=%llu, total=%s %s, min=%s, max=%s, avg=%s %s%s\n",duration_label,percent_str,s->num,total_str,total_units,min_str,max_str,avg_str,avg_units,in_progress_str );                                                                                      
        }

        if (s->user_sum != 0) {
            if (just_value) {
                total_units = "";
                total_mult = 1;
            } else {
                total_units = pmc_sys_get_units(s->user_sum * convert_value_to_microseconds,&total_mult);
            }
            pmc_sys_print_time(s->user_sum * convert_value_to_microseconds,total_mult,total_str); 

            avg_user = (s->user_sum * convert_value_to_microseconds * 1.0)/s->num;
            if (just_value) {
                avg_units = "";
                avg_mult = 1;
            } else {
                avg_units = pmc_sys_get_units(avg_user,&avg_mult);
            }

            if (just_value) {
                *percent_str = 0;
            } else {
                ratio_user_over_period = (avg_user) / (avg_period);
                pmc_sys_print_time(ratio_user_over_period,100,percent_str);
                strncat(percent_str,"% ",sizeof(percent_str)-1);
                percent_str[sizeof(percent_str)-1] = '\0'; /* ensure that there is still a null terminator */
            }

            pmc_sys_print_time(avg_user,avg_mult,avg_str);
            pmc_sys_print_time(s->user_min * convert_value_to_microseconds,avg_mult,min_str);
            pmc_sys_print_time(s->user_max * convert_value_to_microseconds,avg_mult,max_str);
            if (first_line_printed)
                FW_LOG("%*s",indent,"");
            first_line_printed = TRUE;
            FW_LOG("%s %sn=%llu, total=%s %s, min=%s, max=%s, avg=%s %s%s\n",user_label,percent_str,s->num,total_str,total_units,min_str,max_str,avg_str,avg_units,in_progress_str );                                                                                      
        }

        total_units = pmc_sys_get_units(s->period_sum * convert_period_to_microseconds,&total_mult);
        pmc_sys_print_time(s->period_sum * convert_period_to_microseconds,total_mult,total_str);    

        avg_period = (s->period_sum * convert_period_to_microseconds * 1.0)/(num_periods);
        avg_units = pmc_sys_get_units(avg_period,&avg_mult);

        pmc_sys_print_time(avg_period,avg_mult,avg_str);
        pmc_sys_print_time(s->period_min * convert_period_to_microseconds,avg_mult,min_str);
        pmc_sys_print_time(s->period_max * convert_period_to_microseconds,avg_mult,max_str);
        if (first_line_printed)
            FW_LOG("%*s",indent,"");
        FW_LOG("%s n=%llu, total=%s %s, min=%s, max=%s, avg=%s %s\n",period_label,s->num-1,total_str,total_units,min_str,max_str,avg_str,avg_units );
                                                                 

    }


}  /* pmc_sys_fw_stat_show */


/*******************************************************************************
*  pmc_sys_stat_show_all
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Displays all updated statistics since the last call to
*   pmc_sys_stat_show_all().
*
*  INPUTS:
*   None.
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   None.
*
*  NOTES:
*    This function works with PMC_SYS_STAT_DECLARE and also with the io layer
*    to track number of register accesses.
*
*  A sample output is:
*       fw_bench_test_stats: n=12000000, total=985.3 milliseconds, avg=82.11 nanoseconds\n
*       driver doorbell waits: n=1566479, total=12.84 seconds, min=2.000, max=1054, avg=8.194 usec\n
*       num_reg_read = 6343256\n
*       num_reg_write = 3132958\n
*       num_msleep_calls = 983\n
*       num_usleep_calls = 3209314\n
*       num_milliseconds_slept = 983\n
*       num_microseconds_slept = 3209314\n
*       system time: 16.67 seconds
*
******************************************************************************/
void pmc_sys_stat_show_all()
{
    pmc_sys_stat_t *s;

    PMC_PRINT("\n<PMC_DIGI_STATS_DUMP_FUNC_DETAIL>\n");
    for (s=gbl_all_stats;s!=NULL;s=s->next)
    {
        if (s->num != 0)
        {
            if (s->display_average) {
                pmc_sys_stat_show_avg(s,"");
            } else {
                pmc_sys_stat_show(s,"");
            }
            s->num = 0;
        }
    }
    PMC_PRINT("</PMC_DIGI_STATS_DUMP_FUNC_DETAIL>\n\n");

    PMC_PRINT("<PMC_DIGI_STATS_DUMP_LOW_LEVEL>\n");
    pmc_os_stats_show(&pmc_os_stats_show_all_snapshot);
    pmc_os_stats_snapshot(&pmc_os_stats_show_all_snapshot);
    PMC_PRINT("</PMC_DIGI_STATS_DUMP_LOW_LEVEL>\n\n");
}

/*******************************************************************************
*  bench_entire_loop
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Call cb(param) num times and record the total time, also call
*   final_cb(param) once at the end and include this time as part of the benchmark.
*
*  INPUTS:
*   cb - function to call num times
*   final_cb - function to call at end of loop
*   param - param to pass to the callbacks
*   num - number of times to call
*
*  OUTPUTS:
*   bench - statistics captured for benchmark
* 
*  RETURNS:
*   None.
*
*  NOTES:
******************************************************************************/
static void bench_entire_loop(void (*cb)(void *), void (*final_cb)(void *), void *param, pmc_sys_stat_t *bench, int num ) /* __attribute__((noinline)) */
{
    int i;
    pmc_sys_stat_mark_start(bench);
    for (i=0;i<num;i++)
    {
        cb(param);
    }
    if (final_cb != NULL) {
        final_cb(param);
    }
    pmc_sys_stat_mark_end(bench);
}

/*******************************************************************************
*  bench_each_iteration
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Call cb(param) for the duration_in_microseconds and record the time to for
*   for each call also call final_cb(param) once at the end and include this
*   time as part of the previous call to cb.
*
*  INPUTS:
*   cb - function to call num times
*   final_cb - function to call at end of loop
*   param - param to pass to the callbacks
*   duration_in_microseconds - how long to run the benchmark for
*
*  OUTPUTS:
*   bench - statistics captured for benchmark
* 
*  RETURNS:
*   None.
*
*  NOTES:
******************************************************************************/
static void bench_each_iteration(void (*cb)(void *), void (*final_cb)(void *), void *param, pmc_sys_stat_t *bench, UINT64 duration_in_microseconds ) /* __attribute__((noinline)) */
{
    pmc_sys_timeval_t now;
    UINT64 microseconds_start;
    UINT64 microseconds_now;
    UINT64 dt;

    PMC_TIMEVAL_READ(&now);
    microseconds_start = tv_to_microseconds(&now);
    do
    {
        pmc_sys_stat_mark_start(bench);
        cb(param);
        microseconds_now = tv_to_microseconds(&bench->mark);
        dt = microseconds_now - microseconds_start;
    } while (dt < duration_in_microseconds);
    if (final_cb != NULL) {
        final_cb(param);
    }
    pmc_sys_stat_mark_end(bench);
}


/*******************************************************************************
*  pmc_sys_stat_bench2
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*    1. loops calling _cb(param) for duration_in_seconds time\n
*    2. displays the detailed statistics for that (including min and max times).\n
*    3. saves the number of iterations that it took as 'num'.\n
*    4. calls the function 'num' times in a simple loop.\n
*    5. displays the overall it takes to call the function 'num' times.
*        (the overall time has no gettimeofday() overhead.)
*
*  INPUTS:
*          _cb - benchmark function
*          _final_cb - final call at end of benchmark
*          param - parameter to pass to the function to be benchmarked.
*          duration_in_seconds - how long to run the benchmark for
*          description - description of the benchmark.
*
*  OUTPUTS:
*   None.
* 
*  RETURNS:
*   DOUBLE - average time to call the function using the overall benchmark (in microseconds)
*
*  NOTES:
*   A sample output is:
*
*    each iteration pmc_sys_burst_write: n=775073, total=199.3 milliseconds, min=0.0000, max=60000, avg=257.1 nanoseconds
*    overall        pmc_sys_burst_write: n=775073, total=199.6 milliseconds, avg=257.5 nanoseconds
*
*    note: the resolution of the timer is 1 microsecond, and various operating system
*    events add delays.  Despite this, the values can be informative, especially if only
*    one task is running.
*
******************************************************************************/
DOUBLE pmc_sys_stat_bench2( void *_cb, void *_final_cb, void *param, DOUBLE duration_in_seconds, const char *description )
{
    pmc_sys_stat_t bench[1];
    void (*cb)(void *) = (void (*)(void*))_cb;
    void (*final_cb)(void *) = (void (*)(void*))_final_cb;
    DOUBLE avg;
    UINT32 num;

    /* initialize a private statistics structure (don't link with the global list of all statistics structures)*/
    bench->num = 0;
    bench->total = 0;
    bench->mark_started = 0;
    bench->initialized = 1;
    bench->description = description;
    bench->description2 = "";

    bench->atomic_num = 0;
    bench->atomic_total = 0;
    bench->atomic_min = 0;
    bench->atomic_max = 0;

    PMC_PRINT("testing %s ... \n",description);
    /* benchmark each iteration until the duration elapses */
    bench_each_iteration(cb,final_cb,param,bench,duration_in_seconds * 1000000);    
    pmc_sys_stat_show(bench,"each iteration ");

    /* save the number of iterations */
    num = bench->num;

    /* reset the statistics */
    bench->num = 0;

    /* benchmark it again,... this time without calling gettimeofday() each iteration */
    bench_entire_loop(cb,final_cb,param,bench,num);

    bench->num = num;
    avg = pmc_sys_stat_show_avg(bench,"overall        ");

    PMC_PRINT("\n");

    return avg;
}
#endif /* DOXYGEN_PUBLIC_ONLY */

/*********************************************************************
* pmc_sys_int_cache_enable_set
* ____________________________________________________________________
*
* DESCRIPTION:
*   Enable/Disable interrupt caching. The cache can be optionally 
*   cleared. The thread id of the caller is captured by the function. 
*   The interrupt caching is enabled only on the thread that has called 
*   this function 
*
* INPUTS:
*    *sys_handle - Pointer to system handle
*    enable      - TRUE enables the cache, FALSE disables the cache
*    clear       - TRUE clears the cache, FALSE is not clearing the the cache
*
* OUTPUTS: 
*   None
*
* RETURNS:
*   None
*
* NOTES:
* 
**********************************************************************/
PUBLIC void pmc_sys_int_cache_enable_set(pmc_sys_handle_t *sys_handle, BOOL8 enable, BOOL8 clear)
{
#ifndef NO_CONFIG_CACHE   
#ifndef NO_CHANGE_SETS    
    l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;

    int_cache_enable_set(sys_handle,
                         &h->int_cache, 
                         enable,clear);
#endif
#endif

} /* pmc_sys_int_cache_enable_set */

/*******************************************************************************
* digi_use_float32_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set use FLOAT or DOUBLE data type in the SDK.
*
* INPUTS:
*   float_mode  - TRUE to use FLOAT
*               - FLASE to use DOUBLE
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    void - None.
*
*******************************************************************************/
PUBLIC void digi_use_float32_set(BOOL8 float_mode)
{
    PMC_ENTRY();
    digi_use_float32_mode = float_mode;

    PMC_RETURN();
}

/*******************************************************************************
* digi_use_float32_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to get float usage mode in the SDK.
*
* INPUTS:
*   none
*  
* OUTPUTS:
*   none
*
* RETURNS:
*   BOOL8 - float mode. TRUE to use FLOAT, while FLASE to use DOUBLE.
*
*******************************************************************************/
PUBLIC BOOL8 digi_use_float32_get(void)
{
    PMC_ENTRY();

    PMC_RETURN(digi_use_float32_mode);
}


