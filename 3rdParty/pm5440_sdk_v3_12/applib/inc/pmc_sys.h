/*******************************************************************************
*   COPYRIGHT (C) 2008-2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                  
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
*    This file contains PMC system access function definitions. This file is
*    setup to support customer systems as well as the PMC EVBD, emulation,
*    simulation, and verification environment. There is no customer porting
*    required in this file that occurs in pmc_customer_os.h and
*    pmc_customer_os.c.
* 
*   NOTES: 
*
******************************************************************************/

#ifndef PMC_SYS_H
#define PMC_SYS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "pmc_types.h"
#include "pmc_err.h"
#include "l1_sys.h"

/* Attribute for unused functions and variables */
#define UNUSED __attribute__ ((unused))

/* mem_pool mem_type enums */
/* Shared memory */
#define PMC_MEM_POOL_TYPE_SHARED 0
/* Memory for saving context in. */
#define PMC_MEM_POOL_TYPE_CONTEXT 1

/* mem_pool alignment enums */
#define PMC_MEM_POOL_ALIGN_NONE (pmc_mem_align_enum)0
#define PMC_MEM_POOL_ALIGN_BYTE (pmc_mem_align_enum)0
/* Use 64-bit alignment for context */
#define PMC_MEM_POOL_ALIGN_CTXT (pmc_mem_align_enum)3
/* Use 17-bit alignment to allow 128KB of space for digi_pmon_t */
#define PMC_MEM_POOL_ALIGN_PMON (pmc_mem_align_enum)17


#define DIGI_REVISION_CODE_REV_A    0x0
#define DIGI_REVISION_CODE_REV_B    0x1
#define PMC_SYS_INT_CACHE_MAX_SIZE 1024
#define PMC_SYS_THREAD_ID_UNSET     0xFFFFFFFF

#ifdef PMC_FUNCTION_PROFILER
void enable_function_profiling(char *filename);
void disable_function_profiling();
void __cyg_profile_func_enter(void *func, void *caller) __attribute__((no_instrument_function));

void __cyg_profile_func_exit(void *func, void *caller) __attribute__((no_instrument_function));
#endif

/** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: pmc_sys_event_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to configure event montoring.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct pmc_sys_event_cfg_t
{
    BOOL forever; /*!< Run the event loop while forever=TRUE */
    BOOL polling_mode; /*!< if TRUE use polling, if False use interrupts. */
    UINT32 polling_mode_wait_us; /*!< Polling mode delay */
    void (*cb)(void *); /*!< Event callback function. */
    void *cb_parameter; /*!< Parameter to pass to the callback function. */
} pmc_sys_event_cfg_t;

/*******************************************************************************
* STRUCTURE: pmc_sys_timeval_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to record time of day.
*
* ELEMENTS:
*       tv_sec            - seconds
*       tv_usec           - microseconds
*******************************************************************************/
typedef struct {
    UINT32 tv_sec;
    UINT32 tv_usec;
} pmc_sys_timeval_t;
  
/*******************************************************************************
* STRUCTURE: pmc_sys_stat_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to gather timing statistics.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct pmc_sys_stat_t
{
    struct pmc_sys_stat_t *next; /*!< next structure (a global linked list of stat structures) */
    UINT8 initialized;        /*!< 0 means uninitialized, 1 means initialized. */         
    const char *description;  /*!< string used in pmc_sys_stat_show_all() */   
    const char *description2; /*!< additional string used in pmc_sys_stat_show_all() */  
    UINT32 num; /*!< number of samples. */
    UINT64 total; /*!< total time in microseconds between all pmc_sys_stat_mark_start() and pmc_sys_stat_mark_end(). */
    UINT64 min; /*!< smallest interval observed (in microseconds). */
    UINT64 max; /*!< largest interval observed (in microseconds). */
    pmc_sys_timeval_t mark; /*!< value of gettimeofday() when pmc_sys_stat_mark_start() was called. */
    INT8 mark_started; /*!< set to 1 by pmc_sys_stat_mark_start(), set to 0 in pmc_sys_stat_mark_end(). */
    INT8 display_average; /*!< flag indicating that min and max are not valid and only total and average should be displayed. */

    UINT32 atomic_num; /*!< number of pmc_atomic samples. */
    UINT64 atomic_total; /*!< total time in microseconds where the mutex was lock */
    UINT64 atomic_min; /*!< smallest interval with mutex lock  observed (in microseconds). */
    UINT64 atomic_max; /*!< largest interval with mutex lock observed (in microseconds). */
} pmc_sys_stat_t;


/*******************************************************************************
* STRUCTURE: pmc_sys_dstat_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to gather statistics on a floating point variable.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct pmc_sys_dstat_t
{
    struct pmc_sys_dstat_t *next; /*!< next structure (a global linked list of stat structures) */
    UINT8 initialized;        /*!< 0 means uninitialized, 1 means initialized. */         
    const char *description;  /*!< string used in pmc_sys_stat_show_all() */   
    const char *description2; /*!< additional string used in pmc_sys_stat_show_all() */  
    UINT32 num; /*!< number of samples. */
    double total; /*!< total time in microseconds between all pmc_sys_stat_mark_start() and pmc_sys_stat_mark_end(). */
    double min; /*!< smallest interval observed (in microseconds). */
    double max; /*!< largest interval observed (in microseconds). */
    pmc_sys_timeval_t mark; /*!< value of gettimeofday() when pmc_sys_stat_mark_start() was called. */
    INT8 mark_started; /*!< set to 1 by pmc_sys_stat_mark_start(), set to 0 in pmc_sys_stat_mark_end(). */
    INT8 display_average; /*!< flag indicating that min and max are not valid and only total and average should be displayed. */
} pmc_sys_dstat_t;


/*******************************************************************************
* STRUCTURE: pmc_sys_fw_stats_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure used to gather timing statistics used by firmware.
*   times are in units of 300 MHz
*
* ELEMENTS:
*   see elements
*******************************************************************************/
typedef struct pmc_sys_fw_stats_t { 
    char   description[18]; /*!< description,... this is so that the host does not need an enumerated list that matches the firmware. */
    BOOL8  initialized;     /*!< has this statistics structure been initialized?                                                      */
    BOOL8  mark_started;    /*!< has stats_mark_start been called?                                                                    */
    UINT32 mark;            /*!< when stats_mark_start was last called                                                                */
    UINT32 isr_mark;        /*!< number of isr ticks when stat was started                                                            */
    UINT64 num;             /*!< number of times stats_mark_end() called.                                                             */
    UINT64 period_sum;      /*!< cumulative value of all periods (time between calls to stats_mark_start).                            */
    UINT32 period_min;      /*!< minimum period (time between calls to stats_mark_start)                                              */
    UINT32 period_max;      /*!< maximum period (time between calls to stats_mark_start)                                              */
    UINT64 duration_sum;    /*!< cumulative time spent in routine (wall clock - including ISRs).                                      */
    UINT32 duration_min;    /*!< minimum duration                                                                                     */
    UINT32 duration_max;    /*!< maximum duration                                                                                     */
    UINT64 user_sum;        /*!< cumulative time spent in routine excluding time spent in ISRs.                                       */
    UINT32 user_min;        /*!< minimum duration excluding time spent in ISRs                                                        */
    UINT32 user_max;        /*!< maximum duration excluding time spent in ISRs                                                        */
                                              
} pmc_sys_fw_stats_t;   
                  
PUBLIC void pmc_sys_dstat_show_to_buffer(pmc_sys_dstat_t *s,const char *prefix,char *buffer);

PUBLIC void pmc_sys_fw_stat_show(pmc_sys_fw_stats_t *s,const char *prefix);

#ifndef PMC_NO_GLOBAL_STATS
/* define a statistics structure,... note: this is a 'static' variable,...
 * on the first call to pmc_sys_stat_mark_start(), this structure is linked with
 * the global list of statistics structures.
 *
 * Note: hostmsg.h also defines statistic records with the macro HOSTMSG_DECLARE_CFG_MSG_STAT()
 */
#define PMC_SYS_STAT_DECLARE( var, _description ) static pmc_sys_stat_t var[1] = {{0,0,_description,""}}
#define PMC_SYS_DSTAT_DECLARE( var, _description ) static pmc_sys_dstat_t var[1] = {{0,0,_description,""}}

/* defines a static statistic structure for use with PMC_ATOMIC_ENTRY */
#define PMC_SYS_STAT_DECLARE_FOR_THIS_FUNCTION( var ) static pmc_sys_stat_t var[1] = {{0,0,__FUNCTION__,""}}

/* defines a statistic structure with one supplied description and another description that is the function name,
 */
#define PMC_SYS_STAT_DECLARE_WITH_FUNCTION( var, _description ) static pmc_sys_stat_t var[1] = {{0,0,_description,__FUNCTION__}}
#else
/* define a statistics structure,... note: this is an 'auto' variable if declared inside a function.
 * this structure is never linked with the global list of statistics structures.
 *
 * Note: hostmsg.h also defines statistic records with the macro HOSTMSG_DECLARE_CFG_MSG_STAT()
 */
#define PMC_SYS_STAT_DECLARE( var, _description )  pmc_sys_stat_t var[1] = {{0,1,_description,""}}
#define PMC_SYS_DSTAT_DECLARE( var, _description )  pmc_sys_dstat_t var[1] = {{0,1,_description,""}}

/* defines a static statistic structure for use with PMC_ATOMIC_ENTRY */
#define PMC_SYS_STAT_DECLARE_FOR_THIS_FUNCTION( var )  pmc_sys_stat_t var[1] = {{0,1,__FUNCTION__,""}}

/* defines a statistic structure with one supplied description and another description that is the function name,
 */
#define PMC_SYS_STAT_DECLARE_WITH_FUNCTION( var, _description )  pmc_sys_stat_t var[1] = {{0,1,_description,__FUNCTION__}}
#endif

/*! Declare type for the statistics callback */
typedef void (*pmc_sys_stat_callback_t)(void*);

#define pmc_sys_stat_bench( _cb, param, duration_in_seconds, description ) pmc_sys_stat_bench2(_cb,NULL,param,duration_in_seconds,description)

PUBLIC DOUBLE pmc_sys_stat_bench2( void *_cb, void *_final_cb, void *param, DOUBLE duration_in_seconds, const char *description );

PUBLIC void pmc_sys_stat_mark_start(pmc_sys_stat_t *s);
PUBLIC void pmc_sys_stat_mark_end(pmc_sys_stat_t *s);
PUBLIC void pmc_sys_stat_mark_cancel(pmc_sys_stat_t *s);
PUBLIC UINT64 pmc_sys_stat_running_time(pmc_sys_stat_t *s);

PUBLIC const char *pmc_sys_get_units(DOUBLE dt,DOUBLE *mult);

PUBLIC char *pmc_sys_print_time(DOUBLE dt,DOUBLE mult,char *buff);

PUBLIC void pmc_sys_stat_set_num(pmc_sys_stat_t *s, UINT32 num);

PUBLIC void pmc_sys_stat_show_all( void );


PUBLIC void pmc_sys_stat_merge(pmc_sys_stat_t *s, pmc_sys_stat_t *x);

PUBLIC void pmc_sys_dstat_merge(pmc_sys_dstat_t *s, pmc_sys_dstat_t *x);

PUBLIC void pmc_sys_dstat_record_duration(pmc_sys_dstat_t *s, double dt);

PUBLIC DOUBLE pmc_sys_stat_calc_avg(pmc_sys_stat_t *s);

PUBLIC DOUBLE pmc_sys_stat_show_avg(pmc_sys_stat_t *s,const char*prefix);

PUBLIC void pmc_sys_stat_show(pmc_sys_stat_t *s,const char *prefix);

#ifdef PMC_TESTS 
void pmc_sys_schedule_crash_event(UINT32 crash_event);
UINT32 pmc_sys_crash_event_get(void);
#endif

#define PMC_STATS_ENTRY() \
    PMC_SYS_STAT_DECLARE(this_function_stats,__FUNCTION__); \
    pmc_sys_stat_mark_start(this_function_stats); \
        PMC_ENTRY();

#define PMC_STATS_RETURN(...) do {    \
        pmc_sys_stat_mark_end(this_function_stats);    \
        PMC_RETURN(__VA_ARGS__);                                        \
} while(0)

PUBLIC void pmc_sys_int_cache_enable_set(pmc_sys_handle_t *sys_handle, BOOL8 enable, BOOL8 clear);

/*---- glocal variables -------------------------------------------------*/
/* Prototypes for functions defined in pmc_sys.c */
PUBLIC BOOL8 pmc_lof_monitoring_in_fw(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_enable_otu4_clock_cfg_in_fw(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_odu_rcp_enable_all_chnls(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_mux_empty_ho_order_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_dmx_empty_ho_order_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_schd_extra_channel_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_asymmetric_msi_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_enable_odujat_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_lineotn_remove_null_chnl_id_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_dcpb_one_trib_slot_min_buffer_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_mpmo_mpma_reg_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_is_digi_rev_a_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_cpb_reset_pointer_access_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_cpb_pmpm_block_remap(pmc_handle_t * pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_mux_empty_ho_keep_in_sync_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_dmx_mgen_ipt_valid_toggle(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_dmx_two_stage_proxy_mo_chnl_prov_revision(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_prov_all_prov_mode_available(pmc_handle_t *pmc_handle_ptr);
PUBLIC BOOL8 pmc_coreotn_odujat_enable_for_128B_pkt(pmc_handle_t *pmc_handle_ptr);

pmc_sys_handle_t *pmc_sys_handle_create(void);
void pmc_sys_handle_destroy(pmc_sys_handle_t **pmc_sys_handle);
PMC_ERROR pmc_sys_init(pmc_sys_handle_t *sys, void *base_addr, const char *name );
PUBLIC FILE* pmc_sys_file_open(const char* path_ptr);
PUBLIC FILE* pmc_sys_output_file_open(const char* path_ptr);
PUBLIC PMC_ERROR pmc_sys_file_close(FILE* stream_ptr);
PUBLIC PMC_ERROR pmc_sys_file_read(FILE *f_ptr, UINT8* buffer_ptr, UINT32 *offset_ptr, UINT8 *rd_len_ptr);
PUBLIC void pmc_mem_init(pmc_sys_handle_t *sys_handle);
PUBLIC pmc_mem_pool_handle pmc_mem_pool_create(pmc_sys_handle_t pmc_sys_handle,
                                               UINT32 buf_size,
                                               UINT32 num_buf,
                                               UINT16 mem_type,
                                               pmc_mem_align_enum alignment);
PUBLIC PMC_ERROR pmc_mem_ctxt_save(void *ctxt_ptr,
                                   UINT32 ctxt_size,
                                   pmc_sys_handle_t *sys_handle);
PUBLIC PMC_ERROR pmc_mem_ctxt_restore(void *ctxt_ptr,
                                      UINT32 ctxt_size,
                                      pmc_sys_handle_t *sys_handle);
PUBLIC void pmc_mem_pool_destroy_all(pmc_sys_handle_t **sys_handle);
PUBLIC void *pmc_mem_pool_alloc(pmc_mem_pool_handle pool, UINT32 buf_size);
PUBLIC void pmc_mem_pool_free(pmc_mem_pool_handle pool, void **buf_pptr);
PUBLIC void pmc_mem_pool_write(pmc_mem_pool_handle pool,
                               void *dst_ptr,
                               void *src_ptr,
                               UINT32 length);
PUBLIC void pmc_mem_pool_read(pmc_mem_pool_handle pool,
                              void *dst_ptr,
                              void *src_ptr,
                              UINT32 length);
PUBLIC UINT64 pmc_convert_mem_pool_addr_to_physical_addr(pmc_mem_pool_handle     pool,
                                                         void                   *local_ptr);
PUBLIC void pmc_sys_event_init(pmc_sys_handle_t *sys_handle);
PUBLIC void pmc_sys_event_destroy(pmc_sys_handle_t *sys_handle);
#ifndef PMC_DOX_FOR_SWIG
PUBLIC void pmc_sys_event_register(pmc_sys_handle_t *sys_handle, pmc_sys_event_cfg_t *event_cfg);
#endif /* PMC_DOX_FOR_SWIG */
PUBLIC void pmc_sys_event_monitor(pmc_sys_handle_t *sys_handle, pmc_sys_event_cfg_t *event_cfg);
PUBLIC UINT8 count_trailing_zeros(UINT16 x);

PUBLIC void* pmc_atomic_create(void);
PUBLIC void pmc_atomic_delete(void **mutex_pptr);
PUBLIC void pmc_atomic_start(void *mutex_ptr);
PUBLIC void pmc_atomic_end(void *mutex_ptr);
PUBLIC void pmc_atomic_internal_delete(pmc_mutex_ctxt_t *mutex_ctxt);
PUBLIC BOOL pmc_atomic_yield_is_allowed(pmc_sys_handle_t *sys_handle);
PUBLIC void pmc_atomic_internal_start(pmc_sys_handle_t *sys_handle);
PUBLIC void pmc_atomic_internal_end(pmc_sys_handle_t *sys_handle);
PUBLIC void pmc_atomic_internal_force(pmc_sys_handle_t *sys_handle, BOOL8 lock);
PUBLIC BOOL pmc_atomic_is_nested(pmc_sys_handle_t *sys_handle);
PUBLIC BOOL8 pmc_atomic_continue(pmc_handle_t *digi_handle);
PUBLIC UINT32 pmc_thread_id_get(pmc_sys_handle_t *sys_handle);
PUBLIC UINT32 pmc_atoi(char *str);

PUBLIC void digi_use_float32_set(BOOL8 float_mode);
PUBLIC BOOL8 digi_use_float32_get(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PMC_SYS_H */
