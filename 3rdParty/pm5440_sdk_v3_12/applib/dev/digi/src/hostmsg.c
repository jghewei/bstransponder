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
*   DESCRIPTION :
*       This module contains all generic host messages. Host messages
*       specific to a particular function of the chip should be declared
*       elsewhere.
*
*   NOTES:
*
*******************************************************************************/

/*
** Include Files
*/
#include "hostmsg_loc.h"
#include "digi_loc.h"
/* don't include digi_loc.h in hostmsg_loc.h because it breaks the compile of lineotn_unit_test */

/* use the firmware queue "fw_cmd" and "fw_query" messages to send firmware cmds and queries */
#define L5SYS_USE_MESSAGE_FOR_FW_CMDS

/*******************************************************************************
 *
 * Public stuff.
 *
*******************************************************************************/
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
/*!
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above
 */
const char HOSTMSG_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    HOSTMSG_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  
/*! no strings */
const char HOSTMSG_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*******************************************************************************
 *
 * Private method prototypes.
 *
*******************************************************************************/
PRIVATE PMC_ERROR hostmsg_send_inbound_message(pmc_handle_t *pmc_handle, UINT32 len);
PRIVATE void *hostmsg_data_buf_get(pmc_handle_t *pmc_handle);
PRIVATE void hostmsg_outbound_mailbox_get(pmc_handle_t *pmc_handle, UINT32 off,
                                         UINT32 len);
PRIVATE void hostmsg_create_inbound_message_header(pmc_sys_stat_t *stat,
                                                   UINT32 *data, UINT32 mod_id,
                                                  UINT32 msg_cat, UINT32 msg_len,
                                                  UINT32 msg_subtype,
                                                  UINT32 msg_ctrl, UINT16 seq_num);
PRIVATE PMC_ERROR hostmsg_check_outbound_rsp_header(pmc_sys_stat_t *stat,
                                                    pmc_handle_t *pmc_handle,
                                                   UINT32 mod_id, UINT32 msg_cat,
                                                   UINT32 subtype, UINT16 seq_num,
                                                   UINT32 *rsp_len, UINT32 *rsp_ctrl);

PUBLIC void hostmsg_clear_outbound_doorbell(pmc_handle_t *pmc_handle);

PRIVATE PMC_ERROR hostmsg_data_store_init(hostmsg_handle_t *hostmsg_handle);
PRIVATE void hostmsg_get_outbound_message_header_from_data(pmc_sys_stat_t *stat,
                                                           UINT32 data[2],
                                                UINT32 *mod_id, UINT32 *msg_cat,
                                                UINT32 *msg_len,
                                                UINT32 *msg_subtype,
                                                UINT32 *msg_ctrl, UINT16 *seq_num);
PRIVATE void hostmsg_get_outbound_message_header(pmc_sys_stat_t *stat,
                                                hostmsg_handle_t *hostmsg_handle,
                                                UINT32 *mod_id, UINT32 *msg_cat,
                                                UINT32 *msg_len,
                                                UINT32 *msg_subtype,
                                                UINT32 *msg_ctrl, UINT16 *seq_num);

/*******************************************************************************
 *
 * Private Methods
 *
*******************************************************************************/

/*******************************************************************************
*  hostmsg_data_store_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Populate hostmsg info.
*
* INPUTS:
*   hostmsg_handle - Host message handle.
*
* OUTPUTS:
*   Initialized host message store.
*
* RETURNS:
*   PMC_SUCCESS - Initialization was successful
*   PMC_ERROR   - Otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR hostmsg_data_store_init(hostmsg_handle_t *hostmsg_handle)
{
    UINT32 mbox_buf_size_max = 0;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);    

    /* This is very important in order to varify integrity of hostmsg_handle. */
    hostmsg_handle->magic = HOSTMSG_T_CLASS_MAGIC;

    /* Initialize sequence number. */
    hostmsg_handle->msg_seq_num = 0;

    /* Init mailbox related data. */
    result = hostmsg_mailbox_data_init(hostmsg_handle);

    if (PMC_SUCCESS != result)
    {
        PMC_RETURN(result);
    }

    /* Data buffer for inbound and outbound messages. */
    mbox_buf_size_max = hostmsg_handle->inbound_mailbox_message_size_max;
    if(hostmsg_handle->outbound_mailbox_message_size_max > mbox_buf_size_max)
    {
        mbox_buf_size_max = hostmsg_handle->outbound_mailbox_message_size_max;
    }

#if  defined(PMC_SW_SIMULATION)
    /*
     * If left to SW_SIMULATION devices then the buf_size will be 0 which will
     * result in memory getting overwritten later and crashing when deleting. 
     * Ensure that we have something valid to play with.
     */
    mbox_buf_size_max = 512;
#endif


    /* If PMC_CTXT_CALLOC fails it will assert. So, we do not need to check
       for errors. */
    hostmsg_handle->hostmsg_data_buf = PMC_CTXT_CALLOC(mbox_buf_size_max * sizeof(UINT32), &hostmsg_handle->base);
    PMC_CTXT_REGISTER_PTR(&hostmsg_handle->hostmsg_data_buf, &hostmsg_handle->base);

    /* initialize hostmsg_data_buf to zero to avoid valgrind reporting using uninitialized memory */
    PMC_MEMSET(hostmsg_handle->hostmsg_data_buf,0,mbox_buf_size_max*sizeof(UINT32));

    PMC_RETURN(result);
} /* hostmsg_data_store_init */


/*******************************************************************************
 *
 * Public Methods
 *
*******************************************************************************/

/*******************************************************************************
* hostmsg_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a hostmsg handle.
*
* INPUTS:
*   parent       - Pointer to parent handle. If no parent pass in NULL.
*   base_address - Base address of the CPU subsystem relative to the
*                  device memory space.
*   sys_handle   - Pointer to user system handle.
*   tsb_name     - Unique string identifier of the block.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   hostmsg_handle_t - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC void hostmsg_ctxt_create(pmc_handle_t *parent,
                                UINT32 base_address,
                                pmc_sys_handle_t *sys_handle,
                                const char *tsb_name)
{
    hostmsg_handle_t *hostmsg_handle = NULL;

    PMC_ENTRY();

    /* 'parent' could be NULL, do not assert on it. */
    PMC_ASSERT((NULL != sys_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != tsb_name), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* PMC_CTXT_CALLOC will assert on any error and there is no need for any
       result check. */
    hostmsg_handle = (hostmsg_handle_t*)PMC_CTXT_CALLOC(sizeof(hostmsg_handle_t), parent);

    /* Add handle into pmc_handle. */    
    parent->common_info->hostmsg_handle = hostmsg_handle;

    /* Init handle: save base address, tsb name, etc. mostly system related
       stuff. */
    pmc_handle_init(parent, hostmsg_handle, sys_handle, PMC_MID_DIGI_HOSTMSG,
                    tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(HOSTMSG_LOG_ERR_STRINGS[0],
                                   HOSTMSG_LOG_ERR_TABLE_BASE,
                                   HOSTMSG_LOG_ERR_COUNT);
    
    PMC_RETURN();

} /* hostmsg_ctxt_create */

/*******************************************************************************
* hostmsg_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes host message handle instance.
*
* INPUTS:
*   pmc_handle     - PMC handle
*
* OUTPUTS:
*   Populated hostmsg_handle.
*
* RETURNS:
*   PMC_SUCCESS - Everything initialized as plan
*   PMC_ERROR   - Error happened
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_handle_init(pmc_handle_t *pmc_handle)
{
    hostmsg_handle_t *hostmsg_handle = NULL;
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT((NULL != pmc_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    hostmsg_handle = (hostmsg_handle_t *)pmc_handle->common_info->hostmsg_handle;
    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Get some important system information and init hostmsg. */
    result = hostmsg_data_store_init(hostmsg_handle);

    if (PMC_SUCCESS == result)
    {
        /* Clear IPI2PCIE interrupt. */
        hostmsg_interrupt_ipi2pcie_clear(hostmsg_handle);

        /* Enable IPI2PCIE interrupt. */
        hostmsg_interrupt_ipi2pcie_enable(hostmsg_handle);
    }

    PMC_RETURN(result);
} /* hostmsg_handle_init */

/*******************************************************************************
* hostmsg_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on hostmsg.
*
*
* INPUTS:
*   *pmc_handle            - pointer to PMC handle
*   *restart_init_cfg_ptr  - pointer to restart init config
*   top_energy_state_reg   - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     reinit was successful
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_handle_restart_init(pmc_handle_t *pmc_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                             pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT((NULL != pmc_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(HOSTMSG_LOG_ERR_STRINGS[0], HOSTMSG_LOG_ERR_TABLE_BASE, HOSTMSG_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* hostmsg_handle_restart_init */


/*******************************************************************************
* hostmsg_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deallocate and reset memory associated with hostmsg store.
*
* INPUTS:
*   pmc_handle - pmc handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   This must occur before pmc_sys_handle is destroyed.
*
*******************************************************************************/
PUBLIC void hostmsg_ctxt_destroy(pmc_handle_t *pmc_handle)
{
    hostmsg_handle_t *hostmsg_handle = NULL;
    PMC_ENTRY();

    PMC_ASSERT((NULL != pmc_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    hostmsg_handle = (hostmsg_handle_t *)pmc_handle->common_info->hostmsg_handle;
    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Reset it all. Do not destroy magic number, it is strictly for buffer
       identification. */
    hostmsg_handle->msg_seq_num = 0;
    hostmsg_handle->inbound_mailbox_base_addr = 0;
    hostmsg_handle->outbound_mailbox_base_addr = 0;
    hostmsg_handle->inbound_mailbox_size = 0;
    hostmsg_handle->outbound_mailbox_size = 0;
    hostmsg_handle->inbound_mailbox_messages_max = 0;
    hostmsg_handle->outbound_mailbox_messages_max = 0;
    hostmsg_handle->inbound_mailbox_message_size_max = 0;
    hostmsg_handle->outbound_mailbox_message_size_max = 0;
    PMC_CTXT_FREE(&(hostmsg_handle->hostmsg_data_buf), pmc_handle);

    PMC_CTXT_FREE(&hostmsg_handle, pmc_handle);

    PMC_RETURN();
} /* hostmsg_ctxt_destroy */



/*******************************************************************************
* hostmsg_create_inbound_message_header
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function formats the message header and returns it to the caller.
*
* INPUTS:
*   stat        - statistic record for gathering timing information and debug messages
*   mod_id      - FW module ID
*   msg_cat     - message category
*   msg_len     - message length
*   msg_subtype - message subtype
*   msg_ctrl    - large message control
*   seq_num     - message sequence number
*
* OUTPUTS:
*   data - both header words in data store.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void hostmsg_create_inbound_message_header(pmc_sys_stat_t *stat, UINT32 *data, UINT32 mod_id,
                                                  UINT32 msg_cat, UINT32 msg_len,
                                                  UINT32 msg_subtype,
                                                  UINT32 msg_ctrl,
                                                  UINT16 seq_num)
{
    PMC_ENTRY();
    const char *description = (stat == NULL)?"":stat->description;
    const char *description2 = (stat == NULL)?"":stat->description2;
    

    PMC_ASSERT(NULL != data, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN();
#endif
    PMC_LOG(PMC_LOG_SEV_LOW, HOSTMSG_LOG_CODE_LOG, 0, 0,
            "%s%s mod_id=0x%x, msg_cat=0x%x, msg_len=0x%x, msg_subtype=0x%x, msg_ctrl=0x%x, seq_num=0x%x,\n",
            description, description2, mod_id, msg_cat, msg_len, msg_subtype, msg_ctrl, seq_num);

    /* assemble header (word 1 of 2) */
    *data = 0x00;
    *data = mSET_FIELD4(*data, mod_id, HOSTMSG_MODID_MSK, HOSTMSG_MODID_OFF);
    *data = mSET_FIELD4(*data, msg_cat,
                        HOSTMSG_MSGCAT_MSK, HOSTMSG_MSGCAT_OFF);
    *data = mSET_FIELD4(*data, msg_len,
                        HOSTMSG_MSGLEN_MSK, HOSTMSG_MSGLEN_OFF);
    data++;

    /* assemble header (word 2 of 2) */
    *data = 0x00;
    *data = mSET_FIELD4(*data, msg_subtype,
                        HOSTMSG_MSGSUBTYP_MSK, HOSTMSG_MSGSUBTYP_OFF);
    *data = mSET_FIELD4(*data, msg_ctrl,
                        HOSTMSG_MSGCTRL_MSK, HOSTMSG_MSGCTRL_OFF);
    *data = mSET_FIELD4(*data, seq_num,
                        HOSTMSG_MSGSEQNUM_MSK, HOSTMSG_MSGSEQNUM_OFF);

    PMC_RETURN();

} /* hostmsg_create_inbound_message_header */


/*******************************************************************************
* hostmsg_send_inbound_message
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function writes a message to the inbound mailbox and notifies FW to
*   retrieve the message by asserting inbound doorbell 0.
*
* INPUTS:
*   pmc_handle - System handle.
*   len        - data len (in words)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_send_inbound_message(pmc_handle_t *pmc_handle,
                                              UINT32 len)
{
    PMC_ERROR result = PMC_SUCCESS;
    hostmsg_handle_t *hostmsg_handle = NULL;

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(result);
#endif
    /* Get hostmsg structure. */    
    hostmsg_handle = (hostmsg_handle_t *)pmc_handle->common_info->hostmsg_handle;
    PMC_ASSERT((NULL != hostmsg_handle),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(IS_HOSTMSG_PTR_MAGIC(hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != hostmsg_handle->hostmsg_data_buf),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((len >= HOSTMSG_NUM_HEADER_WORDS),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((len <= hostmsg_handle->inbound_mailbox_message_size_max),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Make sure inbound doorbell is cleared. */
    result =  hostmsg_check_fw_ready_to_receive_inbound_message(hostmsg_handle);

    if (PMC_SUCCESS == result)
    {

        /* Write data which must be in hostmsg_handle->hostmsg_data_buf . */
        hostmsg_write(hostmsg_handle,
                      HOSTMSG_IBND_MAILBOX_ADDR(hostmsg_handle),                      
                      len, (UINT32*)hostmsg_handle->hostmsg_data_buf);


        /* Trigger inbound message sending (notify fw that inbound message
           is pending. */
        hostmsg_inbound_doorbell_register_set(hostmsg_handle);
    }
       
    PMC_RETURN(result);

} /* hostmsg_send_inbound_message */


/*******************************************************************************
* hostmsg_clear_outbound_doorbell
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clears outbound doorbell.
*
* INPUTS:
*   pmc_handle - System handle.
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
PUBLIC void hostmsg_clear_outbound_doorbell(pmc_handle_t *pmc_handle)
{
    hostmsg_handle_t *hostmsg_handle = NULL;

    PMC_ENTRY();

    PMC_ASSERT((NULL != pmc_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    hostmsg_handle = (hostmsg_handle_t *)pmc_handle->common_info->hostmsg_handle;
    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    hostmsg_interrupt_ipi2pcie_clear(hostmsg_handle);


    PMC_RETURN();
} /* hostmsg_clear_outbound_doorbell */


/*******************************************************************************
* hostmsg_check_fw_ready_to_receive_inbound_message
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks if firmware is ready to receive any messages.
*
* INPUTS:
*   hostmsg_handle - Host message handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if everything went OK, number of different error codes
*   otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_check_fw_ready_to_receive_inbound_message(hostmsg_handle_t *hostmsg_handle)
{
    PMC_POLL_RETURN_TYPE poll_result;
    UINT32 doorbell_status_expect = 1; /* Expected status of inbound doorbell. */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* poll once with a wait of 0 between polls,... this is the same as a "check that the field is the expected value' */
    poll_result = int_agr_field_IPI_BITS_poll( NULL, (pmc_handle_t *)hostmsg_handle, 0, doorbell_status_expect, PMC_POLL_EQUAL, 1, NULL, 0 );
    if (poll_result != PMC_SUCCESS) {
        result = HOSTMSG_ERR_INVALID_DATA;
    }

    PMC_RETURN(result);
} /* hostmsg_check_fw_ready_to_receive_inbound_message */


/*******************************************************************************
*  hostmsg_check_outbound_rsp_header_from_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the message header from the outbound mailbox and
*   confirms if it is a RSP_REQ message type. Header checks include FW module
*   ID, category, subtype and sequence number.
*
* INPUTS:
*   hostmsg_handle - host message handle
*   stat           - statistic for recording timing information
*   data           - first 2 header words
*   response       - pointer to response (first word is the response code for NAK messages)
*   mod_id         - expected FW module ID
*   msg_cat        - expected message category.
*   subtype        - expected subtype
*   seq_num        - expected sequence number
*   rsp_len        - pointer to storage for response message's length field.
*   rsp_ctrl       - pointer to storage for response message's control field. \n
*                    If the response from FW is NACK, this will return the data_ptr \n
*                    message for more information from FW.
*
* OUTPUTS:
*   rsp_len  - Response length.
*   rsp_ctrl - Response control field.
*
* RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise. If
*   return is PMC_SUCCESS, rsp_len and rsp_ctrl are not valid.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR hostmsg_check_outbound_rsp_header_from_data(hostmsg_handle_t *hostmsg_handle,
                                                              pmc_sys_stat_t *stat,
                                                              UINT32 data[2],UINT32 *response,
                                                              UINT32 mod_id, UINT32 msg_cat,
                                                              UINT32 subtype, UINT16 seq_num,
                                                              UINT32 *rsp_len, UINT32 *rsp_ctrl) UNUSED;
PRIVATE PMC_ERROR hostmsg_check_outbound_rsp_header_from_data(hostmsg_handle_t *hostmsg_handle,
                                                              pmc_sys_stat_t *stat,
                                                              UINT32 data[2],UINT32 *response,
                                                              UINT32 mod_id, UINT32 msg_cat,
                                                              UINT32 subtype, UINT16 seq_num,
                                                              UINT32 *rsp_len, UINT32 *rsp_ctrl)
{
    UINT32 rsp_mod_id = 0;
    UINT32 rsp_msg_cat = 0;
    UINT32 rsp_msg_len = 0;
    UINT32 rsp_msg_subtype = 0;
    UINT32 rsp_msg_ctrl = 0;
    UINT16 rsp_seq_num = 0;
    PMC_ERROR result = PMC_SUCCESS;
    const char *description = (stat == NULL)?"":stat->description;
    const char *description2 = (stat == NULL)?"":stat->description2;

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != rsp_len), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(mod_id <= MAX_MODID, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(result);
#endif

    if (PMC_SUCCESS == result)
    {
        /* read outbound message header info */
        hostmsg_get_outbound_message_header_from_data(stat,
                                                      data, &rsp_mod_id,
                                                      &rsp_msg_cat, &rsp_msg_len,
                                                      &rsp_msg_subtype,
                                                      &rsp_msg_ctrl,
                                                      &rsp_seq_num);

        /* check FW module ID (word 0, byte 0). */
        if (rsp_mod_id != mod_id)
        {
            PMC_LOG(PMC_LOG_SEV_HIGHEST, HOSTMSG_ERR_MOD_ID_MISMATCH,
                    rsp_mod_id, mod_id,
                    "ERROR: %s%s rsp_mod_id=0x%x not equal expected mod_id=0x%x\n",
                    description, description2, rsp_mod_id, mod_id);
            result = HOSTMSG_FAILURE;
            l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_MOD_ID_MISMATCH",description2);
        }
    }

    if (PMC_SUCCESS == result)
    {
        /* check message category (word 0, byte 1). */        
        if ( (rsp_msg_cat != msg_cat) && !((mod_id == MODID_PMON) && (subtype == HOSTMSG_CMD_PMON_REQ_DDEG_PARAM_ST)) )
        {
            if (rsp_msg_cat == CATEGORY_RSP_NACK)
            {
                result = HOSTMSG_ERR_RSP_NACK;

                PMC_LOG(PMC_LOG_SEV_HIGH, HOSTMSG_ERR_RSP_NACK, rsp_mod_id, *data,
                        "NACK: %s%s mod_id = 0x%x, subtype = 0x%x, data[2] = 0x%x\n",
                        description, description2, rsp_mod_id, rsp_msg_subtype, *response);
                if (rsp_ctrl) {
                    *rsp_ctrl = *response;
                } else {
                    l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_RSP_NACK",(char *)description2);
                }
            }
            else
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, HOSTMSG_ERR_MSG_CAT_MISMATCH,
                        rsp_msg_cat, msg_cat,
                        "ERROR: %s%s rsp_msg_cat=0x%x not equal expected msg_cat=0x%x\n",
                        description, description2, rsp_msg_cat, msg_cat);
                result = HOSTMSG_FAILURE;
                l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_MSG_CAT_MISMATCH",(char *)description2);
            }
        }
    }

    if (PMC_SUCCESS == result)
    {
        /* check message subtype (word 1, byte 0) */
        if (rsp_msg_subtype != subtype)
        {
            PMC_LOG(PMC_LOG_SEV_HIGHEST, HOSTMSG_ERR_MSG_SUBTYPE_MISMATCH,
                    rsp_msg_subtype, subtype,
                    "ERROR: %s%s rsp_msg_subtype=0x%x not equal expected subtype=0x%x\n",
                    description, description2, rsp_msg_subtype, subtype);
            l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_MSG_SUBTYPE_MISMATCH",(char *)description2);
            result = HOSTMSG_FAILURE;
        }
    }

    if (PMC_SUCCESS == result)
    {
        /* check sequence number (word 1, bytes 2 and 3). */
        if (rsp_seq_num != seq_num)
        {
            PMC_LOG(PMC_LOG_SEV_HIGHEST, HOSTMSG_ERR_SEQ_NUM_MISMATCH,
                    rsp_seq_num, seq_num,
                    "ERROR: %s%s rsp_seq_num=0x%x not equal expected seq_num=0x%x\n",
                    description, description2, rsp_seq_num, seq_num);
            l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_SEQ_NUM_MISMATCH",(char *)description2);
            result = HOSTMSG_FAILURE;
        }
    }

    /* Return message length and control fields. They are not valid if result
       is not SUCCESS. */
    if (PMC_SUCCESS == result)
    {
        if (rsp_msg_len >= HOSTMSG_NUM_HEADER_WORDS)
        {        
            *rsp_len = rsp_msg_len;
            if (rsp_ctrl) {
                *rsp_ctrl = rsp_msg_ctrl;
            }
        }
        else
        {
            l5sys_display_queue(hostmsg_handle,"HOSTMSG_SHORT_REPLY",(char *)description2);
            result = HOSTMSG_FAILURE;
        }
    }


    PMC_RETURN(result);

} /* hostmsg_check_outbound_rsp_header */

/*******************************************************************************
*  hostmsg_check_outbound_rsp_header
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the message header from the outbound mailbox and
*   confirms if it is a RSP_REQ message type. Header checks include FW module
*   ID, category, subtype and sequence number.
*
* INPUTS:
*   stat           - statistic for timing information
*   pmc_handle     - PMC handle
*   mod_id         - expected FW module ID
*   msg_cat        - expected message category.
*   subtype        - expected subtype
*   seq_num        - expected sequence number
*   rsp_len        - pointer to storage for response message's length field.
*   rsp_ctrl       - pointer to storage for response message's control field.
*                    If the response from FW is NACK, this will return the data_ptr
*                    message for more information from FW. 
*
* OUTPUTS:
*   rsp_len  - Response length.
*   rsp_ctrl - Response control field.
*
* RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise. If
*   return is PMC_SUCCESS, rsp_len and rsp_ctrl are not valid.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR hostmsg_check_outbound_rsp_header(pmc_sys_stat_t *stat,
                                                    pmc_handle_t *pmc_handle,
                                                   UINT32 mod_id, UINT32 msg_cat,
                                                   UINT32 subtype, UINT16 seq_num,
                                                   UINT32 *rsp_len, UINT32 *rsp_ctrl)
{
    UINT32 rsp_mod_id = 0;
    UINT32 rsp_msg_cat = 0;
    UINT32 rsp_msg_len = 0;
    UINT32 rsp_msg_subtype = 0;
    UINT32 rsp_msg_ctrl = 0;
    UINT16 rsp_seq_num = 0;
    UINT32 *data_ptr = NULL;
    hostmsg_handle_t *hostmsg_handle = NULL;
    PMC_ERROR result = PMC_SUCCESS;
    const char *description = (stat == NULL)?"":stat->description;
    const char *description2 = (stat == NULL)?"":stat->description2;

    PMC_SYS_STAT_DECLARE( gbl_driver_doorbell_waits, "driver doorbell waits" );

    PMC_ENTRY();
    
    PMC_ASSERT((NULL != pmc_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    hostmsg_handle = (hostmsg_handle_t *)pmc_handle->common_info->hostmsg_handle;
    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != rsp_len), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(mod_id <= MAX_MODID, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(result);
#endif


    pmc_sys_stat_mark_start(gbl_driver_doorbell_waits);
    /* Poll for outbound doorbell interrupt. Poll time is in ms. */
    result = hostmsg_poll_outbound_doorbell_int(hostmsg_handle,
                                                HOSTMSG_DOORBELL_POLL_TIME);

    if (PMC_SUCCESS == result)
    {
        /* read outbound message header info */
        hostmsg_get_outbound_message_header(stat, hostmsg_handle, &rsp_mod_id,
                                            &rsp_msg_cat, &rsp_msg_len,
                                            &rsp_msg_subtype,
                                            &rsp_msg_ctrl,
                                            &rsp_seq_num);

        /* check FW module ID (word 0, byte 0). */
        if (rsp_mod_id != mod_id)
        {
            PMC_LOG(PMC_LOG_SEV_HIGHEST, HOSTMSG_ERR_MOD_ID_MISMATCH,
                    rsp_mod_id, mod_id,
                    "ERROR: %s%s rsp_mod_id=0x%x not equal expected mod_id=0x%x\n",
                    description, description2, rsp_mod_id, mod_id);
            /* show queue stall information */
            l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_MOD_ID_MISMATCH",(char *)description2);
            result = HOSTMSG_FAILURE;
        }
    }

    if (PMC_SUCCESS == result)
    {
        /* check message category (word 0, byte 1). */
        if (rsp_msg_cat != msg_cat)
        {
            if (rsp_msg_cat == CATEGORY_RSP_NACK)
            {
                result = HOSTMSG_ERR_RSP_NACK;

                /* Get error code from FW */
                data_ptr = (UINT32 *)hostmsg_data_buf_get(pmc_handle);    
                /* data_ptr will contain the data once this method returns. */
                hostmsg_outbound_mailbox_get(pmc_handle,
                                             (sizeof(UINT32) * HOSTMSG_NUM_HEADER_WORDS),
                                             rsp_msg_len - HOSTMSG_NUM_HEADER_WORDS);
                PMC_LOG(PMC_LOG_SEV_HIGH, HOSTMSG_ERR_RSP_NACK, rsp_mod_id, *data_ptr,
                        "NACK: %s%s mod_id = 0x%x, subtype = 0x%x, data[2] = 0x%x\n",
                        description, description2, rsp_mod_id, rsp_msg_subtype, *data_ptr);
                if (rsp_ctrl) {
                    *rsp_ctrl = *data_ptr;
                } else {
                    l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_RSP_NACK",(char *)description2);
                }
            }
            else
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, HOSTMSG_ERR_MSG_CAT_MISMATCH,
                        rsp_msg_cat, msg_cat,
                        "ERROR: %s%s rsp_msg_cat=0x%x not equal expected msg_cat=0x%x\n",
                        description, description2, rsp_msg_cat, msg_cat);
                l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_MSG_CAT_MISMATCH",(char *)description2);
                result = HOSTMSG_FAILURE;
            }
        }
    }

    if (PMC_SUCCESS == result)
    {
        /* check message subtype (word 1, byte 0) */
        if (rsp_msg_subtype != subtype)
        {
            PMC_LOG(PMC_LOG_SEV_HIGHEST, HOSTMSG_ERR_MSG_SUBTYPE_MISMATCH,
                    rsp_msg_subtype, subtype,
                    "ERROR: %s%s rsp_msg_subtype=0x%x not equal expected subtype=0x%x\n",
                    description, description2, rsp_msg_subtype, subtype);
            l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_MSG_SUBTYPE_MISMATCH",(char *)description2);
            result = HOSTMSG_FAILURE;
        }
    }

    if (PMC_SUCCESS == result)
    {
        /* check sequence number (word 1, bytes 2 and 3). */
        if (rsp_seq_num != seq_num)
        {
            PMC_LOG(PMC_LOG_SEV_HIGHEST, HOSTMSG_ERR_SEQ_NUM_MISMATCH,
                    rsp_seq_num, seq_num,
                    "ERROR: %s%s rsp_seq_num=0x%x not equal expected seq_num=0x%x\n",
                    description, description2, rsp_seq_num, seq_num);
            l5sys_display_queue(hostmsg_handle,"HOSTMSG_ERR_SEQ_NUM_MISMATCH",(char *)description2);
            result = HOSTMSG_FAILURE;
        }
    }

    /* Return message length and control fields. They are not valid if result
       is not SUCCESS. */
    if (PMC_SUCCESS == result)
    {
        if (rsp_msg_len >= HOSTMSG_NUM_HEADER_WORDS)
        {        
            *rsp_len = rsp_msg_len;
            if (rsp_ctrl) {
                *rsp_ctrl = rsp_msg_ctrl;
            }
        }
        else
        {
            l5sys_display_queue(hostmsg_handle,"HOSTMSG_SHORT_REPLY",(char *)description2);
            result = HOSTMSG_FAILURE;
        }
    }
    pmc_sys_stat_mark_end(gbl_driver_doorbell_waits);

    PMC_RETURN(result);

} /* hostmsg_check_outbound_rsp_header */


/*******************************************************************************
*  hostmsg_poll_outbound_doorbell_int
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function polls for outbound doorbell interrupt 0.
*
* INPUTS:
*   hostmsg_handle - Host message handle.
*   num_attempts   - Numbed of polling attempts.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
* NOTES:
*   This method will sleep 1ms between attempts.
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_poll_outbound_doorbell_int(hostmsg_handle_t *hostmsg_handle,
                                                    UINT32 num_attempts)
{
    PMC_ERROR result = HOSTMSG_ERR_TIMEOUT;

    PMC_ENTRY();

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((0 != num_attempts), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);




#ifdef DISABLE_Q_FOR_FW_MESSAGES
    /* investigating race condition in serdes_adaptation start,... for now we won't use the firmware queue for sending host messages */
    l5sys_firmware_queue_disable(hostmsg_handle);
#endif

    result = int_agr_field_IPI2PCIE_0_I0_poll(NULL, (pmc_handle_t *)hostmsg_handle,1,PMC_POLL_EQUAL,num_attempts*1000,NULL,1);
    if (PMC_SUCCESS != result) {
        l5sys_display_queue(hostmsg_handle,"TIMEOUT WAITING FOR DOORBELL",__FUNCTION__);
    }

#ifdef DISABLE_Q_FOR_FW_MESSAGES
    l5sys_firmware_queue_enable(hostmsg_handle);
#endif


    

    PMC_RETURN(result);

} /* hostmsg_poll_outbound_doorbell_int */


/*******************************************************************************
*  hostmsg_check_outbound_doorbell_int
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks if outbound doorbell interrupt 0 is set.

*
* INPUTS:
*   hostmsg_handle - Host message handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if doorbell is set, false otherwise.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC BOOL hostmsg_check_outbound_doorbell_int(hostmsg_handle_t *hostmsg_handle)
{
    BOOL result = TRUE;
    UINT32 data = 0;
    UINT32 expected_status = 1;
    UINT32 status = expected_status;


    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    data = hostmsg_outbound_doorbell_int_register_get(hostmsg_handle);
    status = (data >> HOSTMSG_OUTBOUND_DOORBELL_NUM) & 0x01;
    if (status != expected_status)
    {
        result = FALSE;
    }

    return(result);
} /* hostmsg_check_outbound_doorbell_int */


/*******************************************************************************
*  hostmsg_get_outbound_message_header_from_data
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads a message header from the outbound mailbox.
*
* INPUTS:
*   data - first two message header words
*   stat - statistic record used for gathering timing information and showing debug messages.
*
* OUTPUTS:
*   mod_id      - pointer to storage for FW module ID
*   msg_cat     - pointer to storage for message category
*   msg_len     - pointer to storage for message length
*   msg_subtype - pointer to storage for message subtype
*   msg_ctrl    - pointer to storage for large message control
*   seq_num     - pointer to storage for message sequence number
*
* RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void hostmsg_get_outbound_message_header_from_data(pmc_sys_stat_t *stat,
                                                           UINT32 data[2],
                                                UINT32 *mod_id, UINT32 *msg_cat,
                                                UINT32 *msg_len,
                                                UINT32 *msg_subtype,
                                                UINT32 *msg_ctrl, UINT16 *seq_num)
{
    const char *description = (stat == NULL)?"":stat->description;
    const char *description2 = (stat == NULL)?"":stat->description2;

    PMC_ENTRY();

    PMC_ASSERT((NULL != mod_id), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != msg_cat), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != msg_len), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != msg_subtype), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != msg_ctrl), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != seq_num), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG(PMC_LOG_SEV_LOW, HOSTMSG_LOG_CODE_LOG, 0, 0,
            "data[0] = %x, data[1] = %x\n",
            data[0], data[1]);

    /* parse header fields (word 1 of 2) */
    *mod_id  = (data[0] & HOSTMSG_MODID_MSK) >> HOSTMSG_MODID_OFF;
    *msg_cat = (data[0] & HOSTMSG_MSGCAT_MSK) >> HOSTMSG_MSGCAT_OFF;
    *msg_len = (data[0] & HOSTMSG_MSGLEN_MSK) >> HOSTMSG_MSGLEN_OFF;

    /* parse header fields (word 2 of 2) */
    *msg_subtype = (data[1] & HOSTMSG_MSGSUBTYP_MSK) >> HOSTMSG_MSGSUBTYP_OFF;
    *msg_ctrl    = (data[1] & HOSTMSG_MSGCTRL_MSK) >> HOSTMSG_MSGCTRL_OFF;
    *seq_num     = (data[1] & HOSTMSG_MSGSEQNUM_MSK) >> HOSTMSG_MSGSEQNUM_OFF;

    PMC_LOG(PMC_LOG_SEV_LOW, HOSTMSG_LOG_CODE_LOG, 0, 0,
            "%s%s mod_id=0x%x, msg_cat=0x%x, msg_len=%d, msg_subtype=0x%x,"
            "msg_ctrl=0x%x, seq_num=%d\n", description, description2, *mod_id, *msg_cat, *msg_len,
            *msg_subtype, *msg_ctrl, *seq_num);

    PMC_RETURN();

} /* hostmsg_get_outbound_message_header_from_data */


/*******************************************************************************
*  hostmsg_get_outbound_message_header
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads a message header from the outbound mailbox.
*
* INPUTS:
*   stat - statistic record used for gathering timing information and showing debug messages.
*   hostmsg_handle - Host message handle.
*
* OUTPUTS:
*   mod_id      - pointer to storage for FW module ID
*   msg_cat     - pointer to storage for message category
*   msg_len     - pointer to storage for message length
*   msg_subtype - pointer to storage for message subtype
*   msg_ctrl    - pointer to storage for large message control
*   seq_num     - pointer to storage for message sequence number
*
* RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void hostmsg_get_outbound_message_header(pmc_sys_stat_t *stat,
                                                hostmsg_handle_t *hostmsg_handle,
                                                UINT32 *mod_id, UINT32 *msg_cat,
                                                UINT32 *msg_len,
                                                UINT32 *msg_subtype,
                                                UINT32 *msg_ctrl, UINT16 *seq_num)
{
    const char *description = (stat == NULL)?"":stat->description;
    const char *description2 = (stat == NULL)?"":stat->description2;
    UINT32 data[2] = {0,0};

    PMC_ENTRY();

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != mod_id), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != msg_cat), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != msg_len), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != msg_subtype), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != msg_ctrl), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != seq_num), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Read header (first two words) from outbound mailbox. */
    hostmsg_read(hostmsg_handle, HOSTMSG_OBND_MAILBOX_ADDR(hostmsg_handle),
                 2, &data[0]);

    PMC_LOG(PMC_LOG_SEV_LOW, HOSTMSG_LOG_CODE_LOG, 0, 0,
            "%s%s Address = %x, data[0] = %x, data[1] = %x\n",
            description,description2,HOSTMSG_OBND_MAILBOX_ADDR(hostmsg_handle), data[0], data[1]);

    /* parse header fields (word 1 of 2) */
    *mod_id  = (data[0] & HOSTMSG_MODID_MSK) >> HOSTMSG_MODID_OFF;
    *msg_cat = (data[0] & HOSTMSG_MSGCAT_MSK) >> HOSTMSG_MSGCAT_OFF;
    *msg_len = (data[0] & HOSTMSG_MSGLEN_MSK) >> HOSTMSG_MSGLEN_OFF;

    /* parse header fields (word 2 of 2) */
    *msg_subtype = (data[1] & HOSTMSG_MSGSUBTYP_MSK) >> HOSTMSG_MSGSUBTYP_OFF;
    *msg_ctrl    = (data[1] & HOSTMSG_MSGCTRL_MSK) >> HOSTMSG_MSGCTRL_OFF;
    *seq_num     = (data[1] & HOSTMSG_MSGSEQNUM_MSK) >> HOSTMSG_MSGSEQNUM_OFF;

    PMC_LOG(PMC_LOG_SEV_LOW, HOSTMSG_LOG_CODE_LOG, 0, 0,
            "%s%s mod_id=0x%x, msg_cat=0x%x, msg_len=%d, msg_subtype=0x%x,"
            "msg_ctrl=0x%x, seq_num=%d\n", description, description2, *mod_id, *msg_cat, *msg_len,
            *msg_subtype, *msg_ctrl, *seq_num);

    PMC_RETURN();

} /* hostmsg_get_outbound_message_header */

/*******************************************************************************
*  hostmsg_data_buf_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns hostmsg_handle data pointer.
*
* INPUTS:
*   pmc_handle - pmc handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   hostmsg data buffer extraced from pmc_handle.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void *hostmsg_data_buf_get(pmc_handle_t *pmc_handle)
{
    void *buf = NULL;
    hostmsg_handle_t * hostmsg_handle;

    PMC_ENTRY();

    PMC_ASSERT((NULL != pmc_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    hostmsg_handle = (hostmsg_handle_t *)(pmc_handle->common_info->hostmsg_handle);
    buf = hostmsg_handle->hostmsg_data_buf;
    PMC_ASSERT((NULL != buf), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_RETURN(buf);
}


/*******************************************************************************
*  hostmsg_outbound_mailbox_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read the content of outbound mailbox.
*
* INPUTS:
*   pmc_handle - system handle.
*   off - offset from the outbound mailbox start.
*   len - length of the data to be read.
*
* OUTPUTS:
*   Data in pmc_handle->common_info->hostmsg_handle->hostmsg_data_buf .
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void hostmsg_outbound_mailbox_get(pmc_handle_t *pmc_handle, UINT32 off,
                                         UINT32 len)
{
    UINT32 addr = 0;
    void *buf = NULL;
    hostmsg_handle_t *hostmsg_handle = NULL;

    PMC_ENTRY();

    PMC_ASSERT((NULL != pmc_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    hostmsg_handle = (hostmsg_handle_t *)pmc_handle->common_info->hostmsg_handle;

    buf = hostmsg_handle->hostmsg_data_buf;
    PMC_ASSERT((NULL != buf), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    addr = hostmsg_handle->outbound_mailbox_base_addr + off;
    hostmsg_read(hostmsg_handle, addr, len, (UINT32*)buf);    

    PMC_RETURN();
}

/*******************************************************************************
* hostmsg_send_cfg_message_internal
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Send a basic configuration message to FW
*
* INPUTS:
*   pmc_handle  - any pmc handle
*   mod_id      - module id
*   msg_subtype - message sub type
*   cfg_msg     - pointer to configuration message
*   len         - len in UINT32 words of the configuration message
*   start - TRUE: start the message and don't wait for completion.\n
*           FALSE: wait for the command to complete and check the response.
*   stat - statistic record for gathering timing information.
*
* OUTPUTS:
*   msg_ctrl_ptr - returned msg_ctrl value.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_send_cfg_message_internal(pmc_handle_t *pmc_handle,
                                                   UINT32 mod_id,
                                                   UINT32 msg_subtype,
                                                   UINT32 *cfg_msg,
                                                   UINT32 len,
                                                   UINT32 *msg_ctrl_ptr,
                                                   BOOL start,
                                                   pmc_sys_stat_t *stat)
{
    UINT32 *data_ptr = NULL;
    UINT32 inbound_seq_num = 0;
    PMC_ERROR result = PMC_SUCCESS;    

    /* Only used for hostmsg_check_outbound_rsp_header function. */
    UINT32 outbound_msg_len = 0;
    hostmsg_handle_t *hostmsg_handle = (hostmsg_handle_t *)pmc_handle->common_info->hostmsg_handle;
    digi_handle_t *digi_handle = (digi_handle_t *)hostmsg_handle->base.parent_handle;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif

    PMC_ASSERT(NULL != pmc_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

	if (digi_handle->digi_fw_status.fw_version_not_supported)
	{

		PMC_LOG(PMC_LOG_SEV_HIGHEST, DIGI_SW_FW_DEPENDANCY, 
				digi_handle->digi_fw_status.fw_build, SW_FW_RECENT_DEPENDANCY_SVN_REVISION);
	}
    /* initialize returned value of *msg_ctrl_ptr */
    if (msg_ctrl_ptr != NULL) {
        *msg_ctrl_ptr = 0;
    }

    if (stat!=NULL) {
        digi_fw_stats->cur_hostmsg_stat = stat;
        pmc_sys_stat_mark_start(stat);
    }

    data_ptr = (UINT32 *)hostmsg_data_buf_get(pmc_handle);

    inbound_seq_num = hostmsg_handle->msg_seq_num++;

    /* Assemble message header. */
    hostmsg_create_inbound_message_header(stat,
                                          data_ptr,
                                          mod_id,
                                          CATEGORY_CMD_CFG,
                                          len+HOSTMSG_NUM_HEADER_WORDS,
                                          msg_subtype,
                                          BASIC_MSG,
                                          inbound_seq_num);

    /* Add message body to the previously created header. */
    if (len > 0 && (cfg_msg != NULL)) {
        memcpy(data_ptr + HOSTMSG_NUM_HEADER_WORDS,cfg_msg,len*sizeof(UINT32));
    }
#ifdef L5SYS_USE_MESSAGE_FOR_FW_CMDS
    if (l5sys_queue_enabled(pmc_handle)) {

        if (start) {
            result = l5sys_fw_send_cfg_message(pmc_handle,data_ptr,len+HOSTMSG_NUM_HEADER_WORDS);
        }
        else {
            UINT32 header[2];
            UINT32 response[1];
            result = l5sys_fw_perform_query(pmc_handle,data_ptr,len+HOSTMSG_NUM_HEADER_WORDS,response,1,header);

            if (PMC_SUCCESS == result) {
                /* check response message header */
                result = hostmsg_check_outbound_rsp_header_from_data(hostmsg_handle,stat,header,response,
                                                           mod_id,
                                                           CATEGORY_RSP_CFG,
                                                           msg_subtype,
                                                           inbound_seq_num,
                                                           &outbound_msg_len,
                                                           msg_ctrl_ptr);
            }
        }
#ifdef USE_FW_LOG
    if (digi_fw_firmware_logging_enabled) {
        /* if compiled with USE_FW_LOG, also show the debug log after every firmware message */
        digi_fw_show_debug_log((digi_handle_t *)hostmsg_handle->base.parent_handle);
        /* if debugging opsa mpmo functions, also show mpmo counts after every firmware function
        digi_fw_show_mpmo_counts((digi_handle_t*)hostmsg_handle->base.parent_handle); */
    }
#endif
    }
    else
#endif
    {

        /* Send message. */
        result = hostmsg_send_inbound_message(pmc_handle,
                                              len+HOSTMSG_NUM_HEADER_WORDS);

        if (PMC_SUCCESS == result) {
            /* check response message header */
            result = hostmsg_check_outbound_rsp_header(stat,pmc_handle,
                                                       mod_id,
                                                       CATEGORY_RSP_CFG,
                                                       msg_subtype,
                                                       inbound_seq_num,
                                                       &outbound_msg_len,
                                                       msg_ctrl_ptr);
        }

        /* Signal firmware that we are done reading outbound message. */
        hostmsg_clear_outbound_doorbell(pmc_handle);
    }

#ifdef USE_FW_LOG
    if (digi_fw_firmware_logging_enabled) {
        /* if compiled with USE_FW_LOG, also show the debug log after every firmware message */
        digi_fw_show_debug_log((digi_handle_t *)hostmsg_handle->base.parent_handle);
        /* if debugging opsa mpmo functions, also show mpmo counts after every firmware function
        digi_fw_show_mpmo_counts((digi_handle_t*)hostmsg_handle->base.parent_handle); */
    }
#endif
    if (digi_fw_stats->cur_hostmsg_stat != NULL)
    {
        digi_fw_stats->prev_hostmsg_stat = digi_fw_stats->cur_hostmsg_stat;
        digi_fw_stats->cur_hostmsg_stat = NULL;
    }

    if (stat!=NULL) {
        pmc_sys_stat_mark_end(stat);
    }

    PMC_RETURN(result);
} /* of hostmsg_send_cfg_message_with_error_info */


/*******************************************************************************
* hostmsg_send_cfg_message_with_error_info
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Send a basic configuration message to FW and return the NAK code if a 
*  NAK is returned.
*
*  Does not display any diagnostic information if a NAK is returned.
*
* INPUTS:
*   pmc_handle  - any pmc handle
*   mod_id      - module id
*   msg_subtype - message sub type
*   cfg_msg     - pointer to configuration message
*   len         - len in UINT32 words of the configuration message
*   stat - statistic record for gathering timing information.
*
* OUTPUTS:
*   msg_ctrl_ptr - returned msg_ctrl value.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_send_cfg_message_with_error_info(pmc_handle_t *pmc_handle,
                                          UINT32 mod_id,
                                          UINT32 msg_subtype,
                                          UINT32 *cfg_msg,
                                          UINT32 len,
                                          UINT32 *msg_ctrl_ptr,
                                          pmc_sys_stat_t *stat)
{
    PMC_ERROR result;
    UINT32 msg_ctrl_if_arg_is_NULL;

    PMC_ENTRY();

    if (msg_ctrl_ptr == NULL) {
        msg_ctrl_ptr = &msg_ctrl_if_arg_is_NULL;
    }
    result = hostmsg_send_cfg_message_internal(pmc_handle,mod_id,msg_subtype,cfg_msg,len,msg_ctrl_ptr,FALSE,stat);

    PMC_RETURN(result);
} /* of hostmsg_send_cfg_message_with_error_info */


/*******************************************************************************
* hostmsg_send_cfg_message
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Send a basic configuration message to FW
*
* INPUTS:
*   pmc_handle  - any pmc handle
*   mod_id      - module id
*   msg_subtype - message sub type
*   cfg_msg     - pointer to configuration message
*   len         - len in UINT32 words of the configuration message
*   stat - statistic record for gathering timing information.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_send_cfg_message(pmc_handle_t *pmc_handle,
                                          UINT32 mod_id,
                                          UINT32 msg_subtype,
                                          UINT32 *cfg_msg,
                                          UINT32 len,
                                          pmc_sys_stat_t *stat)
{
    PMC_ERROR result;

    PMC_ENTRY();

    result = hostmsg_send_cfg_message_internal(pmc_handle,mod_id,msg_subtype,cfg_msg,len,NULL,FALSE,stat);

    PMC_RETURN(result);
} /* of hostmsg_send_cfg_message */

/*******************************************************************************
* hostmsg_start_cfg_message
* ___________________________________________________________________________
*
* DESCRIPTION:
*  sends a configuration message to FW and returns without waiting for the firmware message to finish.
*
* INPUTS:
*   pmc_handle  - any pmc handle
*   mod_id      - module id
*   msg_subtype - message sub type
*   cfg_msg     - pointer to configuration message
*   len         - len in UINT32 words of the configuration message
*   stat - statistic record for gathering timing information.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_start_cfg_message(pmc_handle_t *pmc_handle,
                                           UINT32 mod_id,
                                           UINT32 msg_subtype,
                                           UINT32 *cfg_msg,
                                           UINT32 len,
                                           pmc_sys_stat_t *stat)
{
    PMC_ERROR result;

    PMC_ENTRY();

    result = hostmsg_send_cfg_message_internal(pmc_handle,mod_id,msg_subtype,cfg_msg,len,NULL,TRUE,stat);

    PMC_RETURN(result);
} /* of hostmsg_send_cfg_message */



/*******************************************************************************
* hostmsg_perform_query
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends a message and retrieves a response
*  
* INPUTS:
*   pmc_handle        - any pmc handle
*   mod_id            - module id
*   msg_subtype       - message sub type
*   query             - pointer to configuration message
*   query_len         - length in UINT32 words of the configuration message
*   response          - pointer to where response is placed
*   max_response_len  - maximum size of the response message (e.g. sizeof(response))
*   length_must_match - does the response length need to be exactly the max_response_len?
*   stat              - statistic record for gathering timing information.
*
* OUTPUTS:
*   actual_response_len - how many bytes where read.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*   HOSTMSG_ERR_RSP_MSG_LEN - if (length_must_match) is true and response_length does not match.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR hostmsg_perform_query(pmc_handle_t *pmc_handle,
                                          UINT32 mod_id,
                                          UINT32 msg_subtype,
                                          UINT32 *query,
                                          UINT32 query_len,
                                          UINT32 *response,
                                          UINT32 max_response_len,
                                          UINT32 *actual_response_len,
                                          BOOL8 length_must_match,
                                          pmc_sys_stat_t *stat)
{
    UINT32 *data_ptr = NULL;
    UINT32 inbound_seq_num = 0;
    PMC_ERROR result = PMC_SUCCESS;    

    /* Only used for hostmsg_check_outbound_rsp_header function. */
    UINT32 outbound_msg_len = 0;
    hostmsg_handle_t *hostmsg_handle = (hostmsg_handle_t *)pmc_handle->common_info->hostmsg_handle;
    digi_handle_t *digi_handle = (digi_handle_t *)hostmsg_handle->base.parent_handle;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    UINT32 zero=0;

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif

    PMC_ASSERT(NULL != pmc_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    if (stat!=NULL) {
        digi_fw_stats->cur_hostmsg_stat = stat;
        pmc_sys_stat_mark_start(stat);
    }
    data_ptr = (UINT32 *)hostmsg_data_buf_get(pmc_handle);

    inbound_seq_num = hostmsg_handle->msg_seq_num++;

    /* 3 seems to be the minimum message length */
    if (query_len == 0) {
        query = &zero;
        query_len = 1;
    }
    /* Assemble message header. */
    hostmsg_create_inbound_message_header(stat,
                                          data_ptr,
                                          mod_id,
                                          CATEGORY_CMD_REQ,
                                          query_len+HOSTMSG_NUM_HEADER_WORDS,
                                          msg_subtype,
                                          BASIC_MSG,
                                          inbound_seq_num);

    /* Add message body to the previously created header. */
    if (query_len > 0 && (query != NULL)) {
        memcpy(data_ptr + HOSTMSG_NUM_HEADER_WORDS,query,query_len*sizeof(UINT32));
    }
#ifdef L5SYS_USE_MESSAGE_FOR_FW_CMDS
    if (l5sys_queue_enabled(pmc_handle)) {
        UINT32 header[2];
        result = l5sys_fw_perform_query(pmc_handle,data_ptr,query_len+HOSTMSG_NUM_HEADER_WORDS,response,max_response_len,header);


        if (PMC_SUCCESS == result) {
            /* check response message header */
            result = hostmsg_check_outbound_rsp_header_from_data(hostmsg_handle,stat,header,response,
                                                       mod_id,
                                                       CATEGORY_RSP_REQ,
                                                       msg_subtype,
                                                       inbound_seq_num,
                                                       &outbound_msg_len,
                                                       NULL);
            /* subtract off the 2 header words */
            outbound_msg_len -= HOSTMSG_NUM_HEADER_WORDS;

            *actual_response_len = outbound_msg_len;
        }
#ifdef USE_FW_LOG
    if (digi_fw_firmware_logging_enabled) {
        UINT32 i;
        digi_handle_t *digi_handle = (digi_handle_t *)hostmsg_handle->base.parent_handle;
        char *digi_name = digi_handle->base.tsb_name;
        UINT32 len = max_response_len;
        if (len > outbound_msg_len) {
            len = outbound_msg_len;
        }

        for(i=0;i<len;i++)
        {
            if(i%16==0)
            {
                if(i!=0)
                {
                    FW_LOG("\n");
                }
                FW_LOG("l5sys_fw_perform_query results:  %s ",digi_name);
            }
            FW_LOG("%08x ",response[i]);
        }
        FW_LOG("\n");
        /* if compiled with USE_FW_LOG, also show the debug log after every firmware message */
        digi_fw_show_debug_log((digi_handle_t *)hostmsg_handle->base.parent_handle);
        /* if debugging opsa mpmo functions, also show mpmo counts after every firmware function
        digi_fw_show_mpmo_counts((digi_handle_t*)hostmsg_handle->base.parent_handle); */
    }
#endif
    } else 
#endif
    {
        /* Send message. */
        result = hostmsg_send_inbound_message(pmc_handle,
                                              query_len+HOSTMSG_NUM_HEADER_WORDS);

        if (PMC_SUCCESS == result) {
            /* check response message header */
            result = hostmsg_check_outbound_rsp_header(stat,pmc_handle,
                                                       mod_id,
                                                       CATEGORY_RSP_REQ,
                                                       msg_subtype,
                                                       inbound_seq_num,
                                                       &outbound_msg_len,
                                                       NULL);
        }

        if (PMC_SUCCESS == result)
        {
            /* subtract off the 2 header words */
            outbound_msg_len -= HOSTMSG_NUM_HEADER_WORDS;

            /* data_ptr will contain the data once this method returns. */
            hostmsg_outbound_mailbox_get(pmc_handle,
                                         (sizeof(UINT32) * HOSTMSG_NUM_HEADER_WORDS),
                                         outbound_msg_len);
            *actual_response_len = outbound_msg_len;

            if (outbound_msg_len > max_response_len) {
                if (length_must_match) {
                    PMC_LOG(PMC_LOG_SEV_LOW, HOSTMSG_ERR_RSP_MSG_LEN, *data_ptr, 0,
                            "expected response length of %d but received %d\n",outbound_msg_len,max_response_len);
                    result = HOSTMSG_ERR_RSP_MSG_LEN;
                }
                outbound_msg_len = max_response_len;
            }

            memcpy(response,data_ptr,sizeof(UINT32)*outbound_msg_len);
        }

        /* Signal firmware that we are done reading outbound message. */
        hostmsg_clear_outbound_doorbell(pmc_handle);
    }

#ifdef USE_FW_LOG
    if (digi_fw_firmware_logging_enabled) {
        /* if compiled with USE_FW_LOG, also show the debug log after every firmware message */
        digi_fw_show_debug_log((digi_handle_t *)hostmsg_handle->base.parent_handle);
        /* if debugging opsa mpmo functions, also show mpmo counts after every firmware function
        digi_fw_show_mpmo_counts((digi_handle_t*)hostmsg_handle->base.parent_handle); */
    }
#endif
    if (digi_fw_stats->cur_hostmsg_stat != NULL)
    {
        digi_fw_stats->prev_hostmsg_stat = digi_fw_stats->cur_hostmsg_stat;
        digi_fw_stats->cur_hostmsg_stat = NULL;
    }

    if (stat!=NULL) {
        pmc_sys_stat_mark_end(stat);
    }
    PMC_RETURN(result);
} /* hostmsg_perform_query */


/*******************************************************************************
* hostmsg_perform_fixed_length_query
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends a message and retrieves a response
*  
* INPUTS:
*   pmc_handle  - any pmc handle
*   mod_id      - module id
*   msg_subtype - message sub type
*   cfg_msg     - pointer to configuration message
*   cfg_len     - length in UINT32 words of the configuration message
*   response    - pointer to where response is placed
*   max_response_len - maximum size of the response message (e.g. sizeof(response)/sizeof(UINT32))
*   stat - statistic record for gathering timing information.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_perform_fixed_length_query(pmc_handle_t *pmc_handle,
                                                    UINT32 mod_id,
                                                    UINT32 msg_subtype,
                                                    UINT32 *cfg_msg,
                                                    UINT32 cfg_len,
                                                    UINT32 *response,
                                                    UINT32 max_response_len,
                                                    pmc_sys_stat_t *stat)
{
    PMC_ERROR result;
    UINT32 actual_response_len;
    BOOL8 length_must_match = TRUE;

    PMC_ENTRY();

    result = hostmsg_perform_query(pmc_handle,mod_id,msg_subtype,cfg_msg,cfg_len,response,max_response_len,&actual_response_len,length_must_match,stat);

    PMC_RETURN(result);
}


/*******************************************************************************
* hostmsg_perform_variable_length_query
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Sends a message and retrieves a response
*  
* INPUTS:
*   pmc_handle  - any pmc handle
*   mod_id      - module id
*   msg_subtype - message sub type
*   cfg_msg     - pointer to configuration message
*   cfg_len     - length in UINT32 words of the configuration message
*   response    - pointer to where response is placed
*   max_response_len - maximum size of the response message (e.g. sizeof(response)/sizeof(UINT32))
*   actual_response_len - how many words where returned in the response.
*   stat - statistic record for gathering timing information.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_perform_variable_length_query(pmc_handle_t *pmc_handle,
                                                       UINT32 mod_id,
                                                       UINT32 msg_subtype,
                                                       UINT32 *cfg_msg,
                                                       UINT32 cfg_len,
                                                       UINT32 *response,
                                                       UINT32 max_response_len,
                                                       UINT32 *actual_response_len,
                                                       pmc_sys_stat_t *stat )
{
    PMC_ERROR result;
    BOOL8 length_must_match = FALSE;
    UINT32 actual_response_len_buffer_if_arg_is_NULL;

    PMC_ENTRY();

    /* if a variable length query is requested, but the caller doesn't care what the result size is, pass in a valid pointer to hostmsg_perform_query() */
    if (NULL == actual_response_len)
    {
        actual_response_len = &actual_response_len_buffer_if_arg_is_NULL;
    }
    result = hostmsg_perform_query(pmc_handle,mod_id,msg_subtype,cfg_msg,cfg_len,response,max_response_len,actual_response_len,length_must_match,stat);

    PMC_RETURN(result);
}
