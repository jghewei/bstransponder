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

#ifndef _CPUIF_API_H
#define _CPUIF_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: cpuif_pcie_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Host interrupt - MSI-X, MSI, legacy, device pin or disabled.
*    
*
* ELEMENTS:
*    CPUIF_INT_DISABLE    - Default - interrupts disabled
*    CPUIF_INT_MSI_X      - PCIe MSI-X interrupts
*    CPUIF_INT_MSI        - PCIe MSI interrupts
*    CPUIF_INT_LEGACY     - PCIe legacy intx
*    CPUIF_INT_DEVICE_PIN - Device pin IRQ
*
*******************************************************************************/
typedef enum
{
    CPUIF_INT_DISABLE = 0,
    CPUIF_INT_MSI_X,
    CPUIF_INT_MSI,
    CPUIF_INT_LEGACY,
    CPUIF_INT_DEVICE_PIN
} cpuif_int_t;


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
** Global variables
*/

/*
** Function Prototypes
*/
                             
#ifdef __cplusplus
}
#endif

#endif /* _CPU_IF_H */

/*
** end of file
*/
