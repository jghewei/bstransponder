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
*      system interface.
* 
*   NOTES:
******************************************************************************/

/*---- Compile Options -----------------------------------------------------*/

/*---- Include Files -------------------------------------------------------*/
#include "l3_sys.h"
#include "change_sets.h"
#include "pmc_log.h"

/*---- Extern Declarations -------------------------------------------------*/
/*---- Private Constant and Type Declarations ------------------------------*/

/*******************************************************************************
* ENUM: cs_operation_enum_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   type of operation
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum {
    cs_reg_write,            /*!< register write operation */
    cs_reg_readmodifywrite,  /*!< register read/modify/write operation */
    cs_reg_burstwrite,       /*!< register burst write operation */
    cs_poll_bit,             /*!< poll bit operation */
    cs_poll,                 /*!< poll operation */
    cs_reg_read,             /*!< register read operation */
    cs_reg_burstread,        /*!< register burst read operation */
    cs_operation,            /*!< generic operation (just a "comment") */
} cs_operation_enum_t;

/******************************************************************************
* STRUCTURE: cs_reg_write_operation_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to store the contents of a register write operation 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    int mem_type;   /*!< memory type */
    UINT32 addr;    /*!< register address */
    UINT32 value;   /*!< register value */
} cs_reg_write_operation_t;


/******************************************************************************
* STRUCTURE: cs_reg_readmodifywrite_operation_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to store the contents of a register read/modify/write operation 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    int mem_type; /*!< memory type */      
    UINT32 addr;  /*!< register address */ 
    UINT32 mask;  /*!< shifted field mask */   
    UINT32 value; /*!< shifted field value */   
} cs_reg_readmodifywrite_operation_t;

/******************************************************************************
* STRUCTURE: cs_poll_bit_operation_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to store the contents of a poll_bit operation 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    int mem_type;                                 /*!< memory type */          
    UINT32 addr;                                  /*!< register address */     
    UINT32 mask;                                  /*!< shifted field mask */   
    UINT32 value;                                 /*!< shifted field value */  
    UINT32 max_count;                             /*!< maximum number of polls before failing */
    UINT32 delay_between_polls_in_microseconds;   /*!< delay between polls in microseconds */
} cs_poll_bit_operation_t;

/******************************************************************************
* STRUCTURE: cs_poll_operation_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to store the contents of a poll_bit operation 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    int mem_type;                                 /*!< memory type */                              
    UINT32 addr;                                  /*!< register address */                         
    UINT32 mask;                                  /*!< shifted field mask */                       
    UINT32 value;                                 /*!< shifted field value */                      
    PMC_POLL_COMPARISON_TYPE cmp;                 /*!< comparison (e.g. PMC_POLL_EQUAL) */
    UINT32 max_count;                             /*!< maximum number of polls before failing */    
    UINT32 delay_between_polls_in_microseconds;   /*!< delay between polls in microseconds */      
} cs_poll_operation_t;

/******************************************************************************
* STRUCTURE: cs_reg_burstwrite_operation_base_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to store the contents of a register burst write operation 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    int mem_type;   /*!< memory type */       
    UINT32 addr;    /*!< first register address */  
    UINT32 len;     /*!< number of registers to write */    
} cs_reg_burstwrite_operation_base_t;

#ifdef WIN32
#pragma warning (disable : 4200) /* nonstandard extension used : zero-sized array in struct/union */
#endif

/******************************************************************************
* STRUCTURE: cs_reg_burstwrite_operation_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to store the contents of a register burst write operation 
*
* ELEMENTS:
*   See typedef.
*
* NOTE:
*   VisualC has a problem with arrrays of arrays with zero length even if you are only using one element in the array,
*   so for the reg_burst_write structure this is the actual structure in memory, but only the base structure is defined in the
*   operation struct.
*******************************************************************************/
typedef struct {
    int mem_type;     /*!< memory type */                 
    UINT32 addr;      /*!< first register address */      
    UINT32 len;       /*!< number of registers to write */
    UINT32 value[0];  /*!< array of register values */
} cs_reg_burstwrite_operation_t;

/******************************************************************************
* STRUCTURE: cs_operation_struct_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to store the contents of an operation 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct cs_operation_struct_t {
    struct cs_operation_struct_t *next;                              /*!< next operation */
    cs_operation_enum_t cs_operation_enum;                           /*!< type of this operation */
    union {
        cs_reg_write_operation_t           reg_write[0];             /*!< register write arguments if this operation is a register write */
        cs_reg_readmodifywrite_operation_t reg_readmodifywrite[0];   /*!< register read/modify/write arguments if this operation is a register read/modify/write */
        cs_reg_burstwrite_operation_base_t reg_burstwrite[0];        /*!< register burst write arguments if this operation is a register burst write */
        cs_poll_bit_operation_t               poll_bit[0];           /*!< poll bit arguments if this operation is a poll bit  */
        cs_poll_operation_t                   poll[0];               /*!< poll arguments if this operation is a poll */
        char                                operation_name[0];       /*!< operation_name if this operation is a cs_operation (comment) */
    } args;                                                          /*!< Union to save space */
} cs_operation_t;

/******************************************************************************
* STRUCTURE: private_change_set_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to store a change set 
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    cs_operation_t *operations;       /*!< linked list of operations in this change set */
    cs_operation_t **next_operation;  /*!< last operation in the change set */
    int count; /*!< keep track of number of operations just for debug messages */
} private_change_set_t;


/*******************************************************************************
* cs_add_operation
* ___________________________________________________________________________
*
* DESCRIPTION:
*  add the name of an operation (just used for annotating the change set script)
*
* INPUTS:
*   public_cs - pointer to a change_set_t
*   fn - function name or 'operation' to write into the change set script. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void cs_add_operation( change_set_t public_cs, const char *fn )
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    size_t size = sizeof(cs_operation_t) + PMC_STRLEN(fn)+1;
    cs_operation_t *op = (cs_operation_t*)PMC_CALLOC(size);
    op->next = NULL;
    op->cs_operation_enum = cs_operation;
    strcpy(op->args.operation_name,fn);
    *(cs->next_operation) = op;
    cs->next_operation = &(op->next);
    cs->count++;
}


/*******************************************************************************
* cs_add_reg_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  record a register write
*
* INPUTS:
*   public_cs - pointer to a change_set_t
*   mem_type - memory type. 
*   addr - register address. 
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
void cs_add_reg_write( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 value )
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    size_t size = sizeof(cs_operation_t) + sizeof(cs_reg_write_operation_t);
    cs_operation_t *op = (cs_operation_t*)PMC_CALLOC(size);
    op->next = NULL;
    op->cs_operation_enum = cs_reg_write;
    op->args.reg_write[0].mem_type = mem_type;
    op->args.reg_write[0].addr = addr;
    op->args.reg_write[0].value = value;
    *(cs->next_operation) = op;
    cs->next_operation = &(op->next);
    cs->count++;
}

/*******************************************************************************
* cs_add_reg_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  record a register read
*
* INPUTS:
*   public_cs - pointer to a change_set_t
*   mem_type - memory type. 
*   addr - register address. 
*   value - register value that was read
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   A register read is not useful during playback since the value is not used.
*******************************************************************************/
void cs_add_reg_read( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 value )
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    size_t size = sizeof(cs_operation_t) + sizeof(cs_reg_write_operation_t);
    cs_operation_t *op = (cs_operation_t*)PMC_CALLOC(size);
    op->next = NULL;
    op->cs_operation_enum = cs_reg_read;
    op->args.reg_write[0].mem_type = mem_type;
    op->args.reg_write[0].addr = addr;
    op->args.reg_write[0].value = value;
    *(cs->next_operation) = op;
    cs->next_operation = &(op->next);
    cs->count++;
}

/*******************************************************************************
* cs_add_reg_read_modify_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  record a register read/modify/write
*
* INPUTS:
*   public_cs - pointer to a change_set_t
*   mem_type - memory type. 
*   addr - register address. 
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
void cs_add_reg_read_modify_write( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 mask, UINT32 value )
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    size_t size = sizeof(cs_operation_t) + sizeof(cs_reg_readmodifywrite_operation_t);
    cs_operation_t *op = (cs_operation_t*)PMC_CALLOC(size);
    op->next = NULL;
    op->cs_operation_enum = cs_reg_readmodifywrite;
    op->args.reg_readmodifywrite[0].mem_type = mem_type;
    op->args.reg_readmodifywrite[0].addr = addr;
    op->args.reg_readmodifywrite[0].mask = mask;
    op->args.reg_readmodifywrite[0].value = value;
    *(cs->next_operation) = op;
    cs->next_operation = &(op->next);
    cs->count++;
}


/*******************************************************************************
* cs_add_reg_burst_write
* ___________________________________________________________________________
*
* DESCRIPTION:
*  record a register burst write
*
* INPUTS:
*   public_cs - pointer to a change_set_t
*   mem_type - memory type. 
*   addr - register address. 
*   len - number of registers to write
*   value - array of [len] 32 bit integers
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void cs_add_reg_burst_write( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 len, UINT32 *value )
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    size_t size = sizeof(cs_operation_t) + sizeof(cs_reg_burstwrite_operation_t) + sizeof(UINT32)*len;
    cs_operation_t *op = (cs_operation_t*)PMC_CALLOC(size);
        void *args = &(op->args.reg_burstwrite);
        cs_reg_burstwrite_operation_t *burst_write_op = (cs_reg_burstwrite_operation_t*)args;
    op->next = NULL;
    op->cs_operation_enum = cs_reg_burstwrite;
    op->args.reg_burstwrite[0].mem_type = mem_type;
    op->args.reg_burstwrite[0].addr = addr;
    op->args.reg_burstwrite[0].len = len;
    PMC_MEMCPY(burst_write_op->value, value, len * sizeof(UINT32));
    *(cs->next_operation) = op;
    cs->next_operation = &(op->next);
    cs->count++;
}

/*******************************************************************************
* cs_add_reg_burst_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  record a register burst read
*
* INPUTS:
*   public_cs - pointer to a change_set_t
*   mem_type - memory type. 
*   addr - register address. 
*   len - number of registers that were read
*   value - array of [len] 32 bit integers
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   A burst read is not useful during playback since the values are ignored.
*******************************************************************************/
void cs_add_reg_burst_read( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 len, UINT32 *value )
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    size_t size = sizeof(cs_operation_t) + sizeof(cs_reg_burstwrite_operation_t) + sizeof(UINT32)*len;
    cs_operation_t *op = (cs_operation_t*)PMC_CALLOC(size);
        void *ptr = &(op->args.reg_burstwrite); /* typecast using void * to avoid type-punned pointer warning */
        cs_reg_burstwrite_operation_t *burst_write_op = (cs_reg_burstwrite_operation_t*)ptr;
    op->next = NULL;
    op->cs_operation_enum = cs_reg_burstwrite;
    op->args.reg_burstwrite[0].mem_type = mem_type;
    op->args.reg_burstwrite[0].addr = addr;
    op->args.reg_burstwrite[0].len = len;
    PMC_MEMCPY(burst_write_op->value, value, len * sizeof(UINT32));
    *(cs->next_operation) = op;
    cs->next_operation = &(op->next);
    cs->count++;
}

/*******************************************************************************
* cs_add_poll_bit
* ___________________________________________________________________________
*
* DESCRIPTION:
*  record a poll bit
*
* INPUTS:
*   public_cs - pointer to a change_set_t
*   mem_type - memory type. 
*   addr - register address. 
*   mask - shifted field mask
*   value - shifted field value
*   max_count - maximum number of polls before returning a failure code.
*   delay_between_polls_in_microseconds - delay between polls in microseconds
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void cs_add_poll_bit( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 mask, UINT32 value, UINT32 max_count, UINT32 delay_between_polls_in_microseconds )
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    size_t size = sizeof(cs_operation_t) + sizeof(cs_poll_bit_operation_t);
    cs_operation_t *op = (cs_operation_t*)PMC_CALLOC(size);
    op->next = NULL;
    op->cs_operation_enum = cs_poll_bit;
    op->args.poll_bit[0].mem_type = mem_type;
    op->args.poll_bit[0].addr = addr;
    op->args.poll_bit[0].mask = mask;
    op->args.poll_bit[0].value = value;
    op->args.poll_bit[0].max_count = max_count;
    op->args.poll_bit[0].delay_between_polls_in_microseconds = delay_between_polls_in_microseconds;
    *(cs->next_operation) = op;
    cs->next_operation = &(op->next);
    cs->count++;
}

/*******************************************************************************
* cs_add_poll
* ___________________________________________________________________________
*
* DESCRIPTION:
*  record a poll operation
*
* INPUTS:
*   public_cs - pointer to a change_set_t
*   mem_type - memory type. 
*   addr - register address. 
*   mask - shifted field mask
*   value - shifted field value
*   cmp - comparison operation (e.g. PMC_POLL_EQUAL)
*   max_count - maximum number of polls before returning a failure code.
*   delay_between_polls_in_microseconds - delay between polls in microseconds
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void cs_add_poll( change_set_t public_cs, int mem_type, UINT32 addr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 delay_between_polls_in_microseconds )
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    size_t size = sizeof(cs_operation_t) + sizeof(cs_poll_operation_t);
    cs_operation_t *op = (cs_operation_t*)PMC_CALLOC(size);
    op->next = NULL;
    op->cs_operation_enum = cs_poll_bit;
    op->args.poll[0].mem_type = mem_type;
    op->args.poll[0].addr = addr;
    op->args.poll[0].mask = mask;
    op->args.poll[0].value = value;
    op->args.poll[0].cmp = cmp;
    op->args.poll[0].max_count = max_count;
    op->args.poll[0].delay_between_polls_in_microseconds = delay_between_polls_in_microseconds;
    *(cs->next_operation) = op;
    cs->next_operation = &(op->next);
    cs->count++;
}

/***
 ***  change sets 
 ***/

/*******************************************************************************
* cs_allocate
* ___________________________________________________________________________
*
* DESCRIPTION:
*  allocate a new change set
*
* INPUTS:
*   None.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   change_set_t - the newly allocated change set.
*
* NOTES:
*******************************************************************************/
change_set_t cs_allocate()
{
    private_change_set_t *cs = (private_change_set_t*)PMC_CALLOC(sizeof(private_change_set_t));
    cs->operations = NULL;
    cs->next_operation = &(cs->operations);
    return cs;
}


/*******************************************************************************
* cs_apply
* ___________________________________________________________________________
*
* DESCRIPTION:
*   apply the operations that were recording in the change set to the device.
*
* INPUTS:
*   sys - pmc_sys_handle_t
*   public_cs - change_set_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void cs_apply(pmc_sys_handle_t sys, change_set_t public_cs)
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    cs_operation_t *op = cs->operations;
    LOG("applying %d operations in change set",cs->count);
    while (op != NULL)
    {
        switch (op->cs_operation_enum)
        {
        case cs_reg_write:
            l3sys_reg_write((void**)sys,op->args.reg_write[0].mem_type,op->args.reg_write[0].addr,op->args.reg_write[0].value);
            break;
        case cs_reg_readmodifywrite:
            l3sys_reg_read_modify_write((void**)sys,op->args.reg_readmodifywrite[0].mem_type,op->args.reg_readmodifywrite[0].addr,op->args.reg_readmodifywrite[0].mask,op->args.reg_readmodifywrite[0].value);
            break;
        case cs_reg_burstwrite:
                {
                        void *ptr = &(op->args.reg_burstwrite); /* typecast using void * to avoid type-punned pointer warning */
                        cs_reg_burstwrite_operation_t *burst_write_op = (cs_reg_burstwrite_operation_t*)ptr;
            l3sys_burst_write((void**)sys,burst_write_op->mem_type,op->args.reg_burstwrite[0].addr,burst_write_op->len,burst_write_op->value);
            break;
                }
        case cs_poll_bit:
            l3sys_poll_bit((void**)sys,
                            /* op->args.poll_bit[0].mem_type, */
                            op->args.poll_bit[0].addr,
                            op->args.poll_bit[0].mask,
                            op->args.poll_bit[0].value,
                            op->args.poll_bit[0].max_count,
                            NULL,
                            op->args.poll_bit[0].delay_between_polls_in_microseconds);
            break;
        case cs_poll:
            l3sys_poll((void**)sys,
                        /* op->args.poll_bit[0].mem_type, */
                        op->args.poll[0].addr,
                        op->args.poll[0].mask,
                        op->args.poll[0].value,
                        op->args.poll[0].cmp,
                        op->args.poll[0].max_count,
                        NULL,
                        op->args.poll[0].delay_between_polls_in_microseconds);
            break;
        case cs_reg_read:
        case cs_reg_burstread:
        case cs_operation:
            /* these functions are just for recording */
            break;
        }
        op = op->next;
    }
}



/*******************************************************************************
* cs_show
* ___________________________________________________________________________
*
* DESCRIPTION:
*   log the contents of the change set
*
* INPUTS:
*   sys - pmc_sys_handle_t
*   public_cs - change_set_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void cs_show(pmc_sys_handle_t sys, change_set_t public_cs)
{
#ifdef LOGGING_DISABLED
#else
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    cs_operation_t *op = cs->operations;
    LOG("showing %d operations in change set",cs->count);
    while (op != NULL)
    {
        switch (op->cs_operation_enum)
        {
        case cs_operation:
            LOG("begin operation: %s",op->args.operation_name);
            break;
        case cs_reg_write:
            LOG("reg_write: %s",pmc_sys_get_description_and_value((void**)sys,op->args.reg_write[0].addr,0xffffffff,op->args.reg_write[0].value));
            LOG("reg_write( addr=0x%08x, value=0x%08x )",op->args.reg_write[0].addr,op->args.reg_write[0].value);
            break;
        case cs_reg_read:
            LOG("reg_read: %s",pmc_sys_get_description_and_value((void**)sys,op->args.reg_write[0].addr,0xffffffff,op->args.reg_write[0].value));
            LOG("reg_read( addr=0x%08x ) value=0x%08x",op->args.reg_write[0].addr,op->args.reg_write[0].value);
            break;
        case cs_reg_readmodifywrite:
            LOG("read_modify_write: %s",pmc_sys_get_description_and_value((void**)sys,op->args.reg_readmodifywrite[0].addr,op->args.reg_readmodifywrite[0].mask,op->args.reg_readmodifywrite[0].value));
            LOG("read_modify_write( addr=0x%08x, mask=0x%08x, value=0x%08x )",op->args.reg_readmodifywrite[0].addr,op->args.reg_readmodifywrite[0].mask,op->args.reg_readmodifywrite[0].value);
            break;
        case cs_reg_burstwrite:
            {
            void *ptr = &(op->args.reg_burstwrite); /* typecast using void * to avoid type-punned pointer warning */
            cs_reg_burstwrite_operation_t *burst_write_op = (cs_reg_burstwrite_operation_t*)ptr;
            LOG("burst_write: %s",pmc_sys_get_description((void**)sys,burst_write_op->addr));
            LOG("burst_write( addr=0x%08x, len=%d )",burst_write_op->addr,burst_write_op->len);
            LOG_DATA(burst_write_op->addr,burst_write_op->value,burst_write_op->len,"burst_write_data");
            break;
            }
        case cs_reg_burstread:
            {
            void *ptr = &(op->args.reg_burstwrite); /* typecast using void * to avoid type-punned pointer warning */
            cs_reg_burstwrite_operation_t *burst_write_op = (cs_reg_burstwrite_operation_t*)ptr;
            LOG("burst_read: %s",pmc_sys_get_description((void**)sys,op->args.reg_burstwrite[0].addr));
            LOG("burst_read( addr=0x%08x, len=%d )",burst_write_op->addr,burst_write_op->len);
            LOG_DATA(burst_write_op->addr,burst_write_op->value,burst_write_op->len,"burst_read_data");
            break;
            }
        case cs_poll_bit:
            LOG("poll_bit: %s",pmc_sys_get_description_and_value((void**)sys,op->args.poll_bit[0].addr,op->args.poll_bit[0].mask,op->args.poll_bit[0].value));
            LOG("poll_bit( addr=0x%08x, msk=0x%08x, val=0x%08x, max_count=%d, delay_in_usec=%d )",
                op->args.poll_bit[0].addr,
                op->args.poll_bit[0].mask,
                op->args.poll_bit[0].value,
                op->args.poll_bit[0].max_count,
                op->args.poll_bit[0].delay_between_polls_in_microseconds);
    
            break;
        case cs_poll:
            LOG("poll: %s",pmc_sys_get_description_and_value((void**)sys,op->args.poll[0].addr,op->args.poll[0].mask,op->args.poll[0].value));
            LOG("poll( addr=0x%08x, msk=0x%08x, val=0x%08x, cmp=%d, max_count=%d, delay_in_usec=%d )",
                op->args.poll[0].addr,
                op->args.poll[0].mask,
                op->args.poll[0].value,
                op->args.poll[0].cmp,
                op->args.poll[0].max_count,
                op->args.poll[0].delay_between_polls_in_microseconds);
    
            break;
        }
        op = op->next;
    }
#endif
}

/*******************************************************************************
* cs_deallocate
* ___________________________________________________________________________
*
* DESCRIPTION:
*   free the contents of the change set.
*
* INPUTS:
*   public_cs - change_set_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*******************************************************************************/
void cs_deallocate(change_set_t public_cs)
{
    private_change_set_t *cs = (private_change_set_t*)public_cs;
    cs_operation_t *op = cs->operations;
    LOG("freeing %d operations in change set",cs->count);
    while (op != NULL)
    {
        cs_operation_t *next = op->next;
        PMC_FREE(&op);
        op = next;
    }
    PMC_FREE(&cs);
}
