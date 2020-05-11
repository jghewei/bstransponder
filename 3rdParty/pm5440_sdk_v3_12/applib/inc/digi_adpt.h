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
*     This head file is general head file customer should add when using DIGI
*     Adapter code.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_H
#define _DIGI_ADPT_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include "digi_api.h"
#include "digi_adpt_basic_cfg.h"
#include "digi_adpt_customer.h"
#include "digi_adpt_datapath.h"
#include "digi_adpt_otn.h"
#include "digi_adpt_enet.h"
#include "digi_adpt_gfp.h"
#include "digi_adpt_debug.h"
#include "digi_adpt_alm.h"
#include "digi_adpt_interrupt.h"
#include "digi_adpt_pmon.h"
#include "digi_adpt_sdh.h"
#include "digi_adpt_serdes.h"
#include "digi_adpt_api.h"
#include "digi_adpt_cbr.h"
#include "digi_adpt_loopback.h"
#include "digi_adpt_interlaken.h"
#include "digi_adpt_port.h"
#include "digi_adpt_misc.h"
#include "digi_adpt_ptp.h"
#include "digi_adpt_ghao.h"

/*
** Enumerated Types
*/


/*
** Constants
*/
extern pmc_log_sev_t adpt_log_sev;


/*
** Macro Definitions
*/
#define ADPT_TRACE(...)                                                                    \
{                                                                                          \
    if (adpt_log_sev >= PMC_LOG_SEV_HIGH)                                                  \
    {                                                                                      \
        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGH, PMC_SHARED_LOG_TRACE, 0, 0, "%s:", __FUNCTION__); \
        PMC_LOG_NOINFO(PMC_LOG_SEV_HIGH, PMC_SHARED_LOG_TRACE, 0, 0, __VA_ARGS__);         \
    }                                                                                      \
}

#define ADPT_ERROR(...)                                                                     \
{                                                                                           \
    if (adpt_log_sev >= PMC_LOG_SEV_FATAL)                                                  \
    {                                                                                       \
        PMC_LOG_NOINFO(PMC_LOG_SEV_FATAL, PMC_SHARED_LOG_TRACE, 0, 0, "%s:", __FUNCTION__); \
        PMC_LOG_NOINFO(PMC_LOG_SEV_FATAL, PMC_SHARED_LOG_TRACE, 0, 0, __VA_ARGS__);         \
    }                                                                                       \
}

#define ADPT_PRINT(...) printf(__VA_ARGS__);

#define ADPT_PARA_VERIFY(para_ptr)          \
if (NULL == para_ptr)                       \
{                                           \
    PMC_LOG(PMC_LOG_SEV_FATAL, PMC_SHARED_LOG_TRACE, 0, 0, "Input pointer is NULL.\r\n"); \
    PMC_RETURN(PMC_ERR_INVALID_PARAMETERS); \
}

#define ADPT_ATOMIC_PARA_VERIFY(digi, para_ptr)          \
if (NULL == para_ptr)                                    \
{                                                        \
    PMC_ATOMIC_RETURN(digi, PMC_ERR_INVALID_PARAMETERS); \
}

#define ADPT_RETURN_VERIFY(rc)   \
if (PMC_SUCCESS != (rc))         \
{                                \
    PMC_LOG(PMC_LOG_SEV_FATAL, PMC_SHARED_LOG_TRACE, 0, 0, "rc[0x%x].\r\n", rc); \
    PMC_RETURN(rc);              \
}

#define ADPT_ATOMIC_RETURN_VERIFY(digi, rc)   \
if (PMC_SUCCESS != (rc))                      \
{                                             \
    PMC_LOG(PMC_LOG_SEV_FATAL, PMC_SHARED_LOG_TRACE, 0, 0, "rc[0x%x].\r\n", rc); \
    PMC_ATOMIC_RETURN(digi, rc);              \
}

/*
** Structures and Unions
*/


/*
** Function Prototypes
*/



#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_H */

/* 
** end of file 
*/

