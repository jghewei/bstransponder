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
*     This head file is common head file for DIGI Adapter.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_COM_H
#define _DIGI_ADPT_COM_H
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
/*******************************************************************************
* ENUM: digi_port_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for traffic direction. The
*   element terms have a syntax:
*      DIGI_PORT_DIR_<application>
*
* ELEMENTS:
*   DIGI_PORT_DIR_RX      - received direction
*   DIGI_PORT_DIR_TX      - transmitted direction
*
*******************************************************************************/
typedef enum
{
    DIGI_PORT_DIR_RX = CBRC_PORT_DIR_RX,
    DIGI_PORT_DIR_TX = CBRC_PORT_DIR_TX,
    
    LAST_DIGI_PORT_DIR, /* this is for out of range values error handling */
} digi_port_dir_t;

/*******************************************************************************
* ENUM: digi_enet_prbs_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for PRBS type. The
*   element terms have a syntax:
*      DIGI_ENET_PRBS_<application>
*
* ELEMENTS:
*   DIGI_ENET_PRBS_9     - PRBS 9 pattern
*   DIGI_ENET_PRBS_31    - PRBS 31 pattern
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_PRBS_9   = 0x0,
    DIGI_ENET_PRBS_31,
    
    LAST_DIGI_ENET_PRBS, /* this is for out of range values error handling */
} digi_enet_prbs_t;

/*******************************************************************************
* ENUM: ilkn_sifd_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type defining interlaken SIFD information.
*
* ELEMENTS:
*   ILKN_SIFD_1      - The first SIFD will be used.
*   ILKN_SIFD_2      - The second SIFD will be used.
*   ILKN_SIFD_1_2    - The first/second SIFD will be used.
*   ILKN_SIFD_NONE   - No SIFD will be used.
*
*******************************************************************************/
typedef enum
{
    ILKN_SIFD_1    = 0,
    ILKN_SIFD_2    = 1,
    ILKN_SIFD_1_2  = 2,
    ILKN_SIFD_NONE = 3,

    LAST_DIGI_ILKN_SIFD,      /* this is for out of range values error handling */
} ilkn_sifd_type_t;

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/


#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_COM_H */

/* 
** end of file 
*/

