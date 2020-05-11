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
*     This file contains functions for DIGI CBR run time configuration.
*
*   NOTES:
*
*******************************************************************************/

/*
** Include Files
*/
#include "digi_loc.h"

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
** Private Functions
*/


/*
** Public Functions
*/

/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_pmon_dlolb_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures FC1200 PMON RX DLOLB insertion options.
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*    *cfg_ptr          - pointer to configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_dlolb_cfg(digi_handle_t                    *digi_handle,
                                                digi_cbr_client_chnl_t           *chnl_ctxt_ptr,
                                                cbrc_port_dir_t                   dir,
                                                fc1200_pmon_dlolb_cfg_t          *cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_pmon_dlolb_cfg(cbrc_handle,
                                            link,
                                            dir,
                                            cfg_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_pmon_dlolb_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_pmon_dlolb_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves FC1200 PMON RX DLOLB insertion options.
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *cfg_ptr          - pointer to configuration structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_dlolb_cfg_get(digi_handle_t                    *digi_handle,
                                                    digi_cbr_client_chnl_t           *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t                   dir,
                                                    fc1200_pmon_dlolb_cfg_t          *cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_pmon_dlolb_cfg_get(cbrc_handle,
                                                link,
                                                dir,
                                                cfg_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_pmon_dlolb_cfg_get */




/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_pmon_dlolb_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Forces DLOLB.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     enable           - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_dlolb_force(digi_handle_t            *digi_handle,
                                                  digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                  cbrc_port_dir_t           dir,
                                                  BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_pmon_dlolb_force(cbrc_handle,
                                              link,
                                              dir,
                                              enable);
    } 


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_pmon_dlolb_force */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_pmon_dlolb_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Retrieves if DLOLB is forced.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_dlolb_force_get(digi_handle_t            *digi_handle,
                                                      digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                      cbrc_port_dir_t           dir,
                                                      BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_pmon_dlolb_force_get(cbrc_handle,
                                                  link,
                                                  dir,
                                                  enable_ptr);
    } 


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_pmon_dlolb_force_get */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_pmon_invalid_block_mode_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures how blocks with invalid block type encodings are handled when
*    encountered during processing.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - Direction of traffic
*     mode             - configures behaviour when an invalid block is received
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_invalid_block_mode_set(
                                   digi_handle_t                    *digi_handle,
                                   digi_cbr_client_chnl_t           *chnl_ctxt_ptr,
                                   cbrc_port_dir_t                   dir,
                                   fc1200_pmon_invalid_block_mode_t  mode) 
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_pmon_invalid_block_mode_set(cbrc_handle,
                                                         link,
                                                         dir,
                                                         mode);
    } 


    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_pmon_invalid_block_mode_set */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_pmon_invalid_block_mode_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves how blocks with invalid block type encodings are handled when
*   encountered during processing.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - Direction of traffic
*
* OUTPUTS:
*    *mode_ptr        - behaviour when an invalid block is received
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_invalid_block_mode_get(
                                   digi_handle_t                    *digi_handle,
                                   digi_cbr_client_chnl_t           *chnl_ctxt_ptr,
                                   cbrc_port_dir_t                   dir,
                                   fc1200_pmon_invalid_block_mode_t *mode_ptr) 
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_pmon_invalid_block_mode_get(cbrc_handle,
                                                         link,
                                                         dir,
                                                         mode_ptr);
    } 


    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_pmon_invalid_block_mode_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_prbs31_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables the PRBS31 generator and monitor. 
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     enable           - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_prbs31_enable(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_prbs31_enable(cbrc_handle,
                                           link,
                                           dir,
                                           enable);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_prbs31_enable */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_prbs31_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the the PRBS31 generator and monitor is enabled or disabled.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_prbs31_enable_get(digi_handle_t            *digi_handle,
                                                   digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                   cbrc_port_dir_t           dir,
                                                   BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_prbs31_enable_get(cbrc_handle,
                                               link,
                                               dir,
                                               enable_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_prbs31_enable_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_lfos_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Inserts LFOS bits into rx or tx stream. LFOS bits can be configured with
*   digi_cbr_fc1200_lfos_set().
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - Direction of traffic
*     enable           - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_lfos_force(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_port_dir_t           dir,
                                            BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_lfos_force(cbrc_handle,
                                        link,
                                        dir,
                                        enable);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_lfos_force */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_lfos_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if LFOS bits are forced into rx or tx stream. 
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - Direction of traffic
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_lfos_force_get(digi_handle_t            *digi_handle,
                                                digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                cbrc_port_dir_t           dir,
                                                BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_lfos_force_get(cbrc_handle,
                                            link,
                                            dir,
                                            enable_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_lfos_force_get */




/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_lfos_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the Local Fault Ordered Sets (LFOS) bits that can be inserted
*   into the rx or tx stream.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - Direction of traffic
*    *lfos_ptr         - pointer to LFOS bits to set
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_lfos_set(digi_handle_t            *digi_handle,
                                          digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                          cbrc_port_dir_t           dir,
                                          cbrc_fc1200_lfos_t       *lfos_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_lfos_set(cbrc_handle,
                                      link,
                                      dir,
                                      lfos_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_lfos_set */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_lfos_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Local Fault Ordered Sets (LFOS) bits that can be inserted
*   into the rx or tx stream.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - Direction of traffic
*
* OUTPUTS:
*    *lfos_ptr         - pointer to configured LFOS bits
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_lfos_get(digi_handle_t            *digi_handle,
                                          digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                          cbrc_port_dir_t           dir,
                                          cbrc_fc1200_lfos_t       *lfos_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_lfos_get(cbrc_handle,
                                      link,
                                      dir,
                                      lfos_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_lfos_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_lfos_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures LFOS insertion during error conditions in the FC1200 RX stream.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointerq
*    *lfos_cfg_ptr     - pointer to LFOS config structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_lfos_cfg(digi_handle_t                 *digi_handle,
                                             digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                             cbrc_fc1200_rx_lfos_cfg_t     *lfos_cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_lfos_cfg(cbrc_handle,
                                         link,
                                         lfos_cfg_ptr);

    } 


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_lfos_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_lfos_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves LFOS insertion config for error conditions in the FC1200 RX stream.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointerq
*
* OUTPUTS:
*    *lfos_cfg_ptr     - pointer to LFOS config structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_lfos_cfg_get(digi_handle_t                 *digi_handle,
                                                 digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                                 cbrc_fc1200_rx_lfos_cfg_t     *lfos_cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_lfos_cfg_get(cbrc_handle,
                                             link,
                                             lfos_cfg_ptr);

    } 


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_lfos_cfg_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_tx_lfos_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures LFOS insertion during error conditions in the FC1200 TX stream.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointerq
*    *lfos_cfg_ptr     - pointer to LFOS config structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_tx_lfos_cfg(digi_handle_t                 *digi_handle,
                                             digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                             cbrc_fc1200_tx_lfos_cfg_t     *lfos_cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_tx_lfos_cfg(cbrc_handle,
                                         link,
                                         lfos_cfg_ptr);
    } 


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_tx_lfos_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_tx_lfos_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves LFOS insertion config for error conditions in the FC1200 RX stream.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointerq
*
* OUTPUTS:
*    *lfos_cfg_ptr     - pointer to LFOS config structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_tx_lfos_cfg_get(digi_handle_t                 *digi_handle,
                                                 digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                                 cbrc_fc1200_tx_lfos_cfg_t     *lfos_cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);


    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_tx_lfos_cfg_get(cbrc_handle,
                                             link,
                                             lfos_cfg_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_tx_lfos_cfg_get */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_restore_invalid_idle_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls how a 66b block with block type 0x1E, but an
*   unrecognized payload, should be handled when received.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     enable           - Enable: The invalid idle block is restored to a valid
*                                Idle/Idle block.
*                        Disable: The invalid idle block is treated as an
*                                 errored block. 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_restore_invalid_idle_enable(
                                           digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_restore_invalid_idle_enable(cbrc_handle,
                                                            link,
                                                            enable);
    } 
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_restore_invalid_idle_enable */




/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_restore_invalid_idle_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configuration for how a 66b block with block type 0x1E, but an
*   unrecognized payload, should be handled when received.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *enable_ptr       - Enable: The invalid idle block is restored to a valid
*                                Idle/Idle block.
*                        Disable: The invalid idle block is treated as an
*                                 errored block. 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_restore_invalid_idle_enable_get(
                                           digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_restore_invalid_idle_enable_get(cbrc_handle,
                                                                link,
                                                                enable_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_restore_invalid_idle_enable_get */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_descrambler_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the FC1200 RX line descrambler.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     enable           - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_descrambler_enable(digi_handle_t            *digi_handle,
                                                       digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                       BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_descrambler_enable(cbrc_handle,
                                                   link,
                                                   enable);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_descrambler_enable */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_descrambler_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the FC1200 RX line descrambler is enabled/disabled.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_descrambler_enable_get(
                                        digi_handle_t            *digi_handle,
                                        digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                        BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_descrambler_enable_get(cbrc_handle,
                                                       link,
                                                       enable_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_descrambler_enable_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_ecb_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Inserts ECB bits into rx stream. ECB bits can be configured with
*   digi_cbr_fc1200_rx_ecb_set().
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     enable           - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_ecb_force(digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_ecb_force(cbrc_handle,
                                          link,
                                          enable);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_ecb_force */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_ecb_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if ECB bit insertion into rx stream is enabled/disabled.
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_ecb_force_get(digi_handle_t            *digi_handle,
                                                  digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                  BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_ecb_force_get(cbrc_handle,
                                              link,
                                              enable_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_ecb_force_get */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_ecb_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the Error Control Block (ECB) bits 
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*    *ecb_ptr          - pointer to ECB bits to set
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_ecb_set(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_fc1200_ecb_t        *ecb_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_ecb_set(cbrc_handle,
                                        link,
                                        ecb_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_ecb_set */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_ecb_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the Error Control Block (ECB) bits 
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *ecb_ptr          - pointer to ECB bits to set
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_ecb_get(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_fc1200_ecb_t        *ecb_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_ecb_get(cbrc_handle,
                                        link,
                                        ecb_ptr);
    } 




    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_ecb_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_gfp_payload_scramble_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables GFP payload scrambling.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     enable           - 1: The GFP payload is scambled with the x43 + 1
*                           self synchronous scrambler.
*                        0: Scrambling is disabled.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_gfp_payload_scramble_enable(
                                              digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_gfp_payload_scramble_enable(cbrc_handle,
                                                            link,
                                                            enable);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_gfp_payload_scramble_enable */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_gfp_payload_scramble_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if GFP payload scrambling is enabled/disabled.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *enable_ptr       - 1: The GFP payload is scambled with the x43 + 1
*                           self synchronous scrambler.
*                        0: Scrambling is disabled.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_gfp_payload_scramble_enable_get(
                                              digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_gfp_payload_scramble_enable_get(cbrc_handle,
                                                                link,
                                                                enable_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_gfp_payload_scramble_enable_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_gfp_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures GFP core header and payload information bytes
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*    *gfp_ptr          - pointer to GFP bytes for configuration

* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_gfp_set(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_fc1200_rx_gfp_t     *gfp_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_gfp_set(cbrc_handle,
                                        link,
                                        gfp_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_gfp_set */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_rx_gfp_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured GFP core header and payload information bytes
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer

* OUTPUTS:
*    *gfp_ptr          - configured GFP bytes
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_rx_gfp_get(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_fc1200_rx_gfp_t     *gfp_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check Arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_rx_gfp_get(cbrc_handle,
                                        link,
                                        gfp_ptr);
    } 


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_rx_gfp_get */



/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_tx_corrupt_blk_conv_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables converting corrupt blocks to error blocks in the FC1200 
*   TX stream.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     enable           - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_tx_corrupt_blk_conv_enable(
                                           digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);


    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_tx_corrupt_blk_conv_enable(cbrc_handle,
                                                        link,
                                                        enable);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_tx_corrupt_blk_conv_enable */




/*******************************************************************************
* FUNCTION: digi_cbr_fc1200_tx_corrupt_blk_conv_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if converting corrupt blocks to error blocks in the FC1200 
*   TX stream is enabled or disabled.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc1200_tx_corrupt_blk_conv_enable_get(
                                           digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc1200_tx_corrupt_blk_conv_enable_get(cbrc_handle,
                                                            link,
                                                            enable_ptr);
    } /* end if */

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc1200_tx_corrupt_blk_conv_enable_get */


/*
** End of file
*/

