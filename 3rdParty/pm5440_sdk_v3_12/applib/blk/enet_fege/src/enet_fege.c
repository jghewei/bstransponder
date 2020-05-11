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
*     Implement ENET_FEGE features.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "enet_fege_loc.h"
#include "enet_fege_io.h"
#include "l1rpp_fege_io.h"
#include "l1rpp_fege_io_inline.h"
#include "l1tpp_fege_io.h"
#include "rmac_io.h"
#include "tmac_io.h"
#include "mstatx_io.h"


/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define ENET_FEGE_TSTMP_CORR_UPD_BYPASS_DFLT     0x1
#define ENET_FEGE_MAC_LOWPWR_DFLT                0x0
#define ENET_FEGE_GE_GFPT_LOWPWR_DFLT            0x1
#define ENET_FEGE_UNIDIRECTIONAL_MODE_DFLT       0x0
#define ENET_FEGE_MODE_DFLT                      0x3

#define ENET_FEGE_TX_TSTMP_FIFO_RST_DFLT         0x1
#define ENET_FEGE_PTP_NTP_RST_DFLT               0x1
#define ENET_FEGE_GE_GFPT_RST_DFLT               0x1
#define ENET_FEGE_MSTATX_RST_DFLT                0x0
#define ENET_FEGE_L1RPP_FEGE_RST_DFLT            0x0
#define ENET_FEGE_RMAC_RST_DFLT                  0x0
#define ENET_FEGE_RX_ECLASS_RST_DFLT             0x0
#define ENET_FEGE_L1TPP_FEGE_RST_DFLT            0x0
#define ENET_FEGE_TMAC_RST_DFLT                  0x0
#define ENET_FEGE_ENET_FEGE_RST_DFLT             0x0


#define ENET_FEGE_L1RPP_AUTO_SENSE_DFLT          0x1
#define ENET_FEGE_L1RPP_MODE_DFLT                0x1
#define ENET_FEGE_L1RPP_AN_ENABLE_DFLT           0x1
#define ENET_FEGE_L1RPP_AN_RESTART_DFLT          0x1
#define ENET_FEGE_L1RPP_LINK_TIMER_DFLT          0x030E
#define ENET_FEGE_L1RPP_MIN_LINK_TIME_DUR_DFLT   0x0
#define ENET_FEGE_L1RPP_MODE_SEL_DFLT            0x0
#define ENET_FEGE_L1RPP_L1RPP_EN_DFLT            0x1

#define ENET_FEGE_RMAC_PAMB_CK_DFLT              0x1
#define ENET_FEGE_RMAC_TAG_AWARE_DFLT            0x1
#define ENET_FEGE_RMAC_INRANGE_DIS_DFLT          0x1
#define ENET_FEGE_RMAC_INRANGE_CK_DFLT           0x3
#define ENET_FEGE_RMAC_MAX_FRM_TRN_DFLT          0x1
#define ENET_FEGE_RMAC_MAX_FRM_DIS_DFLT          0x1
#define ENET_FEGE_RMAC_MAX_FRM_CK_DFLT           0x1
#define ENET_FEGE_RMAC_MIN_FRM_DIS_DFLT          0x1
#define ENET_FEGE_RMAC_MIN_FRM_CK_DFLT           0x1
#define ENET_FEGE_RMAC_CRC_DIS_DFLT              0x1
#define ENET_FEGE_RMAC_CRC_CK_DFLT               0x1
#define ENET_FEGE_RMAC_TRANSPARENT_DFLT          0x0
#define ENET_FEGE_RMAC_ETHER_MODE_DFLT           0x1
#define ENET_FEGE_RMAC_RMAC_ENBL_DFLT            0x1

#define ENET_FEGE_RMAC_INTERPKT_L1_EN_DFLT       0x0
#define ENET_FEGE_RMAC_BLIND_DFLT                0x0
#define ENET_FEGE_RMAC_MIN_FRM_LEN_DFLT          0x0
#define ENET_FEGE_RMAC_PAD_STRIP_EN_DFLT         0x0
#define ENET_FEGE_RMAC_CRC_STRIP_EN_DFLT         0x1
#define ENET_FEGE_RMAC_CARRIER_EN_DFLT           0x0
#define ENET_FEGE_RMAC_INPAUSE_EN_DFLT           0x1
#define ENET_FEGE_RMAC_PAUSEFRM_DROP_DFLT        0x1
#define ENET_FEGE_RMAC_CTRLFRM_DROP_DFLT         0x0
#define ENET_FEGE_RMAC_DA_DROP_DFLT              0x0


#define ENET_FEGE_TMAC_CRC_CK_DFLT               0x0
#define ENET_FEGE_TMAC_PAD_ADD_EN_DFLT           0x0
#define ENET_FEGE_TMAC_CRC_ADD_EN_DFLT           0x1
#define ENET_FEGE_TMAC_MIN_FRM_CK_DFLT           0x1
#define ENET_FEGE_TMAC_TRUNC_EN_DFLT             0x1
#define ENET_FEGE_TMAC_MAX_FRM_CK_DFLT           0x1
#define ENET_FEGE_TMAC_EBUF_PAUSE_EN_DFLT        0x0
#define ENET_FEGE_TMAC_IBUF_HTHR_EN_DFLT         0x1
#define ENET_FEGE_TMAC_XON_EN_DFLT               0x1
#define ENET_FEGE_TMAC_RMAC_PAUSE_EN_DFLT        0x1
#define ENET_FEGE_TMAC_EBUF_ERR_EN_DFLT          0x1
#define ENET_FEGE_TMAC_TRANSPARENT_DFLT          0x0
#define ENET_FEGE_TMAC_HW_STOP_EN_DFLT           0x0
#define ENET_FEGE_TMAC_TMAC_ENBL_DFLT            0x1

#define ENET_FEGE_L1TPP_RD_ALIGN_EN_DFLT         0x0
#define ENET_FEGE_L1TPP_MODE_DFLT                0x1
#define ENET_FEGE_L1TPP_L1TPP_EN_DFLT            0x1

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
** Log Messaging
*/
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/* 
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above 
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char ENET_FEGE_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ENET_FEGE_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ENET_FEGE_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

PRIVATE void enet_fege_basic_cfg(enet_fege_handle_t *hndl,
                                 BOOL8 ohp,
                                 BOOL8 enable_tx,
                                 BOOL8 enable_rx);

PRIVATE void enet_fege_var_default_init(enet_fege_handle_t *hndl);


PRIVATE void enet_fege_ctxt_state_set(enet_fege_handle_t      *hndl,
                                      enet_fege_state_t   state);

PRIVATE PMC_ERROR enet_fege_ctxt_state_transition_request(enet_fege_handle_t      *hndl,
                                                          enet_fege_state_t   next_state);

PRIVATE PMC_ERROR enet_fege_int_init(enet_fege_handle_t    *enet_fege_handle,
                                     BOOL                   enable);

/*
** Public Functions
*/

/*******************************************************************************
* enet_fege_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an ENET_FEGE block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the ENET_FEGE subsystem relative
*                           to the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   enet_fege_handle_t  - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC enet_fege_handle_t *enet_fege_ctxt_create(pmc_handle_t       *parent, 
                                                 UINT32              base_address, 
                                                 pmc_sys_handle_t   *sys_handle, 
                                                 const char         *tsb_name)
{
    enet_fege_handle_t *enet_fege_handle;

    PMC_ENTRY();
    enet_fege_handle = (enet_fege_handle_t *)PMC_CTXT_CALLOC(sizeof(enet_fege_handle_t), parent);

    pmc_handle_init(parent, enet_fege_handle, sys_handle, PMC_MID_DIGI_ENET_FEGE, tsb_name, base_address);
    
    enet_fege_handle->var.ctxt.state = LAST_ENET_FEGE_STATE;
    
    PMC_RETURN(enet_fege_handle);
} /* enet_fege_ctxt_create */

 
/*******************************************************************************
* enet_fege_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an ENET_FEGE block instance.
*
*
* INPUTS:
*   *enet_fege_handle     - pointer to ENET_FEGE handle instance
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
PUBLIC void enet_fege_ctxt_destroy(enet_fege_handle_t *enet_fege_handle)
{
    PMC_ENTRY();
    
    PMC_CTXT_FREE(&enet_fege_handle, enet_fege_handle);
    
    PMC_RETURN();
} /* enet_fege_ctxt_destroy */

/*******************************************************************************
* enet_fege_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ENET_FEGE block handle instance.
*
*
* INPUTS:
*   *enet_fege_handle     - pointer to ENET_FEGE handle instance
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
PUBLIC void enet_fege_handle_init(enet_fege_handle_t *enet_fege_handle)
{
    /* variable declaration */
    PMC_ENTRY();

    enet_fege_var_default_init(enet_fege_handle);

    PMC_RETURN();
     
} /* enet_fege_handle_init */


/*******************************************************************************
* enet_fege_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an ENET_FEGE block instance.
*
*
* INPUTS:
*   *enet_fege_handle     - pointer to ENET_FEGE handle instance
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
PUBLIC UINT32 enet_fege_base_address_get(enet_fege_handle_t *enet_fege_handle)
{
    PMC_ENTRY();

    PMC_RETURN(enet_fege_handle->base.base_address);
} /* enet_fege_base_address_get */


/*******************************************************************************
* enet_fege_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions ENET_FEGE block instance.
*
*
* INPUTS:
*   *hndl               - pointer to ENET_FEGE handle instance
*    ohp                - ohp (if true) or rcp  (if false)
*    enable_tx          - when TRUE MAC TX datapath is enabled, otherwise it is disabled
*    enable_rx          - when TRUE MAC RX datapath  datapath is enabled, otherwise it is disabled
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
PUBLIC PMC_ERROR enet_fege_prov(enet_fege_handle_t   *hndl,
                                BOOL8                 ohp,
                                BOOL8                 enable_tx,
                                BOOL8                 enable_rx)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    rc = enet_fege_ctxt_state_transition_request(hndl, ENET_FEGE_STATE_OPERATIONAL);

    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(rc);
    }

    enet_fege_basic_cfg(hndl, ohp, enable_tx, enable_rx);

    rmac_field_RMAC_ENBL_set(NULL, hndl, (UINT32)enable_rx);
    tmac_field_TMAC_ENBL_set(NULL, hndl, (UINT32)enable_tx);

    hndl->var.ctxt.rx_enabled = enable_rx;
    hndl->var.ctxt.tx_enabled = enable_tx;

    /* enable interrupts steering */
    enet_fege_int_init(hndl, TRUE);

    enet_fege_ctxt_state_set(hndl, ENET_FEGE_STATE_OPERATIONAL);

    PMC_RETURN(rc);
} /* enet_fege_prov */

/*******************************************************************************
* enet_fege_basic_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures common features of ENET_FEGE block instance.
*
*
* INPUTS:
*   *hndl                 - pointer to ENET_FEGE handle instance
*    ohp                  - BOOL8 ohp
*    enable_tx            - when TRUE MAC TX datapath is enabled, otherwise it is disabled
*    enable_rx            - when TRUE MAC RX datapath  datapath is enabled, otherwise it is disabled
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
PRIVATE void enet_fege_basic_cfg(enet_fege_handle_t *hndl,
                                 BOOL8 ohp,
                                 BOOL8 enable_tx,
                                 BOOL8 enable_rx)
{
    
    PMC_ENTRY();


    /* Configure ENET FEGE TOP */
    enet_fege_field_TSTMP_CORR_UPD_BYPASS_set(NULL, hndl, ENET_FEGE_TSTMP_CORR_UPD_BYPASS_DFLT);
    enet_fege_field_MAC_LOWPWR_set(NULL, hndl, ENET_FEGE_MAC_LOWPWR_DFLT);
    enet_fege_field_UNIDIRECTIONAL_MODE_set(NULL, hndl, ENET_FEGE_UNIDIRECTIONAL_MODE_DFLT);
    enet_fege_field_FEGE_MODE_set(NULL, hndl, ENET_FEGE_MODE_DFLT);

    if (ohp)
    {
        enet_fege_field_IPG_RED_ON_THRESH_set(NULL,hndl, 3);
        enet_fege_field_IPG_RED_OFF_THRESH_set(NULL,hndl, 1);
        enet_fege_field_PAUSE_ON_THRESH_set(NULL,hndl, 3);
        enet_fege_field_PAUSE_OFF_THRESH_set(NULL,hndl, 2);
        enet_fege_field_TX_ECLASS_RST_set(NULL,hndl, 0);
        enet_fege_field_RX_TSTMP_OFFSET_UPD_set(NULL,hndl, 0);
        enet_fege_field_TX_TSTMP_OFFSET_UPD_set(NULL,hndl, 0);
        enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_set(NULL,hndl, 1);
        enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_set(NULL,hndl, 0);
        enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_set(NULL,hndl, 0);
        enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_set(NULL,hndl, 0);
        enet_fege_field_CORR_POSITION_set(NULL,hndl,0);
        enet_fege_field_TSTMP_POSITION_set(NULL,hndl,0);
        enet_fege_field_TAG_FOR_UPD_set(NULL,hndl,0);
        enet_fege_field_ONE_STEP_CLK_FCS_set(NULL,hndl,0);
        enet_fege_field_TSTMP_MODE_set(NULL,hndl,0);
        enet_fege_field_PTP_NTP_EN_set(NULL,hndl,0);
        enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_set(NULL,hndl,1);
        enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_set(NULL,hndl,0xFFFFFE55 );
        enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_set(NULL,hndl,0 );
        enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_set(NULL,hndl,0x000001EB );
        enet_fege_field_AUTO_BIT_ALIGN_OFFSET_set(NULL,hndl,1);
        enet_fege_field_ERR_PKT_DROP_set(NULL,hndl,1);
        enet_fege_field_EXTRACT_TAG_set(NULL,hndl,1);
    }

    enet_fege_field_TX_TSTMP_FIFO_RST_set(NULL, hndl, ENET_FEGE_TX_TSTMP_FIFO_RST_DFLT);
    enet_fege_field_GE_GFPT_RST_set(NULL, hndl, ENET_FEGE_GE_GFPT_RST_DFLT);
    enet_fege_field_GE_GFPT_RST_set(NULL, hndl, ENET_FEGE_GE_GFPT_RST_DFLT);
    enet_fege_field_MSTATX_RST_set(NULL, hndl, ENET_FEGE_MSTATX_RST_DFLT);
    enet_fege_field_L1RPP_FEGE_RST_set(NULL, hndl, ENET_FEGE_L1RPP_FEGE_RST_DFLT);
    enet_fege_field_RMAC_RST_set(NULL, hndl, ENET_FEGE_RMAC_RST_DFLT);
    enet_fege_field_RX_ECLASS_RST_set(NULL, hndl, ENET_FEGE_RX_ECLASS_RST_DFLT);
    enet_fege_field_L1TPP_FEGE_RST_set(NULL, hndl, ENET_FEGE_L1TPP_FEGE_RST_DFLT);
    enet_fege_field_TMAC_RST_set(NULL, hndl, ENET_FEGE_TMAC_RST_DFLT);
    enet_fege_field_ENET_FEGE_RST_set(NULL, hndl, ENET_FEGE_ENET_FEGE_RST_DFLT);

    /* configure RMAC */
    if (TRUE == enable_rx)
    {
        /* configure L1RPP */
        l1rpp_fege_field_MODE_set(NULL, hndl, ENET_FEGE_L1RPP_MODE_DFLT);
        l1rpp_fege_field_AN_ENABLE_set(NULL, hndl, ENET_FEGE_L1RPP_AN_ENABLE_DFLT);  
        l1rpp_fege_field_AUTO_SENSE_set(NULL, hndl, ENET_FEGE_L1RPP_AUTO_SENSE_DFLT);
        l1rpp_fege_field_AN_ABLTY_ADV_set(NULL, hndl, ENET_FEGE_L1RPP_AN_ABLTY_ADV_DFLT);  
        l1rpp_fege_field_AN_ABLTY_ADV_UPD_set(NULL, hndl, 1); 
        l1rpp_fege_field_AN_RESTART_set(NULL, hndl, 1); 
        l1rpp_fege_field_LINK_TIMER_set(NULL, hndl, ENET_FEGE_L1RPP_LINK_TIMER_DFLT);
        l1rpp_fege_field_MIN_LINK_TIME_DUR_set(NULL, hndl, ENET_FEGE_L1RPP_MIN_LINK_TIME_DUR_DFLT);
        l1rpp_fege_field_MODE_SEL_set(NULL, hndl, ENET_FEGE_L1RPP_MODE_SEL_DFLT);
        l1rpp_fege_field_L1RPP_EN_set(NULL, hndl, ENET_FEGE_L1RPP_L1RPP_EN_DFLT);

        if (ohp)
        {
            l1rpp_fege_field_LS_RESET_set(NULL, hndl,0);
            l1rpp_fege_field_RESYNC_COUNT_set(NULL, hndl,4);
            l1rpp_fege_field_LOS_COUNT_set(NULL, hndl,4);
            l1rpp_fege_field_SYNC_COUNT_set(NULL, hndl,3);


            l1rpp_fege_field_PTRN_SEL_set(NULL, hndl,0);
            l1rpp_fege_field_EN_CDET_SEL_set(NULL, hndl,0);
        }

        rmac_field_PAMB_CK_set(NULL, hndl, ENET_FEGE_RMAC_PAMB_CK_DFLT);
        rmac_field_TAG_AWARE_set(NULL, hndl, ENET_FEGE_RMAC_TAG_AWARE_DFLT);
        rmac_field_INRANGE_DIS_set(NULL, hndl, ENET_FEGE_RMAC_INRANGE_DIS_DFLT);
        rmac_field_INRANGE_CK_set(NULL, hndl, ENET_FEGE_RMAC_INRANGE_CK_DFLT);
        rmac_field_MAX_FRM_TRN_set(NULL, hndl, ENET_FEGE_RMAC_MAX_FRM_TRN_DFLT);
        rmac_field_MAX_FRM_DIS_set(NULL, hndl, ENET_FEGE_RMAC_MAX_FRM_DIS_DFLT);
        rmac_field_MAX_FRM_CK_set(NULL, hndl, ENET_FEGE_RMAC_MAX_FRM_CK_DFLT);
        rmac_field_MIN_FRM_DIS_set(NULL, hndl, ENET_FEGE_RMAC_MIN_FRM_DIS_DFLT);
        rmac_field_MIN_FRM_CK_set(NULL, hndl, ENET_FEGE_RMAC_MIN_FRM_CK_DFLT);
        rmac_field_CRC_DIS_set(NULL, hndl, ENET_FEGE_RMAC_CRC_DIS_DFLT);
        rmac_field_CRC_CK_set(NULL, hndl, ENET_FEGE_RMAC_CRC_CK_DFLT);
        rmac_field_TRANSPARENT_set(NULL, hndl, ENET_FEGE_RMAC_TRANSPARENT_DFLT);
        rmac_field_ETHER_MODE_set(NULL, hndl, ENET_FEGE_RMAC_ETHER_MODE_DFLT);

        rmac_field_INTERPKT_L1_EN_set(NULL, hndl, ENET_FEGE_RMAC_INTERPKT_L1_EN_DFLT);
        rmac_field_BLIND_set(NULL, hndl, ENET_FEGE_RMAC_BLIND_DFLT);
        rmac_field_MIN_FRM_LEN_set(NULL, hndl, ENET_FEGE_RMAC_MIN_FRM_LEN_DFLT);
        rmac_field_PAD_STRIP_EN_set(NULL, hndl, ENET_FEGE_RMAC_PAD_STRIP_EN_DFLT);
        rmac_field_CRC_STRIP_EN_set(NULL, hndl, ENET_FEGE_RMAC_CRC_STRIP_EN_DFLT);
        rmac_field_CARRIER_EN_set(NULL, hndl, ENET_FEGE_RMAC_CARRIER_EN_DFLT);
        rmac_field_INPAUSE_EN_set(NULL, hndl, ENET_FEGE_RMAC_INPAUSE_EN_DFLT);
        rmac_field_PAUSEFRM_DROP_set(NULL, hndl, ENET_FEGE_RMAC_PAUSEFRM_DROP_DFLT);
        rmac_field_CTRLFRM_DROP_set(NULL, hndl, ENET_FEGE_RMAC_CTRLFRM_DROP_DFLT);
        rmac_field_DA_DROP_set(NULL, hndl, ENET_FEGE_RMAC_DA_DROP_DFLT);

        if (ohp)
        {
            rmac_field_MAX_FRM_LEN_set(NULL, hndl,  0x2592);
            rmac_field_CTRLFRM_STAT_EN_set(NULL, hndl, 0);
            rmac_field_PAUSEFRM_STAT_EN_set(NULL, hndl,1);
            rmac_field_GPO_set(NULL, hndl, 0);
        }
    }


    /* configure TMAC */
    if (TRUE == enable_tx)
    {
        tmac_field_CRC_CK_set(NULL, hndl, ENET_FEGE_TMAC_CRC_CK_DFLT);
        tmac_field_PAD_ADD_EN_set(NULL, hndl, ENET_FEGE_TMAC_PAD_ADD_EN_DFLT);
        tmac_field_CRC_ADD_EN_set(NULL, hndl, ENET_FEGE_TMAC_CRC_ADD_EN_DFLT);
        tmac_field_MIN_FRM_CK_set(NULL, hndl, ENET_FEGE_TMAC_MIN_FRM_CK_DFLT);
        tmac_field_TRUNC_EN_set(NULL, hndl, ENET_FEGE_TMAC_TRUNC_EN_DFLT);
        tmac_field_MAX_FRM_CK_set(NULL, hndl, ENET_FEGE_TMAC_MAX_FRM_CK_DFLT);
        tmac_field_EBUF_PAUSE_EN_set(NULL, hndl, ENET_FEGE_TMAC_EBUF_PAUSE_EN_DFLT);
        tmac_field_IBUF_HTHR_EN_set(NULL, hndl, ENET_FEGE_TMAC_IBUF_HTHR_EN_DFLT);
        tmac_field_XON_EN_set(NULL, hndl, ENET_FEGE_TMAC_XON_EN_DFLT);
        tmac_field_RMAC_PAUSE_EN_set(NULL, hndl, ENET_FEGE_TMAC_RMAC_PAUSE_EN_DFLT);
        tmac_field_EBUF_ERR_EN_set(NULL, hndl, ENET_FEGE_TMAC_EBUF_ERR_EN_DFLT);
        tmac_field_TRANSPARENT_set(NULL, hndl, ENET_FEGE_TMAC_TRANSPARENT_DFLT);
        tmac_field_HW_STOP_EN_set(NULL, hndl, ENET_FEGE_TMAC_HW_STOP_EN_DFLT);

        if(ohp) 
        {

            tmac_field_LATE_COL_EN_set(NULL, hndl,0); 
            tmac_field_SFD_EN_set(NULL, hndl,1);
            tmac_field_PAMB_LEN_set(NULL, hndl,0x7);
            tmac_field_FIPG1_LEN_set(NULL, hndl,0xC);
            tmac_field_FIPG2_LEN_set(NULL, hndl,0x8);
            tmac_field_MAX_FRM_LEN_set(NULL, hndl,0x2592);
            tmac_field_MIN_FRM_TAG_set(NULL, hndl,0);
            tmac_field_MIN_FRM_LEN_set(NULL, hndl,0);
            tmac_field_PAUSE_TIME_set(NULL, hndl,0xFFFF);
            tmac_field_PAUSE_IVAL_set(NULL, hndl,0xEFFF);
            
        }
        l1tpp_fege_field_RD_ALIGN_EN_set(NULL, hndl, ENET_FEGE_L1TPP_RD_ALIGN_EN_DFLT);
        l1tpp_fege_field_MODE_set(NULL, hndl, ENET_FEGE_L1TPP_MODE_DFLT);
        l1tpp_fege_field_L1TPP_EN_set(NULL, hndl, ENET_FEGE_L1TPP_L1TPP_EN_DFLT);

        if (ohp)
        {
             l1tpp_fege_field_TENB_ERR_RDNEG_set(NULL, hndl, 0xF1);
             l1tpp_fege_field_CSF_ERR_CODE_set(NULL, hndl, 0x1FE);
             l1tpp_fege_field_PTRN_SEL_set(NULL, hndl, 0);
             l1tpp_fege_field_CUS_PTRN_set(NULL, hndl, 0xF4);
             l1tpp_fege_field_FEF_GEN_EN_set(NULL, hndl,0);
        }
    }

    hndl->var.ctxt.rx_enabled = FALSE;
    hndl->var.ctxt.tx_enabled = FALSE;

    PMC_RETURN();
} /* enet_fege_basic_cfg */



/*******************************************************************************
* enet_fege_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions ENET_FEGE block instance.
*
*
* INPUTS:
*   *hndl     - pointer to ENET_FEGE handle instance
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
PUBLIC PMC_ERROR enet_fege_deprov(enet_fege_handle_t *hndl)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    rc = enet_fege_ctxt_state_transition_request(hndl, ENET_FEGE_STATE_START);

    if (PMC_SUCCESS != rc)
    {
        PMC_RETURN(rc);
    }

    /* shutdown ENET FEGE sub blocks */

    enet_fege_field_TX_TSTMP_FIFO_RST_set(NULL, hndl, (~ENET_FEGE_TX_TSTMP_FIFO_RST_DFLT) & 0x1);
    enet_fege_field_GE_GFPT_RST_set(NULL, hndl, (~ENET_FEGE_GE_GFPT_RST_DFLT) & 0x1);
    enet_fege_field_GE_GFPT_RST_set(NULL, hndl, (~ENET_FEGE_GE_GFPT_RST_DFLT) & 0x1);
    enet_fege_field_MSTATX_RST_set(NULL, hndl, (~ENET_FEGE_MSTATX_RST_DFLT) & 0x1);
    enet_fege_field_L1RPP_FEGE_RST_set(NULL, hndl, (~ENET_FEGE_L1RPP_FEGE_RST_DFLT) & 0x1);
    enet_fege_field_RMAC_RST_set(NULL, hndl, (~ENET_FEGE_RMAC_RST_DFLT) & 0x1);
    enet_fege_field_RX_ECLASS_RST_set(NULL, hndl, (~ENET_FEGE_RX_ECLASS_RST_DFLT) & 0x1);
    enet_fege_field_L1TPP_FEGE_RST_set(NULL, hndl, (~ENET_FEGE_L1TPP_FEGE_RST_DFLT) & 0x1);
    enet_fege_field_TMAC_RST_set(NULL, hndl, (~ENET_FEGE_TMAC_RST_DFLT) & 0x1);
    enet_fege_field_ENET_FEGE_RST_set(NULL, hndl, (~ENET_FEGE_ENET_FEGE_RST_DFLT) & 0x1);

    if (TRUE == hndl->var.ctxt.rx_enabled)
    {
        l1rpp_fege_field_AN_ENABLE_set(NULL, hndl, (~ENET_FEGE_L1RPP_AN_ENABLE_DFLT) & 0x1);  
        l1rpp_fege_field_AN_RESTART_set(NULL, hndl, (~ENET_FEGE_L1RPP_AN_RESTART_DFLT) & 0x1); 
        l1rpp_fege_field_L1RPP_EN_set(NULL, hndl, (~ENET_FEGE_L1RPP_L1RPP_EN_DFLT) & 0x1);
        
        rmac_field_RMAC_ENBL_set(NULL, hndl, 0);
    }

    if (TRUE == hndl->var.ctxt.tx_enabled)
    {
        tmac_field_TMAC_ENBL_set(NULL, hndl, 0);
    }

    enet_fege_field_MAC_LOWPWR_set(NULL, hndl, (~ENET_FEGE_MAC_LOWPWR_DFLT) & 0x1);

    /* disable interrupts steering  */
    enet_fege_int_init(hndl, FALSE);

    enet_fege_ctxt_state_set(hndl, ENET_FEGE_STATE_START);

    PMC_RETURN(rc);
} /* enet_fege_deprov */

/*******************************************************************************
* FUNCTION: enet_fege_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in 
*   enet_fege interrupt table.
*   
*
* INPUTS:
*   enet_fege_handle  - Pointer to subsystem handle
*   int_table_ptr     - pointer to enet_fege interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_fege_int_enable(enet_fege_handle_t    *enet_fege_handle,
                                      enet_fege_int_t       *int_table_ptr,
                                      enet_fege_int_t       *int_en_table_ptr,
                                      BOOL                   enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    #ifdef ENET_FEGE_INT
    #undef ENET_FEGE_INT
    #endif
    #define ENET_FEGE_INT     UTIL_GLOBAL_INT_ENABLE_SINGLE

    ENET_FEGE_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* enet_fege_int_enable */


/*******************************************************************************
* FUNCTION: enet_fege_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the enet_fege interrupt table.
*   
*
* INPUTS:
*   enet_fege_handle  - Pointer to subsystem handle
*   filt_table_ptr    - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_table_ptr     - pointer to enet_fege interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_fege_int_retrieve(enet_fege_handle_t    *enet_fege_handle,
                                        enet_fege_int_t       *filt_table_ptr,
                                        enet_fege_int_t       *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(enet_fege_handle, enet_fege, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(enet_fege_handle, mstatx, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(enet_fege_handle, l1rpp_fege, UTIL_GLOBAL_INT_OPTIMIZE);


    #ifdef ENET_FEGE_INT
    #undef ENET_FEGE_INT
    #endif
    #define ENET_FEGE_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
    ENET_FEGE_INT_TABLE_DEFINE();


    #ifdef ENET_FEGE_STATUS
    #undef ENET_FEGE_STATUS
    #endif
    #define ENET_FEGE_STATUS UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE

     ENET_FEGE_STATUS_TABLE_DEFINE();

     UTIL_GLOBAL_INT_BUFFER_FLUSH(enet_fege_handle, enet_fege);
     UTIL_GLOBAL_INT_BUFFER_FLUSH(enet_fege_handle, mstatx);
     UTIL_GLOBAL_INT_BUFFER_FLUSH(enet_fege_handle, l1rpp_fege);   

    PMC_RETURN(result);
} /* enet_fege_int_retrieve */

/*******************************************************************************
* FUNCTION: enet_fege_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in 
*   enet_fege interrupt table.
*   
*
* INPUTS:
*   enet_fege_handle  - Pointer to subsystem handle
*   int_table_ptr     - pointer to enet_fege interrupt table with
*                       bits set to clear interrupts
*                       If NULL, clears all interrupts.
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_fege_int_clear(enet_fege_handle_t    *enet_fege_handle,
                                     enet_fege_int_t       *int_table_ptr,
                                     enet_fege_int_t       *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();


    #ifdef ENET_FEGE_INT
    #undef ENET_FEGE_INT
    #endif
    #define ENET_FEGE_INT    UTIL_GLOBAL_INT_CLEAR_SINGLE

    ENET_FEGE_INT_CLEAR_TABLE_DEFINE();


    PMC_RETURN(result);
} /* enet_fege_int_clear */

/*******************************************************************************
* FUNCTION: enet_fege_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches ENET_FEGE active interrupts.
*   
*
* INPUTS:
*   enet_fege_handle  - Pointer to subsystem handle
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR enet_fege_int_enabled_check(enet_fege_handle_t    *enet_fege_handle,
                                             enet_fege_int_t       *int_en_table_ptr,
                                             BOOL                  *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();


    #ifdef ENET_FEGE_INT
    #undef ENET_FEGE_INT
    #endif
    #define ENET_FEGE_INT    UTIL_GLOBAL_INT_CHECK_SINGLE

    ENET_FEGE_INT_CLEAR_TABLE_DEFINE();


    PMC_RETURN(result);
} /* enet_fege_int_enabled_chec */

/*******************************************************************************
*  enet_fege_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes ENET_FEGE var_t variables.
*
* INPUTS:
*   *hndl             - pointer to ENET_FEGE handle instance to be operated on 
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
PRIVATE void enet_fege_var_default_init(enet_fege_handle_t   *hndl)
{    
    PMC_ENTRY();

    PMC_ASSERT(hndl != NULL, ENET_FEGE_ERR_INVALID_ARG, 0, 0);
   
    enet_fege_ctxt_state_set(hndl, ENET_FEGE_STATE_START);
    hndl->var.ctxt.rx_enabled = FALSE;
    hndl->var.ctxt.tx_enabled = FALSE;

    PMC_RETURN();
} /* enet_fege_var_default_init */

/*******************************************************************************
*  enet_fege_ctxt_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets ENET_FEGE instance state.
*
* INPUTS:
*   *hndl             - pointer to ENET handle instance to be operated on  
*    state            - RI or DI ODU RCP provisioning state
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
PRIVATE void enet_fege_ctxt_state_set(enet_fege_handle_t      *hndl,
                                      enet_fege_state_t        state)
{
    
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ENET_FEGE_ERR_INVALID_ARG, 0, 0);
    
    hndl->var.ctxt.state = state;
    
    PMC_RETURN();
}


/*******************************************************************************
*  enet_fege_ctxt_state_transition_request
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks that ENET FEGE state transition is allowed.
*
* INPUTS:
*   *hndl             - pointer to ENET handle instance to be operated on  
*    next_state       - next transition state
*
* OUTPUTS:
*    None.
*
* RETURNS:
*   PMC_SUCCESS if state transition is allowed otherwise a descriptive error
*   code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_fege_ctxt_state_transition_request(enet_fege_handle_t      *hndl,
                                                          enet_fege_state_t   next_state)
{
    enet_fege_state_t   state;
    
    PMC_ENTRY();
    
    state = hndl->var.ctxt.state;    
    
    if (ENET_FEGE_STATE_START == state &&
        ENET_FEGE_STATE_OPERATIONAL == next_state)
    {
        PMC_RETURN(PMC_SUCCESS);
    }
    else if (ENET_FEGE_STATE_START == state &&
             ENET_FEGE_STATE_START == next_state)
    {
        PMC_RETURN(ENET_FEGE_STATE_ALREADY_START);
    }
    
    
    if (ENET_FEGE_STATE_OPERATIONAL == state &&
        ENET_FEGE_STATE_START == next_state)
    {
        PMC_RETURN(PMC_SUCCESS);
    } 
    else if (ENET_FEGE_STATE_OPERATIONAL == state &&
             ENET_FEGE_STATE_OPERATIONAL == next_state)
    {
        PMC_RETURN(ENET_FEGE_STATE_ALREADY_PROVISIONED);
    }

    
    PMC_RETURN(ENET_FEGE_STATE_UNKOWN);
}



/*******************************************************************************
* FUNCTION: enet_fege_int_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes/deinitializes interrupt and status support.
*   
*
* INPUTS:
*   enet_fege_handle  - Pointer to enet_fege handle
*   enable            - Enables/disables interrupt and status support.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR enet_fege_int_init(enet_fege_handle_t    *enet_fege_handle,
                                     BOOL                   enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    enet_fege_field_FEGE_D_E_set(NULL, enet_fege_handle, enable);
    enet_fege_field_MSTATX_D_E_set(NULL, enet_fege_handle, enable);
    enet_fege_field_L1RPP_FEGE_D_E_set(NULL, enet_fege_handle, enable);
    
    enet_fege_field_FEGE_P_E_set(NULL, enet_fege_handle, enable);
    enet_fege_field_MSTATX_P_E_set(NULL, enet_fege_handle, enable);
    enet_fege_field_L1RPP_FEGE_P_E_set(NULL, enet_fege_handle, enable);

    PMC_RETURN(result);
} /* enet_fege_int_init */

/*******************************************************************************
* FUNCTION: enet_fege_mstatx_trigger()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs MSTATX snap and clear transfer of counters to holding registers.
*   Also disables the external LCLK from impacting MSTATX with the LCLK_MODE
*   setting.
*   
*
* INPUTS:
*   enet_fege_handle  - Pointer to enet_fege handle
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
PUBLIC void enet_fege_mstatx_trigger(enet_fege_handle_t    *enet_fege_handle)
{
    PMC_ENTRY();

    mstatx_field_LCLK_MODE_set(NULL, enet_fege_handle, 0);
    mstatx_field_TX_SNAP_CLEAR_set(NULL, enet_fege_handle, 1);
    mstatx_field_RX_SNAP_CLEAR_set(NULL, enet_fege_handle, 1);

    PMC_RETURN();
} /* enet_fege_mstatx_trigger */


/*******************************************************************************
* FUNCTION: enet_fege_mstatx_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs MSTATX snap and clear transfer of counters to holding registers.
*   Also disables the external LCLK from impacting MSTATX with the LCLK_MODE
*   setting.
*   
*
* INPUTS:
*   enet_fege_handle  - Pointer to enet_fege handle
*   count_addr        - Counter address 0 to 64
*   byte              - FALSE: LSB, TRUE: MSB
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32            - MSB or LSB counter data read from device.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 enet_fege_mstatx_retrieve(enet_fege_handle_t    *enet_fege_handle,
                                        UINT32 count_addr, BOOL8 byte)
{
    UINT32 temp = 0;    

    PMC_ENTRY();

    PMC_ASSERT(count_addr < 65, ENET_FEGE_ERR_INVALID_ARG, 0, 0);

    if(byte == FALSE)
    {
        temp = mstatx_field_COUNTL_get(NULL, enet_fege_handle, count_addr);
        temp += (mstatx_field_COUNTM_get(NULL, enet_fege_handle, count_addr)<<16);    
    }
    else
    {
        temp = mstatx_field_COUNTH_get(NULL, enet_fege_handle, count_addr);
    }

    PMC_RETURN(temp);
} /* enet_fege_mstatx_retrieve */

/*
** End of file
*/
