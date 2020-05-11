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
*     The application log is created by the application and stores user inserted log 
*     entries.  The user inserted application log entries are each 32-byte (8 32-bit word)
*     long in the following format:\n
*\code     
*     |----------------------------------------------------------------------------------------------|
*     | 4-bit event severity level | 16-bit rsvd (=0)  | 9-bit log sub type      |  3-bit num words  |
*     |         (event_sev)        |                   | (osf_log_sub_type_enum) |                   |
*     |----------------------------------------------------------------------------------------------|
*     | 32-bit sequence number                                                                       |
*     |                                                                                              |
*     |----------------------------------------------------------------------------------------------|
*     | 32-bit log code (shall be in the format [16-bit module ID | 16-bit user defined field])      |
*     |                                                                                              |
*     |----------------------------------------------------------------------------------------------|
*     | 32-bit log word 2                                                                            |
*     |                                                                                              |
*     |----------------------------------------------------------------------------------------------|
*     | 32-bit log word 3                                                                            |
*     |                                                                                              |
*     |----------------------------------------------------------------------------------------------|
*     | 32-bit log word 4                                                                            |
*     |                                                                                              |
*     |----------------------------------------------------------------------------------------------|
*\endcode          
*     An application log is created in memory if the user specifies a
*     log size app_size >= sizeof(pmc_log_app_entry_struct).  User inserted 
*     entries are logged into the application log based on two sets of
*     filters: an application severity filter level and a log word1 filter
*     bank. The application filters and other application log parameters are
*     initialized to their default values in osf_log_app_init().         
*
*     The application log is circular buffer in the form of an array of 
*     "pmc_log_app_entry_struct".  There is a write index and an read index
*     to the application log.  The log is considered "empty" when the 
*     (write index == read index).  "Empty" condition means there are no unread
*     entries in the log (e.g., if no entry has been written, or if all written
*     entries have already been read.)  If the write index has wrapped around, 
*     (i.e. one full buffer of entries have not been read,) the read index will
*     be pushed one entry ahead so there there will always be at least an 
*     entry's gap between the read index and the write index.  It is the user's
*     responsibility to ensure that entries are read before the log wraps 
*     around and entries are lost.
*
*     ------------------
*     A normal sequence of starting up the log is as follows:
*     - pmc_log_init()  - Responsible for calling functions to create OSF logs
*                         and set up the log filters to their default settings:\n
*                           = Event filter is set to disable logging to the 
*                             event log.\n
*                           = Application severity filter level is set to
*                             disable logging to the application log.\n
*                           = Application log word1 filter bank is set to
*                             disable logging to the application log.\n
*                             (0 mask, 0 patter, "filter out")\n
* 
*
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "pmc_log_loc.h"
#include "pmc_log.h"
#include "pthread.h"

/*
 * If using multiple threads turn off indentation of the log file.
 */
#ifndef PMC_LOG_MULTI_THREAD_ENABLE
#define PMC_LOG_USE_INDENT
#ifdef PMC_LOG_USE_INDENT
    #define MAX_NESTING 30
#endif /* PMC_LOG_USE_INDENT */ 
#endif
/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

static const char *pmc_log_unregistered_str_output = "No string registered."; /*!< Default string when log message not found */
static const char *pmc_log_pmc_success_str_output = "PMC SUCCESS"; /*!< Default string for success */


#ifdef PMC_LOG_USE_INDENT
PRIVATE const UINT32 PMC_LOG_INDENT_MAX = 40; /*!< Set the maximum indendentation */
#endif /* PMC_LOG_USE_INDENT */ 

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/
#ifdef PMC_SW_SIMULATION
static UINT32 pmc_log_seq_counter = 0;
#endif 

#ifndef PMC_LOG_APP_PRINT_SIMPLE
/*! The following text strings are used when outputting the severity of LOG messages */
PRIVATE const char * const PMC_LOG_SEV_STR[] = {
    "PMC_LOG_SEV_DISABLE",
    "PMC_LOG_SEV_HIGHEST",
    "PMC_LOG_SEV_FATAL",
    "PMC_LOG_SEV_HIGH",
    "PMC_LOG_SEV_MEDIUM",
    "PMC_LOG_SEV_LOW",
    "PMC_LOG_SEV_LOWEST",
};
#endif

/*! digi120_pmc_log_data is used for all the logger data */
pmc_log_data_t digi120_pmc_log_data;
/*! static_buf is the circular buffer info for digi120_pmc_log_data */
PRIVATE pmc_circbuf_t static_buf;

#ifndef PMC_LOG_MULTI_THREAD_ENABLE

#define PMC_LOG_DEFAULT_STATIC_BUF_SIZE 64
/*! Set the default buffer size for the log */
static UINT32 pmc_log_default_buf_size = 1000; 
#else
#define PMC_LOG_DEFAULT_STATIC_BUF_SIZE 0
/*! Set the default buffer size for the log */
static UINT32 pmc_log_default_buf_size = 0;
#endif
/*! static_circbuf is used to store the  log during initialization before its final size is set */ 
PRIVATE pmc_buf_item_t static_circbuf[PMC_LOG_DEFAULT_STATIC_BUF_SIZE];

#ifdef PMC_LOG_USE_INDENT
/*! pmc_log_indent_level is a counter that is incremented on PMC_ENTRY
   and decremented on PMC_RETURN, cannot be negative */
PRIVATE UINT32 pmc_log_indent_level = 0;

/*! pmc_log_indent_last is used to hold the previously accepted indent value */
PRIVATE UINT32 pmc_log_indent_last = 0;

/*! indent_init tracks the initialization of the indent method, set to 1
    on the first call of the get_indent */
PRIVATE UINT8 indent_init = 0;

#endif /* PMC_LOG_USE_INDENT */ 

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

/*! Define error strings when logging text output */
const char PMC_SHARED_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    PMC_SHARED_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE
#else /* no strings */
/*! No error strings */
const char PMC_SHARED_LOG_ERR_STRINGS[1][1] = {};
#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/


/*****************************************************************************
* pmc_circbuf_init 
* ______________________________________________________________________________
* 
* DESCRIPTION: Initializes the circular buffer used by the logger
*
* INPUTS: 
*   digi_handle - the digi instance you want to set up. The logger is
*                 stored in that digi's context.
*
* OUTPUTS: Initializes and stores the cirbuf in: 
*                   digi_handle->common_info->log_buffer
* 
* RETURNS:
*   pmc_circbuf_t - pointer to the newly allocated buffer
*
* NOTES:
*
*****************************************************************************/
PUBLIC pmc_circbuf_t* pmc_circbuf_init(void* digi_handle) 
{
#ifdef PMC_LOG_MULTI_THREAD_ENABLE
    return((pmc_circbuf_t *)NULL);
#endif
    pmc_circbuf_t * retval, *buf;
    pmc_buf_item_t itm;
    UINT32 size = pmc_log_default_buf_size;
    retval = (pmc_circbuf_t*)PMC_CTXT_CALLOC(sizeof(pmc_circbuf_t),digi_handle);
    retval->buf = (pmc_buf_item_t *)PMC_CTXT_CALLOC(sizeof(pmc_buf_item_t)*size,digi_handle);
    PMC_CTXT_REGISTER_PTR(&(retval->buf), digi_handle);

    PMC_ASSERT(retval != NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0);
    retval->size = size;

    /* this doens't need to be guarded, because this only is called when
     * digi_handle is not null
     */
    buf = pmc_log_app_get();
    /* transfer all the items from the general buf to the */
    while (buf->not_empty) 
    {
        pmc_circbuf_rd(buf,&itm);
        pmc_circbuf_wr(retval,itm);
    }

    /* set up default values */
    return(retval);
}

/*****************************************************************************
* pmc_circbuf_destroy
* ______________________________________________________________________________
* 
* DESCRIPTION: destroys and frees the buffer from the context
*
* INPUTS:
*   digi_handle - digi_handle you want to free from
*   buf - pmc_circbuf that you want to free
* 
* OUTPUTS:
*   None
* 
* RETURNS:
*   None
* 
* NOTES:
*
*****************************************************************************/
PUBLIC void pmc_circbuf_destroy(void * digi_handle,pmc_circbuf_t * buf)
{
#ifdef PMC_LOG_MULTI_THREAD_ENABLE
    return;
#endif
    PMC_CTXT_FREE(&(buf->buf),digi_handle);
    PMC_CTXT_FREE(&(buf),digi_handle);
}

/*****************************************************************************
* pmc_circbuf_inc_wrap
* ______________________________________________________________________________
* 
* DESCRIPTION: a helper function to increment and couunter with a wrap around
*
* INPUTS: 
*   count - number you want to increment from
*   wrap - modulus to wrap at
*
* OUTPUTS:
*
* RETURNS:
*   UINT32 - counter + 1 % wrap
*
* NOTES:
*
*****************************************************************************/
PUBLIC UINT32 pmc_circbuf_inc_wrap(UINT32 count, UINT32 wrap)
{
#ifdef PMC_LOG_MULTI_THREAD_ENABLE
    return 0;
#endif
    return((count + 1) % wrap);
}

/*****************************************************************************
* pmc_circbuf_wr_idx_inc
* ______________________________________________________________________________
* 
* DESCRIPTION: function to increment the read counter
*
* INPUTS:
*   buf- buffer you want to increment the readpointer on
*
* OUTPUTS:
*   buf->wr_idx incremented by 1, with a wrap around at the buffer length
*
*
* RETURNS:
*
* NOTES:
*   This will increment the rd counter if the wr coutner catches it in the 
*   circbuf. This will only occure onces the read poitner has gone all the
*   way around the buffer
*
*****************************************************************************/
void pmc_circbuf_wr_idx_inc(pmc_circbuf_t * buf)
{
#ifdef PMC_LOG_MULTI_THREAD_ENABLE
    return;
#endif
    PMC_ASSERT(buf != NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0);
    /* becaues the write pointer is on the read pointer isn't bad */
    /* because the wr poitner is the first avaiable point */
    if ( buf->wr_idx == buf->rd_idx && buf->not_empty == 1)
    {
        buf->rd_idx = pmc_circbuf_inc_wrap(buf->rd_idx,buf->size);
    }
    buf->wr_idx = pmc_circbuf_inc_wrap(buf->wr_idx,buf->size);
}

/*****************************************************************************
* pmc_circbuf_rd_idx_inc
* ______________________________________________________________________________
* 
* DESCRIPTION: incrmenet the read index
*
* INPUTS:
*   buf- buffer you want to increment the read index on
*
* OUTPUTS:
*   buf->rd_idx incremented by 1 wrapped at the buffer length
* RETURNS:*
* NOTES:
*
*****************************************************************************/
void pmc_circbuf_rd_idx_inc(pmc_circbuf_t * buf)
{
#ifdef PMC_LOG_MULTI_THREAD_ENABLE
    return;
#endif
    PMC_ASSERT(buf != NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0);
    buf->rd_idx = pmc_circbuf_inc_wrap(buf->rd_idx,buf->size);
}

/*****************************************************************************
* pmc_circbuf_rd
* ______________________________________________________________________________
* 
* DESCRIPTION: read an entry out of the circular buffer
*
* INPUTS: 
*   buf- the buffer you want to read from
*   retval- a pointer to a buf item that is returned
*
* OUTPUTS:
*   retval- a read from the rd_idx of the buffer
*   if buf->not_empty == 1, buf->not_empty =0,
*   rd_idx increased by 1
* RETURNS:
*
* NOTES:
*
*****************************************************************************/
PUBLIC void pmc_circbuf_rd(pmc_circbuf_t * buf,pmc_buf_item_t * retval )
{
#ifdef PMC_LOG_MULTI_THREAD_ENABLE
    return;
#endif
    PMC_ASSERT(buf != NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0);
    /* make a return value, and grab the value from the buffer
     * The read pointer will always point at the oldest unread
     * item
     */
    
    if ( buf->not_empty ==1)
    {
        *retval = buf->buf[buf->rd_idx];
        pmc_circbuf_rd_idx_inc(buf);
        if (buf->rd_idx == buf->wr_idx)
        {
            buf->not_empty = 0;
        }
    } else {
        /* If the buffer is empty set seq num and logword 0 to -1, to show
         * that there is nothing to return
         */
        retval->ts_s = -1;
        retval->log_word0 = -1;
    }
    /* increment the read index */
}

/*****************************************************************************
* pmc_circbuf_wr    
* ______________________________________________________________________________
* 
* DESCRIPTION: write an item to the circular buffer
*
* INPUTS:
*   buf- buffer you want to write to
*   itm- the item you want to write to the circbuf
* OUTPUTS:
*   wr_indx increase by 1
*   if buf->not_empy == 0, buf->not_empty will be 1
* RETURNS:
*
* NOTES:
*
*****************************************************************************/
PUBLIC void pmc_circbuf_wr(pmc_circbuf_t * buf, pmc_buf_item_t itm)
{
#ifdef PMC_LOG_MULTI_THREAD_ENABLE
    return;
#endif
    PMC_ASSERT(buf != NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0);

    buf->buf[buf->wr_idx] = itm;
    pmc_circbuf_wr_idx_inc(buf);
    if (buf->not_empty == 0)
    {
        buf->not_empty =1;
    }
}

/*****************************************************************************
* pmc_default_buf_size_set 
* ______________________________________________________________________________
* 
* DESCRIPTION: write an item to the circular buffer
*
* INPUTS:
*   size - size you want the new argument to be
* 
* OUTPUTS: sets the default buf size to the argument
* 
* RETURNS:
* 
* NOTES:
*****************************************************************************/
PUBLIC void pmc_default_buf_size_set(UINT32 size) 
{
#ifdef PMC_LOG_MULTI_THREAD_ENABLE
    return;
#endif
    pmc_log_default_buf_size = size;
}

/*****************************************************************************
* pmc_default_buf_size_get    
* ______________________________________________________________________________
* 
* DESCRIPTION: get the default buffer size 
*  
* INPUTS: 
*   None
*  
* OUTPUTS: 
*   None
*  
* RETURNS: 
*   UINT32 - the default buffer size
*  
* NOTES:
*****************************************************************************/
PUBLIC UINT32 pmc_default_buf_size_get() 
{
    return pmc_log_default_buf_size;
}

/*atomic entry methods to set and release active handles */
/*****************************************************************************
* pmc_set_log_handle
* ______________________________________________________________________________
* 
* DESCRIPTION: a helper function used to set the the active handle on every 
*               atomic entry
*
* INPUTS:
*       pmc_handle- pmc handle passed into the atomic entry
*
* OUTPUTS:
*       digi120_pmc_log_data.active_handle = input
* RETURNS:
*
* NOTES:
*
*****************************************************************************/
void pmc_set_log_handle(pmc_handle_t * pmc_handle) 
{
    PMC_ASSERT(pmc_handle != NULL,PMC_SHARED_ERR_CODE_NULL_PTR,0,0);
    digi120_pmc_log_data.active_handle =  (void *)pmc_handle;
}

/*****************************************************************************
* pmc_release_log_handle
* ______________________________________________________________________________
* 
* DESCRIPTION: sets the active handle to null on every atomic exit
*
* INPUTS:
*   pmc_handle- the exiting pmc_handle
* OUTPUTS:
*   active_handle = NULL
* RETURNS:
*
* NOTES:
*
*****************************************************************************/
void pmc_release_log_handle(pmc_handle_t * pmc_handle)
{
    digi120_pmc_log_data.active_handle = NULL;
}

/*****************************************************************************
* pmc_log_writable
* ______________________________________________________________________________
* 
* DESCRIPTION:
*
* INPUTS:
*   event_sev   - Event severity (5 levels from highest(0) to lowest(4)) 
*   log_code    - 32-bit log code.  The format is:
*                 [ Module ID (16-bit) | Generic (16-bit) ]
* OUTPUTS:
*   None
*
* RETURNS:
*   BOOL - True if the current module/logcode is enabled for output
*            False otherwise
*
* NOTES:
* - It is the responsibility of the caller function to ensure that this
*   function is called in an atomic region. 
*
*****************************************************************************/
PUBLIC BOOL pmc_log_writable( pmc_log_sev_t event_sev,
                               UINT32        log_code ) 
{
    return  (event_sev > digi120_pmc_log_data.verbosity ? FALSE : TRUE);
}


/*****************************************************************************
* pmc_log_app_event3
* ______________________________________________________________________________
* 
* DESCRIPTION:
*   Inserts an event into the application log if application log is enabled.  
*
* INPUTS:
*   event_sev   - Event severity (5 levels from highest(0) to lowest(4)) 
*   num_words   - Number of words being logged (typically 4)
*                 application log entry.
*   log_code    - 32-bit log code.  The format is:
*                 [ Module ID (16-bit) | Generic (16-bit) ]
*   log_word2   - 32-bit log word 2.  Generic.
*   log_word3   - 32-bit log word 3.  Generic.
*   log_word4   - 32-bit log word 4.  Generic.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None if successful; otherwise, assert and never returns.
*
* NOTES:
* - It is the responsibility of the caller function to ensure that this
*   function is called in an atomic region. 
*
*****************************************************************************/
PUBLIC void pmc_log_app_event3(pmc_log_sev_t event_sev,
                               UINT8         num_words,
                               UINT32        log_code, 
                               UINT32        log_word2, 
                               UINT32        log_word3,
                               UINT32        log_word4)
{
    /* if active handle isn't set, use the default log */
    pmc_circbuf_t * buf;
    pmc_buf_item_t itm;
    itm.ts_s =0;
    itm.ts_u =0;
    itm.log_word0 = (UINT32)event_sev;
    itm.log_code  = log_code;
    itm.log_word2 = log_word2;
    itm.log_word3 = log_word3;
    itm.log_word4 = log_word4;
    PMC_TIME_GET(itm.ts_s,itm.ts_u);
    buf = pmc_log_app_get();
    
    
    pmc_circbuf_wr(buf,itm);
} /* pmc_log_app_event3() */

/*****************************************************************************
* pmc_log_app_get 
* ______________________________________________________________________________
* 
* DESCRIPTION:
*   Returns a pointer to the current active log buffer
*
* INPUTS:
*   None
*
* OUTPUTS:
*   None
*
* RETURNS:
*   pmc_circbuf_t - a pointer to the current active log
*
* NOTES: if the active handle is null, the default log is used
*
*****************************************************************************/
PUBLIC pmc_circbuf_t * pmc_log_app_get()
{
    pmc_circbuf_t * buf;

    if ( digi120_pmc_log_data.active_handle == NULL ) 
    {
        buf = digi120_pmc_log_data.default_buf;
        
    } 
    else 
    {
        pmc_handle_t * handle = (pmc_handle_t *)digi120_pmc_log_data.active_handle;
        buf = (pmc_circbuf_t*)handle->common_info->log_buffer;  
    }

    if (buf == NULL && digi120_pmc_log_data.active_handle == NULL)
    {
        digi120_pmc_log_data.default_buf = &static_buf;
        digi120_pmc_log_data.default_buf->buf = static_circbuf;
        digi120_pmc_log_data.default_buf->not_empty = 0;
        digi120_pmc_log_data.default_buf->rd_idx = 0;
        digi120_pmc_log_data.default_buf->wr_idx = 0;
        digi120_pmc_log_data.default_buf->size = PMC_LOG_DEFAULT_STATIC_BUF_SIZE;
        buf = digi120_pmc_log_data.default_buf;
    }

    return(buf);
}

/*****************************************************************************
* pmc_log_app_print
* ______________________________________________________________________________
* 
* DESCRIPTION:
*   prints an event from the active handles log buffer 
*
* INPUTS:
*   num_lines - the number of lines to print/read from the buffer.  
*
* OUTPUTS:
*   prints the lines from the log
*
* RETURNS:
*   UINT32 - number of lines printed from the log
*
* NOTES:
*   It is the responsibility of the caller function to ensure that this
*   function is called in an atomic region. 
*
*****************************************************************************/
PUBLIC UINT32 pmc_log_app_print(UINT32 num_lines)
{
    UINT32 i = 0;
    UINT32 count = 0;
    pmc_buf_item_t itm;
    char time_str[PMC_LOG_TIMESTAMP_LEN];
    pmc_circbuf_t * buf;
    char digi_name[PMC_HANDLE_MAX_TSB_NAME_LEN];
    pmc_handle_t * handle;

    if ( digi120_pmc_log_data.active_handle == NULL )
    {
        buf = digi120_pmc_log_data.default_buf; 
        PMC_SPRINTF(digi_name,"NONE");
    } else {
        handle = (pmc_handle_t *)digi120_pmc_log_data.active_handle;
        buf = (pmc_circbuf_t*)handle->common_info->log_buffer;  
        PMC_STRCPY(digi_name,handle->tsb_name);
    }
    
    for ( i = 0; i < num_lines; ++i)
    {
        pmc_circbuf_rd(buf,&itm);
        if (( itm.log_word0 == (UINT32)-1 && itm.ts_s == (UINT32)-1) || buf->not_empty == 0)
            return count;
        count = i+1;
        PMC_LOG_TIME2STR(time_str,itm.ts_s,itm.ts_u);

        #ifdef PMC_LOG_APP_PRINT_SIMPLE
        PMC_PRINT("0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\n", itm.ts_s,itm.ts_u,
            itm.log_word0,itm.log_code,itm.log_word2,itm.log_word3,
            itm.log_word4);
        #else
         
        PMC_PRINT("[%s][%s][%s] %s: 0x%08X 0x%08X 0x%08X\n",time_str, digi_name,
            PMC_LOG_SEV_STR[itm.log_word0],pmc_log_string_get(itm.log_code),
            itm.log_word2, itm.log_word3, itm.log_word4);
        #endif
    }

    return count;
}
/*******************************************************************************
* pmc_log_verbosity_level_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the verbosity level.
*
* INPUTS:
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Verbosity level
* 
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC pmc_log_sev_t pmc_log_verbosity_level_get()
{
    return digi120_pmc_log_data.verbosity;
} /* pmc_log_verbosity_level_get() */

/*******************************************************************************
* pmc_log_module_verbosity_level_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the verbosity level of a module.
*
* INPUTS:
*   log_code - log code to check the verbosity against (currently unused)
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   Verbosity level of a module. If module does not exist, it returns
*   PMC_LOG_SEV_NUM .
* 
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC pmc_log_sev_t pmc_log_module_verbosity_level_get(UINT32 log_code)
{
    return digi120_pmc_log_data.verbosity;
}


/*******************************************************************************
* pmc_log_verbosity_level_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the verbosity level as passed in by the parameter
*   level.
*
* INPUTS:
*   level - the verbosity level to set. Any messages at this level or higher
*           are logged/output.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC void pmc_log_verbosity_level_set(pmc_log_sev_t level)
{
    /* check against the compiler override symbols */
#if defined(PMC_LOG_GLOBAL_SEV_DISABLE)
    level = PMC_LOG_SEV_DISABLE;
#elif defined(PMC_LOG_GLOBAL_SEV_HIGHEST)
    level = PMC_LOG_SEV_HIGHEST;
#elif defined(PMC_LOG_GLOBAL_SEV_FATAL)
    level = PMC_LOG_SEV_FATAL;
#elif defined(PMC_LOG_GLOBAL_SEV_HIGH)
    level = PMC_LOG_SEV_HIGH;
#elif defined(PMC_LOG_GLOBAL_SEV_MEDIUM)
    level = PMC_LOG_SEV_MEDIUM;
#elif defined(PMC_LOG_GLOBAL_SEV_LOW)
    level = PMC_LOG_SEV_LO;
#elif defined(PMC_LOG_GLOBAL_SEV_LOWEST)
    level = PMC_LOG_SEV_LOWEST;
#endif

    digi120_pmc_log_data.verbosity = level;

} /* pmc_log_verbosity_level_set() */

/*******************************************************************************
* pmc_log_module_verbosity_level_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the verbosity level of a module as passed in by the
*   parameter level and log_code.
*
* INPUTS:
*   level - the verbosity level to set. Any messages at this level or higher
*           are logged/output.
*   log_code - fo module id level is for. Module must be in digi120_pmc_log_data table,
*               in order to its verbosity level to be set.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC void pmc_log_module_verbosity_level_set(pmc_log_sev_t level, UINT32 log_code)
{   
}

/*******************************************************************************
* pmc_log_all_module_verbosity_level_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set verbosity level of all registered modules to level.
*
* INPUTS:
*   level - the verbosity level to set. Any messages at this level or higher
*           are logged/output.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC void pmc_log_all_module_verbosity_level_set(pmc_log_sev_t level)
{    
    UINT32 i=0;

    /* Check if severity supplied is in range. */
    if((level< PMC_LOG_SEV_DISABLE) || (level>=PMC_LOG_SEV_NUM))
    {
        /* Can't do anything here, out of range. */
        return;
    }

    /* Loop through all the arrays and set verbosity of all registered modules. */
    i = 0;
    while (i < digi120_pmc_log_data.num_registered)
    {        
        digi120_pmc_log_data.verbosity_list[i++]=level;
    }    
}

/*******************************************************************************
* pmc_log_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the log information. Since logging is a very low
*   level module and has global verbosity settings there is no handle returned
*   from this module.  When this module returns the verbosity level is set so
*   only the highest level messages are displayed.
*   
* INPUTS:
*   size - size of the log to create/init
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC void pmc_log_init(UINT32 size)
{
    if (TRUE == pmc_is_logger_initialized())
    {
        return;
    }

    /* Init number of registered modules. */
    digi120_pmc_log_data.num_registered = 0;               

    /* Set a global sev that can be assigned by the macro definition 
     * passed to the makefile
     */
    pmc_log_sev_t global_log_sev = PMC_LOG_SEV_MEDIUM;

    /* no need to check against the gcc symbols, thats done in verbosity level set */

    /* set the global verbosity */
    pmc_log_verbosity_level_set(global_log_sev);

    /* set initial active handle to null */
    digi120_pmc_log_data.active_handle = NULL;

    /* Make sure that logs could be dumped to console. */
    digi120_pmc_log_data.print_to_stdout = TRUE;

    /* Register log strings. */
    pmc_log_block_strings_register(PMC_SHARED_LOG_ERR_STRINGS[0], PMC_SHARED_LOG_ERR_TABLE_BASE, PMC_SHARED_LOG_ERR_COUNT);
    /*set the size of the log */
    pmc_default_buf_size_set(size);
    if ( NULL == digi120_pmc_log_data.default_buf){
        digi120_pmc_log_data.default_buf = &static_buf;
        digi120_pmc_log_data.default_buf->buf = static_circbuf;
        digi120_pmc_log_data.default_buf->not_empty = 0;
        digi120_pmc_log_data.default_buf->rd_idx = 0;
        digi120_pmc_log_data.default_buf->wr_idx = 0;
        digi120_pmc_log_data.default_buf->size = PMC_LOG_DEFAULT_STATIC_BUF_SIZE;
    }
    /* set up filters */
/*    pmc_log_filter_init();*/
    
} /* pmc_log_init() */


/*******************************************************************************
* pmc_log_string_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function searches and returns the string associated with an error or
*   log code. Strings and their range are registered using the function
*   pmc_log_block_strings_register. If the string is found it is returned, if not
*   found than an generic string is returned so that progress in the program
*   can continue.
*
* INPUTS:
*   log_code - log/error code to search and find the string for. This
*              should be in standard log string format although the algorithm
*              will search for any range returning the string it first finds
*              in range.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Pointer to a string to output.
* 
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC const char *pmc_log_string_get(PMC_ERROR log_code)
{
    const char *string_ptr = pmc_log_unregistered_str_output;
    BOOL found = FALSE;
    UINT32 i;
    UINT32 log_offset;

    /* Check for PMC_SUCCESS */
    if (PMC_SUCCESS == (log_code & 0xffff0000))
    {
        return pmc_log_pmc_success_str_output;
    }
    
    /* Loop through all the arrays looking for the index */
    i = 0; 
    while (found == FALSE && i < digi120_pmc_log_data.num_registered) 
    {
        /* Check if the correct module ID*/
        if ((log_code & 0xffff0000) == (digi120_pmc_log_data.base_code_list[i] & 0xffff0000)) 
        {   /* Correct module ID, check range to ensure string exists */
            found = TRUE;

            if (log_code < digi120_pmc_log_data.last_code_list[i])
            {
                /* Log code in range so pull out of array */
                log_offset = log_code & 0xffff;
                string_ptr = digi120_pmc_log_data.string_list_ptr[i]; /* This is the base string */
                string_ptr += (PMC_LOG_STRING_SIZEOF * log_offset); /* This is the base string */
            }
            else
            {   /* Log code out of range so use generic string */
                string_ptr = pmc_log_unregistered_str_output;
            }
        }
        i++;
    }

    /* Check that the string was found - maybe this is a generic error later */
    if (FALSE == found) 
    {
        string_ptr = pmc_log_unregistered_str_output;
    }

    return(string_ptr);
} /* pmc_log_string_get() */


/*******************************************************************************
* pmc_log_block_strings_register
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function allows a block to register its list of strings with the log
*   module and the error code base and range so that when an error string is
*   to be output the code can be used to find the string.
*
* INPUTS:
*   strings_ptr           - base string in the list
*   base_code             - base error code to start the compare
*   last_code             - last code in the range
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
* 
* NOTES:
*   This function asserts if the list is full since all the blocks should be
*   registered during test.
*
*******************************************************************************/
PUBLIC void pmc_log_block_strings_register(const char *strings_ptr, UINT32 base_code, UINT32 last_code)
{
    UINT32 idx;
    BOOL   module_registered = FALSE;

    /*
     * Lock the mutex.
     */
    pmc_logger_mutex_set(TRUE);

    /* Make sure there aren't too many registerd */
    PMC_ASSERT(digi120_pmc_log_data.num_registered < PMC_LOG_MAX_NUM_REGISTERED_MODULES, 
               PMC_SHARED_ERR_CODE_TOO_MANY_LOG_MODULE, 
               digi120_pmc_log_data.num_registered, 0x0);

    /* Check if the module is already registered. */
    for (idx = 0; idx < digi120_pmc_log_data.num_registered; idx++)
    {
        if (base_code == digi120_pmc_log_data.base_code_list[idx])
        {
            module_registered = TRUE;
            break;
        }
    }

    /* Register the module if not yet registered. */
    if (!module_registered)
    {
        digi120_pmc_log_data.string_list_ptr[digi120_pmc_log_data.num_registered] = strings_ptr;
        digi120_pmc_log_data.base_code_list[digi120_pmc_log_data.num_registered] = base_code;
        digi120_pmc_log_data.last_code_list[digi120_pmc_log_data.num_registered] = last_code;
        digi120_pmc_log_data.verbosity_list[digi120_pmc_log_data.num_registered] = PMC_LOG_SEV_NUM;
        digi120_pmc_log_data.num_registered++;
    } 
   /*
     * Unlock the mutex
     */
    pmc_logger_mutex_set(FALSE);
} /* pmc_log_block_strings_register() */


/*****************************************************************************
* pmc_log_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear the application log.  This function resets the write and read indices 
*   to the application log, but does not "clear" the memory.  
*
* INPUTS:
*   None
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
*
* NOTES:
* 
*****************************************************************************/
PUBLIC void pmc_log_clear(void)
{
}


/*****************************************************************************
* pmc_log_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reads the oldest unread entry from the application log.  The read index 
*   is moved ahead after the read.  Therefore, the entry can only be read once.
*
* INPUTS:
*   None
*
* OUTPUTS:
*   entry_ptr   - Pointer to where the read entry is written to.
*   num_missed_entries_ptr  - Pointer to number of missed entries since
*                             last read.
*
* RETURNS:
*   PMC_SUCCESS         - If a previously unread entry is read 
*   PMC_ERR_LOG_EMPTY   - No unread entries found
*
* NOTES:
* 
*****************************************************************************/
PUBLIC PMC_ERROR pmc_log_read(pmc_log_app_entry_struct *entry_ptr,
                              UINT32                   *num_missed_entries_ptr)
{
   return(PMC_SUCCESS);
}


/* if no indent is used, make the methods return nothing */
#ifndef PMC_LOG_USE_INDENT 
PUBLIC UINT32 pmc_log_indent_get(void * stack_ptr)
{
    return 0;
}
PUBLIC void pmc_log_indent_inc(void) 
{
}

PUBLIC void pmc_log_indent_dec(void) 
{
}

#else /* PMC_LOG_USE_INDENT DEFINED */
/*******************************************************************************
* pmc_log_indent_get 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   method to determine the correct indentation of the current frame stack
*   combines 2 methods for determineing the stack size
*
* INPUTS:
*   stack_ptr - a void pointer to a memory location of the calling function
*       used by the pmc_log_nesting_get function
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - the current accept indentation level as an unsigned integer
* 
* NOTES:
*   calculates the indentation level of the program by choosing the smaller
*    of the indentatin levels generate from the current indentation level
*   and the get_netsing function.
*   if the current indentation level is greater than the max, the max will be
*   returned
*******************************************************************************/
PUBLIC UINT32 pmc_log_indent_get(void * stack_ptr)
{
    /* the first time through the method, zero the indent_level */
    /* this is to make this behave similar to the nesting function */
    if (indent_init == 0)
    {
        indent_init = 1;
        pmc_log_indent_level = 0; 
    }
    
    return(pmc_log_indent_level);

} /* pmc_log_indent_get() */

/*******************************************************************************
*  pmc_log_indent_inc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   helper function to increment the indent counter
*
* INPUTS:
*   None
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
* 
* NOTES:
*   
*******************************************************************************/
PUBLIC void pmc_log_indent_inc(void) 
{
    pmc_log_indent_level++;
}

/*******************************************************************************
* pmc_log_indent_dec 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   helper method to decrement the indent counter, if the counter is not zero
*
* INPUTS:
*   None
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None
* 
* NOTES:
*   
*******************************************************************************/
PUBLIC void pmc_log_indent_dec(void) 
{
    if ( pmc_log_indent_level > 0 )
    {
        pmc_log_indent_level--;
    }
}


#endif /* PMC_LOG_USE_INDENT */

/*******************************************************************************
* vaLog 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   legacy method for producing logs, modified to use PMC_LOG_TRACE
*
* INPUTS:
*   file_name - filename at the point of logging 
*   line_num - line number at the point of logging
*   function_name - function name at the point of logging
*   fmt - Variadic arguments similar to printf()
* 
* OUTPUTS:
*   filename, current line, function name and any other message string
*   passed into the method
*
* RETURNS:
*   None.
* 
* NOTES:
*   this code is present for legacy reasons. some of the subsystems have there
*   own logging marcos that call this
*
*******************************************************************************/
void vaLog( const char *file_name, int line_num, const char *function_name, const char *fmt, ... )
{
    pmc_log_sev_t sev = PMC_LOG_SEV_LOW;
    va_list ap; 
    if ( pmc_log_writable(sev,PMC_SHARED_LOG_CODE_LEGACY) == TRUE)
    {
        if (NULL == digi120_pmc_log_data.active_handle)
        {
            PMC_PRINT("IO_LOG: %s(%d): ", function_name, line_num);
        }
        else
        {
            PMC_PRINT("DIGI %0llx: IO_LOG: %s(%d): ",(UINT64)((PMC_UINTPTR)digi120_pmc_log_data.active_handle), function_name, line_num);
        }
        va_start(ap,fmt);
        vprintf(fmt,ap); /* vprintf() allowed in porting function */
        va_end(ap);
        PMC_PRINT_NO_INDENT(" \n");
    }
}

#ifdef USE_FW_LOG
static FILE *firmware_log = NULL;
static void open_firmware_log()
{
    if (firmware_log == NULL) {
        firmware_log = fopen("fwlog.txt","w");
    }
}

/*******************************************************************************
* fwLog
* ___________________________________________________________________________
*
* DESCRIPTION:
*  print a line to the firmware log file "fwlog.txt"
*
* INPUTS:
*    fmt - format string similar to printf()
*    ... - a variable number of arguments similar to printf()
*
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void fwLog( const char *fmt, ... )
{
    va_list ap; 
    
    /* display message to stdout */
    va_start(ap,fmt);
    vprintf(fmt,ap); /* vprintf() allowed in porting function */

    /* also write message to fwlog.txt */
    open_firmware_log();
    if (firmware_log != NULL) {
        va_start(ap,fmt);
        vfprintf(firmware_log,fmt,ap); /* vfprintf() allowed in porting function */
        fflush(firmware_log); /* fflush() allowed in porting function */ 
    }

    va_end(ap);    
}

/*******************************************************************************
* fwLog
* ___________________________________________________________________________
*
* DESCRIPTION:
*  print a line to the firmware log file "fwlog.txt"
*
* INPUTS:
*    fmt - format string similar to printf()
*    ... - a variable number of arguments similar to printf()
*
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void fwvLog( const char *fmt, va_list ap )
{
    /* also write message to fwlog.txt */
    open_firmware_log();
    if (firmware_log != NULL) {
        vfprintf(firmware_log,fmt,ap); /* vfprintf() allowed in porting function */
        fflush(firmware_log); /* fflush() allowed in porting function */ 
    }
}
#endif

/*
** End of file
*/
