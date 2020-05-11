/******************************************************************************/
/**  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
/**--------------------------------------------------------------------------**/
/** This software embodies materials and concepts which are proprietary and  **/
/** confidential to PMC-Sierra, Inc. PMC-Sierra distributes this software    **/
/** to its customers pursuant to the terms and conditions of the Software    **/
/** License Agreement contained in the text file software.lic that is        **/
/** distributed along with the software. This software can only be utilized  **/
/** if all terms and conditions of the Software License Agreement are        **/
/** accepted. If there are any questions, concerns, or if the                **/
/** Software License Agreement text file, software.lic, is missing please    **/
/** contact PMC-Sierra for assistance.                                       **/
/**--------------------------------------------------------------------------**/
/**                                                                          **/
/******************************************************************************/
/*******************************************************************************
**  MODULE      : SWIG interface to DIGI library
**
**  FILE        : digi.i
**
**  $Date$
**
**  $Revision$
**
**  DESCRIPTION :  This file contains SWIG stubs for interfacing DIGI
**                 library to a Tcl shell.
**
**  NOTES       :
**
*******************************************************************************/
%module digi
%include cpointer.i
%include carrays.i
%{

/* PMC */
#include "pmc_types.h"
#include "pmc_sys.h"
#include "pmc_log.h"
#include "pmc_err.h"

/* DIGI Top APIs */
#include "digi.h"
#include "digi_api.h"
#include "digi_cbr_api.h"
#include "digi_enet_api.h"
#include "digi_ghao_api.h"
#include "digi_map_api.h"
#include "digi_otn_api.h"
#include "digi_pmon_api.h"
#include "digi_resource_api.h"
#include "digi_sifd_api.h"
#include "digi_timesync_api.h"
#include "digi_util_api.h"

/* DIGI Utils */
#include "digi_util_loc.h"
#include "util_global_api.h" /* Needed for CPB_EMUL_PORT_USE_SET */
#include "util_global.h"
#include "util_schd.h"

/* DIGI Subsystem */
#include "cbrc_api.h"
#include "coreotn_api.h"
#include "odu_struct_api.h"
#include "cpb_api.h"
#include "enet_api.h"
#include "lifd_api.h"
#include "lineotn_api.h"
#include "mapotn_api.h"
#include "sifd_api.h"

/* DIGI Block */
#include "cbr_fsgm_api.h"
#include "cfc_api.h"
#include "cpuif_api.h"
#include "d8_tx_2x_api.h"
#include "eclass_api.h"
#include "emac_api.h"
#include "empei_e_api.h"
#include "empei_i_api.h"
#include "enet_fege_api.h"
#include "epmm_api.h"
#include "etime_api.h"
#include "etrans_api.h"
#include "exco3m_api.h"
#include "exco4m_api.h"
#include "expand_ibuf_api.h"
#include "fc1200_pmon_api.h"
#include "fc800_pmg_api.h"
#include "gsup43_c73_api.h"
#include "mgmt_fege_api.h"
#include "mpma_api.h"
#include "mpmo_api.h"
#include "odtu_dmx_api.h"
#include "rgmp_api.h"
#include "odtu_mux_api.h"
#include "tgmp_api.h"
#include "odu_ohp_api.h"
#include "odu_rcp_api.h"
#include "odu_rfrm_api.h"
#include "odu_tfrm_api.h"
#include "odujat_api.h"
#include "oduksc_api.h"
#include "ohfs_insert_api.h"
#include "otu_frm_api.h"
#include "s16_api.h"
#include "sdh_pmg_api.h"
#include "txjc_proc_api.h"

/* PV Type Accessors */
#include "pv_accessors.h"
  
#ifdef PMC_TESTS

/* common component */
#include "digi_test.h"
#include "digi_test_fw.h"
#include "digi_test_device_cfg_template.h"
#include "digi_test_span_cfg_template.h"
#include "digi_test_pmon_int_util.h"
#include "digi_test_coreotn.h"
#include "digi_test_recovery_generic_benchmark.h"
 
/* tests */
#include "digi_test_template_m1.h"
#include "digi_test_template_m2.h"
#include "digi_test_pmon_lclk.h"
#include "digi_test_mapotn_runtime.h"
#include "digi_test_mapotn_restart.h"
#include "digi_test_enet_runtime.h"
#include "digi_test_enet_runtime_sw_update.h"
#include "digi_test_enet_restart.h"
#include "digi_test_enet_enh_pmon.h"
#include "digi_test_coreotn_restart.h"
#include "digi_test_coreotn_asym_msi.h"
#include "digi_test_coreotn_asym_trib.h"
#include "digi_test_coreotn_tcm.h"
#include "digi_test_coreotn_tcm_enh.h"
#include "digi_test_coreotn_maint_sig.h"
#include "digi_test_coreotn_ohp.h"
#include "digi_test_coreotn_rcp.h"
#include "digi_test_coreotn_def_act_dis.h"
#include "digi_test_crash_warm_restart.h"
#include "digi_test_multi_thread_tests.h"
#include "digi_test_lineotn_restart.h"
#include "digi_test_cbrc_restart.h"
#include "digi_test_serdes_port.h"
#include "digi_test_bp_serdes.h"
#include "digi_test_sifd_cbrc.h"
#include "digi_test_sifd_api.h"
#include "digi_test_sifd_api_others.h"
#include "digi_test_cbrc_oc192.h"
#include "digi_test_cbrc_fc800.h"
#include "digi_test_cbrc_fc1200.h"
#include "digi_test_cbrc_sdh_prbs.h"
#include "digi_test_lineotn_runtime.h"
#include "digi_test_lineotn_runtime_sw_update.h"
#include "digi_test_lineotn_inactive_ch.h"
#include "digi_test_lineotn_more_ddeg.h"
#include "digi_test_sysotn_ddeg.h"
#include "digi_test_sysotn_ddeg_stress.h"
#include "digi_test_sysotn_runtime.h"
#include "digi_test_loopbacks.h"
#include "digi_test_cpb_block_remap.h"
#include "digi_test_coreotn_features.h"
#include "digi_test_coreotn_layers.h"
#include "digi_test_pmon_status.h"
#include "digi_test_ohp_example.h"
#include "digi_test_art_prototypes.h"
#include "digi_test_cwr_prototypes.h"
#include "digi_test_device_int.h"
#include "digi_test_device_int_def.h"
#include "digi_test_line_core_map_int.h"
#include "digi_test_enet_line_int.h"
#include "digi_test_enet_sys_int.h"
#include "digi_test_enet_mgmt_int.h"
#include "digi_test_lldp.h"
#include "digi_test_cbrc_int.h"
#include "digi_test_ptp_bc.h"
#include "digi_test_ptp_bc_sw_update.h"
#include "digi_test_ptp_bc_mapotn.h"
#include "digi_test_ptp_mgmt_simultaneous.h"
#include "digi_test_ilkn_unidirectional_path.h"
#include "digi_test_100ge_mac_line_sys.h"
#include "digi_test_100ge_mated_protection.h"
#include "digi_test_serdes_port_reconfig.h"
#include "digi_test_cbrc_sfi51_restart.h"
#include "digi_test_gsup43_c73.h"
#include "digi_test_cpb_broadcast.h"
#include "digi_test_d8_serdes.h"
#include "digi_test_pcie_int_example.h"
#include "digi_test_datapath_sys_odu4_odu2_to_gsup73_pmon.h"
#include "digi_test_cmf_force_lf_on_bringup.h"
#include "digi_test_2B_header_ilkn.h"
#endif 

#ifdef PMC_SS_EMULATION
#include "util_global.h"
#include "enet.h"
#include "sifd.h"
#include "cpb.h"
#include "cbrc.h"
#include "mapotn.h"
#include "coreotn.h"
#include "oduksc.h"
#endif

/* Global data structure to store swig environment information */
typedef struct swig_info_
{
    digi_handle_t       *digi_handle_ptr;
    pmc_sys_handle_t    *sys_handle_ptr;
} swig_info_t;

typedef struct
{
    digi_ilkn_chnl_target_t  target;
    UINT32  tx_ilkn_bcw_chnl;
    UINT16  tx_2b_header;
    UINT32  tx_4b_header;
    UINT32  rx_ilkn_bcw_chnl;
    UINT16  rx_2b_header;
    UINT32  rx_4b_header;
}digi_ilkn_channel_desc_t;

Tcl_Interp    *digi_interp;
swig_info_t   *swig_global = NULL;

PMC_ERROR dummy_int_cb(digi_handle_t         *digi_handle,
                       void                  *token,
                       void                  *channel_handle,
                       digi_int_type_t        int_type)
{
    return PMC_SUCCESS;
};

%}

/* PMC */
%include "pmc_types.h"
%include "pmc_sys.h"
%include "pmc_log.h"
%include "pmc_err.h"

/* DIGI Top APIs */
%include "digi.h"
%include "digi_api.h"
%include "digi_cbr_api.h"
%include "digi_enet_api.h"
%include "digi_ghao_api.h"
%include "digi_map_api.h"
%include "digi_otn_api.h"
%include "digi_pmon_api.h"
%include "digi_resource_api.h"
%include "digi_sifd_api.h"
%include "digi_timesync_api.h"
%include "digi_util_api.h"

/* DIGI Utils */
%include "digi_util_loc.h"
%include "util_global_api.h" /* Needed for CPB_EMUL_PORT_USE_SET */
%include "util_global.h"
%include "util_schd.h"

/* DIGI Subsystem */
%include "cbrc_api.h"
%include "coreotn_api.h"
%include "odu_struct_api.h"
%include "cpb_api.h"
%include "enet_api.h"
%include "lifd_api.h"
%include "lineotn_api.h"
%include "mapotn_api.h"
%include "sifd_api.h"

/* DIGI Block */
%include "cbr_fsgm_api.h"
%include "cfc_api.h"
%include "cpuif_api.h"
%include "d8_tx_2x_api.h"
%include "eclass_api.h"
%include "emac_api.h"
%include "empei_e_api.h"
%include "empei_i_api.h"
%include "enet_fege_api.h"
%include "epmm_api.h"
%include "etime_api.h"
%include "etrans_api.h"
%include "exco3m_api.h"
%include "exco4m_api.h"
%include "expand_ibuf_api.h"
%include "fc1200_pmon_api.h"
%include "fc800_pmg_api.h"
%include "gsup43_c73_api.h"
%include "mgmt_fege_api.h"
%include "mpma_api.h"
%include "mpmo_api.h"
%include "odtu_dmx_api.h"
%include "rgmp_api.h"
%include "odtu_mux_api.h"
%include "tgmp_api.h"
%include "odu_ohp_api.h"
%include "odu_rcp_api.h"
%include "odu_rfrm_api.h"
%include "odu_tfrm_api.h"
%include "odujat_api.h"
%include "oduksc_api.h"
%include "ohfs_insert_api.h"
%include "otu_frm_api.h"
%include "s16_api.h"
%include "sdh_pmg_api.h"
%include "txjc_proc_api.h"

/* PV Type Accessors */
%include "pv_accessors.h"

%constant PMC_ERROR (*DUMMY_INT_CB)(digi_handle_t *, void *, void *, digi_int_type_t) = dummy_int_cb;

#ifdef PMC_TESTS
/* common component */
%include "digi_test.h"
%include "digi_test_fw.h"
%include "digi_test_device_cfg_template.h"
%include "digi_test_span_cfg_template.h"
%include "digi_test_pmon_int_util.h"
%include "digi_test_coreotn.h"
%include "digi_test_recovery_generic_benchmark.h"

/* tests */
%include "digi_test_template_m1.h"
%include "digi_test_template_m2.h"
%include "digi_test_pmon_lclk.h"
%include "digi_test_mapotn_runtime.h"
%include "digi_test_mapotn_restart.h"
%include "digi_test_enet_runtime.h"
%include "digi_test_enet_runtime_sw_update.h"
%include "digi_test_enet_restart.h"
%include "digi_test_enet_enh_pmon.h"
%include "digi_test_coreotn_restart.h"
%include "digi_test_coreotn_tcm.h"
%include "digi_test_coreotn_tcm_enh.h"
%include "digi_test_coreotn_maint_sig.h"
%include "digi_test_coreotn_ohp.h"
%include "digi_test_coreotn_rcp.h"
%include "digi_test_coreotn_def_act_dis.h"
%include "digi_test_coreotn_asym_msi.h"
%include "digi_test_coreotn_asym_trib.h"
%include "digi_test_crash_warm_restart.h"
%include "digi_test_multi_thread_tests.h"
%include "digi_test_lineotn_restart.h"
%include "digi_test_cbrc_restart.h"
%include "digi_test_serdes_port.h"
%include "digi_test_bp_serdes.h"
%include "digi_test_sifd_cbrc.h"
%include "digi_test_sifd_api.h"
%include "digi_test_sifd_api_others.h"
%include "digi_test_cbrc_oc192.h"
%include "digi_test_cbrc_fc800.h"
%include "digi_test_cbrc_fc1200.h"
%include "digi_test_cbrc_sdh_prbs.h"
%include "digi_test_lineotn_runtime.h"
%include "digi_test_lineotn_runtime_sw_update.h"
%include "digi_test_lineotn_inactive_ch.h"
%include "digi_test_lineotn_more_ddeg.h"
%include "digi_test_sysotn_ddeg.h"
%include "digi_test_sysotn_ddeg_stress.h"
%include "digi_test_sysotn_runtime.h"
%include "digi_test_loopbacks.h"
%include "digi_test_cpb_block_remap.h"
%include "digi_test_coreotn_features.h"
%include "digi_test_coreotn_layers.h"
%include "digi_test_pmon_status.h"
%include "digi_test_ohp_example.h"
%include "digi_test_art_prototypes.h"
%include "digi_test_cwr_prototypes.h"
%include "digi_test_device_int.h"
%include "digi_test_device_int_def.h"
%include "digi_test_line_core_map_int.h"
%include "digi_test_enet_line_int.h"
%include "digi_test_enet_sys_int.h"
%include "digi_test_enet_mgmt_int.h"
%include "digi_test_lldp.h"
%include "digi_test_cbrc_int.h"
%include "digi_test_ptp_bc.h"
%include "digi_test_ptp_bc_sw_update.h"
%include "digi_test_ptp_bc_mapotn.h"
%include "digi_test_ptp_mgmt_simultaneous.h"
%include "digi_test_ilkn_unidirectional_path.h"
%include "digi_test_100ge_mac_line_sys.h"
%include "digi_test_100ge_mated_protection.h"
%include "digi_test_serdes_port_reconfig.h"
%include "digi_test_cbrc_sfi51_restart.h"
%include "digi_test_gsup43_c73.h"
%include "digi_test_cpb_broadcast.h"
%include "digi_test_d8_serdes.h"
%include "digi_test_pcie_int_example.h"
%include "digi_test_datapath_sys_odu4_odu2_to_gsup73_pmon.h"
%include "digi_test_cmf_force_lf_on_bringup.h"
%include "digi_test_2B_header_ilkn.h"
#endif 

#ifdef PMC_SS_EMULATION
%include "util_global.h"
%include "enet.h"
%include "sifd.h"
%include "cpb.h"
%include "cbrc.h"
%include "mapotn.h"
%include "coreotn.h"
%include "oduksc.h"
#endif

#if !defined(PMC_VERIFICATION) && !defined(PMC_SW_SIMULATION)

/* Accessors requested by SWIGPIG (PV-Only) */
%include "swigpig_accessors.i"

/* Convenient function to cast uint32 as a PMC_ERROR. 
> digi_err_to_string [uint32_to_pmc_error 0x12900005] */
%pointer_cast(UINT32, PMC_ERROR, uint32_to_pmc_error);

/* Create pmc_mem_align_enum */
%pointer_cast(UINT64, pmc_mem_align_enum, uint32_to_pmc_mem_align_enum);
 
#endif

%define TYPE_ACCESSORS(type)
%inline %{
type *scalar_ ## type ## _new() { return (type *)malloc(sizeof(type)); }
void scalar_ ## type ## _delete(type *ptr) { free(ptr); }
void scalar_ ## type ## _set(type *ptr, type val) { *ptr = val; }
type scalar_ ## type ## _get(type *ptr) { return (*ptr); }
type *vector_ ## type ## _new(int size) { return (type *)malloc(size*sizeof(type)); }
void vector_ ## type ## _delete(type v[]) { free(v); }
void vector_ ## type ## _set(type v[], int i, type val) { v[i] = val; }
type vector_ ## type ## _get(type v[], int i) { return v[i]; }
%}
%enddef

TYPE_ACCESSORS(UINT8)
TYPE_ACCESSORS(UINT16)
TYPE_ACCESSORS(UINT32)
TYPE_ACCESSORS(UINT64)
TYPE_ACCESSORS(BOOL)
TYPE_ACCESSORS(BOOL8)
TYPE_ACCESSORS(DOUBLE)

%inline %{ 
#include <stdlib.h>
#include "tcl.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "pmc_types.h"
#include "pmc_sys.h"


#define STR1(x) #x
#define STR(x)  STR1(x)

static const char swig_version[] = STR(BUILD_VERSION);

/* Stores Build Version for digi swig lib */
const char* swig_get_version()
{
    return swig_version;
}

pmc_sys_handle_t *get_pmc_sys_handle(void) {
    return swig_global->sys_handle_ptr;
}

digi_handle_t *swig_init() {
    PMC_ERROR rc;
    
    PMC_LOG_TRACE("Initialization: Starting Swig\n");

    /* Check if swig_init has already been called */ 
    if (NULL != swig_global)
    {
        PMC_LOG_TRACE("Initialization: swig_init() has already been called. Please use swig_destroy() to cleanup existing instance first.\n");
        return NULL;
    }

    /* Allocate memory for global swig info structure */
    swig_global = (swig_info_t*)PMC_MALLOC(sizeof(swig_info_t));
    if (NULL == swig_global)
    {
        PMC_LOG_TRACE("Initialization: Unable to allocate memory for global info structure\n");
        return NULL;
    }

    pmc_log_init(PMC_LOG_DEFAULT_SIZE);
    pmc_log_verbosity_level_set(PMC_LOG_SEV_LOWEST);
    pmc_log_block_strings_register(PMC_SHARED_LOG_ERR_STRINGS[0], PMC_SHARED_LOG_ERR_TABLE_BASE, PMC_SHARED_LOG_ERR_COUNT);


    PMC_LOG_TRACE("Initialization: Creating sys_handle\n");
    swig_global->sys_handle_ptr = pmc_sys_handle_create();
    if (NULL == swig_global->sys_handle_ptr)
    {
        PMC_LOG_TRACE("Initialization: Creating sys_handle failed\n");
        return NULL;
    }


    PMC_LOG_TRACE("Initialization: Mapping DIGI_0\n");
    rc = pmc_sys_init(swig_global->sys_handle_ptr, NULL, "DIGI_0");
    if (PMC_SUCCESS != rc) 
    {
        PMC_LOG_TRACE("Initialization: Mapping DIGI_0 failed\n");
        return NULL;
    }

    PMC_LOG_TRACE("Initialization: Creating DIGI context\n");
    swig_global->digi_handle_ptr = digi_ctxt_create(NULL, DIGI0_BASE_ADDRESS, swig_global->sys_handle_ptr, "DIGI_0");
    if (NULL == swig_global->digi_handle_ptr)
    {
        PMC_LOG_TRACE("Initialization: Creating DIGI context failed\n");
        return NULL;
    } 

    PMC_LOG_TRACE("Initialization: Complete.\n");
    return swig_global->digi_handle_ptr;
}


PMC_ERROR swig_destroy()
{
    if (NULL == swig_global)
    {
        PMC_LOG_TRACE("Cleanup: No swig info to cleanup. Please use swig_init() first.\n");
        return PMC_SUCCESS;
    }

    PMC_LOG_TRACE("Cleanup: Destroying DIGI context\n");
    digi_ctxt_destroy(&swig_global->digi_handle_ptr);
    
    PMC_LOG_TRACE("Cleanup: Destroying sys_handle\n");
    pmc_sys_handle_destroy(&swig_global->sys_handle_ptr);

    PMC_LOG_TRACE("Cleanup: Freeing global swig info\n");
    /* PMC_FREE sets swig_global back to NULL so that swig_init can be called again */
    PMC_FREE(&swig_global);

    PMC_LOG_TRACE("Cleanup: Complete.\n");
    return PMC_SUCCESS;
}

/* SWIG does not manage the double pointers so for the context destroy
   function which just sets the pointer to NULL this wrapper drops the
   NULL setting of the returned handle */
void swig_ctxt_destroy(digi_handle_t *digi_handle)
{
    PMC_ENTRY();

    digi_handle_t *h;

    h = digi_handle;
    PMC_LOG_TRACE("digi_handle = 0x%llx", (UINT64)digi_handle);
    PMC_LOG_TRACE("h           = 0x%llx", (UINT64)h);

    digi_ctxt_destroy(&h);

    PMC_RETURN();
}

void swig_sys_handle_destroy(pmc_sys_handle_t *sys_handle)
{
    PMC_ENTRY();

    pmc_sys_handle_destroy(&sys_handle);

    PMC_RETURN();
}


UINT32 digi_read(digi_handle_t *digi_handle, UINT32 reg)
{
    pmc_handle_t *h;

    h = (pmc_handle_t *) digi_handle;

    return l2sys_reg_read(h->sys_handle,
                          1,
                          h->base_address + reg);
}

void digi_write(digi_handle_t *digi_handle, UINT32 reg, UINT32 value)
{
    pmc_handle_t *h;

    h = (pmc_handle_t *) digi_handle;
    l2sys_reg_write(h->sys_handle,
                    1,
                    h->base_address + reg,
                    value);
}


/* Prints human readable string from PMC_ERROR code */
const char *digi_err_to_string(PMC_ERROR log_code)
{
    return pmc_log_string_get(log_code);
}







/*
 * Swig Test Functions
 */
PMC_ERROR swig_scratch_test()
{
    /* variable declaration */
    pmc_sys_handle_t *sys_handle = NULL;
    digi_handle_t    *digi_handle = NULL;
    UINT32            wr_val;
    UINT32            rd_val;
    UINT32            reg;
    UINT32            i, j;

    pmc_log_init(PMC_LOG_DEFAULT_SIZE);
    pmc_log_verbosity_level_set(PMC_LOG_SEV_HIGHEST);
    
    PMC_ENTRY();

    PMC_LOG_TRACE("Initialization: Creating sys_handle\n");
    sys_handle = pmc_sys_handle_create();
    /* PMC_SYS_INIT_DEFAULTS_AND_DESCRIPTIONS(sys_handle); */

    pmc_sys_init(sys_handle, NULL, "DIGI_0");
    PMC_LOG_TRACE("Initialization: Mmapping device dut.S\n");
    
    PMC_LOG_TRACE("Initialization: Creating DIGI context\n");
    digi_handle = digi_ctxt_create(NULL, 0, sys_handle, "DIGI");
 
    
    for (i = 0; i < 4; i++)
    {
        reg = 0x20 + 0x4*i;
        PMC_LOG_TRACE("Performing test read/writes to CTRL FPGA Reg 0x%x\n", reg);
        
        wr_val = 0;

        for (j = 0; j < 0xFFFF; j++) 
        {
            wr_val += 0xABCD1234;
            digi_write(digi_handle, reg, wr_val);

            rd_val = digi_read(digi_handle, reg);

            if (wr_val != rd_val)
            {
                PMC_LOG_TRACE("Test failed. reg 0x%x, read 0x%x, expected 0x%x\n", reg, rd_val, wr_val);
                PMC_RETURN(PMC_ERR_FAIL);
            }
        }
    }

                
    PMC_LOG_TRACE("SWIG Scratch test complete.\n"); 
    PMC_RETURN(PMC_SUCCESS);
}



PMC_ERROR swig_log_test()
{
    /* variable declaration */
    pmc_sys_handle_t *sys_handle = NULL;
    digi_handle_t    *digi_handle = NULL;
    
    pmc_log_init(PMC_LOG_DEFAULT_SIZE);
    pmc_log_verbosity_level_set(PMC_LOG_SEV_HIGHEST);
    
    PMC_ENTRY();

    PMC_LOG_TRACE("Initialization: Creating sys_handle\n");
    sys_handle = pmc_sys_handle_create();
    /* PMC_SYS_INIT_DEFAULTS_AND_DESCRIPTIONS(sys_handle); */

    pmc_sys_init(sys_handle, NULL, "DIGI_0");
    PMC_LOG_TRACE("Initialization: Mmapping device dut.S\n");

    PMC_LOG_TRACE("Initialization: Creating DIGI context\n");
    digi_handle = digi_ctxt_create(NULL, 0, sys_handle, "DIGI");


    PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_ERR_INVALID_ARG, 0, 0, "digi test_val = %u", 5678);  
    PMC_LOG_TRACE("Test: %s\n", digi_err_to_string(DIGI_ERR_INVALID_ARG));
    PMC_LOG_TRACE("Test: %s\n", digi_err_to_string(DIGI_ERR_UNKNOWN));
    PMC_LOG_TRACE("Test: %s\n", digi_err_to_string(DIGI_ERR_NULL_HANDLE));
    PMC_LOG_TRACE("Test: %s\n", digi_err_to_string(CPB_ERR_CTXT_NOT_INITIALIZED));
    PMC_LOG_TRACE("SWIG Error Strings test complete.\n"); 
    PMC_RETURN(PMC_SUCCESS);
}

PMC_ERROR swig_assert_test()
{
    PMC_ENTRY();
    PMC_ASSERT(FALSE,DIGI_ERR_CODE_ASSERT, 0, 0);
    
    PMC_LOG_TRACE("SWIG Assert test complete.\n"); 
    PMC_RETURN(PMC_SUCCESS);
}

PMC_ERROR swig_remove_wall_errors()
{
    /* digi_swig.c complains on compilation that some static functions are left
       unused. The best way to deal with this is to use them here directly */

    /* variable declaration */
    UINT32 rc = 0;
    UINT32 rv = 0;
    pmc_sys_handle_t *sys_handle = NULL;
    pmc_handle_t *pmc_handle;
    pmc_handle = NULL;

    /* The following functions are in pmc_sys.h */
    
    void *foo = PMC_MALLOC(42);
    void *bar = PMC_CALLOC(42);
    void *baz = PMC_CTXT_CALLOC(42, pmc_handle);
    PMC_CTXT_REGISTER_PTR(&baz, pmc_handle);
    PMC_STRCPY((char *)bar,(const char *)foo);
    PMC_STRCAT((char *)bar,(const char *)foo);
    (void)PMC_MEMCMP(foo, bar, 0);
    rc = PMC_STRLEN(bar);
    PMC_FREE(foo);
    PMC_FREE(bar);
    PMC_CTXT_FREE(baz, pmc_handle);

    PMC_LOG_TRACE("Creating sys_handle\n");
    sys_handle = pmc_sys_handle_create();
    /* PMC_SYS_INIT_DEFAULTS_AND_DESCRIPTIONS(sys_handle); */

    rc = pmc_sys_reg_read(sys_handle, 0x0);
    pmc_sys_reg_write(sys_handle, 0x0, 42);

    pmc_sys_burst_read(sys_handle, 0x0, 0x1, &rv);
    pmc_sys_burst_write(sys_handle, 0x0, 0x1, &rv);
    pmc_sys_reg_read_modify_write(sys_handle, 0x0, 0x0, rv, &rv);
    pmc_sys_mem_write(sys_handle, 0x0, 0x1, (UINT8 *)&rv);

    /* The following functions are in l5_sys.h */
    l5sys_reg_read(sys_handle, 0x0);
    l5sys_reg_write(sys_handle,0x0,0x1);
    l5sys_reg_read_modify_write(sys_handle, 0x0, 0x0, 0x0, &rv);
    l5sys_poll(sys_handle, 0x0, 0x0, 0x0, PMC_POLL_EQUAL, 0x1, &rv, 0x1);
    l5sys_burst_read(sys_handle, 0x0, 0x0, &rv);
    l5sys_burst_write(sys_handle, 0x0, 0x0, &rv);

    /* The following functions are in l1_sys.h */
    l1sys_init((coalesce_buffer_t*)(sys_handle), sys_handle);
    l1sys_force_read_modify_write((coalesce_buffer_t*)(sys_handle));
    l1sys_field_set2((coalesce_buffer_t*)(sys_handle), 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x42);
    l1sys_action_on_write_field_set((coalesce_buffer_t*)(sys_handle), 0x0, 0x0, 0x0, 0x1, 0x0, 0x42);

    l1sys_reg_write((coalesce_buffer_t*)(sys_handle), 0x0, 0x0, 0x0, 0x42);
    l1sys_reg_read((coalesce_buffer_t*)(sys_handle), 0x0, 0x0, 0x0);
    l1sys_burst_write((coalesce_buffer_t*)(sys_handle), 0x0, 0x0, 0x0, 0x01, &rv);
    l1sys_burst_read((coalesce_buffer_t*)(sys_handle), 0x0, 0x0, 0x0, 0x01, &rv);
    l1sys_poll_bit((coalesce_buffer_t*)(sys_handle), 0x0, 0x0, 0x0, 0x01, 0x42, 0x01, &rv, 0x42);
    l1sys_poll((coalesce_buffer_t*)(sys_handle), 0x0, 0x0, 0x0, 0x01, 0x42, PMC_POLL_EQUAL, 0x01, &rv, 0x42);



#ifdef PMC_SPECMAN
    pmc_sys_poll((pmc_sys_handle_t *) (sys_handle), 0x0, 0x0, 0x0, PMC_POLL_EQUAL, 0x0, &rv, 0x0);
    pmc_sys_poll_bit((pmc_sys_handle_t *)(sys_handle), 0x0, 0x0, 0x0, 0x01, &rv, 0x42);
#endif 

#ifdef PMC_COMEXPRESS
    /* The following functions are in pmc_emulation.h */
    emulation_sys_burst_read(sys_handle, 0x0, 0x1, &rv);
    emulation_sys_reg_write(sys_handle, 0x0, 0x42);
    emulation_sys_burst_write(sys_handle, 0x0, 0x1, &rv);
    emulation_sys_reg_read_modify_write(sys_handle, 0x0, 0x0, 0x42, &rv);
    emulation_sys_mem_write(sys_handle, 0x0, 0x1, (UINT8 *)&rv);
#endif

    pmc_sys_handle_destroy(&sys_handle);

    PMC_RETURN(PMC_SUCCESS);
}

static swig_command_info swig_commands[];
static swig_var_info swig_variables[];

int Digi_Unload(Tcl_Interp *interp, int flags)
{
    int i;
          
    PMC_LOG_TRACE("Started unloading Digi drivers\n");
    
    PMC_LOG_TRACE("Deleting Swig Commands\n");
    for (i = 0; swig_commands[i].name; i++) {
        Tcl_DeleteCommand(digi_interp, (char *) swig_commands[i].name);
    }
    
    PMC_LOG_TRACE("Deleting Swig Variables\n");
    for (i = 0; swig_variables[i].name; i++) {
        Tcl_UnsetVar(digi_interp, (char *) swig_variables[i].name, TCL_GLOBAL_ONLY);
    }
    
    Tcl_UnsetVar(interp, (char *)"swig_runtime_data_type_pointer" SWIG_RUNTIME_VERSION SWIG_TYPE_TABLE_NAME, TCL_GLOBAL_ONLY);
    swig_module.next = NULL;

    return TCL_OK;
}

int Digi_SafeUnload(Tcl_Interp *interp, int flags)
{
    return Digi_Unload(interp, flags);
}
%}


/* SWIG External Initialization Routine */
%init %{ 
    digi_interp = interp;
    PMC_LOG_TRACE("Loaded libdigi_swig.so: %s\n", swig_get_version());
%}
