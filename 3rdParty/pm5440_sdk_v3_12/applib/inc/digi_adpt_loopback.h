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
*     The file describes the Loopback Run-Time functionality of DIGI device.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_LOOPBACK_H
#define _DIGI_ADPT_LOOPBACK_H
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
* ENUM: digi_loopback_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for traffic loopback.  The
*   element terms have a syntax:
*      DIGI_LPBK_<application>
*
* ELEMENTS:
*   DIGI_LPBK_DISABLE         - disable all loopbacks
*   DIGI_LPBK_OUTWARD         - from ingress(Rx) to egress(Tx) loopback
*   DIGI_LPBK_INWARD          - from egress(Tx) to ingress(Rx) loopback
*   DIGI_LPBK_INWARD_WITH_TX  - from egress(Tx) to ingress(Rx) loopback with
*                               egress(Tx)
*
*******************************************************************************/
typedef enum
{
    DIGI_LPBK_DISABLE = 0,
    DIGI_LPBK_OUTWARD,
    DIGI_LPBK_INWARD,
    DIGI_LPBK_INWARD_WITH_TX,

    LAST_DIGI_LPBK,      /* this is for out of range values error handling */
} digi_loopback_type_t;

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_loopback_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_loopback_type_t loop_type);
PUBLIC PMC_ERROR adpt_loopback_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_loopback_type_t *loop_type);
PUBLIC PMC_ERROR adpt_internal_loopback_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_loopback_t loop_type,
                                       BOOL enable);
PUBLIC PMC_ERROR adpt_internal_loopback_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_loopback_t loop_type,
                                       BOOL *enable);
PUBLIC PMC_ERROR adpt_ge_loopback_set(UINT32 dev_id,
                                   digi_ge_serdes_id_t intf_type,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_ge_loopback_get(UINT32 dev_id,
                                   digi_ge_serdes_id_t intf_type,
                                   BOOL *enable);
PUBLIC PMC_ERROR adpt_ilkn_loopback_set(UINT32 dev_id,
                                    digi_ilkn_intf_t ilkn_type,
                                    BOOL enable);
PUBLIC PMC_ERROR adpt_ilkn_loopback_get(UINT32 dev_id,
                                    digi_ilkn_intf_t ilkn_type,
                                    BOOL *enable);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_LOOPBACK_H */

/* 
** end of file 
*/

