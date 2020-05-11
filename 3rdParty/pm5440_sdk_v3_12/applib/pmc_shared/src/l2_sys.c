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
*   DESCRIPTION:
*     The level 2 system layer provides functionality for 'change sets',
*     if change sets are enabled, it saves the list of operations,
*     when change sets are applied it replays the list of operations.
* 
*   NOTES:
*
********************************************************************************/

#include "l2_sys.h"

#ifndef NO_CHANGE_SETS

#define CHANGE_SET_BUILD   3
#define CHANGE_SET_RECORD  2
#define CHANGE_SET_DISABLE 0

/* record the change set if bit 1 is set */
#define USE_CHANGE_SET( s ) (s->use_change_set & 0x2)

/* pass reads & writes to the device if bit 0 is clear */
#define USE_DEVICE( s )     ((s->use_change_set & 0x1) == 0)


/*******************************************************************************
* sys_build_change_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  register operations are no longer are applied to the device, rather the are recorded
*  to the specified change set, and can be applied to the device using cs_apply_change_set 
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   cs - change_set_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void sys_build_change_set( pmc_sys_handle_t *sys_handle, change_set_t cs )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    h->use_change_set = CHANGE_SET_BUILD;
    h->change_set = cs;
}

/*******************************************************************************
* sys_monitor_change_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  similar to sys_build_change_set, but register operations are also applied 
*  to the device, and also recorded to the specified change set.
*  they can be re-applied to the device using cs_apply_change_set
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   cs - change_set_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void sys_monitor_change_set( pmc_sys_handle_t *sys_handle, change_set_t cs )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    h->use_change_set = CHANGE_SET_RECORD;
    h->change_set = cs;
}

/*******************************************************************************
* sys_disable_change_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  stop recording the current change set
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void sys_disable_change_set( pmc_sys_handle_t *sys_handle )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    h->use_change_set = CHANGE_SET_DISABLE;
}

/*******************************************************************************
* cs_apply_change_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*  apply the register settings previously recorded to the device.
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   cs - change_set_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void cs_apply_change_set( pmc_sys_handle_t *sys_handle, change_set_t cs )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    h->use_change_set = CHANGE_SET_DISABLE;
    cs_apply(sys_handle,cs);
}

/*******************************************************************************
* l2sys_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*  initializes l2pmc_sys_handle_t fields.
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l2sys_init(  pmc_sys_handle_t *sys_handle )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    l3sys_init(sys_handle);

    h->use_change_set = FALSE;
    h->change_set = NULL;
}

/*******************************************************************************
* l2sys_halt
* ___________________________________________________________________________
*
* DESCRIPTION:
*  performs any cleanup of l2pmc_sys_handle_t fields.
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l2sys_halt(  pmc_sys_handle_t *sys_handle )
{
    l3sys_halt(sys_handle);
}


/*******************************************************************************
* l2sys_reg_read_modify_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  perform a read/modify/write on the register
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   mem_type - memory type
*   reg_addr - register address
*   mask - shifted field mask
*   value - shifted field value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l2sys_reg_read_modify_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 mask, UINT32 value )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;

    if (USE_CHANGE_SET(h))
    {
        cs_add_reg_read_modify_write(h->change_set,mem_type,reg_addr,mask,value);
    }
    if (USE_DEVICE(h))
    {
        l3sys_reg_read_modify_write( sys_handle, mem_type, reg_addr, mask, value );
    }
}

/*******************************************************************************
* l2sys_reg_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  perform a register write
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   mem_type - memory type
*   reg_addr - register address
*   value - register value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void l2sys_reg_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 value )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    
    if (USE_CHANGE_SET(h))
    {
        cs_add_reg_write(h->change_set,mem_type,reg_addr,value);
    }
    if (USE_DEVICE(h))
    {
        l3sys_reg_write( sys_handle, mem_type, reg_addr, value );
    }
}

/*******************************************************************************
* l2sys_reg_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  perform a register read
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   mem_type - memory type
*   reg_addr - register address
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - register value.
*
* NOTES:
*******************************************************************************/
UINT32 l2sys_reg_read( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    UINT32 regval;
    regval = l3sys_reg_read( sys_handle, mem_type, reg_addr );
    if (!USE_DEVICE(h))
    {
    if (mem_type != MEM_TYPE_CONFIG)
        LOG ("WARNING, read of status register while creating a change set (RD 0x%08x -> 0x%08x)",reg_addr,regval);
    }
    if (USE_CHANGE_SET(h))
    {
        cs_add_reg_read(h->change_set,mem_type,reg_addr,regval);
    }
    return regval;
}

/*******************************************************************************
* l2sys_burst_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  write a contiguous span of registers
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   mem_type - memory type
*   reg_addr - first register address of the span
*   len - number of 32 bit registers to write.
*   data - data[0..(len-1)] - register values.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:   
*******************************************************************************/
void l2sys_burst_write( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 len, UINT32 *data )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    if (USE_CHANGE_SET(h))
    {
        cs_add_reg_burst_write(h->change_set,mem_type,reg_addr,len,data);
    }
  
    if (USE_DEVICE(h))
    {
        l3sys_burst_write( sys_handle, mem_type, reg_addr, len, data );
    }
}


/*******************************************************************************
* l2sys_burst_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  read a contiguous span of registers
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   mem_type - memory type
*   reg_addr - first register address of the span
*   len - number of registers to read (each register is a 32 bit word)
*   data - pointer to at least 'len' UINT32's
*
* OUTPUTS:
*   *data - data[0..(len-1)] - value of each register
*
* RETURNS:
*   None
*
* NOTES:
*******************************************************************************/
void l2sys_burst_read( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 reg_addr, UINT32 len, UINT32 *data )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    if (!USE_DEVICE(h))
    {
        if (mem_type != MEM_TYPE_CONFIG)
            LOG ("WARNING, burst read of status registers while creating a change set (RD 0x%08x, len=%d)",reg_addr,len);
    }
    l3sys_burst_read( sys_handle, mem_type, reg_addr, len, data );
    if (USE_CHANGE_SET(h))
    {
        cs_add_reg_burst_read(h->change_set,mem_type,reg_addr,len,data);
    }
}


/*******************************************************************************
* l2sys_poll_bit
* ___________________________________________________________________________
*
* DESCRIPTION:
*  read a register until the indicated field is the specified value, or until the 
*  maximum number of reads is performed without satisfying the condition.
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   mem_type - memory type
*   addr - register address
*   mask - shifted field mask.
*   value - shifted value.
*   max_count - maximum number of reads before returning an PMC_SHARED_ERR_CODE_POLL_TIMEOUT
*   num_failed_polls - pointer to a UINT32 to return the number of failed polls, or NULL
*   delay_between_polls_in_microseconds - delay between polls in microseconds.
*
* OUTPUTS:
*   num_failed_polls - number of register reads that returned failed. (if num_failed_polls is not NULL)
*
* RETURNS:
*   PMC_SUCCESS - poll succeeded
*   PMC_SHARED_ERR_CODE_POLL_TIMEOUT - poll failed
*
* NOTES:
*   The successful register read is not included in the number of failed polls, so if the first
*   register read satisfies the poll condition then *num_failed_polls is 0.
*******************************************************************************/
PMC_ERROR l2sys_poll_bit( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    
    if (USE_CHANGE_SET(h))
    {
        cs_add_poll_bit(h->change_set,mem_type,addr,mask,value,max_count,delay_between_polls_in_microseconds);
    }
    if (USE_DEVICE(h))
    {
        return l3sys_poll_bit(sys_handle,addr,mask,value,max_count,num_failed_polls,delay_between_polls_in_microseconds);
    }
    else
        return PMC_SUCCESS;
}


/*******************************************************************************
* l2sys_poll
* ___________________________________________________________________________
*
* DESCRIPTION:
*  read a register until the field satisfies the condition, or until the 
*  maximum number of reads is performed without satisfying the condition.
*
* INPUTS:
*   sys_handle - pointer to a pmc_sys_handle_t
*   mem_type - memory type
*   addr - register address
*   mask - shifted field mask.
*   value - shifted value.
*   cmp - comparison type - (PMC_POLL_EQUAL, etc)
*   max_count - maximum number of reads before returning an PMC_SHARED_ERR_CODE_POLL_TIMEOUT
*   num_failed_polls - pointer to a UINT32 to return the number of failed polls or NULL
*   delay_between_polls_in_microseconds - delay between polls in microseconds.
*
* OUTPUTS:
*   num_failed_polls - number of register reads that returned failed. (if num_failed_polls is not NULL)
*
* RETURNS:
*   PMC_SUCCESS - poll succeeded
*   PMC_SHARED_ERR_CODE_POLL_TIMEOUT - poll failed
*
* NOTES:
*   The successful register read is not included in the number of failed polls, so if the first
*   register read satisfies the poll condition then *num_failed_polls is 0.
*******************************************************************************/
PMC_ERROR l2sys_poll( pmc_sys_handle_t *sys_handle, int mem_type, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
{
    l2pmc_sys_handle_t *h = *(l2pmc_sys_handle_t**)sys_handle;
    
    if (USE_CHANGE_SET(h))
    {
        cs_add_poll(h->change_set,mem_type,addr,mask,value,cmp,max_count,delay_between_polls_in_microseconds);
    }
    if (USE_DEVICE(h))
    {
        return l3sys_poll(sys_handle,addr,mask,value,cmp,max_count,num_failed_polls,delay_between_polls_in_microseconds);
    }
    else
        return PMC_SUCCESS;
}

#endif /* NO_CHANGE_SET */
