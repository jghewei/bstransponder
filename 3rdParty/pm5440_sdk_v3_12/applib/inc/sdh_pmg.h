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

#ifndef _SDH_PMG_H
#define _SDH_PMG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "sdh_pmg_api.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#define SDH_PMG_RESET                      0x1
#define SDH_PMG_ACTIVE                     0x0

#define SDH_PMG_ENABLE                     0x1
#define SDH_PMG_DISABLE                    0x0



/*
** Macro Definitions
*/
/* SDH_PMG_INT(handle, tsb_name, tsb_field, struct_field) */
#define SDH_PMG_INT_TABLE_DEFINE() \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, COK2 , cok2_i ); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, LREIE, lreie_i); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, LBIPE, lbipe_i); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, SBIPE, sbipe_i); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, COSSM, cossm_i); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, COAPS, coaps_i); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, APSBF, apsbf_i); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, LRDI , lrdi_i ); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, LAIS , lais_i ); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, LOS  , los_i  ); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, LOF  , lof_i  ); \
    SDH_PMG_INT(sdh_pmg_handle, rrmp, OOF  , oof_i  ); \
    SDH_PMG_INT_IND(sdh_pmg_handle, rttp, NULL, tim_i  ); \
    SDH_PMG_INT_IND(sdh_pmg_handle, rttp, NULL, tiu_i  ); \
    SDH_PMG_INT_IND(sdh_pmg_handle, sber, NULL, sf_i   ); \
    SDH_PMG_INT_IND(sdh_pmg_handle, sber, NULL, sd_i   ); \


#define SDH_PMG_STATUS_TABLE_DEFINE() \
    SDH_PMG_STATUS(sdh_pmg_handle, rrmp, APSBF_V, apsbf_v); \
    SDH_PMG_STATUS(sdh_pmg_handle, rrmp, LRDI_V , lrdi_v ); \
    SDH_PMG_STATUS(sdh_pmg_handle, rrmp, LAIS_V , lais_v ); \
    SDH_PMG_STATUS(sdh_pmg_handle, rrmp, LOS_V  , los_v  ); \
    SDH_PMG_STATUS(sdh_pmg_handle, rrmp, LOF_V  , lof_v  ); \
    SDH_PMG_STATUS(sdh_pmg_handle, rrmp, OOF_V  , oof_v  ); \
    SDH_PMG_STATUS_IND(sdh_pmg_handle, rttp, NULL, tim_v); \
    SDH_PMG_STATUS_IND(sdh_pmg_handle, rttp, NULL, tiu_v); \
    SDH_PMG_STATUS_IND(sdh_pmg_handle, sber, NULL, sf_v ); \
    SDH_PMG_STATUS_IND(sdh_pmg_handle, sber, NULL, sd_v ); \


/*
** Structures and Unions
*/
/*******************************************************************************
* ENUM: sdh_pmg_port_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to describe the direction of traffic: receive and transmit.
*
* ELEMENTS:
*    SDH_PMG_PORT_DIR_TX - Egress direction to be activated
*    SDH_PMG_PORT_DIR_RX - Ingress direction to be activated
*
*******************************************************************************/
typedef enum {
    SDH_PMG_PORT_DIR_TX = 0,
    SDH_PMG_PORT_DIR_RX,

    LAST_SDH_PMG_PORT_DIR,
} sdh_pmg_port_dir_t;

/*******************************************************************************
* ENUM: sdh_pmg_sts_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to describe the client traffic modes that can be provisioned
*   through the CBRC. This is used with cbrc_port_provision().
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    SDH_PMG_STS_MODE_40G_STS768,
    SDH_PMG_STS_MODE_40G_STM256,
   
    SDH_PMG_STS_MODE_10G_STS192,
    SDH_PMG_STS_MODE_10G_STM64,
    SDH_PMG_STS_MODE_10G_STS192_PRBS,
    SDH_PMG_STS_MODE_10G_STM64_PRBS,

    LAST_SDH_PMG_STS_MODE,
} sdh_pmg_sts_mode_t;

/*******************************************************************************
* ENUM: sdh_pmg_rrmp_cfg_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for RRMP configuration types.
*
* ELEMENTS:
*   SDH_PMG_RRMP_CFG_TYPE_ALARM     - Configures RRMP alarms
*   SDH_PMG_RRMP_CFG_TYPE_SCRAMBLE  - Configures RRMP overhead descrambling
*   SDH_PMG_RRMP_CFG_TYPE_FRAMING   - Configures RRMP framing
*
*******************************************************************************/
typedef enum {
    SDH_PMG_RRMP_CFG_TYPE_ALARM = 0,
    SDH_PMG_RRMP_CFG_TYPE_SCRAMBLE,
    SDH_PMG_RRMP_CFG_TYPE_FRAMING,

    LAST_SDH_PMG_RRMP_CFG_TYPE,
} sdh_pmg_rrmp_cfg_type_t;


/*******************************************************************************
* ENUM: sdh_pmg_trmp_cfg_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for TRMP configuration types.
*
* ELEMENTS:
*   SDH_PMG_RRMP_CFG_TYPE_ALARM     - Configures TRMP alarms
*   SDH_PMG_RRMP_CFG_TYPE_SCRAMBLE  - Configures TRMP overhead descrambling
*   SDH_PMG_RRMP_CFG_TYPE_FRAMING   - Configures TRMP framing
*
*******************************************************************************/
typedef enum {
    SDH_PMG_TRMP_CFG_TYPE_ALARM = 0,
    SDH_PMG_TRMP_CFG_TYPE_SCRAMBLE,
    SDH_PMG_TRMP_CFG_TYPE_FRAMING,

    LAST_SDH_PMG_TRMP_CFG_TYPE,
} sdh_pmg_trmp_cfg_type_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC sdh_pmg_handle_t *sdh_pmg_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void sdh_pmg_ctxt_destroy(sdh_pmg_handle_t *sdh_pmg_handle_ptr);
PUBLIC void sdh_pmg_handle_init(sdh_pmg_handle_t *sdh_pmg_handle_ptr);
PUBLIC PMC_ERROR sdh_pmg_init(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                              pmc_energy_state_t   energy_state);
PUBLIC BOOL sdh_pmg_start_state_test(sdh_pmg_handle_t *sdh_pmg_handle);
PUBLIC PMC_ERROR sdh_pmg_prov(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                              UINT32               index,
                              sdh_pmg_sts_mode_t   mode);
PUBLIC PMC_ERROR sdh_pmg_unprov(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                UINT32               index);
PUBLIC PMC_ERROR sdh_pmg_activate(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                  UINT32               index);
PUBLIC PMC_ERROR sdh_pmg_deactivate(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                    UINT32               index);

PUBLIC void sdh_pmg_cleanup(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                            UINT32                slice_start_index,
                            UINT32                num_link_to_clean,
                            UINT32               *links);

PUBLIC PMC_ERROR sdh_pmg_sber_byte_cfg(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                       sdh_pmg_sber_byte_t       byte);

PUBLIC PMC_ERROR sdh_pmg_sber_byte_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                       sdh_pmg_sber_byte_t      *byte_ptr);

PUBLIC PMC_ERROR sdh_pmg_sber_ber_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                      UINT32                    index,
                                      sdh_pmg_sber_alarm_t      alarm,
                                      sdh_pmg_sber_ber_t       *ber_ptr);

PUBLIC PMC_ERROR sdh_pmg_sber_enable(sdh_pmg_handle_t             *sdh_pmg_handle_ptr,
                                     UINT32                        index,
                                     sdh_pmg_sber_alarm_t          alarm,
                                     sdh_pmg_sber_ber_t            ber);

PUBLIC PMC_ERROR sdh_pmg_rttp_cfg(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                  UINT32                    index,
                                  sdh_pmg_rttp_cfg_t       *rttp_cfg_ptr);

PUBLIC PMC_ERROR sdh_pmg_rttp_cfg_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                      UINT32                    index,
                                      sdh_pmg_rttp_cfg_t       *rttp_cfg_ptr);

PUBLIC PMC_ERROR sdh_pmg_rttp_expect_trace_set(sdh_pmg_handle_t      *cbrc_handle_ptr,
                                               UINT32                 index,
                                               sdh_pmg_rttp_trace_t  *expect_trace_ptr);

PUBLIC PMC_ERROR sdh_pmg_rttp_trace_get(sdh_pmg_handle_t           *sdh_pmg_handle_ptr,
                                        UINT32                      index,
                                        sdh_pmg_rttp_trace_type_t   trace_type,
                                        sdh_pmg_rttp_trace_t       *trace_ptr);

PUBLIC PMC_ERROR sdh_pmg_pn11_ins(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                  UINT32                    index,
                                  sdh_pmg_pn11_insert_t     lof_los,
                                  BOOL                      ins_enable);

PUBLIC PMC_ERROR sdh_pmg_pn11_ins_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                      UINT32                    index,
                                      sdh_pmg_pn11_insert_t     lof_los,
                                      BOOL                     *ins_enable_ptr);

PUBLIC PMC_ERROR sdh_pmg_oof_force(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                   UINT32               index);
PUBLIC PMC_ERROR sdh_pmg_rrmp_cfg(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                  UINT32                    index,
                                  sdh_pmg_rrmp_cfg_type_t   type,
                                  void                     *rrmp_cfg_ptr);
PUBLIC PMC_ERROR sdh_pmg_rrmp_cfg_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                      UINT32                    index,
                                      sdh_pmg_rrmp_cfg_type_t   type,
                                      void                     *rrmp_cfg_ptr);
PUBLIC PMC_ERROR sdh_pmg_rrmp_byte_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                       UINT32                    index,
                                       sdh_pmg_rrmp_byte_type_t  type,
                                       UINT8                    *byte_ptr);
PUBLIC PMC_ERROR sdh_pmg_int_chnl_enable(sdh_pmg_handle_t    *sdh_pmg_handle,
                                         UINT32               index,
                                         sdh_pmg_int_chnl_t  *int_table_ptr,
                                         sdh_pmg_int_chnl_t  *int_en_table_ptr,
                                         BOOL                 enable);
PUBLIC PMC_ERROR sdh_pmg_int_chnl_clear(sdh_pmg_handle_t    *sdh_pmg_handle,
                                        UINT32               index,
                                        sdh_pmg_int_chnl_t  *int_table_ptr,
                                        sdh_pmg_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR sdh_pmg_int_chnl_retrieve(sdh_pmg_handle_t    *sdh_pmg_handle,
                                           UINT32               index,
                                           sdh_pmg_int_chnl_t  *filt_table_ptr,
                                           sdh_pmg_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR sdh_pmg_int_chnl_enabled_check(sdh_pmg_handle_t     *sdh_pmg_handle,
                                                UINT32                index,
                                                sdh_pmg_int_chnl_t   *int_en_table_ptr,
                                                BOOL                 *int_found_ptr);

PUBLIC BOOL8 sdh_pmg_prov_status_get(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                     UINT32               index);

PUBLIC void sdh_pmg_cfg_mode_get(sdh_pmg_handle_t      *sdh_pmg_handle_ptr,
                                 INT32                  index,
                                 sdh_pmg_sts_mode_t    *mode_ptr);
    
PUBLIC UINT32 sdh_pmg_sber_index_get(sdh_pmg_handle_t    *sdh_pmg_handle,
                                     UINT32               index);



#ifdef __cplusplus
}
#endif

#endif /* _SDH_PMG_H */

/*
** end of file
*/
