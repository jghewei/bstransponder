/******************************************************************************
 *   COPYRIGHT (C) 2013 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
 * -------------------------------------------------------------------------
 *   DESCRIPTION:
 *     contains the inline implementation of all the field accessor functions
 *     and register accessor functions for the rxjc_proc block
 *****************************************************************************/
#ifndef _RXJC_PROC_IO_INLINE_H
#define _RXJC_PROC_IO_INLINE_H

#include "rxjc_proc_loc.h"
#include "rxjc_proc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RXJC_PROC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for rxjc_proc
 * ==================================================================================
 */
/* if logging is disabled then so is IO_LOGGING */
#ifndef NO_IO_LOGGING
# ifdef LOGGING_DISABLED
#  define NO_IO_LOGGING
# endif
#endif

/* log messages in the generated files log the caller's file/line/function instead of itself.*/
#ifndef IOLOG
# ifdef NO_IO_LOGGING
#  define IOLOG(...)
# else
#  define IOLOG(...) LOG( __VA_ARGS__)
# endif
#endif
#ifndef IO_RANGE_CHECK
# ifdef NO_IO_LOGGING
#  define IO_RANGE_CHECK(...)
# else
#  define IO_RANGE_CHECK(...) LOG( "** range check ** " __VA_ARGS__)
# endif
#endif
#ifndef IOLOG_DATA
# ifdef NO_IO_LOGGING
#  define IOLOG_DATA(...)
# else
#  define IOLOG_DATA(...) LOG_DATA( __VA_ARGS__)
# endif
#endif
typedef struct {
    coalesce_buffer_t coalesce_handle[1]; /* only used if register coalescing is enabled */
    rxjc_proc_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} rxjc_proc_buffer_t;
static INLINE void rxjc_proc_buffer_init( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void rxjc_proc_buffer_init( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "rxjc_proc_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void rxjc_proc_buffer_flush( rxjc_proc_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rxjc_proc_buffer_flush( rxjc_proc_buffer_t *b_ptr )
{
    IOLOG( "rxjc_proc_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 rxjc_proc_reg_read( rxjc_proc_buffer_t *b_ptr,
                                         rxjc_proc_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_reg_read( rxjc_proc_buffer_t *b_ptr,
                                         rxjc_proc_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                b_ptr->base_address,
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                ((pmc_handle_t *)h_ptr)->base_address + reg);
    return value;
}
static INLINE void rxjc_proc_reg_write( rxjc_proc_buffer_t *b_ptr,
                                        rxjc_proc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_write( rxjc_proc_buffer_t *b_ptr,
                                        rxjc_proc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         b_ptr->base_address,
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         ((pmc_handle_t *)h_ptr)->base_address + reg,
                         value);
}

static INLINE void rxjc_proc_field_set( rxjc_proc_buffer_t *b_ptr,
                                        rxjc_proc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_set( rxjc_proc_buffer_t *b_ptr,
                                        rxjc_proc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_field_set2( b_ptr->coalesce_handle,
                          mem_type,
                          b_ptr->base_address,
                          reg,
                          mask,
                          unused_mask,
                          ofs,
                          value);
    else
    {
        if ((mask | unused_mask) == 0xffffffff)
        {
            l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                             mem_type,
                             ((pmc_handle_t *)h_ptr)->base_address + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         ((pmc_handle_t *)h_ptr)->base_address + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void rxjc_proc_action_on_write_field_set( rxjc_proc_buffer_t *b_ptr,
                                                        rxjc_proc_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_action_on_write_field_set( rxjc_proc_buffer_t *b_ptr,
                                                        rxjc_proc_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         b_ptr->base_address,
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         ((pmc_handle_t *)h_ptr)->base_address + reg,
                         value<<ofs);
}

static INLINE void rxjc_proc_burst_read( rxjc_proc_buffer_t *b_ptr,
                                         rxjc_proc_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_burst_read( rxjc_proc_buffer_t *b_ptr,
                                         rxjc_proc_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          b_ptr->base_address,
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          ((pmc_handle_t *)h_ptr)->base_address + reg,
                          len,
                          value);
}

static INLINE void rxjc_proc_burst_write( rxjc_proc_buffer_t *b_ptr,
                                          rxjc_proc_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_burst_write( rxjc_proc_buffer_t *b_ptr,
                                          rxjc_proc_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           b_ptr->base_address,
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           ((pmc_handle_t *)h_ptr)->base_address + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE rxjc_proc_poll( rxjc_proc_buffer_t *b_ptr,
                                                   rxjc_proc_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rxjc_proc_poll( rxjc_proc_buffer_t *b_ptr,
                                                   rxjc_proc_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (b_ptr != NULL)
        return l1sys_poll( b_ptr->coalesce_handle,
                           mem_type,
                           b_ptr->base_address,
                           reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
    else
        return l2sys_poll( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           ((pmc_handle_t *)h_ptr)->base_address + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for rxjc_proc
 * ==================================================================================
 */

static INLINE void rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_write( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_write( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_write", value );
    rxjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE,
                         value);
}

static INLINE void rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_field_set( rxjc_proc_buffer_t *b_ptr,
                                                                   rxjc_proc_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_field_set( rxjc_proc_buffer_t *b_ptr,
                                                                   rxjc_proc_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_field_set", mask, ofs, value );
    rxjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE,
                         mask,
                         PMC_RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_read( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_read( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rxjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE);

    IOLOG( "%s -> 0x%08x;", "rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_read", reg_value);
    return reg_value;
}

static INLINE void rxjc_proc_reg_PMON_SYNCLOSS_CHID_write( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_PMON_SYNCLOSS_CHID_write( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rxjc_proc_reg_PMON_SYNCLOSS_CHID_write", value );
    rxjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID,
                         value);
}

static INLINE void rxjc_proc_reg_PMON_SYNCLOSS_CHID_field_set( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_PMON_SYNCLOSS_CHID_field_set( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rxjc_proc_reg_PMON_SYNCLOSS_CHID_field_set", mask, ofs, value );
    rxjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID,
                         mask,
                         PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 rxjc_proc_reg_PMON_SYNCLOSS_CHID_read( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_reg_PMON_SYNCLOSS_CHID_read( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rxjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID);

    IOLOG( "%s -> 0x%08x;", "rxjc_proc_reg_PMON_SYNCLOSS_CHID_read", reg_value);
    return reg_value;
}

static INLINE void rxjc_proc_reg_COMMON_CFG_1_array_write( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_COMMON_CFG_1_array_write( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rxjc_proc_reg_COMMON_CFG_1_array_write", value );
    rxjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_RXJC_PROC_REG_COMMON_CFG_1(N),
                         value);
}

static INLINE void rxjc_proc_reg_COMMON_CFG_1_array_field_set( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_COMMON_CFG_1_array_field_set( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "rxjc_proc_reg_COMMON_CFG_1_array_field_set", N, mask, ofs, value );
    rxjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_RXJC_PROC_REG_COMMON_CFG_1(N),
                         mask,
                         PMC_RXJC_PROC_REG_COMMON_CFG_1_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 rxjc_proc_reg_COMMON_CFG_1_array_read( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_reg_COMMON_CFG_1_array_read( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = rxjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RXJC_PROC_REG_COMMON_CFG_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "rxjc_proc_reg_COMMON_CFG_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void rxjc_proc_reg_COMMON_CFG_2_array_write( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_COMMON_CFG_2_array_write( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rxjc_proc_reg_COMMON_CFG_2_array_write", value );
    rxjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_RXJC_PROC_REG_COMMON_CFG_2(N),
                         value);
}

static INLINE void rxjc_proc_reg_COMMON_CFG_2_array_field_set( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_COMMON_CFG_2_array_field_set( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "rxjc_proc_reg_COMMON_CFG_2_array_field_set", N, mask, ofs, value );
    rxjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_RXJC_PROC_REG_COMMON_CFG_2(N),
                         mask,
                         PMC_RXJC_PROC_REG_COMMON_CFG_2_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 rxjc_proc_reg_COMMON_CFG_2_array_read( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_reg_COMMON_CFG_2_array_read( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = rxjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RXJC_PROC_REG_COMMON_CFG_2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "rxjc_proc_reg_COMMON_CFG_2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void rxjc_proc_reg_COMMON_CFG_3_array_write( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_COMMON_CFG_3_array_write( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rxjc_proc_reg_COMMON_CFG_3_array_write", value );
    rxjc_proc_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_RXJC_PROC_REG_COMMON_CFG_3(N),
                         value);
}

static INLINE void rxjc_proc_reg_COMMON_CFG_3_array_field_set( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_reg_COMMON_CFG_3_array_field_set( rxjc_proc_buffer_t *b_ptr,
                                                               rxjc_proc_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "rxjc_proc_reg_COMMON_CFG_3_array_field_set", N, mask, ofs, value );
    rxjc_proc_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_RXJC_PROC_REG_COMMON_CFG_3(N),
                         mask,
                         PMC_RXJC_PROC_REG_COMMON_CFG_3_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 rxjc_proc_reg_COMMON_CFG_3_array_read( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_reg_COMMON_CFG_3_array_read( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = rxjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RXJC_PROC_REG_COMMON_CFG_3(N));

    IOLOG( "%s -> 0x%08x; N=%d", "rxjc_proc_reg_COMMON_CFG_3_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 rxjc_proc_reg_CM_STAT_REG_array_read( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_reg_CM_STAT_REG_array_read( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = rxjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RXJC_PROC_REG_CM_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "rxjc_proc_reg_CM_STAT_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 rxjc_proc_reg_PMON_SYNCLOSS_CNT_read( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_reg_PMON_SYNCLOSS_CNT_read( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rxjc_proc_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CNT);

    IOLOG( "%s -> 0x%08x;", "rxjc_proc_reg_PMON_SYNCLOSS_CNT_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void rxjc_proc_field_CM_ST_UPDATE_DISBLE_set( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_CM_ST_UPDATE_DISBLE_set( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rxjc_proc_field_CM_ST_UPDATE_DISBLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rxjc_proc_field_CM_ST_UPDATE_DISBLE_set", value );

    /* (0x00000000 bits 0) field CM_ST_UPDATE_DISBLE of register PMC_RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE */
    rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_field_set( b_ptr,
                                                    h_ptr,
                                                    RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE_BIT_CM_ST_UPDATE_DISBLE_MSK,
                                                    RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE_BIT_CM_ST_UPDATE_DISBLE_OFF,
                                                    value);
}

static INLINE UINT32 rxjc_proc_field_CM_ST_UPDATE_DISBLE_get( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_CM_ST_UPDATE_DISBLE_get( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field CM_ST_UPDATE_DISBLE of register PMC_RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE */
    reg_value = rxjc_proc_reg_CM_STAT_UPDATE_DISABLE_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE_BIT_CM_ST_UPDATE_DISBLE_MSK) >> RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE_BIT_CM_ST_UPDATE_DISBLE_OFF;
    IOLOG( "%s -> 0x%08x", "rxjc_proc_field_CM_ST_UPDATE_DISBLE_get", value );

    return value;
}
static INLINE void rxjc_proc_field_PMON_SYNCLOSS_CHID_set( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_PMON_SYNCLOSS_CHID_set( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rxjc_proc_field_PMON_SYNCLOSS_CHID_set", value, 127);
    IOLOG( "%s <= 0x%08x", "rxjc_proc_field_PMON_SYNCLOSS_CHID_set", value );

    /* (0x00000004 bits 6:0) field PMON_SYNCLOSS_CHID of register PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID */
    rxjc_proc_reg_PMON_SYNCLOSS_CHID_field_set( b_ptr,
                                                h_ptr,
                                                RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_MSK,
                                                RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_OFF,
                                                value);
}

static INLINE UINT32 rxjc_proc_field_PMON_SYNCLOSS_CHID_get( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_PMON_SYNCLOSS_CHID_get( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 6:0) field PMON_SYNCLOSS_CHID of register PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID */
    reg_value = rxjc_proc_reg_PMON_SYNCLOSS_CHID_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_MSK) >> RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_OFF;
    IOLOG( "%s -> 0x%08x", "rxjc_proc_field_PMON_SYNCLOSS_CHID_get", value );

    return value;
}
static INLINE void rxjc_proc_field_range_PMON_SYNCLOSS_CHID_set( rxjc_proc_buffer_t *b_ptr,
                                                                 rxjc_proc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_range_PMON_SYNCLOSS_CHID_set( rxjc_proc_buffer_t *b_ptr,
                                                                 rxjc_proc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_PMON_SYNCLOSS_CHID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_PMON_SYNCLOSS_CHID_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_PMON_SYNCLOSS_CHID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 6:0) field PMON_SYNCLOSS_CHID of register PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000004 bits 6:0) field PMON_SYNCLOSS_CHID of register PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID */
        rxjc_proc_reg_PMON_SYNCLOSS_CHID_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_OFF + subfield_offset),
                                                    RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 rxjc_proc_field_range_PMON_SYNCLOSS_CHID_get( rxjc_proc_buffer_t *b_ptr,
                                                                   rxjc_proc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_PMON_SYNCLOSS_CHID_get( rxjc_proc_buffer_t *b_ptr,
                                                                   rxjc_proc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_PMON_SYNCLOSS_CHID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_PMON_SYNCLOSS_CHID_get", stop_bit, 6 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 6) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 6;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000004 bits 6:0) field PMON_SYNCLOSS_CHID of register PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID */
    reg_value = rxjc_proc_reg_PMON_SYNCLOSS_CHID_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_MSK)
                  >> RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_MSK, RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_PMON_SYNCLOSS_CHID_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void rxjc_proc_field_CM_MAX_set( rxjc_proc_buffer_t *b_ptr,
                                               rxjc_proc_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_CM_MAX_set( rxjc_proc_buffer_t *b_ptr,
                                               rxjc_proc_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_CM_MAX_set", N, 95);
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rxjc_proc_field_CM_MAX_set", value, 16383);
    IOLOG( "%s <= N=%d 0x%08x", "rxjc_proc_field_CM_MAX_set", N, value );

    /* ((0x00000800 + (N) * 0x10) bits 29:16) field CM_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
    rxjc_proc_reg_COMMON_CFG_1_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_MSK,
                                                RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_OFF,
                                                value);
}

static INLINE UINT32 rxjc_proc_field_CM_MAX_get( rxjc_proc_buffer_t *b_ptr,
                                                 rxjc_proc_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_CM_MAX_get( rxjc_proc_buffer_t *b_ptr,
                                                 rxjc_proc_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_CM_MAX_get", N, 95);
    /* ((0x00000800 + (N) * 0x10) bits 29:16) field CM_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_MSK) >> RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "rxjc_proc_field_CM_MAX_get", N, value );

    return value;
}
static INLINE void rxjc_proc_field_range_CM_MAX_set( rxjc_proc_buffer_t *b_ptr,
                                                     rxjc_proc_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_range_CM_MAX_set( rxjc_proc_buffer_t *b_ptr,
                                                     rxjc_proc_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_CM_MAX_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_CM_MAX_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_CM_MAX_set", stop_bit, 13 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_CM_MAX_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000800 + (N) * 0x10) bits 29:16) field CM_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 13) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 13;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000800 + (N) * 0x10) bits 29:16) field CM_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
        rxjc_proc_reg_COMMON_CFG_1_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_OFF + subfield_offset),
                                                    RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 rxjc_proc_field_range_CM_MAX_get( rxjc_proc_buffer_t *b_ptr,
                                                       rxjc_proc_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_CM_MAX_get( rxjc_proc_buffer_t *b_ptr,
                                                       rxjc_proc_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_CM_MAX_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_CM_MAX_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_CM_MAX_get", stop_bit, 13 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 13) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 13;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000800 + (N) * 0x10) bits 29:16) field CM_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_MSK)
                  >> RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_MSK, RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_CM_MAX_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void rxjc_proc_field_CM_MIN_set( rxjc_proc_buffer_t *b_ptr,
                                               rxjc_proc_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_CM_MIN_set( rxjc_proc_buffer_t *b_ptr,
                                               rxjc_proc_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_CM_MIN_set", N, 95);
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rxjc_proc_field_CM_MIN_set", value, 16383);
    IOLOG( "%s <= N=%d 0x%08x", "rxjc_proc_field_CM_MIN_set", N, value );

    /* ((0x00000800 + (N) * 0x10) bits 13:0) field CM_MIN of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
    rxjc_proc_reg_COMMON_CFG_1_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_MSK,
                                                RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_OFF,
                                                value);
}

static INLINE UINT32 rxjc_proc_field_CM_MIN_get( rxjc_proc_buffer_t *b_ptr,
                                                 rxjc_proc_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_CM_MIN_get( rxjc_proc_buffer_t *b_ptr,
                                                 rxjc_proc_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_CM_MIN_get", N, 95);
    /* ((0x00000800 + (N) * 0x10) bits 13:0) field CM_MIN of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_MSK) >> RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "rxjc_proc_field_CM_MIN_get", N, value );

    return value;
}
static INLINE void rxjc_proc_field_range_CM_MIN_set( rxjc_proc_buffer_t *b_ptr,
                                                     rxjc_proc_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_range_CM_MIN_set( rxjc_proc_buffer_t *b_ptr,
                                                     rxjc_proc_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_CM_MIN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_CM_MIN_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_CM_MIN_set", stop_bit, 13 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_CM_MIN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000800 + (N) * 0x10) bits 13:0) field CM_MIN of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 13) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 13;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000800 + (N) * 0x10) bits 13:0) field CM_MIN of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
        rxjc_proc_reg_COMMON_CFG_1_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_OFF + subfield_offset),
                                                    RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 rxjc_proc_field_range_CM_MIN_get( rxjc_proc_buffer_t *b_ptr,
                                                       rxjc_proc_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_CM_MIN_get( rxjc_proc_buffer_t *b_ptr,
                                                       rxjc_proc_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_CM_MIN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_CM_MIN_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_CM_MIN_get", stop_bit, 13 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 13) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 13;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000800 + (N) * 0x10) bits 13:0) field CM_MIN of register PMC_RXJC_PROC_REG_COMMON_CFG_1 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_MSK)
                  >> RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_MSK, RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_CM_MIN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void rxjc_proc_field_M_set( rxjc_proc_buffer_t *b_ptr,
                                          rxjc_proc_handle_t *h_ptr,
                                          UINT32  N,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_M_set( rxjc_proc_buffer_t *b_ptr,
                                          rxjc_proc_handle_t *h_ptr,
                                          UINT32  N,
                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_M_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rxjc_proc_field_M_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "rxjc_proc_field_M_set", N, value );

    /* ((0x00000804 + (N) * 0x10) bits 22:16) field M of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
    rxjc_proc_reg_COMMON_CFG_2_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                RXJC_PROC_REG_COMMON_CFG_2_BIT_M_MSK,
                                                RXJC_PROC_REG_COMMON_CFG_2_BIT_M_OFF,
                                                value);
}

static INLINE UINT32 rxjc_proc_field_M_get( rxjc_proc_buffer_t *b_ptr,
                                            rxjc_proc_handle_t *h_ptr,
                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_M_get( rxjc_proc_buffer_t *b_ptr,
                                            rxjc_proc_handle_t *h_ptr,
                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_M_get", N, 95);
    /* ((0x00000804 + (N) * 0x10) bits 22:16) field M of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_2_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & RXJC_PROC_REG_COMMON_CFG_2_BIT_M_MSK) >> RXJC_PROC_REG_COMMON_CFG_2_BIT_M_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "rxjc_proc_field_M_get", N, value );

    return value;
}
static INLINE void rxjc_proc_field_range_M_set( rxjc_proc_buffer_t *b_ptr,
                                                rxjc_proc_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_range_M_set( rxjc_proc_buffer_t *b_ptr,
                                                rxjc_proc_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_M_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_M_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_M_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_M_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000804 + (N) * 0x10) bits 22:16) field M of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000804 + (N) * 0x10) bits 22:16) field M of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
        rxjc_proc_reg_COMMON_CFG_2_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (RXJC_PROC_REG_COMMON_CFG_2_BIT_M_OFF + subfield_offset),
                                                    RXJC_PROC_REG_COMMON_CFG_2_BIT_M_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 rxjc_proc_field_range_M_get( rxjc_proc_buffer_t *b_ptr,
                                                  rxjc_proc_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_M_get( rxjc_proc_buffer_t *b_ptr,
                                                  rxjc_proc_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_M_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_M_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_M_get", stop_bit, 6 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 6) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 6;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000804 + (N) * 0x10) bits 22:16) field M of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_2_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & RXJC_PROC_REG_COMMON_CFG_2_BIT_M_MSK)
                  >> RXJC_PROC_REG_COMMON_CFG_2_BIT_M_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_COMMON_CFG_2_BIT_M_MSK, RXJC_PROC_REG_COMMON_CFG_2_BIT_M_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_M_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void rxjc_proc_field_CND_MAX_set( rxjc_proc_buffer_t *b_ptr,
                                                rxjc_proc_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_CND_MAX_set( rxjc_proc_buffer_t *b_ptr,
                                                rxjc_proc_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_CND_MAX_set", N, 95);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rxjc_proc_field_CND_MAX_set", value, 1023);
    IOLOG( "%s <= N=%d 0x%08x", "rxjc_proc_field_CND_MAX_set", N, value );

    /* ((0x00000804 + (N) * 0x10) bits 9:0) field CND_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
    rxjc_proc_reg_COMMON_CFG_2_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_MSK,
                                                RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_OFF,
                                                value);
}

static INLINE UINT32 rxjc_proc_field_CND_MAX_get( rxjc_proc_buffer_t *b_ptr,
                                                  rxjc_proc_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_CND_MAX_get( rxjc_proc_buffer_t *b_ptr,
                                                  rxjc_proc_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_CND_MAX_get", N, 95);
    /* ((0x00000804 + (N) * 0x10) bits 9:0) field CND_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_2_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_MSK) >> RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "rxjc_proc_field_CND_MAX_get", N, value );

    return value;
}
static INLINE void rxjc_proc_field_range_CND_MAX_set( rxjc_proc_buffer_t *b_ptr,
                                                      rxjc_proc_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_range_CND_MAX_set( rxjc_proc_buffer_t *b_ptr,
                                                      rxjc_proc_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_CND_MAX_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_CND_MAX_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_CND_MAX_set", stop_bit, 9 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_CND_MAX_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000804 + (N) * 0x10) bits 9:0) field CND_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000804 + (N) * 0x10) bits 9:0) field CND_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
        rxjc_proc_reg_COMMON_CFG_2_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_OFF + subfield_offset),
                                                    RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 rxjc_proc_field_range_CND_MAX_get( rxjc_proc_buffer_t *b_ptr,
                                                        rxjc_proc_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_CND_MAX_get( rxjc_proc_buffer_t *b_ptr,
                                                        rxjc_proc_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_CND_MAX_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_CND_MAX_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_CND_MAX_get", stop_bit, 9 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 9) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 9;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000804 + (N) * 0x10) bits 9:0) field CND_MAX of register PMC_RXJC_PROC_REG_COMMON_CFG_2 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_2_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_MSK)
                  >> RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_MSK, RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_CND_MAX_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void rxjc_proc_field_NJO_PJO_MODE_set( rxjc_proc_buffer_t *b_ptr,
                                                     rxjc_proc_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_NJO_PJO_MODE_set( rxjc_proc_buffer_t *b_ptr,
                                                     rxjc_proc_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_NJO_PJO_MODE_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rxjc_proc_field_NJO_PJO_MODE_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "rxjc_proc_field_NJO_PJO_MODE_set", N, value );

    /* ((0x00000808 + (N) * 0x10) bits 25:24) field NJO_PJO_MODE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
    rxjc_proc_reg_COMMON_CFG_3_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_MSK,
                                                RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_OFF,
                                                value);
}

static INLINE UINT32 rxjc_proc_field_NJO_PJO_MODE_get( rxjc_proc_buffer_t *b_ptr,
                                                       rxjc_proc_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_NJO_PJO_MODE_get( rxjc_proc_buffer_t *b_ptr,
                                                       rxjc_proc_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_NJO_PJO_MODE_get", N, 95);
    /* ((0x00000808 + (N) * 0x10) bits 25:24) field NJO_PJO_MODE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_3_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_MSK) >> RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "rxjc_proc_field_NJO_PJO_MODE_get", N, value );

    return value;
}
static INLINE void rxjc_proc_field_range_NJO_PJO_MODE_set( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_range_NJO_PJO_MODE_set( rxjc_proc_buffer_t *b_ptr,
                                                           rxjc_proc_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_NJO_PJO_MODE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_NJO_PJO_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_NJO_PJO_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_NJO_PJO_MODE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000808 + (N) * 0x10) bits 25:24) field NJO_PJO_MODE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 1) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 1;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000808 + (N) * 0x10) bits 25:24) field NJO_PJO_MODE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
        rxjc_proc_reg_COMMON_CFG_3_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_OFF + subfield_offset),
                                                    RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 rxjc_proc_field_range_NJO_PJO_MODE_get( rxjc_proc_buffer_t *b_ptr,
                                                             rxjc_proc_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_NJO_PJO_MODE_get( rxjc_proc_buffer_t *b_ptr,
                                                             rxjc_proc_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_NJO_PJO_MODE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_NJO_PJO_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_NJO_PJO_MODE_get", stop_bit, 1 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 1) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 1;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000808 + (N) * 0x10) bits 25:24) field NJO_PJO_MODE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_3_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_MSK)
                  >> RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_MSK, RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_NJO_PJO_MODE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void rxjc_proc_field_SERVER_SIZE_set( rxjc_proc_buffer_t *b_ptr,
                                                    rxjc_proc_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_SERVER_SIZE_set( rxjc_proc_buffer_t *b_ptr,
                                                    rxjc_proc_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_SERVER_SIZE_set", N, 95);
    if (value > 2097151)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rxjc_proc_field_SERVER_SIZE_set", value, 2097151);
    IOLOG( "%s <= N=%d 0x%08x", "rxjc_proc_field_SERVER_SIZE_set", N, value );

    /* ((0x00000808 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
    rxjc_proc_reg_COMMON_CFG_3_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_MSK,
                                                RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_OFF,
                                                value);
}

static INLINE UINT32 rxjc_proc_field_SERVER_SIZE_get( rxjc_proc_buffer_t *b_ptr,
                                                      rxjc_proc_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_SERVER_SIZE_get( rxjc_proc_buffer_t *b_ptr,
                                                      rxjc_proc_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_SERVER_SIZE_get", N, 95);
    /* ((0x00000808 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_3_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_MSK) >> RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "rxjc_proc_field_SERVER_SIZE_get", N, value );

    return value;
}
static INLINE void rxjc_proc_field_range_SERVER_SIZE_set( rxjc_proc_buffer_t *b_ptr,
                                                          rxjc_proc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rxjc_proc_field_range_SERVER_SIZE_set( rxjc_proc_buffer_t *b_ptr,
                                                          rxjc_proc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_SERVER_SIZE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_SERVER_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 20) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_SERVER_SIZE_set", stop_bit, 20 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_SERVER_SIZE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000808 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 0 - start_bit;
            subfield_start_bit = 0;
        }
        if (stop_bit < 20) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 20;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000808 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
        rxjc_proc_reg_COMMON_CFG_3_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_OFF + subfield_offset),
                                                    RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 rxjc_proc_field_range_SERVER_SIZE_get( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_SERVER_SIZE_get( rxjc_proc_buffer_t *b_ptr,
                                                            rxjc_proc_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_SERVER_SIZE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_SERVER_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 20) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_SERVER_SIZE_get", stop_bit, 20 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 20) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 20;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000808 + (N) * 0x10) bits 20:0) field SERVER_SIZE of register PMC_RXJC_PROC_REG_COMMON_CFG_3 index N=0..95 */
    reg_value = rxjc_proc_reg_COMMON_CFG_3_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_MSK)
                  >> RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_MSK, RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_SERVER_SIZE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 rxjc_proc_field_CM_STATUS_get( rxjc_proc_buffer_t *b_ptr,
                                                    rxjc_proc_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_CM_STATUS_get( rxjc_proc_buffer_t *b_ptr,
                                                    rxjc_proc_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_CM_STATUS_get", N, 95);
    /* ((0x00000e00 + (N) * 0x4) bits 13:0) field CM_STATUS of register PMC_RXJC_PROC_REG_CM_STAT_REG index N=0..95 */
    reg_value = rxjc_proc_reg_CM_STAT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & RXJC_PROC_REG_CM_STAT_REG_BIT_CM_STATUS_MSK) >> RXJC_PROC_REG_CM_STAT_REG_BIT_CM_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "rxjc_proc_field_CM_STATUS_get", N, value );

    return value;
}
static INLINE UINT32 rxjc_proc_field_range_CM_STATUS_get( rxjc_proc_buffer_t *b_ptr,
                                                          rxjc_proc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_CM_STATUS_get( rxjc_proc_buffer_t *b_ptr,
                                                          rxjc_proc_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rxjc_proc_field_range_CM_STATUS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_CM_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_CM_STATUS_get", stop_bit, 13 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 13) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 13;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000e00 + (N) * 0x4) bits 13:0) field CM_STATUS of register PMC_RXJC_PROC_REG_CM_STAT_REG index N=0..95 */
    reg_value = rxjc_proc_reg_CM_STAT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & RXJC_PROC_REG_CM_STAT_REG_BIT_CM_STATUS_MSK)
                  >> RXJC_PROC_REG_CM_STAT_REG_BIT_CM_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_CM_STAT_REG_BIT_CM_STATUS_MSK, RXJC_PROC_REG_CM_STAT_REG_BIT_CM_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_CM_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 rxjc_proc_field_PMON_SYNCLOSS_CNT_get( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_PMON_SYNCLOSS_CNT_get( rxjc_proc_buffer_t *b_ptr, rxjc_proc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 31:0) field PMON_SYNCLOSS_CNT of register PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CNT */
    reg_value = rxjc_proc_reg_PMON_SYNCLOSS_CNT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & RXJC_PROC_REG_PMON_SYNCLOSS_CNT_BIT_PMON_SYNCLOSS_CNT_MSK) >> RXJC_PROC_REG_PMON_SYNCLOSS_CNT_BIT_PMON_SYNCLOSS_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "rxjc_proc_field_PMON_SYNCLOSS_CNT_get", value );

    return value;
}
static INLINE UINT32 rxjc_proc_field_range_PMON_SYNCLOSS_CNT_get( rxjc_proc_buffer_t *b_ptr,
                                                                  rxjc_proc_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rxjc_proc_field_range_PMON_SYNCLOSS_CNT_get( rxjc_proc_buffer_t *b_ptr,
                                                                  rxjc_proc_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rxjc_proc_field_range_PMON_SYNCLOSS_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rxjc_proc_field_range_PMON_SYNCLOSS_CNT_get", stop_bit, 31 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 31) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 31;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000008 bits 31:0) field PMON_SYNCLOSS_CNT of register PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CNT */
    reg_value = rxjc_proc_reg_PMON_SYNCLOSS_CNT_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & RXJC_PROC_REG_PMON_SYNCLOSS_CNT_BIT_PMON_SYNCLOSS_CNT_MSK)
                  >> RXJC_PROC_REG_PMON_SYNCLOSS_CNT_BIT_PMON_SYNCLOSS_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RXJC_PROC_REG_PMON_SYNCLOSS_CNT_BIT_PMON_SYNCLOSS_CNT_MSK, RXJC_PROC_REG_PMON_SYNCLOSS_CNT_BIT_PMON_SYNCLOSS_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rxjc_proc_field_range_PMON_SYNCLOSS_CNT_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RXJC_PROC_IO_INLINE_H */
