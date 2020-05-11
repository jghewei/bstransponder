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
*     The head file describes basic functionality, including global configuration,
*     device level configuration, initialziation configuraiton.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_BASIC_CFG_H
#define _DIGI_ADPT_BASIC_CFG_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "digi_api.h"

/*
** Enumerated Types
*/


/*
** Constants
*/


/*
** Macro Definitions
*/

/* device identity */
#define DIGI_CARD_ID_GET(dev_id)    ((dev_id) & 0xFFFF)
#define DIGI_DIGI_DEV_GET(dev_id)   (((dev_id) >> 16) & 0xFF)
#define DIGI_SLOT_ID_GET(dev_id)    ((dev_id) >> 24)

/*
** Structures and Unions
*/
typedef struct
{
    /* DIGI device identity */
    UINT32 device_id;
    /* SDK version */
    UINT32 sdk_version;
    /* firmware version */
    UINT32 firm_version;
    /* DIGI device version */
    UINT32 device_version;
    /* boot loader version */
    UINT32 boot_version;
    /* firmware build */
    UINT32 firm_build;
} digi_version_t;

/*******************************************************************************
* ENUM: digi_init_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for DIGI initialization. The
*   element terms have a syntax:
*      DIGI_INIT_<application>
*
* ELEMENTS:
*   DIGI_INIT_COLD                  - cold restart DIGI
*   DIGI_INIT_WARM                  - warm restart DIGI
*   DIGI_INIT_CRASH                 - crash warm restart DIGI
*   DIGI_INIT_WARM_FW_RELOAD        - warm restart DIGI with FW reload
*   DIGI_INIT_CRASH_FW_RELOAD       - crash warm restart DIGI with FW reload
*
*******************************************************************************/
typedef enum
{
    DIGI_INIT_COLD             = 0,
    DIGI_INIT_WARM             = 1,
    DIGI_INIT_CRASH            = 2,
    DIGI_INIT_WARM_FW_RELOAD   = 3,
    DIGI_INIT_CRASH_FW_RELOAD  = 4,

    LAST_DIGI_INIT,      /* this is for out of range values error handling */
} digi_init_type_t;

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_log_init(pmc_log_sev_t log_sev);
PUBLIC PMC_ERROR adpt_init(UINT32 dev_id, UINT32 base_addr, digi_init_type_t init_type);
PUBLIC PMC_ERROR adpt_uninit(UINT32 dev_id, BOOL shutdown);
PUBLIC PMC_ERROR adpt_version_get(UINT32 dev_id, digi_version_t *ver_info);
PUBLIC PMC_ERROR adpt_device_status_get(UINT32 dev_id, 
                                    digi_sw_status_t *sw_status,
                                    digi_fw_status_t *fw_status);
PUBLIC PMC_ERROR adpt_warm_restart_handle(UINT32 dev_id);
PUBLIC UINT32 adpt_reg_read(UINT32 dev_id, UINT32 addr);
PUBLIC PMC_ERROR adpt_reg_write(UINT32 dev_id, UINT32 addr, UINT32 value);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_BASIC_CFG_H */

/* 
** end of file 
*/

