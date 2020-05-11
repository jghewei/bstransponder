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
*  DESCRIPTION :
*  This module contains host message hardware related methods.
*
*   NOTES:
*
*******************************************************************************/
#include "hostmsg_loc.h"
#include "digi_loc.h"
/* don't include digi_loc.h in hostmsg_loc.h because it breaks the compile of lineotn_unit_test */


/*******************************************************************************
*  hostmsg_interrupt_ipi2pcie_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear IPI2PCIE IPI interrupt.
*
* INPUTS:
*   hostmsg_handle - Host message handle.
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
PUBLIC void hostmsg_interrupt_ipi2pcie_clear(hostmsg_handle_t *hostmsg_handle)
{
    UINT32 mask = 0x01 << HOSTMSG_INBOUND_DOORBELL_NUM;

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN();
#endif

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

#ifdef DISABLE_Q_FOR_FW_MESSAGES
    /* investigating race condition in serdes_adaptation start,... for now we won't use the firmware queue for sending host messages */
    l5sys_firmware_queue_disable(hostmsg_handle);
#endif

    int_agr_field_IPI2PCIE_0_I0_set_to_clear(NULL, (pmc_handle_t *)hostmsg_handle, mask);

#ifdef DISABLE_Q_FOR_FW_MESSAGES
    /* investigating race condition in serdes_adaptation start,... for now we won't use the firmware queue for sending host messages */
    l5sys_firmware_queue_enable(hostmsg_handle);
#endif

    PMC_RETURN();
}


/*******************************************************************************
*  hostmsg_interrupt_ipi2pcie_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable IPI2PCIE IPI interrupt.
*
* INPUTS:
*   hostmsg_handle - Host message handle.
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
PUBLIC void hostmsg_interrupt_ipi2pcie_enable(hostmsg_handle_t *hostmsg_handle)
{
    UINT32 mask = 0x01 << HOSTMSG_INBOUND_DOORBELL_NUM;

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN();
#endif

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Enable IPI2PCIE_0_EN . Writing 1 will enable and writing 0 will disable
       interrupt. This is read-modify-write operation if mask != 0xffff */
    int_agr_reg_IPI2PCIE_0_INT_EN_field_set( NULL, (pmc_handle_t *)hostmsg_handle,
                                             mask, 0, mask);

    PMC_RETURN();
}

/*******************************************************************************
* hostmsg_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read hostmsg data.
*
* INPUTS:
*   hostmsg_handle - Host message handle.
*   vaddr          - chip address to which to write data. Note that this
*                    address is relative to base address of the chip.
*   len            - length of data to be written.
*
* OUTPUTS:
*   data - data read.
*
* RETURNS:
*   None.
*
* NOTES:
*   Technically this method does not touch the hardware, but it is called
*   by higher or same order methods, some of which do.
*
*******************************************************************************/
PUBLIC void hostmsg_read(hostmsg_handle_t *hostmsg_handle, UINT32 vaddr,
                         UINT32 len, UINT32 *data)
{
    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN();
#endif

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(len > 1)
    {
        l5sys_burst_read(hostmsg_handle->base.sys_handle, vaddr, len, data);
    }
    else
    {
        *data = l5sys_reg_read(hostmsg_handle->base.sys_handle, vaddr);
    }
#ifdef USE_FW_LOG
    if(digi_fw_firmware_logging_enabled)
    {
        UINT32 i;
        digi_handle_t *digi_handle = (digi_handle_t *)hostmsg_handle->base.parent_handle;
        char *digi_name = digi_handle->base.tsb_name;

        for(i=0;i<len;i++)
        {
            if(i%16==0)
            {
                if(i!=0)
                {
                    FW_LOG("\n");
                }
                FW_LOG("hostmsg_read:  %s ",digi_name);
            }
            FW_LOG("%08x ",data[i]);
        }
        FW_LOG("\n");
    }
#endif

    PMC_RETURN();
}


/*******************************************************************************
* hostmsg_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Write hostmsg data.
*
* INPUTS:
*   hostmsg_handle - Host message handle.
*   vaddr          - address relative to chip.
*   len            - length of data to be written.
*   data           - data to be written.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*  Technically this method does not touch the hardware, but it is called
*  by higher or same order methods, some of which are.
*
*******************************************************************************/
PUBLIC void hostmsg_write(hostmsg_handle_t *hostmsg_handle, UINT32 vaddr,
                          UINT32 len, UINT32 *data)
{
    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN();
#endif

    PMC_ASSERT((NULL != hostmsg_handle),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);    
    PMC_ASSERT((NULL != hostmsg_handle->base.sys_handle),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

#ifdef USE_FW_LOG
    if(digi_fw_firmware_logging_enabled)
    {
        UINT32 i;
        digi_handle_t *digi_handle = (digi_handle_t *)hostmsg_handle->base.parent_handle;
        char *digi_name = digi_handle->base.tsb_name;

        for(i=0;i<len;i++)
        {
            if(i%16==0)
            {
                if(i!=0)
                {
                    FW_LOG("\n");
                }
                FW_LOG("hostmsg_write: %s ",digi_name);
            }
            FW_LOG("%08x ",data[i]);
        }
        FW_LOG("\n");
    }
#endif
    if(len > 1)
    {
        l5sys_burst_write(hostmsg_handle->base.sys_handle, vaddr, len, data);
    }
    else
    {
        l5sys_reg_write(hostmsg_handle->base.sys_handle, vaddr, *data);
    }

    PMC_RETURN();
}

/*******************************************************************************
* hostmsg_inbound_doorbell_register_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read inbound doorbell register.
*
* INPUTS:
*   hostmsg_handle - host message handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Content of inbound doorbell register.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 hostmsg_inbound_doorbell_register_get(hostmsg_handle_t *hostmsg_handle)
{
    UINT32 ret = 0;

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    /*
     * Caller checks the response and expects something with an ending 1 value.
     */
    ret = 0x01;
    PMC_RETURN(ret);
#endif

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    ret = int_agr_reg_INTER_PROCESSOR_INTS_array_read(NULL,
                                                      (pmc_handle_t *)hostmsg_handle,
                                                      0);

    PMC_RETURN(ret);
}

/*******************************************************************************
* hostmsg_inbound_doorbell_register_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Write inbound doorbell register.
*
* INPUTS:
*   hostmsg_handle - host message handle.
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
PUBLIC void hostmsg_inbound_doorbell_register_set(hostmsg_handle_t *hostmsg_handle)
{
    UINT32 data = 0x01 << HOSTMSG_INBOUND_DOORBELL_NUM;

    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN();
#endif

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

#ifdef DISABLE_Q_FOR_FW_MESSAGES
    /* investigating race condition in serdes_adaptation start,... for now we won't use the firmware queue for sending host messages */
    l5sys_firmware_queue_disable(hostmsg_handle);
#endif
    int_agr_reg_INTER_PROCESSOR_INTS_array_field_set(NULL,
                                                     (pmc_handle_t *)hostmsg_handle,
                                                     0, data, 0, 0);
#ifdef DISABLE_Q_FOR_FW_MESSAGES
    /* investigating race condition in serdes_adaptation start,... for now we won't use the firmware queue for sending host messages */
    l5sys_firmware_queue_enable(hostmsg_handle);
#endif
    PMC_RETURN();
}

/*******************************************************************************
* hostmsg_outbound_doorbell_int_register_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read outbound doorbell interrupt register.

*
* INPUTS:
*   hostmsg_handle - host message handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Content of outbound doorbell interrupt register.
*
* NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
*
*******************************************************************************/
PUBLIC UINT32 hostmsg_outbound_doorbell_int_register_get(hostmsg_handle_t *hostmsg_handle)
{
    UINT32 ret = 0;


#if  defined(PMC_SW_SIMULATION)
    /*
     * Called checks the return value is valid. Returning 1 satisfies the check.
     */
    ret = 0x01;
    PMC_RETURN(ret);
#endif

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    ret = int_agr_reg_IPI2PCIE_0_INT_read(NULL, (pmc_handle_t *)hostmsg_handle);

    return(ret);

} /* hostmsg_outbound_doorbell_int_register_get */

/*******************************************************************************
* hostmsg_mailbox_data_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Initialize mailbox related data.
*
* INPUTS:
*   hostmsg_handle - Host message handle.
*
* OUTPUTS:
*   Initialized mailbox related data.
*
* RETURNS:
*   PMC_SUCCESS - Allocations were successful
*   PMC_ERROR   - Allocation of a mailbox value failed.
*
* NOTES:
*   Warning: The only way this is going to work is under the assumption
*   that 'base' is first member in hostmsg_handle_t object.
*
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_mailbox_data_init(hostmsg_handle_t *hostmsg_handle)
{
    PMC_ENTRY();

#if  defined(PMC_SW_SIMULATION)
    PMC_RETURN(PMC_SUCCESS);
#endif

    PMC_ASSERT((NULL != hostmsg_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    
    hostmsg_handle->inbound_mailbox_base_addr =    
    cpup34k_field_BASE_ADD_INBOUND_MAILBOX_get(NULL, (pmc_handle_t *)hostmsg_handle);
    /*
     * IF there is an issue with PCIe interface then either 0 or 
     * 0xFFFFFFFF are typically returned. Check for these values to catch
     * this case. Note that DIGI_SERDES_UNASSIGNED is defined as 
     * 0xFFFFFFFF and thus is used here eventhough the name is not ideal for
     * the condition.
     */
    if (0 == hostmsg_handle->inbound_mailbox_base_addr ||
        DIGI_SERDES_UNASSIGNED == hostmsg_handle->inbound_mailbox_base_addr)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Issue retrieving Inbound Mailbox. Value=%d. PCIe interface maybe down\n", hostmsg_handle->inbound_mailbox_base_addr); 
        PMC_RETURN(DIGI_ERR_DEVICE_NOT_INIT);
    }

    hostmsg_handle->outbound_mailbox_base_addr =    
    cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_get(NULL, (pmc_handle_t *)hostmsg_handle);

    if (0 == hostmsg_handle->outbound_mailbox_base_addr ||
        DIGI_SERDES_UNASSIGNED == hostmsg_handle->outbound_mailbox_base_addr)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Issue retrieving Outbound Mailbox. Value=%d. PCIe interface maybe down\n", hostmsg_handle->outbound_mailbox_base_addr);
        PMC_RETURN(DIGI_ERR_DEVICE_NOT_INIT);
    }

    hostmsg_handle->inbound_mailbox_size =
    cpup34k_field_SIZE_INBOUND_MAILBOX_get(NULL, (pmc_handle_t *)hostmsg_handle);

    if (0 == hostmsg_handle->inbound_mailbox_size ||
        DIGI_SERDES_UNASSIGNED == hostmsg_handle->inbound_mailbox_size)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Issue retrieving Inbound Mailbox Size. Value=%d. PCIe interface maybe down\n", hostmsg_handle->inbound_mailbox_size);
        PMC_RETURN(DIGI_ERR_DEVICE_NOT_INIT);
    }

    hostmsg_handle->outbound_mailbox_size =    
    cpup34k_field_SIZE_OUTBOUND_MAILBOX_get(NULL, (pmc_handle_t *)hostmsg_handle);

    if (0 == hostmsg_handle->outbound_mailbox_size ||
        DIGI_SERDES_UNASSIGNED == hostmsg_handle->outbound_mailbox_size)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Issue retrieving Outbound Mailbox Size. Value=%d. PCIe interface maybe down\n", hostmsg_handle->outbound_mailbox_size);
        PMC_RETURN(DIGI_ERR_DEVICE_NOT_INIT);
    }

    hostmsg_handle->inbound_mailbox_messages_max =
    cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_get(NULL, (pmc_handle_t *)hostmsg_handle);

    if (0 == hostmsg_handle->inbound_mailbox_messages_max ||
        DIGI_SERDES_UNASSIGNED == hostmsg_handle->inbound_mailbox_messages_max)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Issue retrieving Inbound Mailbox Messages Max. Value=%d. PCIe interface maybe down\n", hostmsg_handle->inbound_mailbox_messages_max);
        PMC_RETURN(DIGI_ERR_DEVICE_NOT_INIT);
    }

    hostmsg_handle->outbound_mailbox_messages_max =    
    cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_get(NULL, (pmc_handle_t *)hostmsg_handle);

    if (0 == hostmsg_handle->outbound_mailbox_messages_max ||
        DIGI_SERDES_UNASSIGNED == hostmsg_handle->outbound_mailbox_messages_max)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Issue retrieving Outbound Mailbox Messages Max. Value=%d. PCIe interface maybe down\n", hostmsg_handle->outbound_mailbox_messages_max);
        PMC_RETURN(DIGI_ERR_DEVICE_NOT_INIT);
    }

    hostmsg_handle->inbound_mailbox_message_size_max =    
    cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_get(NULL, (pmc_handle_t *)hostmsg_handle);

    if (0 == hostmsg_handle->inbound_mailbox_message_size_max ||
        DIGI_SERDES_UNASSIGNED == hostmsg_handle->inbound_mailbox_message_size_max)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Issue retrieving Inbound Mailbox Message Size Max. Value=%d. PCIe interface maybe down\n", hostmsg_handle->inbound_mailbox_message_size_max);
        PMC_RETURN(DIGI_ERR_DEVICE_NOT_INIT);
    }

    hostmsg_handle->outbound_mailbox_message_size_max =    
    cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_get(NULL, (pmc_handle_t *)hostmsg_handle);

    if (0 == hostmsg_handle->outbound_mailbox_message_size_max ||
        DIGI_SERDES_UNASSIGNED == hostmsg_handle->outbound_mailbox_message_size_max)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                "Issue retrieving Outbound Mailbox Message Size Max. Value=%d. PCIe interface maybe down\n", hostmsg_handle->outbound_mailbox_message_size_max);
        PMC_RETURN(DIGI_ERR_DEVICE_NOT_INIT);
    }    

    PMC_RETURN(PMC_SUCCESS);
}

