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
 *     and register accessor functions for the int_agr block
 *****************************************************************************/
#ifndef _INT_AGR_IO_INLINE_H
#define _INT_AGR_IO_INLINE_H

#include "pmc_handle.h"
#include "int_agr_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define INT_AGR_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for int_agr
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
    pmc_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} int_agr_buffer_t;
static INLINE void int_agr_buffer_init( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void int_agr_buffer_init( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "int_agr_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void int_agr_buffer_flush( int_agr_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void int_agr_buffer_flush( int_agr_buffer_t *b_ptr )
{
    IOLOG( "int_agr_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 int_agr_reg_read( int_agr_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_read( int_agr_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
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
static INLINE void int_agr_reg_write( int_agr_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_write( int_agr_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
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

static INLINE void int_agr_field_set( int_agr_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_set( int_agr_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
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

static INLINE void int_agr_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
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

static INLINE void int_agr_burst_read( int_agr_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void int_agr_burst_read( int_agr_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
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

static INLINE void int_agr_burst_write( int_agr_buffer_t *b_ptr,
                                        pmc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void int_agr_burst_write( int_agr_buffer_t *b_ptr,
                                        pmc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE int_agr_poll( int_agr_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_poll( int_agr_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
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
 *  register access functions for int_agr
 * ==================================================================================
 */

static INLINE void int_agr_reg_INTER_PROCESSOR_INTS_array_write( int_agr_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INTER_PROCESSOR_INTS_array_write( int_agr_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_INTER_PROCESSOR_INTS_array_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_INT_AGR_REG_INTER_PROCESSOR_INTS(N),
                       value);
}

static INLINE void int_agr_reg_INTER_PROCESSOR_INTS_array_field_set( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INTER_PROCESSOR_INTS_array_field_set( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_INTER_PROCESSOR_INTS_array_field_set", N, mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_INT_AGR_REG_INTER_PROCESSOR_INTS(N),
                       mask,
                       PMC_INT_AGR_REG_INTER_PROCESSOR_INTS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_INTER_PROCESSOR_INTS_array_read( int_agr_buffer_t *b_ptr,
                                                                  pmc_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INTER_PROCESSOR_INTS_array_read( int_agr_buffer_t *b_ptr,
                                                                  pmc_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_INT_AGR_REG_INTER_PROCESSOR_INTS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "int_agr_reg_INTER_PROCESSOR_INTS_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_INTER_PROCESSOR_INTS_array_poll( int_agr_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_INTER_PROCESSOR_INTS_array_poll( int_agr_buffer_t *b_ptr,
                                                                                pmc_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_INTER_PROCESSOR_INTS_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_INT_AGR_REG_INTER_PROCESSOR_INTS(N),
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_INT_AGGREGATOR_DEVICE_SET_write( int_agr_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INT_AGGREGATOR_DEVICE_SET_write( int_agr_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_INT_AGGREGATOR_DEVICE_SET_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET,
                       value);
}

static INLINE void int_agr_reg_INT_AGGREGATOR_DEVICE_SET_field_set( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INT_AGGREGATOR_DEVICE_SET_field_set( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_INT_AGGREGATOR_DEVICE_SET_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET,
                       mask,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_SET_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_SET_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_DEVICE_SET_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_write( int_agr_buffer_t *b_ptr,
                                                                  pmc_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_write( int_agr_buffer_t *b_ptr,
                                                                  pmc_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR,
                       value);
}

static INLINE void int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_field_set( int_agr_buffer_t *b_ptr,
                                                                      pmc_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_field_set( int_agr_buffer_t *b_ptr,
                                                                      pmc_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR,
                       mask,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_INT_AGGREGATOR_MISC_SET_write( int_agr_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INT_AGGREGATOR_MISC_SET_write( int_agr_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_INT_AGGREGATOR_MISC_SET_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET,
                       value);
}

static INLINE void int_agr_reg_INT_AGGREGATOR_MISC_SET_field_set( int_agr_buffer_t *b_ptr,
                                                                  pmc_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INT_AGGREGATOR_MISC_SET_field_set( int_agr_buffer_t *b_ptr,
                                                                  pmc_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_INT_AGGREGATOR_MISC_SET_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET,
                       mask,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_SET_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_SET_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_MISC_SET_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_write( int_agr_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_write( int_agr_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR,
                       value);
}

static INLINE void int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_field_set( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_field_set( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR,
                       mask,
                       PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_IPI2PCIE_0_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_0_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2PCIE_0_INT_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_0_INT,
                       value);
}

static INLINE void int_agr_reg_IPI2PCIE_0_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_0_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2PCIE_0_INT_action_on_write_field_set", mask, ofs, value );
    int_agr_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_INT_AGR_REG_IPI2PCIE_0_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 int_agr_reg_IPI2PCIE_0_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2PCIE_0_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2PCIE_0_INT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2PCIE_0_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_0_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_0_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_IPI2PCIE_0_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_INT_AGR_REG_IPI2PCIE_0_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_IPI2PCIE_1_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_1_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2PCIE_1_INT_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_1_INT,
                       value);
}

static INLINE void int_agr_reg_IPI2PCIE_1_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_1_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2PCIE_1_INT_action_on_write_field_set", mask, ofs, value );
    int_agr_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_INT_AGR_REG_IPI2PCIE_1_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 int_agr_reg_IPI2PCIE_1_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2PCIE_1_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2PCIE_1_INT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2PCIE_1_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_1_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_1_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_IPI2PCIE_1_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_INT_AGR_REG_IPI2PCIE_1_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_IPI2PCIE_2_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_2_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2PCIE_2_INT_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_2_INT,
                       value);
}

static INLINE void int_agr_reg_IPI2PCIE_2_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_2_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2PCIE_2_INT_action_on_write_field_set", mask, ofs, value );
    int_agr_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_INT_AGR_REG_IPI2PCIE_2_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 int_agr_reg_IPI2PCIE_2_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2PCIE_2_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2PCIE_2_INT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2PCIE_2_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_2_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_2_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_IPI2PCIE_2_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_INT_AGR_REG_IPI2PCIE_2_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_IPI2PCIE_3_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_3_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2PCIE_3_INT_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_3_INT,
                       value);
}

static INLINE void int_agr_reg_IPI2PCIE_3_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_3_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2PCIE_3_INT_action_on_write_field_set", mask, ofs, value );
    int_agr_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_INT_AGR_REG_IPI2PCIE_3_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 int_agr_reg_IPI2PCIE_3_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2PCIE_3_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2PCIE_3_INT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2PCIE_3_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_3_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_3_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_IPI2PCIE_3_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_INT_AGR_REG_IPI2PCIE_3_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_IPI2MIPS_0_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_0_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2MIPS_0_INT_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_0_INT,
                       value);
}

static INLINE void int_agr_reg_IPI2MIPS_0_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_0_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2MIPS_0_INT_action_on_write_field_set", mask, ofs, value );
    int_agr_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_INT_AGR_REG_IPI2MIPS_0_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 int_agr_reg_IPI2MIPS_0_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2MIPS_0_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2MIPS_0_INT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2MIPS_0_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_0_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_0_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_IPI2MIPS_0_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_INT_AGR_REG_IPI2MIPS_0_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_IPI2MIPS_1_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_1_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2MIPS_1_INT_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_1_INT,
                       value);
}

static INLINE void int_agr_reg_IPI2MIPS_1_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_1_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2MIPS_1_INT_action_on_write_field_set", mask, ofs, value );
    int_agr_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_INT_AGR_REG_IPI2MIPS_1_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 int_agr_reg_IPI2MIPS_1_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2MIPS_1_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2MIPS_1_INT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2MIPS_1_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_1_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_1_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_IPI2MIPS_1_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_INT_AGR_REG_IPI2MIPS_1_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_IPI2MIPS_2_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_2_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2MIPS_2_INT_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_2_INT,
                       value);
}

static INLINE void int_agr_reg_IPI2MIPS_2_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_2_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2MIPS_2_INT_action_on_write_field_set", mask, ofs, value );
    int_agr_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_INT_AGR_REG_IPI2MIPS_2_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 int_agr_reg_IPI2MIPS_2_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2MIPS_2_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2MIPS_2_INT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2MIPS_2_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_2_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_2_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_IPI2MIPS_2_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_INT_AGR_REG_IPI2MIPS_2_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_IPI2MIPS_3_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_3_INT_write( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2MIPS_3_INT_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_3_INT,
                       value);
}

static INLINE void int_agr_reg_IPI2MIPS_3_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_3_INT_action_on_write_field_set( int_agr_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2MIPS_3_INT_action_on_write_field_set", mask, ofs, value );
    int_agr_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_INT_AGR_REG_IPI2MIPS_3_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 int_agr_reg_IPI2MIPS_3_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2MIPS_3_INT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2MIPS_3_INT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2MIPS_3_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_3_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_3_INT_poll( int_agr_buffer_t *b_ptr,
                                                                    pmc_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "int_agr_reg_IPI2MIPS_3_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return int_agr_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_INT_AGR_REG_IPI2MIPS_3_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void int_agr_reg_IPI2PCIE_0_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_0_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2PCIE_0_INT_EN_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN,
                       value);
}

static INLINE void int_agr_reg_IPI2PCIE_0_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_0_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2PCIE_0_INT_EN_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN,
                       mask,
                       PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_IPI2PCIE_0_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2PCIE_0_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2PCIE_0_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_IPI2PCIE_1_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_1_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2PCIE_1_INT_EN_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN,
                       value);
}

static INLINE void int_agr_reg_IPI2PCIE_1_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_1_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2PCIE_1_INT_EN_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN,
                       mask,
                       PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_IPI2PCIE_1_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2PCIE_1_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2PCIE_1_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_IPI2PCIE_2_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_2_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2PCIE_2_INT_EN_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN,
                       value);
}

static INLINE void int_agr_reg_IPI2PCIE_2_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_2_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2PCIE_2_INT_EN_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN,
                       mask,
                       PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_IPI2PCIE_2_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2PCIE_2_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2PCIE_2_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_IPI2PCIE_3_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_3_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2PCIE_3_INT_EN_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN,
                       value);
}

static INLINE void int_agr_reg_IPI2PCIE_3_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2PCIE_3_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2PCIE_3_INT_EN_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN,
                       mask,
                       PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_IPI2PCIE_3_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2PCIE_3_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2PCIE_3_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_IPI2MIPS_0_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_0_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2MIPS_0_INT_EN_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN,
                       value);
}

static INLINE void int_agr_reg_IPI2MIPS_0_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_0_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2MIPS_0_INT_EN_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN,
                       mask,
                       PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_IPI2MIPS_0_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2MIPS_0_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2MIPS_0_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_IPI2MIPS_1_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_1_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2MIPS_1_INT_EN_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN,
                       value);
}

static INLINE void int_agr_reg_IPI2MIPS_1_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_1_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2MIPS_1_INT_EN_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN,
                       mask,
                       PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_IPI2MIPS_1_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2MIPS_1_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2MIPS_1_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_IPI2MIPS_2_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_2_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2MIPS_2_INT_EN_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN,
                       value);
}

static INLINE void int_agr_reg_IPI2MIPS_2_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_2_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2MIPS_2_INT_EN_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN,
                       mask,
                       PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_IPI2MIPS_2_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2MIPS_2_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2MIPS_2_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void int_agr_reg_IPI2MIPS_3_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_3_INT_EN_write( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "int_agr_reg_IPI2MIPS_3_INT_EN_write", value );
    int_agr_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN,
                       value);
}

static INLINE void int_agr_reg_IPI2MIPS_3_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_reg_IPI2MIPS_3_INT_EN_field_set( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "int_agr_reg_IPI2MIPS_3_INT_EN_field_set", mask, ofs, value );
    int_agr_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN,
                       mask,
                       PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 int_agr_reg_IPI2MIPS_3_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_IPI2MIPS_3_INT_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_IPI2MIPS_3_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_DEVICE_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_STAT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_STAT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_DEVICE_STAT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_RAW_STAT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_DEVICE_RAW_STAT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_DEVICE_RAW_STAT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_EN_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_EN);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_MISC_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_STAT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_STAT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_STAT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_MISC_STAT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_RAW_STAT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_reg_INT_AGGREGATOR_MISC_RAW_STAT_read( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = int_agr_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT);

    IOLOG( "%s -> 0x%08x;", "int_agr_reg_INT_AGGREGATOR_MISC_RAW_STAT_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size64
 * ==================================================================================
 */
static INLINE void int_agr_field_IPI_BITS_set( int_agr_buffer_t *b_ptr,
                                               pmc_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI_BITS_set( int_agr_buffer_t *b_ptr,
                                               pmc_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "int_agr_field_IPI_BITS_set", N, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI_BITS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "int_agr_field_IPI_BITS_set", N, value );

    /* ((0x00001230 + (N) * 4) bits 0) field IPI_BITS of register PMC_INT_AGR_REG_INTER_PROCESSOR_INTS index N=0..63 */
    int_agr_reg_INTER_PROCESSOR_INTS_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_MSK,
                                                      INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_OFF,
                                                      value);
}

static INLINE UINT32 int_agr_field_IPI_BITS_get( int_agr_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI_BITS_get( int_agr_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 63)
        IO_RANGE_CHECK("%s N is %d but max is %d", "int_agr_field_IPI_BITS_get", N, 63);
    /* ((0x00001230 + (N) * 4) bits 0) field IPI_BITS of register PMC_INT_AGR_REG_INTER_PROCESSOR_INTS index N=0..63 */
    reg_value = int_agr_reg_INTER_PROCESSOR_INTS_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_MSK) >> INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "int_agr_field_IPI_BITS_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI_BITS_poll( int_agr_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI_BITS_poll( int_agr_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "int_agr_field_IPI_BITS_poll", N, value );

    /* ((0x00001230 + (N) * 4) bits 0) field IPI_BITS of register PMC_INT_AGR_REG_INTER_PROCESSOR_INTS index N=0..63 */
    return int_agr_reg_INTER_PROCESSOR_INTS_array_poll( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_MSK,
                                                        (value<<INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void int_agr_field_DEVICE_SET_set( int_agr_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_DEVICE_SET_set( int_agr_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "int_agr_field_DEVICE_SET_set", value );

    /* (0x000010c8 bits 31:0) bits 0:31 use field DEVICE_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET */
    int_agr_reg_INT_AGGREGATOR_DEVICE_SET_field_set( b_ptr,
                                                     h_ptr,
                                                     INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_MSK,
                                                     INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_OFF,
                                                     value);
}

static INLINE UINT32 int_agr_field_DEVICE_SET_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_DEVICE_SET_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010c8 bits 31:0) bits 0:31 use field DEVICE_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_SET_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_MSK) >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_DEVICE_SET_get", value );

    return value;
}
static INLINE void int_agr_field_range_DEVICE_SET_set( int_agr_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_DEVICE_SET_set( int_agr_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_DEVICE_SET_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_DEVICE_SET_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_DEVICE_SET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000010c8 bits 31:0) bits 0:31 use field DEVICE_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000010c8 bits 31:0) bits 0:31 use field DEVICE_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET */
        int_agr_reg_INT_AGGREGATOR_DEVICE_SET_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_OFF + subfield_offset),
                                                         INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_DEVICE_SET_get( int_agr_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_DEVICE_SET_get( int_agr_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_DEVICE_SET_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_DEVICE_SET_get", stop_bit, 31 );
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
    /* (0x000010c8 bits 31:0) bits 0:31 use field DEVICE_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_SET_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_MSK, INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_DEVICE_SET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_DEVICE_CLR_set( int_agr_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_DEVICE_CLR_set( int_agr_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "int_agr_field_DEVICE_CLR_set", value );

    /* (0x000010cc bits 31:0) bits 0:31 use field DEVICE_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR */
    int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_field_set( b_ptr,
                                                       h_ptr,
                                                       INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_MSK,
                                                       INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_OFF,
                                                       value);
}

static INLINE UINT32 int_agr_field_DEVICE_CLR_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_DEVICE_CLR_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010cc bits 31:0) bits 0:31 use field DEVICE_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_MSK) >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_DEVICE_CLR_get", value );

    return value;
}
static INLINE void int_agr_field_range_DEVICE_CLR_set( int_agr_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_DEVICE_CLR_set( int_agr_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_DEVICE_CLR_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_DEVICE_CLR_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_DEVICE_CLR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000010cc bits 31:0) bits 0:31 use field DEVICE_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000010cc bits 31:0) bits 0:31 use field DEVICE_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR */
        int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_OFF + subfield_offset),
                                                           INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_DEVICE_CLR_get( int_agr_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_DEVICE_CLR_get( int_agr_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_DEVICE_CLR_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_DEVICE_CLR_get", stop_bit, 31 );
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
    /* (0x000010cc bits 31:0) bits 0:31 use field DEVICE_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_MSK, INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_DEVICE_CLR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_MISC_SET_set( int_agr_buffer_t *b_ptr,
                                               pmc_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_MISC_SET_set( int_agr_buffer_t *b_ptr,
                                               pmc_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "int_agr_field_MISC_SET_set", value );

    /* (0x000010dc bits 31:0) bits 0:31 use field MISC_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET */
    int_agr_reg_INT_AGGREGATOR_MISC_SET_field_set( b_ptr,
                                                   h_ptr,
                                                   INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_MSK,
                                                   INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_OFF,
                                                   value);
}

static INLINE UINT32 int_agr_field_MISC_SET_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_MISC_SET_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010dc bits 31:0) bits 0:31 use field MISC_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_SET_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_MSK) >> INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_MISC_SET_get", value );

    return value;
}
static INLINE void int_agr_field_range_MISC_SET_set( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_MISC_SET_set( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_MISC_SET_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_MISC_SET_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_MISC_SET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000010dc bits 31:0) bits 0:31 use field MISC_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000010dc bits 31:0) bits 0:31 use field MISC_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET */
        int_agr_reg_INT_AGGREGATOR_MISC_SET_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_OFF + subfield_offset),
                                                       INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_MISC_SET_get( int_agr_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_MISC_SET_get( int_agr_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_MISC_SET_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_MISC_SET_get", stop_bit, 31 );
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
    /* (0x000010dc bits 31:0) bits 0:31 use field MISC_SET of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_SET_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_MSK, INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_MISC_SET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_MISC_CLR_set( int_agr_buffer_t *b_ptr,
                                               pmc_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_MISC_CLR_set( int_agr_buffer_t *b_ptr,
                                               pmc_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "int_agr_field_MISC_CLR_set", value );

    /* (0x000010e0 bits 31:0) bits 0:31 use field MISC_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR */
    int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_field_set( b_ptr,
                                                     h_ptr,
                                                     INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_MSK,
                                                     INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_OFF,
                                                     value);
}

static INLINE UINT32 int_agr_field_MISC_CLR_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_MISC_CLR_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010e0 bits 31:0) bits 0:31 use field MISC_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_MSK) >> INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_MISC_CLR_get", value );

    return value;
}
static INLINE void int_agr_field_range_MISC_CLR_set( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_MISC_CLR_set( int_agr_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_MISC_CLR_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_MISC_CLR_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_MISC_CLR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000010e0 bits 31:0) bits 0:31 use field MISC_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000010e0 bits 31:0) bits 0:31 use field MISC_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR */
        int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_OFF + subfield_offset),
                                                         INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_MISC_CLR_get( int_agr_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_MISC_CLR_get( int_agr_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_MISC_CLR_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_MISC_CLR_get", stop_bit, 31 );
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
    /* (0x000010e0 bits 31:0) bits 0:31 use field MISC_CLR of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_MSK, INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_MISC_CLR_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void int_agr_field_IPI2PCIE_0_I0_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2PCIE_0_I0_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2PCIE_0_I0_set_to_clear", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2PCIE_0_I0_set_to_clear", value );

    /* (0x00001330 bits 15:0) bits 0:15 use field IPI2PCIE_0_I0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT */
    int_agr_reg_IPI2PCIE_0_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_MSK,
                                                          INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF,
                                                          value);
}

static INLINE UINT32 int_agr_field_IPI2PCIE_0_I0_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2PCIE_0_I0_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001330 bits 15:0) bits 0:15 use field IPI2PCIE_0_I0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT */
    reg_value = int_agr_reg_IPI2PCIE_0_INT_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_MSK) >> INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2PCIE_0_I0_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2PCIE_0_I0_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2PCIE_0_I0_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_0_I0_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_0_I0_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_0_I0_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001330 bits 15:0) bits 0:15 use field IPI2PCIE_0_I0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001330 bits 15:0) bits 0:15 use field IPI2PCIE_0_I0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT */
        int_agr_reg_IPI2PCIE_0_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF + subfield_offset),
                                                              INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2PCIE_0_I0_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2PCIE_0_I0_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_0_I0_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_0_I0_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001330 bits 15:0) bits 0:15 use field IPI2PCIE_0_I0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT */
    reg_value = int_agr_reg_IPI2PCIE_0_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_MSK)
                  >> INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_MSK, INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_0_I0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_0_I0_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_0_I0_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_0_I0_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_0_I0_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_0_I0_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001330 bits 15:0) bits 0:15 use field IPI2PCIE_0_I0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001330 bits 15:0) bits 0:15 use field IPI2PCIE_0_I0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT */
        return int_agr_reg_IPI2PCIE_0_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF + subfield_offset),
                                                value << (INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_0_I0_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_0_I0_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "int_agr_field_IPI2PCIE_0_I0_poll", value );

    /* (0x00001330 bits 15:0) bits 0:15 use field IPI2PCIE_0_I0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT */
    return int_agr_reg_IPI2PCIE_0_INT_poll( b_ptr,
                                            h_ptr,
                                            INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_MSK,
                                            (value<<INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void int_agr_field_IPI2PCIE_1_I1_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2PCIE_1_I1_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2PCIE_1_I1_set_to_clear", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2PCIE_1_I1_set_to_clear", value );

    /* (0x00001334 bits 15:0) bits 0:15 use field IPI2PCIE_1_I1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT */
    int_agr_reg_IPI2PCIE_1_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_MSK,
                                                          INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF,
                                                          value);
}

static INLINE UINT32 int_agr_field_IPI2PCIE_1_I1_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2PCIE_1_I1_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001334 bits 15:0) bits 0:15 use field IPI2PCIE_1_I1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT */
    reg_value = int_agr_reg_IPI2PCIE_1_INT_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_MSK) >> INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2PCIE_1_I1_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2PCIE_1_I1_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2PCIE_1_I1_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_1_I1_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_1_I1_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_1_I1_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001334 bits 15:0) bits 0:15 use field IPI2PCIE_1_I1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001334 bits 15:0) bits 0:15 use field IPI2PCIE_1_I1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT */
        int_agr_reg_IPI2PCIE_1_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF + subfield_offset),
                                                              INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2PCIE_1_I1_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2PCIE_1_I1_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_1_I1_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_1_I1_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001334 bits 15:0) bits 0:15 use field IPI2PCIE_1_I1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT */
    reg_value = int_agr_reg_IPI2PCIE_1_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_MSK)
                  >> INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_MSK, INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_1_I1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_1_I1_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_1_I1_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_1_I1_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_1_I1_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_1_I1_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001334 bits 15:0) bits 0:15 use field IPI2PCIE_1_I1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001334 bits 15:0) bits 0:15 use field IPI2PCIE_1_I1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT */
        return int_agr_reg_IPI2PCIE_1_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF + subfield_offset),
                                                value << (INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_1_I1_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_1_I1_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "int_agr_field_IPI2PCIE_1_I1_poll", value );

    /* (0x00001334 bits 15:0) bits 0:15 use field IPI2PCIE_1_I1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT */
    return int_agr_reg_IPI2PCIE_1_INT_poll( b_ptr,
                                            h_ptr,
                                            INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_MSK,
                                            (value<<INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void int_agr_field_IPI2PCIE_2_I2_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2PCIE_2_I2_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2PCIE_2_I2_set_to_clear", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2PCIE_2_I2_set_to_clear", value );

    /* (0x00001338 bits 15:0) bits 0:15 use field IPI2PCIE_2_I2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT */
    int_agr_reg_IPI2PCIE_2_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_MSK,
                                                          INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF,
                                                          value);
}

static INLINE UINT32 int_agr_field_IPI2PCIE_2_I2_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2PCIE_2_I2_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001338 bits 15:0) bits 0:15 use field IPI2PCIE_2_I2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT */
    reg_value = int_agr_reg_IPI2PCIE_2_INT_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_MSK) >> INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2PCIE_2_I2_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2PCIE_2_I2_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2PCIE_2_I2_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_2_I2_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_2_I2_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_2_I2_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001338 bits 15:0) bits 0:15 use field IPI2PCIE_2_I2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001338 bits 15:0) bits 0:15 use field IPI2PCIE_2_I2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT */
        int_agr_reg_IPI2PCIE_2_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF + subfield_offset),
                                                              INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2PCIE_2_I2_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2PCIE_2_I2_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_2_I2_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_2_I2_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001338 bits 15:0) bits 0:15 use field IPI2PCIE_2_I2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT */
    reg_value = int_agr_reg_IPI2PCIE_2_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_MSK)
                  >> INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_MSK, INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_2_I2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_2_I2_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_2_I2_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_2_I2_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_2_I2_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_2_I2_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001338 bits 15:0) bits 0:15 use field IPI2PCIE_2_I2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001338 bits 15:0) bits 0:15 use field IPI2PCIE_2_I2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT */
        return int_agr_reg_IPI2PCIE_2_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF + subfield_offset),
                                                value << (INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_2_I2_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_2_I2_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "int_agr_field_IPI2PCIE_2_I2_poll", value );

    /* (0x00001338 bits 15:0) bits 0:15 use field IPI2PCIE_2_I2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT */
    return int_agr_reg_IPI2PCIE_2_INT_poll( b_ptr,
                                            h_ptr,
                                            INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_MSK,
                                            (value<<INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void int_agr_field_IPI2PCIE_3_I3_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2PCIE_3_I3_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2PCIE_3_I3_set_to_clear", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2PCIE_3_I3_set_to_clear", value );

    /* (0x0000133c bits 15:0) bits 0:15 use field IPI2PCIE_3_I3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT */
    int_agr_reg_IPI2PCIE_3_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_MSK,
                                                          INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF,
                                                          value);
}

static INLINE UINT32 int_agr_field_IPI2PCIE_3_I3_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2PCIE_3_I3_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000133c bits 15:0) bits 0:15 use field IPI2PCIE_3_I3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT */
    reg_value = int_agr_reg_IPI2PCIE_3_INT_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_MSK) >> INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2PCIE_3_I3_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2PCIE_3_I3_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2PCIE_3_I3_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_3_I3_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_3_I3_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_3_I3_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000133c bits 15:0) bits 0:15 use field IPI2PCIE_3_I3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000133c bits 15:0) bits 0:15 use field IPI2PCIE_3_I3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT */
        int_agr_reg_IPI2PCIE_3_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF + subfield_offset),
                                                              INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2PCIE_3_I3_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2PCIE_3_I3_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_3_I3_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_3_I3_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000133c bits 15:0) bits 0:15 use field IPI2PCIE_3_I3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT */
    reg_value = int_agr_reg_IPI2PCIE_3_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_MSK)
                  >> INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_MSK, INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_3_I3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_3_I3_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_3_I3_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_3_I3_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_3_I3_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_3_I3_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000133c bits 15:0) bits 0:15 use field IPI2PCIE_3_I3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000133c bits 15:0) bits 0:15 use field IPI2PCIE_3_I3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT */
        return int_agr_reg_IPI2PCIE_3_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF + subfield_offset),
                                                value << (INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_3_I3_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_3_I3_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "int_agr_field_IPI2PCIE_3_I3_poll", value );

    /* (0x0000133c bits 15:0) bits 0:15 use field IPI2PCIE_3_I3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT */
    return int_agr_reg_IPI2PCIE_3_INT_poll( b_ptr,
                                            h_ptr,
                                            INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_MSK,
                                            (value<<INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void int_agr_field_IPI2MIPS_0_I4_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2MIPS_0_I4_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2MIPS_0_I4_set_to_clear", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2MIPS_0_I4_set_to_clear", value );

    /* (0x00001350 bits 15:0) bits 0:15 use field IPI2MIPS_0_I4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT */
    int_agr_reg_IPI2MIPS_0_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_MSK,
                                                          INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF,
                                                          value);
}

static INLINE UINT32 int_agr_field_IPI2MIPS_0_I4_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2MIPS_0_I4_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001350 bits 15:0) bits 0:15 use field IPI2MIPS_0_I4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT */
    reg_value = int_agr_reg_IPI2MIPS_0_INT_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_MSK) >> INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2MIPS_0_I4_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2MIPS_0_I4_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2MIPS_0_I4_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_0_I4_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_0_I4_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_0_I4_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001350 bits 15:0) bits 0:15 use field IPI2MIPS_0_I4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001350 bits 15:0) bits 0:15 use field IPI2MIPS_0_I4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT */
        int_agr_reg_IPI2MIPS_0_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF + subfield_offset),
                                                              INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2MIPS_0_I4_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2MIPS_0_I4_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_0_I4_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_0_I4_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001350 bits 15:0) bits 0:15 use field IPI2MIPS_0_I4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT */
    reg_value = int_agr_reg_IPI2MIPS_0_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_MSK)
                  >> INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_MSK, INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_0_I4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_0_I4_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_0_I4_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_0_I4_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_0_I4_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_0_I4_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001350 bits 15:0) bits 0:15 use field IPI2MIPS_0_I4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001350 bits 15:0) bits 0:15 use field IPI2MIPS_0_I4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT */
        return int_agr_reg_IPI2MIPS_0_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF + subfield_offset),
                                                value << (INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_0_I4_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_0_I4_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "int_agr_field_IPI2MIPS_0_I4_poll", value );

    /* (0x00001350 bits 15:0) bits 0:15 use field IPI2MIPS_0_I4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT */
    return int_agr_reg_IPI2MIPS_0_INT_poll( b_ptr,
                                            h_ptr,
                                            INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_MSK,
                                            (value<<INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void int_agr_field_IPI2MIPS_1_I5_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2MIPS_1_I5_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2MIPS_1_I5_set_to_clear", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2MIPS_1_I5_set_to_clear", value );

    /* (0x00001354 bits 15:0) bits 0:15 use field IPI2MIPS_1_I5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT */
    int_agr_reg_IPI2MIPS_1_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_MSK,
                                                          INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF,
                                                          value);
}

static INLINE UINT32 int_agr_field_IPI2MIPS_1_I5_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2MIPS_1_I5_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001354 bits 15:0) bits 0:15 use field IPI2MIPS_1_I5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT */
    reg_value = int_agr_reg_IPI2MIPS_1_INT_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_MSK) >> INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2MIPS_1_I5_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2MIPS_1_I5_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2MIPS_1_I5_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_1_I5_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_1_I5_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_1_I5_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001354 bits 15:0) bits 0:15 use field IPI2MIPS_1_I5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001354 bits 15:0) bits 0:15 use field IPI2MIPS_1_I5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT */
        int_agr_reg_IPI2MIPS_1_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF + subfield_offset),
                                                              INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2MIPS_1_I5_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2MIPS_1_I5_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_1_I5_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_1_I5_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001354 bits 15:0) bits 0:15 use field IPI2MIPS_1_I5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT */
    reg_value = int_agr_reg_IPI2MIPS_1_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_MSK)
                  >> INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_MSK, INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_1_I5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_1_I5_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_1_I5_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_1_I5_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_1_I5_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_1_I5_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001354 bits 15:0) bits 0:15 use field IPI2MIPS_1_I5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001354 bits 15:0) bits 0:15 use field IPI2MIPS_1_I5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT */
        return int_agr_reg_IPI2MIPS_1_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF + subfield_offset),
                                                value << (INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_1_I5_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_1_I5_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "int_agr_field_IPI2MIPS_1_I5_poll", value );

    /* (0x00001354 bits 15:0) bits 0:15 use field IPI2MIPS_1_I5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT */
    return int_agr_reg_IPI2MIPS_1_INT_poll( b_ptr,
                                            h_ptr,
                                            INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_MSK,
                                            (value<<INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void int_agr_field_IPI2MIPS_2_I6_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2MIPS_2_I6_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2MIPS_2_I6_set_to_clear", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2MIPS_2_I6_set_to_clear", value );

    /* (0x00001358 bits 15:0) bits 0:15 use field IPI2MIPS_2_I6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT */
    int_agr_reg_IPI2MIPS_2_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_MSK,
                                                          INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF,
                                                          value);
}

static INLINE UINT32 int_agr_field_IPI2MIPS_2_I6_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2MIPS_2_I6_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001358 bits 15:0) bits 0:15 use field IPI2MIPS_2_I6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT */
    reg_value = int_agr_reg_IPI2MIPS_2_INT_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_MSK) >> INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2MIPS_2_I6_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2MIPS_2_I6_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2MIPS_2_I6_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_2_I6_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_2_I6_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_2_I6_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001358 bits 15:0) bits 0:15 use field IPI2MIPS_2_I6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001358 bits 15:0) bits 0:15 use field IPI2MIPS_2_I6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT */
        int_agr_reg_IPI2MIPS_2_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF + subfield_offset),
                                                              INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2MIPS_2_I6_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2MIPS_2_I6_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_2_I6_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_2_I6_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001358 bits 15:0) bits 0:15 use field IPI2MIPS_2_I6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT */
    reg_value = int_agr_reg_IPI2MIPS_2_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_MSK)
                  >> INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_MSK, INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_2_I6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_2_I6_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_2_I6_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_2_I6_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_2_I6_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_2_I6_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001358 bits 15:0) bits 0:15 use field IPI2MIPS_2_I6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001358 bits 15:0) bits 0:15 use field IPI2MIPS_2_I6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT */
        return int_agr_reg_IPI2MIPS_2_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF + subfield_offset),
                                                value << (INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_2_I6_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_2_I6_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "int_agr_field_IPI2MIPS_2_I6_poll", value );

    /* (0x00001358 bits 15:0) bits 0:15 use field IPI2MIPS_2_I6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT */
    return int_agr_reg_IPI2MIPS_2_INT_poll( b_ptr,
                                            h_ptr,
                                            INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_MSK,
                                            (value<<INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void int_agr_field_IPI2MIPS_3_I7_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2MIPS_3_I7_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2MIPS_3_I7_set_to_clear", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2MIPS_3_I7_set_to_clear", value );

    /* (0x0000135c bits 15:0) bits 0:15 use field IPI2MIPS_3_I7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT */
    int_agr_reg_IPI2MIPS_3_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_MSK,
                                                          INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF,
                                                          value);
}

static INLINE UINT32 int_agr_field_IPI2MIPS_3_I7_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2MIPS_3_I7_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000135c bits 15:0) bits 0:15 use field IPI2MIPS_3_I7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT */
    reg_value = int_agr_reg_IPI2MIPS_3_INT_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_MSK) >> INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2MIPS_3_I7_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2MIPS_3_I7_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2MIPS_3_I7_set_to_clear( int_agr_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_3_I7_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_3_I7_set_to_clear", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_3_I7_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000135c bits 15:0) bits 0:15 use field IPI2MIPS_3_I7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000135c bits 15:0) bits 0:15 use field IPI2MIPS_3_I7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT */
        int_agr_reg_IPI2MIPS_3_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF + subfield_offset),
                                                              INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2MIPS_3_I7_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2MIPS_3_I7_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_3_I7_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_3_I7_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000135c bits 15:0) bits 0:15 use field IPI2MIPS_3_I7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT */
    reg_value = int_agr_reg_IPI2MIPS_3_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_MSK)
                  >> INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_MSK, INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_3_I7_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_3_I7_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_3_I7_poll( int_agr_buffer_t *b_ptr,
                                                                           pmc_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_3_I7_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_3_I7_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_3_I7_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000135c bits 15:0) bits 0:15 use field IPI2MIPS_3_I7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000135c bits 15:0) bits 0:15 use field IPI2MIPS_3_I7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT */
        return int_agr_reg_IPI2MIPS_3_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF + subfield_offset),
                                                value << (INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_3_I7_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_3_I7_poll( int_agr_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "int_agr_field_IPI2MIPS_3_I7_poll", value );

    /* (0x0000135c bits 15:0) bits 0:15 use field IPI2MIPS_3_I7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT */
    return int_agr_reg_IPI2MIPS_3_INT_poll( b_ptr,
                                            h_ptr,
                                            INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_MSK,
                                            (value<<INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void int_agr_field_IPI2PCIE_0_E0_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2PCIE_0_E0_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2PCIE_0_E0_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2PCIE_0_E0_set", value );

    /* (0x00001340 bits 15:0) bits 0:15 use field IPI2PCIE_0_E0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN */
    int_agr_reg_IPI2PCIE_0_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_MSK,
                                             INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_OFF,
                                             value);
}

static INLINE UINT32 int_agr_field_IPI2PCIE_0_E0_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2PCIE_0_E0_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001340 bits 15:0) bits 0:15 use field IPI2PCIE_0_E0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN */
    reg_value = int_agr_reg_IPI2PCIE_0_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_MSK) >> INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2PCIE_0_E0_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2PCIE_0_E0_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2PCIE_0_E0_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_0_E0_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_0_E0_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_0_E0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001340 bits 15:0) bits 0:15 use field IPI2PCIE_0_E0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001340 bits 15:0) bits 0:15 use field IPI2PCIE_0_E0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN */
        int_agr_reg_IPI2PCIE_0_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_OFF + subfield_offset),
                                                 INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2PCIE_0_E0_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2PCIE_0_E0_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_0_E0_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_0_E0_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001340 bits 15:0) bits 0:15 use field IPI2PCIE_0_E0 of register PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN */
    reg_value = int_agr_reg_IPI2PCIE_0_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_MSK)
                  >> INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_MSK, INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_0_E0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_IPI2PCIE_1_E1_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2PCIE_1_E1_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2PCIE_1_E1_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2PCIE_1_E1_set", value );

    /* (0x00001344 bits 15:0) bits 0:15 use field IPI2PCIE_1_E1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN */
    int_agr_reg_IPI2PCIE_1_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_MSK,
                                             INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_OFF,
                                             value);
}

static INLINE UINT32 int_agr_field_IPI2PCIE_1_E1_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2PCIE_1_E1_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001344 bits 15:0) bits 0:15 use field IPI2PCIE_1_E1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN */
    reg_value = int_agr_reg_IPI2PCIE_1_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_MSK) >> INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2PCIE_1_E1_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2PCIE_1_E1_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2PCIE_1_E1_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_1_E1_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_1_E1_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_1_E1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001344 bits 15:0) bits 0:15 use field IPI2PCIE_1_E1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001344 bits 15:0) bits 0:15 use field IPI2PCIE_1_E1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN */
        int_agr_reg_IPI2PCIE_1_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_OFF + subfield_offset),
                                                 INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2PCIE_1_E1_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2PCIE_1_E1_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_1_E1_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_1_E1_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001344 bits 15:0) bits 0:15 use field IPI2PCIE_1_E1 of register PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN */
    reg_value = int_agr_reg_IPI2PCIE_1_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_MSK)
                  >> INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_MSK, INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_1_E1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_IPI2PCIE_2_E2_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2PCIE_2_E2_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2PCIE_2_E2_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2PCIE_2_E2_set", value );

    /* (0x00001348 bits 15:0) bits 0:15 use field IPI2PCIE_2_E2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN */
    int_agr_reg_IPI2PCIE_2_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_MSK,
                                             INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_OFF,
                                             value);
}

static INLINE UINT32 int_agr_field_IPI2PCIE_2_E2_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2PCIE_2_E2_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001348 bits 15:0) bits 0:15 use field IPI2PCIE_2_E2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN */
    reg_value = int_agr_reg_IPI2PCIE_2_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_MSK) >> INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2PCIE_2_E2_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2PCIE_2_E2_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2PCIE_2_E2_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_2_E2_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_2_E2_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_2_E2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001348 bits 15:0) bits 0:15 use field IPI2PCIE_2_E2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001348 bits 15:0) bits 0:15 use field IPI2PCIE_2_E2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN */
        int_agr_reg_IPI2PCIE_2_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_OFF + subfield_offset),
                                                 INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2PCIE_2_E2_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2PCIE_2_E2_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_2_E2_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_2_E2_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001348 bits 15:0) bits 0:15 use field IPI2PCIE_2_E2 of register PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN */
    reg_value = int_agr_reg_IPI2PCIE_2_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_MSK)
                  >> INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_MSK, INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_2_E2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_IPI2PCIE_3_E3_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2PCIE_3_E3_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2PCIE_3_E3_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2PCIE_3_E3_set", value );

    /* (0x0000134c bits 15:0) bits 0:15 use field IPI2PCIE_3_E3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN */
    int_agr_reg_IPI2PCIE_3_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_MSK,
                                             INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_OFF,
                                             value);
}

static INLINE UINT32 int_agr_field_IPI2PCIE_3_E3_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2PCIE_3_E3_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000134c bits 15:0) bits 0:15 use field IPI2PCIE_3_E3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN */
    reg_value = int_agr_reg_IPI2PCIE_3_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_MSK) >> INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2PCIE_3_E3_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2PCIE_3_E3_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2PCIE_3_E3_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_3_E3_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_3_E3_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_3_E3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000134c bits 15:0) bits 0:15 use field IPI2PCIE_3_E3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000134c bits 15:0) bits 0:15 use field IPI2PCIE_3_E3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN */
        int_agr_reg_IPI2PCIE_3_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_OFF + subfield_offset),
                                                 INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2PCIE_3_E3_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2PCIE_3_E3_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2PCIE_3_E3_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2PCIE_3_E3_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000134c bits 15:0) bits 0:15 use field IPI2PCIE_3_E3 of register PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN */
    reg_value = int_agr_reg_IPI2PCIE_3_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_MSK)
                  >> INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_MSK, INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2PCIE_3_E3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_IPI2MIPS_0_E4_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2MIPS_0_E4_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2MIPS_0_E4_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2MIPS_0_E4_set", value );

    /* (0x00001360 bits 15:0) bits 0:15 use field IPI2MIPS_0_E4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN */
    int_agr_reg_IPI2MIPS_0_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_MSK,
                                             INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_OFF,
                                             value);
}

static INLINE UINT32 int_agr_field_IPI2MIPS_0_E4_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2MIPS_0_E4_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001360 bits 15:0) bits 0:15 use field IPI2MIPS_0_E4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN */
    reg_value = int_agr_reg_IPI2MIPS_0_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_MSK) >> INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2MIPS_0_E4_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2MIPS_0_E4_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2MIPS_0_E4_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_0_E4_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_0_E4_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_0_E4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001360 bits 15:0) bits 0:15 use field IPI2MIPS_0_E4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001360 bits 15:0) bits 0:15 use field IPI2MIPS_0_E4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN */
        int_agr_reg_IPI2MIPS_0_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_OFF + subfield_offset),
                                                 INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2MIPS_0_E4_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2MIPS_0_E4_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_0_E4_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_0_E4_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001360 bits 15:0) bits 0:15 use field IPI2MIPS_0_E4 of register PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN */
    reg_value = int_agr_reg_IPI2MIPS_0_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_MSK)
                  >> INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_MSK, INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_0_E4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_IPI2MIPS_1_E5_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2MIPS_1_E5_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2MIPS_1_E5_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2MIPS_1_E5_set", value );

    /* (0x00001364 bits 15:0) bits 0:15 use field IPI2MIPS_1_E5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN */
    int_agr_reg_IPI2MIPS_1_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_MSK,
                                             INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_OFF,
                                             value);
}

static INLINE UINT32 int_agr_field_IPI2MIPS_1_E5_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2MIPS_1_E5_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001364 bits 15:0) bits 0:15 use field IPI2MIPS_1_E5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN */
    reg_value = int_agr_reg_IPI2MIPS_1_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_MSK) >> INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2MIPS_1_E5_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2MIPS_1_E5_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2MIPS_1_E5_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_1_E5_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_1_E5_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_1_E5_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001364 bits 15:0) bits 0:15 use field IPI2MIPS_1_E5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001364 bits 15:0) bits 0:15 use field IPI2MIPS_1_E5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN */
        int_agr_reg_IPI2MIPS_1_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_OFF + subfield_offset),
                                                 INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2MIPS_1_E5_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2MIPS_1_E5_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_1_E5_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_1_E5_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001364 bits 15:0) bits 0:15 use field IPI2MIPS_1_E5 of register PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN */
    reg_value = int_agr_reg_IPI2MIPS_1_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_MSK)
                  >> INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_MSK, INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_1_E5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_IPI2MIPS_2_E6_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2MIPS_2_E6_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2MIPS_2_E6_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2MIPS_2_E6_set", value );

    /* (0x00001368 bits 15:0) bits 0:15 use field IPI2MIPS_2_E6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN */
    int_agr_reg_IPI2MIPS_2_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_MSK,
                                             INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_OFF,
                                             value);
}

static INLINE UINT32 int_agr_field_IPI2MIPS_2_E6_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2MIPS_2_E6_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001368 bits 15:0) bits 0:15 use field IPI2MIPS_2_E6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN */
    reg_value = int_agr_reg_IPI2MIPS_2_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_MSK) >> INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2MIPS_2_E6_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2MIPS_2_E6_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2MIPS_2_E6_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_2_E6_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_2_E6_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_2_E6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001368 bits 15:0) bits 0:15 use field IPI2MIPS_2_E6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001368 bits 15:0) bits 0:15 use field IPI2MIPS_2_E6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN */
        int_agr_reg_IPI2MIPS_2_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_OFF + subfield_offset),
                                                 INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2MIPS_2_E6_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2MIPS_2_E6_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_2_E6_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_2_E6_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001368 bits 15:0) bits 0:15 use field IPI2MIPS_2_E6 of register PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN */
    reg_value = int_agr_reg_IPI2MIPS_2_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_MSK)
                  >> INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_MSK, INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_2_E6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void int_agr_field_IPI2MIPS_3_E7_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_IPI2MIPS_3_E7_set( int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "int_agr_field_IPI2MIPS_3_E7_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "int_agr_field_IPI2MIPS_3_E7_set", value );

    /* (0x0000136c bits 15:0) bits 0:15 use field IPI2MIPS_3_E7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN */
    int_agr_reg_IPI2MIPS_3_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_MSK,
                                             INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_OFF,
                                             value);
}

static INLINE UINT32 int_agr_field_IPI2MIPS_3_E7_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_IPI2MIPS_3_E7_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000136c bits 15:0) bits 0:15 use field IPI2MIPS_3_E7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN */
    reg_value = int_agr_reg_IPI2MIPS_3_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_MSK) >> INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_IPI2MIPS_3_E7_get", value );

    return value;
}
static INLINE void int_agr_field_range_IPI2MIPS_3_E7_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void int_agr_field_range_IPI2MIPS_3_E7_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_3_E7_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_3_E7_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_3_E7_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000136c bits 15:0) bits 0:15 use field IPI2MIPS_3_E7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000136c bits 15:0) bits 0:15 use field IPI2MIPS_3_E7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN */
        int_agr_reg_IPI2MIPS_3_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_OFF + subfield_offset),
                                                 INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 int_agr_field_range_IPI2MIPS_3_E7_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_IPI2MIPS_3_E7_get( int_agr_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_IPI2MIPS_3_E7_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_IPI2MIPS_3_E7_get", stop_bit, 15 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 15) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 15;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000136c bits 15:0) bits 0:15 use field IPI2MIPS_3_E7 of register PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN */
    reg_value = int_agr_reg_IPI2MIPS_3_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_MSK)
                  >> INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_MSK, INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_IPI2MIPS_3_E7_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 int_agr_field_DEVICE_EN_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_DEVICE_EN_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010d0 bits 31:0) bits 0:31 use field DEVICE_EN of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_EN_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_BIT_DEVICE_EN_MSK) >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_BIT_DEVICE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_DEVICE_EN_get", value );

    return value;
}
static INLINE UINT32 int_agr_field_range_DEVICE_EN_get( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_DEVICE_EN_get( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_DEVICE_EN_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_DEVICE_EN_get", stop_bit, 31 );
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
    /* (0x000010d0 bits 31:0) bits 0:31 use field DEVICE_EN of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_EN_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_BIT_DEVICE_EN_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_BIT_DEVICE_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_BIT_DEVICE_EN_MSK, INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_BIT_DEVICE_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_DEVICE_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 int_agr_field_DEVICE_STAT_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_DEVICE_STAT_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010d4 bits 31:0) bits 0:31 use field DEVICE_STAT of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_STAT_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_BIT_DEVICE_STAT_MSK) >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_BIT_DEVICE_STAT_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_DEVICE_STAT_get", value );

    return value;
}
static INLINE UINT32 int_agr_field_range_DEVICE_STAT_get( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_DEVICE_STAT_get( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_DEVICE_STAT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_DEVICE_STAT_get", stop_bit, 31 );
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
    /* (0x000010d4 bits 31:0) bits 0:31 use field DEVICE_STAT of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_STAT_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_BIT_DEVICE_STAT_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_BIT_DEVICE_STAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_BIT_DEVICE_STAT_MSK, INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_BIT_DEVICE_STAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_DEVICE_STAT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 int_agr_field_DEVICE_RSTAT_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_DEVICE_RSTAT_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010d8 bits 31:0) bits 0:31 use field DEVICE_RSTAT of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_RAW_STAT_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_BIT_DEVICE_RSTAT_MSK) >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_BIT_DEVICE_RSTAT_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_DEVICE_RSTAT_get", value );

    return value;
}
static INLINE UINT32 int_agr_field_range_DEVICE_RSTAT_get( int_agr_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_DEVICE_RSTAT_get( int_agr_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_DEVICE_RSTAT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_DEVICE_RSTAT_get", stop_bit, 31 );
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
    /* (0x000010d8 bits 31:0) bits 0:31 use field DEVICE_RSTAT of register PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT */
    reg_value = int_agr_reg_INT_AGGREGATOR_DEVICE_RAW_STAT_read( b_ptr,
                                                                 h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_BIT_DEVICE_RSTAT_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_BIT_DEVICE_RSTAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_BIT_DEVICE_RSTAT_MSK, INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_BIT_DEVICE_RSTAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_DEVICE_RSTAT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 int_agr_field_MISC_EN_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_MISC_EN_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010e4 bits 31:0) bits 0:31 use field MISC_EN of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_EN */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_EN_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_EN_BIT_MISC_EN_MSK) >> INT_AGR_REG_INT_AGGREGATOR_MISC_EN_BIT_MISC_EN_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_MISC_EN_get", value );

    return value;
}
static INLINE UINT32 int_agr_field_range_MISC_EN_get( int_agr_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_MISC_EN_get( int_agr_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_MISC_EN_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_MISC_EN_get", stop_bit, 31 );
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
    /* (0x000010e4 bits 31:0) bits 0:31 use field MISC_EN of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_EN */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_EN_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_EN_BIT_MISC_EN_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_MISC_EN_BIT_MISC_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_MISC_EN_BIT_MISC_EN_MSK, INT_AGR_REG_INT_AGGREGATOR_MISC_EN_BIT_MISC_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_MISC_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 int_agr_field_MISC_STAT_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_MISC_STAT_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010e8 bits 31:0) bits 0:31 use field MISC_STAT of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_STAT */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_BIT_MISC_STAT_MSK) >> INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_BIT_MISC_STAT_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_MISC_STAT_get", value );

    return value;
}
static INLINE UINT32 int_agr_field_range_MISC_STAT_get( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_MISC_STAT_get( int_agr_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_MISC_STAT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_MISC_STAT_get", stop_bit, 31 );
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
    /* (0x000010e8 bits 31:0) bits 0:31 use field MISC_STAT of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_STAT */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_STAT_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_BIT_MISC_STAT_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_BIT_MISC_STAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_BIT_MISC_STAT_MSK, INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_BIT_MISC_STAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_MISC_STAT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 int_agr_field_MISC_RSTAT_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_MISC_RSTAT_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000010ec bits 31:0) bits 0:31 use field MISC_RSTAT of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_RAW_STAT_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_BIT_MISC_RSTAT_MSK) >> INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_BIT_MISC_RSTAT_OFF;
    IOLOG( "%s -> 0x%08x", "int_agr_field_MISC_RSTAT_get", value );

    return value;
}
static INLINE UINT32 int_agr_field_range_MISC_RSTAT_get( int_agr_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 int_agr_field_range_MISC_RSTAT_get( int_agr_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "int_agr_field_range_MISC_RSTAT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "int_agr_field_range_MISC_RSTAT_get", stop_bit, 31 );
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
    /* (0x000010ec bits 31:0) bits 0:31 use field MISC_RSTAT of register PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT */
    reg_value = int_agr_reg_INT_AGGREGATOR_MISC_RAW_STAT_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_BIT_MISC_RSTAT_MSK)
                  >> INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_BIT_MISC_RSTAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_BIT_MISC_RSTAT_MSK, INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_BIT_MISC_RSTAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "int_agr_field_range_MISC_RSTAT_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INT_AGR_IO_INLINE_H */
