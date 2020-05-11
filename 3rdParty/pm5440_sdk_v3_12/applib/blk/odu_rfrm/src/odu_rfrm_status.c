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
#include "odu_rfrm_loc.h"

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
* odu_rfrm_pm_near_end_pmon_ds_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves near end defect second for the ODU channel's PM
*   layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *near_end_ds        - status of near end defect second (pN_DS) for the
*                         channel's PM layer \n
*                         0 -- The previous PMON transfer interval is not a near
*                              end defect second. \n
*                         1 -- The previous PMON transfer interval is a near end
*                              defect second.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_near_end_pmon_ds_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  UINT32 *near_end_ds)
{
    PMC_ENTRY();

    /* poll PMON_DONE_I bit to make sure that the transfer of PMON primitives
       is complete */
    odu_rfrm_field_PMON_DONE_I_poll(NULL, odu_rfrm_handle, 1, PMC_POLL_EQUAL, 1000, NULL, 1);

    /* read the status */
    *near_end_ds = odu_rfrm_field_PN_DS_PM_get(NULL, odu_rfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_near_end_pmon_ds_get */

/*******************************************************************************
* odu_rfrm_pm_far_end_pmon_ds_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves far end defect second for the ODU channel's PM
*   layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   *far_end_ds         - pointer to storage of far end defect second (pF_DS)
*                         for the channel's PM layer
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_far_end_pmon_ds_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 UINT32 *far_end_ds)
{
    PMC_ENTRY();

    /* poll PMON_DONE_I bit to make sure that the transfer of PMON primitives
       is complete */
    odu_rfrm_field_PMON_DONE_I_poll(NULL, odu_rfrm_handle, 1, PMC_POLL_EQUAL, 1000, NULL, 1);

    *far_end_ds = odu_rfrm_field_PF_DS_PM_get(NULL, odu_rfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_far_end_pmon_ds_get */

/*******************************************************************************
* odu_rfrm_pm_near_end_pmon_ebc_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves near end errored block counter (pN_EBC) for the ODU
*   channel's PM layer. This near end errored block counter value is transfered
*   during a PMON transfer event.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   *near_end_ebc_cnt   - pointer to storage of near end errored block counter
*                         for the channel's PM layer
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_near_end_pmon_ebc_cnt_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                       UINT32 chnl_id,
                                                       UINT32 *near_end_ebc_cnt)
{
    PMC_ENTRY();

    /* poll PMON_DONE_I bit to make sure that the transfer of PMON primitives
       is complete */
    odu_rfrm_field_PMON_DONE_I_poll(NULL, odu_rfrm_handle, 1, PMC_POLL_EQUAL, 1000, NULL, 1);

    /* read the status */
    *near_end_ebc_cnt = odu_rfrm_field_PN_EBC_PM_get(NULL, odu_rfrm_handle,
                                                     chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_near_end_pmon_ebc_cnt_get */

/*******************************************************************************
* odu_rfrm_pm_far_end_pmon_ebc_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves near end errored block counter (pN_EBC) for the ODU
*   channel's PM layer. This near end errored block counter value is transfered
*   during a PMON transfer event.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   *far_end_ebc_cnt    - pointer to storage of far end errored block counter
*                         for the channel's PM layer
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_far_end_pmon_ebc_cnt_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                      UINT32 chnl_id,
                                                      UINT32 *far_end_ebc_cnt)
{
    PMC_ENTRY();

    /* poll PMON_DONE_I bit to make sure that the transfer of PMON primitives
       is complete */
    odu_rfrm_field_PMON_DONE_I_poll(NULL, odu_rfrm_handle, 1, PMC_POLL_EQUAL, 1000, NULL, 1);

    /* read the status */
    *far_end_ebc_cnt = odu_rfrm_field_PF_EBC_PM_get(NULL, odu_rfrm_handle,
                                                    chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_far_end_pmon_ebc_cnt_get */

/*******************************************************************************
* odu_rfrm_pm_near_end_defect_ebc_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves near end errored block counter (pN_EBC) for the ODU
*   channel's PM layer. This near end errored block counter value is transfered
*   during a defects and consequential actions transfer event.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   *near_end_ebc_cnt    - pointer to storage of far end errored block counter
*                         for the channel's PM layer
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_near_end_defect_ebc_cnt_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                         UINT32 chnl_id,
                                                         UINT32 *near_end_ebc_cnt)
{
    PMC_ENTRY();

    /* poll PMON_DONE_I bit to make sure that the transfer of PMON primitives
       is complete */
    odu_rfrm_field_DEF_ACT_DONE_I_poll(NULL, odu_rfrm_handle, 1, PMC_POLL_EQUAL, 1000, NULL, 1);

    /* read the status */
    *near_end_ebc_cnt = odu_rfrm_field_NE_EBC_PM_get(NULL, odu_rfrm_handle,
                                                    chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_near_end_defect_ebc_cnt_get */

/*******************************************************************************
* odu_rfrm_pm_far_end_defect_ebc_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves near end errored block counter (pN_EBC) for the ODU
*   channel's PM layer. This near end errored block counter value is transfered
*   during a defects and consequential actions transfer event.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   *far_end_ebc_cnt    - pointer to storage of far end errored block counter
*                         for the channel's PM layer
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_far_end_defect_ebc_cnt_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                        UINT32 chnl_id,
                                                        UINT32 *far_end_ebc_cnt)
{
    PMC_ENTRY();

    /* poll PMON_DONE_I bit to make sure that the transfer of PMON primitives
       is complete */
    odu_rfrm_field_DEF_ACT_DONE_I_poll(NULL, odu_rfrm_handle, 1, PMC_POLL_EQUAL, 1000, NULL, 1);

    /* read the status */
    *far_end_ebc_cnt = odu_rfrm_field_FE_EBC_PM_get(NULL, odu_rfrm_handle,
                                                    chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_near_end_defect_ebc_cnt_get */

/*******************************************************************************
* odu_rfrm_pm_tcm_defect_intr_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the degraded defect monitoring interval and
*   degraded defect errored block count threshold for detecting dDEG.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- Floating TCM \n
*                         8      -- PM_NIM
*
* OUTPUTS:
*   *intr_status        - pointer to storage of defect interrupt status for
*                         the ODU channel
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcm_defect_intr_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT32 *intr_status)
{
    /* variable declaration */
    UINT32 tcm_pool_id;
    PMC_ERROR ret_val;

    PMC_ENTRY();

    if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
    {   /* PM layer */
        *intr_status = odu_rfrm_lfield_range_PM_DEF_I_get(NULL, odu_rfrm_handle,
                                                          chnl_id, chnl_id);
    }
    else
    {
        /* POOL based */
        /* given chnl_id and pm_tcm_num, find TCM_POOL_ID */
        ret_val = odu_rfrm_tcm_pool_id_search(odu_rfrm_handle, chnl_id,
                                              pm_tcm_num, &tcm_pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        *intr_status = odu_rfrm_lfield_range_PID_DEF_I_get(NULL, odu_rfrm_handle,
                                                           tcm_pool_id,
                                                           tcm_pool_id);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_defect_intr_get */

/*******************************************************************************
* odu_rfrm_comb_defect_intr_enable_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the combined defect interrupt status in the ODU_RFRM.
*   If any defect change of state or consequential action change of state has
*   been detected in any of the sublayers of the channel or in one or more
*   frames, the corresponding COMB_XXXX_I will be set.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   comb_intr_enbl      - Combined interrupt status.
*                         Set the bit to 1 to enable
*                         interrupt processing. Set the bit to 0 to process
*                         the interrupt via polling. \n
*                         Bit 15 -- COMB_ABDI_I \n
*                         Bit 14 -- COMB_AAIS_I \ n
*                         Bit 13 -- COMB_ATSD_I \n
*                         Bit 12 -- COMB_ATSF_I \n
*                         Bit 11 -- unused \n
*                         Bit 10 -- COMB_OOF_I \n
*                         Bit 9  -- COMB_DLOFLOM_I \n
*                         Bit 8  -- COMB_DBIAE_I \n
*                         Bit 7  -- COMB_DIAE_I \ n
*                         Bit 6  -- COMB_DLTC_I \n
*                         Bit 5  -- COMB_DBDI_I \n
*                         Bit 4  -- COMB_DDEG_I \n
*                         Bit 3  -- COMB_DTIM_I \n
*                         Bit 2  -- COMB_DAIS_I \n
*                         Bit 1  -- COMB_DOCI_I \n
*                         Bit 0  -- COMB_DLCK_I
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_comb_defect_intr_enable_set(odu_rfrm_handle_t *odu_rfrm_handle,
                                                      UINT32 comb_intr_enbl)
{
    /* Read Defect interrupt status register */
    UINT32 i;
    UINT32 mask = 0x1;

    PMC_ENTRY();

    /* Process comb_intr_enbl mask input to find bit locations for all enabled
       interrupts */
    for (i = 0; i < 16; i++)
    {
        if ((comb_intr_enbl >> i) & mask)
        {
            odu_rfrm_reg_DEFECT_INT_EN_field_set(NULL, odu_rfrm_handle, mask, i, 1);
        }
    }

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* odu_rfrm_comb_defect_intr_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the combined defect interrupt status in the ODU_RFRM.
*   If any defect change of state of consequential action change of state has
*   been detected in any of the sublayers of the channel, the corresponding
*   COMB_XXXX_I will be set.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   *intr_stat          - Combined interrupt status \n
*                         Bit 15 -- COMB_ABDI_I \n
*                         Bit 14 -- COMB_AAIS_I \n
*                         Bit 13 -- COMB_ATSD_I \n
*                         Bit 12 -- COMB_ATSF_I \n
*                         Bit 11 -- unused \n
*                         Bit 10 -- COMB_OOF_I \n
*                         Bit 9  -- COMB_DLOFLOM_I \n
*                         Bit 8  -- COMB_DBIAE_I \n
*                         Bit 7  -- COMB_DIAE_I \n
*                         Bit 6  -- COMB_DLTC_I \ n
*                         Bit 5  -- COMB_DBDI_I \n
*                         Bit 4  -- COMB_DDEG_I \n
*                         Bit 3  -- COMB_DTIM_I \n
*                         Bit 2  -- COMB_DAIS_I \n
*                         Bit 1  -- COMB_DOCI_I \n
*                         Bit 0  -- COMB_DLCK_I
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_comb_defect_intr_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                    UINT32 *intr_stat)
{
    PMC_ENTRY();

    /* Read Defect interrupt status register */
    *intr_stat = odu_rfrm_reg_DEFECT_INT_STAT_read(NULL, odu_rfrm_handle);

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* odu_rfrm_chnl_pm_defect_intr_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves PM layer defect status per channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *pm_defect_stat     - Defect interrupt status for the channel. \n
*                         If set to 1, one or more defect and/or action sticky
*                         bits are set in the DEFECTS_ACTIONS register
*                         associated with this channel. \n
*                         If set to 0, no defect or action sticky bits are set.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_chnl_pm_defect_intr_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id, UINT32 *pm_defect_stat)
{
    /* variable declaration */
    UINT32 tmp_status;

    PMC_ENTRY();

    PMC_LOG_TRACE("calling odu_rfrm_chnl_pm_defect_stat...\n");

    /* read from CHANNEL DEFECT INTERRUPT SUMMARY0/1/2 */
    tmp_status = odu_rfrm_lfield_range_PM_DEF_I_get(NULL, odu_rfrm_handle, chnl_id, chnl_id);

    /* print out PM_DEF_I for simulation test */
    PMC_LOG_TRACE("tmp_status = %u...\n", tmp_status);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_chnl_pm_defect_intr_get */

/*******************************************************************************
* odu_rfrm_tcmi_defect_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the status of defects in the PM or a TCMi layer of
*   an ODUk channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance \n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- Floating TCM
*   *defect_stat        - Defects actions status for the TCMi layer of the
*                         channel. \n
*                         bit 12    -- ABDI_V  \n
*                         bit 11    -- AAIS_V  \n
*                         bit 10    -- ATSD_V  \n
*                         bit 9     -- ATSF_V  \n
*                         bit 8     -- DIAE_V  \n
*                         bit 7     -- DLTC_V  \n
*                         bit 6     -- DBIAE_V \n
*                         bit 5     -- DBDI_V  \n
*                         bit 4     -- DDEG_V  \n
*                         bit 3     -- DTIM_V  \n
*                         bit 2     -- DAIS_V  \n
*                         bit 1     -- DOCI_V  \n
*                         bit 0     -- DLCK_V  \n
*                         If set to 1, the defect was asserted at the end of the
*                         previous transfer interval. \n
*                         If set to 0, the defect was cleared at the end of the
*                         previous transfer interval.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_tcmi_defect_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                               UINT32 chnl_id,
                                               odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                               UINT32 *defect_stat)
{
    /* variable declaration */
    UINT32 value;
    UINT32 tcm_pool_id;
    UINT32 valid;
    PMC_ERROR ret_val;

    PMC_ENTRY();

    PMC_LOG_TRACE("calling odu_rfrm_chnl_tcm_defect_stat...\n");

    ret_val = odu_rfrm_tcm_pool_id_search(odu_rfrm_handle, chnl_id, pm_tcm_num,
                                          &tcm_pool_id);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }

    /* Check if retrieved TCM_POOL_ID is valid (i.e. assigned) */
    valid = odu_rfrm_tcm_pool_id_check(odu_rfrm_handle, tcm_pool_id,
                                       chnl_id, pm_tcm_num);

    if (valid == 0)
    {
        PMC_LOG_TRACE("tcm_pool_id = %u...is INVALID.\n", tcm_pool_id);
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    else
    {
        value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read(NULL, odu_rfrm_handle, tcm_pool_id);
    }

       /* value = odu_rfrm_reg_DEFECTS_ACTIONS_array_read(NULL, odu_rfrm_handle, tcm_pool_id);*/
    *defect_stat = ((value >> 16) & 0x1FF) + (((value >> 28) & 0xF) << 9);

     /* print out TCMi DEFECT ACTIONS for simulation test */
     PMC_LOG_TRACE("defect_stat = %u...\n",*defect_stat);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_tcmi_defect_stat_get */

/*******************************************************************************
* odu_rfrm_pm_defect_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the status of defects in a PM layer of
*   an ODUk channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_id               - PM layer instance. \n
*                         0 -- PM \n
*                         8 -- PM NIM
*   *defect_stat        - Defects actions status for the TCMi layer of the
*                         channel. \n
*                         bit 10    -- ABDI_V \n
*                         bit 9     -- ATSD_V \n
*                         bit 8     -- ATSF_V \n
*                         bit 7     -- DLOFLOM_V \n
*                         bit 6     -- DOOF_V \n
*                         bit 5     -- DBDI_V \n
*                         bit 4     -- DDEG_V \ n
*                         bit 3     -- DTIM_V \n
*                         bit 2     -- DAIS_V \n
*                         bit 1     -- DOCI_V \n
*                         bit 0     -- DLCK_V
*                         If set to 1, the defect was asserted at the end of the
*                         previous transfer interval. \n
*                         If set to 0, the defect was cleared at the end of the
*                         previous transfer interval.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_defect_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_id,
                                             UINT32 *defect_stat)
{
    /* variable declaration */
    UINT32 value;
    UINT32 tcm_pool_id;
    UINT32 valid;
    PMC_ERROR ret_val;

    PMC_ENTRY();

    PMC_LOG_TRACE("calling odu_rfrm_chnl_tcm_defect_stat...\n");

    /* find the TCM_POOL_ID */
    if (pm_id == ODU_RFRM_MESSAGE_TYPE_PM_NIM)
    {
        ret_val = odu_rfrm_tcm_pool_id_search(odu_rfrm_handle, chnl_id, pm_id,
                                              &tcm_pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }

        /* Check if retrieved TCM_POOL_ID is valid (i.e. assigned) */
        valid = odu_rfrm_tcm_pool_id_check(odu_rfrm_handle, tcm_pool_id,
                                           chnl_id, pm_id);

        if (valid == 0)
        {
            PMC_LOG_TRACE("tcm_pool_id = %u...is INVALID.\n", tcm_pool_id);
            PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }

        /* read from TCMi DEFECT ACTIONS */
        value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read(NULL, odu_rfrm_handle, tcm_pool_id);
    }
    else
    {
        /*pm_layer = chnl_id + 256;*/

        /* read from TCMi DEFECT ACTIONS per channel */
        value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read(NULL, odu_rfrm_handle, chnl_id);
        /*value = odu_rfrm_reg_DEFECTS_ACTIONS_array_read(NULL, odu_rfrm_handle, pm_layer);*/
    }

    /* return status value */
    *defect_stat = ((value >> 16) & 0x3F) + (((value >> 23) & 0x3) << 6) + (((value >> 28) & 0x3) << 8)
                    + (((value >> 31) & 0x1) << 10);                    

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_defect_stat_get */

/*******************************************************************************
* odu_rfrm_pm_defect_intr_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves interrupt status of defects and consequential
*   actions for the channel's PM or PM_NIM layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_num              - PM layer instance \n
*                         0 -- PM \n
*                         8 -- PM NIM
*   *defect_intr_stat   - Defects actions status for the TCMi layer of the
*                         channel. \n
*                         bit 10    -- ABDI_I \n
*                         bit 9     -- ATSD_I \n
*                         bit 8     -- ATSF_I \n
*                         bit 7     -- DLOFLOM_I \n
*                         bit 6     -- DOOF_I \n
*                         bit 5     -- DBDI_I \n
*                         bit 4     -- DDEG_I \n
*                         bit 3     -- DTIM_I \n
*                         bit 2     -- DAIS_I \n
*                         bit 1     -- DOCI_I \n
*                         bit 0     -- DLCK_I
*                         If set to 1, the defect was asserted at the end of the
*                         previous transfer interval. \n
*                         If set to 0, the defect was cleared at the end of the
*                         previous transfer interval.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_defect_intr_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_num,
                                                  UINT32 *defect_intr_stat)
{
    /* variable declaration */
    UINT32 value;
    UINT32 tcm_pool_id;
    UINT32 valid;
    PMC_ERROR ret_val;

    PMC_ENTRY();

    if (pm_num == ODU_RFRM_MESSAGE_TYPE_PM_NIM)
    {   /* PM_NIM layer */
        /* find the tcm pool id for the PM NIM layer of the channel */

        ret_val = odu_rfrm_tcm_pool_id_search(odu_rfrm_handle, chnl_id, pm_num,
                                              &tcm_pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        /* Check if retrieved TCM_POOL_ID is valid (i.e. assigned) */
        valid = odu_rfrm_tcm_pool_id_check(odu_rfrm_handle, tcm_pool_id,
                                           chnl_id, pm_num);

        if (valid == 0)
        {
            PMC_LOG_TRACE("tcm_pool_id = %u...is INVALID.\n", tcm_pool_id);
            PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }

        /* read from TCMi DEFECT ACTIONS */
        value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read(NULL, odu_rfrm_handle, tcm_pool_id);
    }
    else
    {
        value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read(NULL, odu_rfrm_handle, chnl_id);
    }

    /* mask out the interrupt bits only */
    value = value & 0x1FFF;
    PMC_LOG_TRACE("value = 0x%x\n", value);

    /* return status value */
    *defect_intr_stat = (value & 0x3F) + (((value >> 7) & 0x3) << 7) +
                        (((value >> 12) & 0x3) << 12);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_defect_intr_stat_get */

/*******************************************************************************
* odu_rfrm_tcm_defect_intr_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves interrupt status of defects and consequential
*   actions for the channel's TCM or Floating TCM layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   tcm_num             - TCMi layer instance. \n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- Floating TCM
*   *defect_intr_stat   - Defects actions status for the TCMi layer of the
*                         channel. \n
*                         bit 10    -- ABDI_I \n
*                         bit 9     -- ATSD_I \n
*                         bit 8     -- ATSF_I \n
*                         bit 7     -- DLOFLOM_I \n
*                         bit 6     -- DOOF_I \n
*                         bit 5     -- DBDI_I \n
*                         bit 4     -- DDEG_I \n
*                         bit 3     -- DTIM_I \n
*                         bit 2     -- DAIS_I \n
*                         bit 1     -- DOCI_I \n
*                         bit 0     -- DLCK_I \n
*                         If set to 1, the defect was asserted at the end of the
*                         previous transfer interval. \n
*                         If set to 0, the defect was cleared at the end of the
*                         previous transfer interval.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_tcm_defect_intr_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id, odu_rfrm_pm_tcm_id_t tcm_num,
                                                   UINT32 *defect_intr_stat)
{
    PMC_ENTRY();

    PMC_RETURN(PMC_SUCCESS);
}
/*
** Private Functions
*/

/*
** End of file
*/
