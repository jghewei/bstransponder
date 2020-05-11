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
*     The file describes the interrupt functionality.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_INTERRUPT_H
#define _DIGI_ADPT_INTERRUPT_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"

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
* ENUM: digi_otn_int_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN interrupt mask.  The
*   element terms have a syntax:
*      DIGI_OTN_INT_MASK_<application>
*
* ELEMENTS:
*   DIGI_OTN_INT_MASK_DLCK          - dLCK interrupt mask
*   DIGI_OTN_INT_MASK_DOCI          - dOCI interrupt mask
*   DIGI_OTN_INT_MASK_DAIS          - dAIS interrupt mask
*   DIGI_OTN_INT_MASK_DTIM          - dTIM interrupt mask
*   DIGI_OTN_INT_MASK_DDEG          - dDEG interrupt mask
*   DIGI_OTN_INT_MASK_DBDI          - dBDI interrupt mask
*   DIGI_OTN_INT_MASK_DLTC          - dLTC interrupt mask
*   DIGI_OTN_INT_MASK_DIAE          - dIAE interrupt mask
*   DIGI_OTN_INT_MASK_DBIAE         - dBIAE interrupt mask
*   DIGI_OTN_INT_MASK_DLOFLOM       - dLOFLOM interrupt mask
*   DIGI_OTN_INT_MASK_OOF           - OOF interrupt mask
*   DIGI_OTN_INT_MASK_ATSF          - ATSF interrupt mask
*   DIGI_OTN_INT_MASK_ATSD          - ATSD interrupt mask
*   DIGI_OTN_INT_MASK_AAIS          - AAIS interrupt mask
*   DIGI_OTN_INT_MASK_ABDI          - ABDI interrupt mask
*   DIGI_OTN_INT_MASK_ALL           - all interrupt masks
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_INT_MASK_DLCK    = 0x0001,
    DIGI_OTN_INT_MASK_DOCI    = 0x0002,
    DIGI_OTN_INT_MASK_DAIS    = 0x0004,
    DIGI_OTN_INT_MASK_DTIM    = 0x0008,
    DIGI_OTN_INT_MASK_DDEG    = 0x0010,
    DIGI_OTN_INT_MASK_DBDI    = 0x0020,
    DIGI_OTN_INT_MASK_DLTC    = 0x0040,
    DIGI_OTN_INT_MASK_DIAE    = 0x0080,
    DIGI_OTN_INT_MASK_DBIAE   = 0x0100,
    DIGI_OTN_INT_MASK_DLOFLOM = 0x0200,
    DIGI_OTN_INT_MASK_OOF     = 0x0400,
    DIGI_OTN_INT_MASK_ATSF    = 0x1000,
    DIGI_OTN_INT_MASK_ATSD    = 0x2000,
    DIGI_OTN_INT_MASK_AAIS    = 0x4000,
    DIGI_OTN_INT_MASK_ABDI    = 0x8000,
    DIGI_OTN_INT_MASK_ALL     = 0xF7FF,

    LAST_OTN_INT_MASK,      /* this is for out of range values error handling */
} digi_otn_int_mask_t;

/*
** Structures and Unions
*/
typedef struct
{
    BOOL phy_los;
    BOOL rx_loc_fault;
    BOOL rx_rem_fault;
    BOOL hi_ber;
    BOOL block_lock;
    BOOL align_status;
    BOOL loam;
}digi_enet_int_en_t;

typedef struct
{
    BOOL dlof;
    BOOL dlom;
    BOOL dlck;
    BOOL doci;
    BOOL dais;
    BOOL dtim;
    BOOL ddeg;
    BOOL dedeg;
    BOOL dbdi;
    BOOL oof;
    BOOL dloflom;
    BOOL dbiae;
    BOOL dltc;
    BOOL diae;
    BOOL dplm;
    BOOL dloomfi;
    BOOL dmsim;
    BOOL dcsf;
    BOOL oom_im;
    BOOL atsf;
    BOOL atsd;
    BOOL abdi;
    BOOL aais;
    BOOL ac_aps;
    BOOL dlol;
    BOOL dotuais;
}digi_otn_int_en_t;

typedef struct
{
    BOOL dlos;
    BOOL doof;
    BOOL dotuais;
    BOOL dloflane;
    BOOL door;
    BOOL dlor;
    BOOL doom;
}digi_mld_int_en_t;

typedef struct
{
    BOOL dlfd;
    BOOL dexm;
    BOOL dupm;
    BOOL dcsf;
    BOOL didle;
}digi_gfp_int_en_t;

typedef struct
{
    BOOL dlos;
    BOOL dgais;
    BOOL dlof;
    BOOL doof;
    BOOL dtim;
    BOOL dtiu;
    BOOL dms_ais;
    BOOL dms_rdi;
}digi_sdh_int_en_t;

typedef struct
{
    BOOL drf; /* remote fault */
    BOOL dlf; /* local fault  */
    BOOL dhigh_ber;
    BOOL dloss_sync;
    BOOL dlos;
}digi_fc1200_int_en_t;

typedef struct
{
    BOOL ddisp_inv; 
    BOOL dxdet;
    BOOL dlolb;
    BOOL dasd;
    BOOL dsync;
    BOOL dlcv;
}digi_fc800_int_en_t;

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_dev_int_en_set(UINT32 dev_id,
                                  cpuif_int_t int_type,
                                  UINT32 int_msk,
                                  BOOL enable);
PUBLIC PMC_ERROR adpt_dev_int_en_get(UINT32 dev_id,
                                  cpuif_int_t int_type,
                                  UINT32 *int_en_msk);
PUBLIC PMC_ERROR adpt_enet_int_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_enet_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_enet_int_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_enet_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_otn_int_en_set(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_otn_int_en_get(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_otn_seg_int_en_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     BOOL enable);
PUBLIC PMC_ERROR adpt_otn_seg_int_en_get(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location,
                                     BOOL *enable);
PUBLIC PMC_ERROR adpt_otn_defect_int_en_set(UINT32 dev_id, 
                                       odu_struct_odu_level_t odu_level,
                                       UINT32 int_mask);
PUBLIC PMC_ERROR adpt_otn_defect_int_en_get(UINT32 dev_id, 
                                       odu_struct_odu_level_t odu_level,
                                       UINT32 *int_mask);
PUBLIC PMC_ERROR adpt_mld_int_en_set(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  UINT32 lane_id,
                                  digi_mld_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_mld_int_en_get(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  UINT32 lane_id,
                                  digi_mld_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_gfp_int_en_set(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_gfp_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_gfp_int_en_get(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_gfp_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_sdh_int_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_sdh_int_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_fc1200_int_en_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_fc1200_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_fc1200_int_en_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_fc1200_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_fc800_int_en_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   digi_fc800_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_fc800_int_en_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   digi_fc800_int_en_t *int_table);
PUBLIC PMC_ERROR adpt_int_check(UINT32 dev_id, digi_blk_type_t blk_msk);
PUBLIC PMC_ERROR adpt_int_clear(UINT32 dev_id, digi_blk_type_t blk_msk);
PUBLIC PMC_ERROR adpt_int_cb_handle(digi_handle_t *digi_handle,
                                 void *token,
                                 void *channel_handle,
                                 digi_int_type_t int_type);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_INTERRUPT_H */

/* 
** end of file 
*/

