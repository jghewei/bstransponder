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
#ifndef _LIFD_LOC_H
#define _LIFD_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/


#include "lifd.h"
#include "lifd_mux_io.h"
#include "line_dsis_io.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define LIFD_MINIMUM_INTERNAL_LANE_NUM      0


#define LIFD_MINIMUM_SERDES_LANE_NUM        0
#define LIFD_MAXIMUM_SERDES_LANE_NUM        19

#define LIFD_SFI5POINT1_TO_OTN              0
#define LIFD_SFI5POINT1_TO_CBRC             1

#define LIFD_SFI5POINT1_DISABLED            0
#define LIFD_SFI5POINT1_ENABLED             1

#define LIFD_CLK_DISABLE                    0
#define LIFD_CBRC_CLK_ENABLE                1
#define LIFD_OTN_CLK_ENABLE                 2
#define LIFD_ENET_CLK_ENABLE                4

#define LIFD_CBRC_DATA_ENABLE               0
#define LIFD_OTN_DATA_ENABLE                1
#define LIFD_ENET_DATA_ENABLE               2


#define LIFD_DSIS_LOS_ENABLE             0x01
#define LIFD_DSIS_LOS_DISABLE            0x00

#define LIFD_DSIS_DLOS_ENABLE            0x01
#define LIFD_DSIS_DLOS_DISABLE           0x00

#define LIFD_SERDES_UNASSIGNED              0xFFFFFFFF
#define LIFD_SERDES_PORT_NO_LOS_SOURCE      0xFD  /* LOS DISABLED */

#define LIFD_ACB_ON_SERDES_MULT             4     /* internal lane index multiplier for ACB_O_SEL field */
#define LIFD_ACB_O_SEL_MAX_VALUE            2

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: lifd_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining static LIFD module data.
*
* ELEMENTS:
*
*******************************************************************************/
typedef struct {

} lifd_cfg_t;

/*******************************************************************************
* STRUCTURE: lifd_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining context that dynamically change during configuration
*   of the device.
*
* ELEMENTS:
*   internal_lane_tx   - Tx DSIS internal resource management
*   internal_lane_rx   - Rx DSIS internal resource management
*   los_ctxt           - See lifd_los_ctxt_t
*   serdes_los_lane    - Serdes LOS lane for each internal los lane
*   serdes_lane        - Serdes Lane for each Internal Lane
*
*
*******************************************************************************/
typedef struct {
    UINT32 internal_lane_tx[LIFD_SERDES_DSIS_LANES_MAX];
    UINT32 internal_lane_rx[LIFD_SERDES_DSIS_LANES_MAX];
    lifd_los_ctxt_t los_ctxt[LIFD_SERDES_DSIS_LANES_MAX];
    UINT32 serdes_los_lane[LIFD_MAXIMUM_INTERNAL_LANE_NUM+1];
    UINT32 serdes_lane[LIFD_MAXIMUM_INTERNAL_LANE_NUM+1];
      
} lifd_var_t;

/*******************************************************************************
* STRUCTURE: lifd_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining LIFD module instance handle.
*
* ELEMENTS:
*       base        - see pmc_handle_t
*       cfg         - static data associated with LIFD module instance
*       var         - dynamic data associated with LIFD module instance
*
*******************************************************************************/
struct lifd_handle_struct_t {
    pmc_handle_t base;
    lifd_cfg_t cfg;
    lifd_var_t var;
};

/*
** Global variables
*/

#ifdef __cplusplus
}
#endif

#endif /* _LIFD_LOC_H */

/* 
** end of file 
*/

