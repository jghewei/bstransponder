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

#include "etime_loc.h"

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
*  etime_frc_ptp_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read local FRC for PTP source.
*
* INPUTS:
*   hndl          - handle to the ETIME instance to be operated on
*
* OUTPUTS:
*   *sec_cntr_ptr          - Pointer to seconds value [1:0]
*   *nsec_cntr_ptr         - Pointer to nanoseconds value [29:0]
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_frc_ptp_get(etime_handle_t *hndl, UINT32 *sec_cntr_ptr, UINT32 *nsec_cntr_ptr)
{
    PMC_ENTRY();

    /* Read counters using *_get() functions from the following registers
     * Register 0x4164: ETIME120 - ETIME Captured local FRC, PTP Source
     */

    PMC_RETURN(PMC_SUCCESS);

} /* etime_frc_ptp_get */


/*******************************************************************************
*  etime_refl_stats_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read the ETIME reflection path packet counters
*
* INPUTS:
*   hndl            - handle to the ETIME instance to be operated on
*   data_rate       - data rate for the link
*   link            - link to configure
*
* OUTPUTS:
*   *count_val_ptr  - Pointer to counter value read for that link
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR etime_refl_stats_cnt_get(etime_handle_t    *hndl, 
                                          UINT32            link,
                                          etime_data_rate_t data_rate,
                                          UINT32            *count_val_ptr)
{
  UINT32 read_value;

  PMC_ENTRY();

  /* Check for valid link */
  if (ETIME_LINK_100G == data_rate) {
    if (ETIME_100G_LINK_0 != link) {
      PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }
  } else if (ETIME_LINK_40G == data_rate) {
    if ((ETIME_40G_LINK_0 != link) && (ETIME_40G_LINK_4 != link) && (ETIME_40G_LINK_8 != link)) {
      PMC_RETURN(ETIME_LOG_ERR_INVALID_PARAMETERS);
    }
  } /* end if */

  switch (link) {
  case  0: read_value = etime_field_REFL_PMON_0_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK0_BIT_REFL_PMON_0_MSK;  break;
  case  1: read_value = etime_field_REFL_PMON_1_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK1_BIT_REFL_PMON_1_MSK;  break;
  case  2: read_value = etime_field_REFL_PMON_2_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK2_BIT_REFL_PMON_2_MSK;  break;
  case  3: read_value = etime_field_REFL_PMON_3_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK3_BIT_REFL_PMON_3_MSK;  break;
  case  4: read_value = etime_field_REFL_PMON_4_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK4_BIT_REFL_PMON_4_MSK;  break;
  case  5: read_value = etime_field_REFL_PMON_5_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK5_BIT_REFL_PMON_5_MSK;  break;
  case  6: read_value = etime_field_REFL_PMON_6_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK6_BIT_REFL_PMON_6_MSK;  break;
  case  7: read_value = etime_field_REFL_PMON_7_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK7_BIT_REFL_PMON_7_MSK;  break;
  case  8: read_value = etime_field_REFL_PMON_8_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK8_BIT_REFL_PMON_8_MSK;  break;
  case  9: read_value = etime_field_REFL_PMON_9_get(NULL, hndl)  & ETIME120_REG_EXT_PMON_LINK9_BIT_REFL_PMON_9_MSK;  break;
  case 10: read_value = etime_field_REFL_PMON_10_get(NULL, hndl) & ETIME120_REG_EXT_PMON_LINK10_BIT_REFL_PMON_10_MSK; break;
  case 11: read_value = etime_field_REFL_PMON_11_get(NULL, hndl) & ETIME120_REG_EXT_PMON_LINK11_BIT_REFL_PMON_11_MSK; break;
  default: read_value = 0x00000000; break;
  } /* end switch */
  
  *count_val_ptr = read_value;

  PMC_RETURN(PMC_SUCCESS);

} /* etime_refl_stats_cnt_get */

/*
** Private Functions
*/

/*
** End of file
*/
