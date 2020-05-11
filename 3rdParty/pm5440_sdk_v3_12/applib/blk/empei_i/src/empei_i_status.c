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
/*
** Include Files
*/

#include "empei_i_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Public Functions
*/
/*******************************************************************************
*  empei_i_intr_enable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set specified interrupt.
*
* INPUTS:
*   hndl        - handle to the EMPEI_I instance to be operated on
*   link        - link to configure
*   enable      - TRUE: Enable interrupt
*                 FALSE: Disable interrupt
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 empei_i_intr_enable_set(empei_i_handle_t *hndl, UINT32 link, BOOL8 enable)
{
    UINT32 mask;
    UINT32 value;

    PMC_ENTRY();

    /* 
     * Configure interrupt enables
     */
/*Pini*/
/*Serial number #19*/
/*Module: EMPEI*/
/*Register : 0x20038 EMPEI Insert and Extract Queue Overflow Enable*/
/*Field: R/W EXT_Q_OVERFLOW_INT_E[11:0] (bits 23:12)*/
/*Field: R/W INS_Q_OVERFLOW_INT_E[11:0] (bits 11:0)*/
/*Description/Action: each bit represent link overflow Interrupt en, set all bits to 0x1*/
    mask =  (0x00000001 << (link + EMPEI120_I_REG_INS_EXT_OF_INT_EN_BIT_INS_Q_OVERFLOW_INT_E_OFF) |
             0x00000001 << (link + EMPEI120_I_REG_INS_EXT_OF_INT_EN_BIT_EXT_Q_OVERFLOW_INT_E_OFF));
    value = (enable) ? mask : 0;
    empei_i_reg_INS_EXT_OF_INT_EN_field_set( NULL, hndl, mask, 0, value );

/*Serial number #20*/
/*Module: EMPEI*/
/*Register : 0x2003C EMPEI Sys data and Internal Queue Overflow Enable*/
/*Field: R/W INTL_Q_OVERFLOW_INT_E[11:0] (bits 23:12)*/
/*Field: R/W SYS_Q_OVERFLOW_INT_E[11:0] (bits 11:0)*/
/*Description/Action: each bit represent link overflow Interrupt en, set all bits to 0x1*/
    mask =  (0x00000001 << (link + EMPEI120_I_REG_SYS_INTL_OF_INT_EN_BIT_SYS_Q_OVERFLOW_INT_E_OFF) |
             0x00000001 << (link + EMPEI120_I_REG_SYS_INTL_OF_INT_EN_BIT_INTL_Q_OVERFLOW_INT_E_OFF));
    value = (enable) ? mask : 0;
    empei_i_reg_SYS_INTL_OF_INT_EN_field_set( NULL, hndl, mask, 0, value );


/*Serial number #21*/
/*Module: EMPEI*/
/*Register : 0x20040 EMPEI Sys data and Internal Queue underrun Enable*/
/*Field: R/W INS_Q_UNDERRUN_INT_E[11:0] (bits 23:12)*/
/*Field: R/W SYS_Q_UNDERRUN_INT_E[11:0] (bits 11:0)*/
/*Description/Action: each bit represent link under run Interrupt en, set all bits to 0x1*/
    mask =  (0x00000001 << (link + EMPEI120_I_REG_SYS_INS_UR_INT_EN_BIT_SYS_Q_UNDERRUN_INT_E_OFF) |
             0x00000001 << (link + EMPEI120_I_REG_SYS_INS_UR_INT_EN_BIT_INS_Q_UNDERRUN_INT_E_OFF));
    value = (enable) ? mask : 0;
    empei_i_reg_SYS_INS_UR_INT_EN_field_set( NULL, hndl, mask, 0, value );


/*Serial number #22*/
/*Module: EMPEI*/
/*Register : 0x20044 EMPEI Sys data and Insert Queue Resynchronizations Enable*/
/*Field: R/W INS_Q_RESYNC_INT_E[11:0] (bits 23:12)*/
/*Field: R/W SYS_Q_RESYNC_INT_E[11:0] (bits 11:0)*/
/*Description/Action: each bit represent link resynch Interrupt en, set all bits to 0x1*/
    mask =  (0x00000001 << (link + EMPEI120_I_REG_SYS_INS_Q_RSYNC_INT_EN_BIT_SYS_Q_RESYNC_INT_E_OFF) |
             0x00000001 << (link + EMPEI120_I_REG_SYS_INS_Q_RSYNC_INT_EN_BIT_INS_Q_RESYNC_INT_E_OFF));
    value = (enable) ? mask : 0;
    empei_i_reg_SYS_INS_Q_RSYNC_INT_EN_field_set( NULL, hndl, mask, 0, value );


/*Serial number #22 - 'b'*/
/*Module: EMPEI*/
/*Register : 0x20048 EMPEI Extract Queue Resynchronizations Enable*/
/*Field: R/W EXT_Q_RESYNC_INT_E[11:0] (bits 11:0)*/
/*Description/Action: each bit represent link resynch Interrupt en, set all bits to 0x1*/
    mask = 0x00000001 << (link + EMPEI120_I_REG_EXT_Q_RSYNC_INT_EN_BIT_EXT_Q_RESYNC_INT_E_OFF);
    value = (enable) ? mask : 0;
    empei_i_reg_EXT_Q_RSYNC_INT_EN_field_set( NULL, hndl, mask, 0, value );


    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_intr_enable_set */

/*******************************************************************************
*  empei_i_intr_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read specified status.
*
* INPUTS:
*   hndl        - handle to the EMPEI_I instance to be operated on
*   status_type - status type to read
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Status value. Bits 11:0 correspond to links 11:0.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 empei_i_intr_status_get(empei_i_handle_t *hndl, empei_i_status_t status_type)
{
    PMC_ENTRY();

    /* Read interrupt status */

    PMC_RETURN(PMC_SUCCESS);

} /* empei_i_intr_status_get */


/*
** Private Functions
*/
   
/*
** End of file
*/
