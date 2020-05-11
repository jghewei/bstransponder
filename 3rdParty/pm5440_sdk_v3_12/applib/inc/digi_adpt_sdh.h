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
*   DESCRIPTION:
*     The file describes the SONET/SDH Run-Time functionality of CBRC subsystem.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_SDH_H
#define _DIGI_ADPT_SDH_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_adpt_com.h"

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_sdh_tti_len_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SONET/SDH TTI length. The
*   element terms have a syntax:
*      DIGI_SDH_TTI_LEN_<application>
*
* ELEMENTS:
*   DIGI_SDH_TTI_DISABLED         - disable TTI detection
*   DIGI_SDH_TTI_LEN_1_BYTE       - 1-byte length TTI mode
*   DIGI_SDH_TTI_LEN_16_BYTE      - 16-byte length TTI mode
*   DIGI_SDH_TTI_LEN_64_BYTE      - 64-byte length TTI mode
*
*******************************************************************************/
typedef enum
{
    DIGI_SDH_TTI_DISABLED    = 0,
    DIGI_SDH_TTI_LEN_1_BYTE  = SDH_PMG_RTTP_LENGTH_1_BYTE,
    DIGI_SDH_TTI_LEN_16_BYTE = SDH_PMG_RTTP_LENGTH_16_BYTES,
    DIGI_SDH_TTI_LEN_64_BYTE = SDH_PMG_RTTP_LENGTH_64_BYTES,

    LAST_DIGI_SDH_TTI_LEN, /* this is for out of range values error handling */
} digi_sdh_tti_len_t;

/*******************************************************************************
* ENUM: digi_sdh_thr_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SONET/SDH B1/B2 threshold. The
*   element terms have a syntax:
*      DIGI_SDH_<application>
*
* ELEMENTS:
*   DIGI_SDH_DISABLED  - threshold disable
*   DIGI_SDH_1E_03     - 1x10e-3 threshold
*   DIGI_SDH_1E_04     - 1x10e-4 threshold
*   DIGI_SDH_1E_05     - 1x10e-5 threshold
*   DIGI_SDH_1E_06     - 1x10e-6 threshold
*   DIGI_SDH_1E_07     - 1x10e-7 threshold
*   DIGI_SDH_1E_08     - 1x10e-8 threshold
*   DIGI_SDH_1E_09     - 1x10e-9 threshold
*   DIGI_SDH_1E_10     - 1x10e-10 threshold
*   DIGI_SDH_1E_11     - 1x10e-11 threshold
*   DIGI_SDH_1E_12     - 1x10e-12 threshold
*
*******************************************************************************/
typedef enum
{
    DIGI_SDH_DISABLED = SDH_PMG_SBER_BER_DISABLED,
    DIGI_SDH_1E_03    = SDH_PMG_SBER_BER_1E_03,
    DIGI_SDH_1E_04    = SDH_PMG_SBER_BER_1E_04,
    DIGI_SDH_1E_05    = SDH_PMG_SBER_BER_1E_05,
    DIGI_SDH_1E_06    = SDH_PMG_SBER_BER_1E_06,
    DIGI_SDH_1E_07    = SDH_PMG_SBER_BER_1E_07,
    DIGI_SDH_1E_08    = SDH_PMG_SBER_BER_1E_08,
    DIGI_SDH_1E_09    = SDH_PMG_SBER_BER_1E_09,
    DIGI_SDH_1E_10    = SDH_PMG_SBER_BER_1E_10,
    DIGI_SDH_1E_11    = SDH_PMG_SBER_BER_1E_11,
    DIGI_SDH_1E_12    = SDH_PMG_SBER_BER_1E_12,

    LAST_DIGI_SDH_THR, /* this is for out of range values error handling */
} digi_sdh_thr_t;

/*******************************************************************************
* ENUM: digi_bip_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SONET/SDH B1/B2 BIP accumulation mode. The
*   element terms have a syntax:
*      DIGI_SDH_<application>
*
* ELEMENTS:
*   DIGI_SDH_BIP_ERR    - BIP accumulation represents BIP-8 errors (a maximum 
*                         of 8 errors per frame or per STS 1/STM-0 per frame).
*   DIGI_SDH_BLOCK_ERR  - BIP accumulation represents BIP-8 block errors (a 
*                         maximum of 1 error per frame) or BIP-24 block errors 
*                         (a maximum of 1 error per STS 3/STM-1 per frame).
*
*******************************************************************************/
typedef enum
{
    DIGI_SDH_BIP_ERR = 0x0,
    DIGI_SDH_BLOCK_ERR,

    LAST_DIGI_SDH_BIP, /* this is for out of range values error handling */
} digi_bip_mode_t;

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_sdh_tti_mode_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    digi_sdh_tti_len_t tti_mode);
PUBLIC PMC_ERROR adpt_sdh_tti_mode_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    digi_sdh_tti_len_t *tti_mode);
PUBLIC PMC_ERROR adpt_sdh_rx_tti_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_port_dir_t dir,
                                 UINT8 *tti_data);
PUBLIC PMC_ERROR adpt_sdh_exp_tti_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT8 tti_data[64]);
PUBLIC PMC_ERROR adpt_sdh_exp_tti_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT8 *tti_data);
PUBLIC PMC_ERROR adpt_sdh_bn_type_set(UINT32 dev_id,
                                   digi_sdh_pm_t bn_type);
PUBLIC PMC_ERROR adpt_sdh_bn_type_get(UINT32 dev_id,
                                   digi_sdh_pm_t *bn_type);
PUBLIC PMC_ERROR adpt_sdh_bn_thr_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_thr_t exc_thr,
                                  digi_sdh_thr_t deg_thr);
PUBLIC PMC_ERROR adpt_sdh_bn_thr_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_thr_t *exc_thr,
                                  digi_sdh_thr_t *deg_thr);
PUBLIC PMC_ERROR adpt_sdh_bip_mode_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     digi_bip_mode_t rs_bip_mode,
                                     digi_bip_mode_t ms_bip_mode);
PUBLIC PMC_ERROR adpt_sdh_bip_mode_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     digi_bip_mode_t *rs_bip_mode,
                                     digi_bip_mode_t *ms_bip_mode);
PUBLIC PMC_ERROR adpt_sdh_rx_oh_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  sdh_pmg_rrmp_byte_type_t type,
                                  UINT8 *oh_byte);
PUBLIC PMC_ERROR adpt_sdh_prbs_en_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL prbs_invert,
                                    BOOL prbs_enable);
PUBLIC PMC_ERROR adpt_sdh_prbs_en_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *prbs_invert,
                                    BOOL *prbs_enable);
PUBLIC PMC_ERROR adpt_sdh_prbs_err_inst(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_sdh_prbs_status_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      BOOL *prbs_syn,
                                      UINT32 *err_cnt);
PUBLIC PMC_ERROR adpt_sdh_gais_inst_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    sdh_pmg_pn11_insert_t alm_type,
                                    BOOL enable);
PUBLIC PMC_ERROR adpt_sdh_gais_inst_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    sdh_pmg_pn11_insert_t alm_type,
                                    BOOL *enable);
PUBLIC PMC_ERROR adpt_sdh_oof_force(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_port_dir_t dir);
PUBLIC PMC_ERROR adpt_sdh_m0_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  BOOL m0_enable);
PUBLIC PMC_ERROR adpt_sdh_m0_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  BOOL *m0_enable);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_SDH_H */

/* 
** end of file 
*/

