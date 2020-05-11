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
 *     and register accessor functions for the t8_adsp block
 *****************************************************************************/
#ifndef _T8_ADSP_IO_INLINE_H
#define _T8_ADSP_IO_INLINE_H

#include "t8.h"
#include "t8_adsp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define T8_ADSP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for t8_adsp
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
    t8_handle_t * h_ptr;
    UINT32 id;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} t8_adsp_buffer_t;
static INLINE void t8_adsp_buffer_init( t8_adsp_buffer_t *b_ptr,
                                        t8_handle_t *h_ptr,
                                        UINT32 id ) ALWAYS_INLINE;
static INLINE void t8_adsp_buffer_init( t8_adsp_buffer_t *b_ptr,
                                        t8_handle_t *h_ptr,
                                        UINT32 id )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->id                       = id;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x600) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s id=%d", "t8_adsp_buffer_init", id);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void t8_adsp_buffer_flush( t8_adsp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void t8_adsp_buffer_flush( t8_adsp_buffer_t *b_ptr )
{
    IOLOG( "t8_adsp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 t8_adsp_reg_read( t8_adsp_buffer_t *b_ptr,
                                       t8_handle_t *h_ptr,
                                       UINT32 id,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_read( t8_adsp_buffer_t *b_ptr,
                                       t8_handle_t *h_ptr,
                                       UINT32 id,
                                       UINT32 mem_type,
                                       UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->id)*0x600),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (id)*0x600) + reg);
    return value;
}
static INLINE void t8_adsp_reg_write( t8_adsp_buffer_t *b_ptr,
                                      t8_handle_t *h_ptr,
                                      UINT32 id,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_write( t8_adsp_buffer_t *b_ptr,
                                      t8_handle_t *h_ptr,
                                      UINT32 id,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->id)*0x600),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (id)*0x600) + reg,
                         value);
}

static INLINE void t8_adsp_field_set( t8_adsp_buffer_t *b_ptr,
                                      t8_handle_t *h_ptr,
                                      UINT32 id,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_set( t8_adsp_buffer_t *b_ptr,
                                      t8_handle_t *h_ptr,
                                      UINT32 id,
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
                          (b_ptr->base_address + (b_ptr->id)*0x600),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (id)*0x600) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (id)*0x600) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void t8_adsp_action_on_write_field_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_action_on_write_field_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         (b_ptr->base_address + (b_ptr->id)*0x600),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (id)*0x600) + reg,
                         value<<ofs);
}

static INLINE void t8_adsp_burst_read( t8_adsp_buffer_t *b_ptr,
                                       t8_handle_t *h_ptr,
                                       UINT32 id,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void t8_adsp_burst_read( t8_adsp_buffer_t *b_ptr,
                                       t8_handle_t *h_ptr,
                                       UINT32 id,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->id)*0x600),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (id)*0x600) + reg,
                          len,
                          value);
}

static INLINE void t8_adsp_burst_write( t8_adsp_buffer_t *b_ptr,
                                        t8_handle_t *h_ptr,
                                        UINT32 id,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void t8_adsp_burst_write( t8_adsp_buffer_t *b_ptr,
                                        t8_handle_t *h_ptr,
                                        UINT32 id,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->id)*0x600),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (id)*0x600) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE t8_adsp_poll( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_poll( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
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
                           (b_ptr->base_address + (b_ptr->id)*0x600),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (id)*0x600) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for t8_adsp
 * ==================================================================================
 */

static INLINE void t8_adsp_reg_ADAPT_DSP_CLK_CTRL_write( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_DSP_CLK_CTRL_write( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_ADAPT_DSP_CLK_CTRL_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL,
                       value);
}

static INLINE void t8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL,
                       mask,
                       PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_ADAPT_DSP_CLK_CTRL_read( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_DSP_CLK_CTRL_read( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_DSP_CLK_CTRL_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_ADAPT_CTRL_1_write( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_CTRL_1_write( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_ADAPT_CTRL_1_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_CTRL_1,
                       value);
}

static INLINE void t8_adsp_reg_ADAPT_CTRL_1_field_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_CTRL_1_field_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_ADAPT_CTRL_1_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_CTRL_1,
                       mask,
                       PMC_ADSP_REG_ADAPT_CTRL_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_ADAPT_CTRL_1_read( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_CTRL_1_read( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_ADAPT_CTRL_1);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_CTRL_1_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_ADAPT_CTRL_2_write( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_CTRL_2_write( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_ADAPT_CTRL_2_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_CTRL_2,
                       value);
}

static INLINE void t8_adsp_reg_ADAPT_CTRL_2_field_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_CTRL_2_field_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_ADAPT_CTRL_2_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_CTRL_2,
                       mask,
                       PMC_ADSP_REG_ADAPT_CTRL_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_ADAPT_CTRL_2_read( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_CTRL_2_read( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_ADAPT_CTRL_2);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_CTRL_2_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_DSS_CTRL_write( t8_adsp_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_DSS_CTRL_write( t8_adsp_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_DSS_CTRL_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_CTRL,
                       value);
}

static INLINE void t8_adsp_reg_DSS_CTRL_field_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_DSS_CTRL_field_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_DSS_CTRL_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_CTRL,
                       mask,
                       PMC_ADSP_REG_DSS_CTRL_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_DSS_CTRL_read( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_DSS_CTRL_read( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_DSS_CTRL);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_DSS_CTRL_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_DSS_TAPS_1_write( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_DSS_TAPS_1_write( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_DSS_TAPS_1_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_1,
                       value);
}

static INLINE void t8_adsp_reg_DSS_TAPS_1_field_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_DSS_TAPS_1_field_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_DSS_TAPS_1_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_1,
                       mask,
                       PMC_ADSP_REG_DSS_TAPS_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_DSS_TAPS_1_read( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_DSS_TAPS_1_read( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_DSS_TAPS_1);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_DSS_TAPS_1_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_DSS_TAPS_2_write( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_DSS_TAPS_2_write( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_DSS_TAPS_2_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_2,
                       value);
}

static INLINE void t8_adsp_reg_DSS_TAPS_2_field_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_DSS_TAPS_2_field_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_DSS_TAPS_2_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_2,
                       mask,
                       PMC_ADSP_REG_DSS_TAPS_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_DSS_TAPS_2_read( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_DSS_TAPS_2_read( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_DSS_TAPS_2);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_DSS_TAPS_2_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_DSS_TAPS_3_write( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_DSS_TAPS_3_write( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_DSS_TAPS_3_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_3,
                       value);
}

static INLINE void t8_adsp_reg_DSS_TAPS_3_field_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_DSS_TAPS_3_field_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_DSS_TAPS_3_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_3,
                       mask,
                       PMC_ADSP_REG_DSS_TAPS_3_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_DSS_TAPS_3_read( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_DSS_TAPS_3_read( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_DSS_TAPS_3);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_DSS_TAPS_3_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_LMS_MU_FACTORS_write( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_LMS_MU_FACTORS_write( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_LMS_MU_FACTORS_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_LMS_MU_FACTORS,
                       value);
}

static INLINE void t8_adsp_reg_LMS_MU_FACTORS_field_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_LMS_MU_FACTORS_field_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_LMS_MU_FACTORS_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_LMS_MU_FACTORS,
                       mask,
                       PMC_ADSP_REG_LMS_MU_FACTORS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_LMS_MU_FACTORS_read( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_LMS_MU_FACTORS_read( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_LMS_MU_FACTORS);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_LMS_MU_FACTORS_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_ROUND_ROBIN_PERIODS_write( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ROUND_ROBIN_PERIODS_write( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_ROUND_ROBIN_PERIODS_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ROUND_ROBIN_PERIODS,
                       value);
}

static INLINE void t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ROUND_ROBIN_PERIODS,
                       mask,
                       PMC_ADSP_REG_ROUND_ROBIN_PERIODS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_ROUND_ROBIN_PERIODS_read( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ROUND_ROBIN_PERIODS_read( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_ROUND_ROBIN_PERIODS);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ROUND_ROBIN_PERIODS_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_1_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_1_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_OBJECT_PRELOAD_VAL_1_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1,
                       value);
}

static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1,
                       mask,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_2_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_2_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_OBJECT_PRELOAD_VAL_2_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2,
                       value);
}

static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2,
                       mask,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_3_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_3_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_OBJECT_PRELOAD_VAL_3_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3,
                       value);
}

static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3,
                       mask,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_OBJECT_PRELOAD_VAL_3_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_4_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_4_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_OBJECT_PRELOAD_VAL_4_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4,
                       value);
}

static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4,
                       mask,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_CLIP_RATE_MON_PARAMS_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_CLIP_RATE_MON_PARAMS_write( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_CLIP_RATE_MON_PARAMS_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_CLIP_RATE_MON_PARAMS,
                       value);
}

static INLINE void t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_CLIP_RATE_MON_PARAMS,
                       mask,
                       PMC_ADSP_REG_CLIP_RATE_MON_PARAMS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_CLIP_RATE_MON_PARAMS);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_CLIP_RATE_MON_PARAMS_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_ADAPT_STAT_write( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_STAT_write( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_ADAPT_STAT_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_ADAPT_STAT,
                       value);
}

static INLINE void t8_adsp_reg_ADAPT_STAT_field_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_STAT_field_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_ADAPT_STAT_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_ADAPT_STAT,
                       mask,
                       PMC_ADSP_REG_ADAPT_STAT_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_ADAPT_STAT_read( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_STAT_read( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_STAT);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_STAT_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST,
                       value);
}

static INLINE void t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( t8_adsp_buffer_t *b_ptr,
                                                                        t8_handle_t *h_ptr,
                                                                        UINT32 id,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( t8_adsp_buffer_t *b_ptr,
                                                                        t8_handle_t *h_ptr,
                                                                        UINT32 id,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST,
                       mask,
                       PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( t8_adsp_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( t8_adsp_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_write( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_write( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS,
                       value);
}

static INLINE void t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS,
                       mask,
                       PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( t8_adsp_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( t8_adsp_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_adsp_reg_STAT_INT_EVENT_write( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_STAT_INT_EVENT_write( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_STAT_INT_EVENT_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_STAT_INT_EVENT,
                       value);
}

static INLINE void t8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( t8_adsp_buffer_t *b_ptr,
                                                                         t8_handle_t *h_ptr,
                                                                         UINT32 id,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( t8_adsp_buffer_t *b_ptr,
                                                                         t8_handle_t *h_ptr,
                                                                         UINT32 id,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set", id, mask, ofs, value );
    t8_adsp_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       id,
                                       MEM_TYPE_STATUS,
                                       PMC_ADSP_REG_STAT_INT_EVENT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 t8_adsp_reg_STAT_INT_EVENT_read( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_STAT_INT_EVENT_read( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_STAT_INT_EVENT);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_STAT_INT_EVENT_read", reg_value, id);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_reg_STAT_INT_EVENT_poll( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_reg_STAT_INT_EVENT_poll( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "t8_adsp_reg_STAT_INT_EVENT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return t8_adsp_poll( b_ptr,
                         h_ptr,
                         id,
                         MEM_TYPE_STATUS,
                         PMC_ADSP_REG_STAT_INT_EVENT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void t8_adsp_reg_STAT_INT_EN_write( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_STAT_INT_EN_write( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_adsp_reg_STAT_INT_EN_write", value );
    t8_adsp_reg_write( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_STAT_INT_EN,
                       value);
}

static INLINE void t8_adsp_reg_STAT_INT_EN_field_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_reg_STAT_INT_EN_field_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_adsp_reg_STAT_INT_EN_field_set", id, mask, ofs, value );
    t8_adsp_field_set( b_ptr,
                       h_ptr,
                       id,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_STAT_INT_EN,
                       mask,
                       PMC_ADSP_REG_STAT_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 t8_adsp_reg_STAT_INT_EN_read( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_STAT_INT_EN_read( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_STAT_INT_EN);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_STAT_INT_EN_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_adsp_reg_STAT_INT_read( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_STAT_INT_read( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_STAT_INT);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_STAT_INT_read", reg_value, id);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_reg_STAT_INT_poll( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_reg_STAT_INT_poll( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "t8_adsp_reg_STAT_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return t8_adsp_poll( b_ptr,
                         h_ptr,
                         id,
                         MEM_TYPE_STATUS,
                         PMC_ADSP_REG_STAT_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( t8_adsp_buffer_t *b_ptr,
                                                                      t8_handle_t *h_ptr,
                                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( t8_adsp_buffer_t *b_ptr,
                                                                      t8_handle_t *h_ptr,
                                                                      UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  id,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read", reg_value, id);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void t8_adsp_field_ADAPT_CLK_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_ADAPT_CLK_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_CLK_ENB_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_ADAPT_CLK_ENB_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_ADAPT_CLK_ENB_set", id, value );

    /* (0x00000500 bits 0) field ADAPT_CLK_ENB of register PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL */
    t8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_MSK,
                                              ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_OFF,
                                              value);
}

static INLINE UINT32 t8_adsp_field_ADAPT_CLK_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADAPT_CLK_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_CLK_ENB_get", id, 2);
    /* (0x00000500 bits 0) field ADAPT_CLK_ENB of register PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL */
    reg_value = t8_adsp_reg_ADAPT_DSP_CLK_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_MSK) >> ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADAPT_CLK_ENB_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_EQ_ADAPT_CTRL_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_EQ_ADAPT_CTRL_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_EQ_ADAPT_CTRL_ENB_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_EQ_ADAPT_CTRL_ENB_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_EQ_ADAPT_CTRL_ENB_set", id, value );

    /* (0x00000504 bits 17) field EQ_ADAPT_CTRL_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_EQ_ADAPT_CTRL_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_EQ_ADAPT_CTRL_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_EQ_ADAPT_CTRL_ENB_get", id, 2);
    /* (0x00000504 bits 17) field EQ_ADAPT_CTRL_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_EQ_ADAPT_CTRL_ENB_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_PCIE_FFE_MODE_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PCIE_FFE_MODE_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PCIE_FFE_MODE_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PCIE_FFE_MODE_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PCIE_FFE_MODE_set", id, value );

    /* (0x00000504 bits 16) field PCIE_FFE_MODE of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_PCIE_FFE_MODE_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_PCIE_FFE_MODE_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_PCIE_FFE_MODE_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PCIE_FFE_MODE_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PCIE_FFE_MODE_get", id, 2);
    /* (0x00000504 bits 16) field PCIE_FFE_MODE of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_PCIE_FFE_MODE_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_PCIE_FFE_MODE_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PCIE_FFE_MODE_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_INV_DFE_OUT_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_INV_DFE_OUT_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_INV_DFE_OUT_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_INV_DFE_OUT_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_INV_DFE_OUT_set", id, value );

    /* (0x00000504 bits 15) field INV_DFE_OUT of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_INV_DFE_OUT_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_INV_DFE_OUT_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_INV_DFE_OUT_get", id, 2);
    /* (0x00000504 bits 15) field INV_DFE_OUT of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_INV_DFE_OUT_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_ADAPT_INITB_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_ADAPT_INITB_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_INITB_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_ADAPT_INITB_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_ADAPT_INITB_set", id, value );

    /* (0x00000504 bits 14) field ADAPT_INITB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_ADAPT_INITB_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADAPT_INITB_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_INITB_get", id, 2);
    /* (0x00000504 bits 14) field ADAPT_INITB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADAPT_INITB_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DP_ADAPT_ORDER_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_ADAPT_ORDER_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_ADAPT_ORDER_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_ADAPT_ORDER_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_ADAPT_ORDER_set", id, value );

    /* (0x00000504 bits 13) field DP_ADAPT_ORDER of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_DP_ADAPT_ORDER_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_DP_ADAPT_ORDER_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_DP_ADAPT_ORDER_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_ADAPT_ORDER_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_ADAPT_ORDER_get", id, 2);
    /* (0x00000504 bits 13) field DP_ADAPT_ORDER of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_DP_ADAPT_ORDER_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_DP_ADAPT_ORDER_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_ADAPT_ORDER_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_FFE_CONSTRAINED_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_FFE_CONSTRAINED_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_FFE_CONSTRAINED_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_FFE_CONSTRAINED_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_FFE_CONSTRAINED_set", id, value );

    /* (0x00000504 bits 12) field FFE_CONSTRAINED of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_FFE_CONSTRAINED_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_FFE_CONSTRAINED_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_FFE_CONSTRAINED_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_FFE_CONSTRAINED_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_FFE_CONSTRAINED_get", id, 2);
    /* (0x00000504 bits 12) field FFE_CONSTRAINED of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_FFE_CONSTRAINED_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_FFE_CONSTRAINED_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_FFE_CONSTRAINED_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_PRELOAD_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PRELOAD_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PRELOAD_ENB_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PRELOAD_ENB_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PRELOAD_ENB_set", id, value );

    /* (0x00000504 bits 11) field PRELOAD_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_PRELOAD_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PRELOAD_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PRELOAD_ENB_get", id, 2);
    /* (0x00000504 bits 11) field PRELOAD_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PRELOAD_ENB_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_ADAPT_DELAY_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_ADAPT_DELAY_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_DELAY_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_ADAPT_DELAY_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_ADAPT_DELAY_set", id, value );

    /* (0x00000504 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_ADAPT_DELAY_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADAPT_DELAY_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_DELAY_get", id, 2);
    /* (0x00000504 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADAPT_DELAY_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_ADAPT_DELAY_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_ADAPT_DELAY_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADAPT_DELAY_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADAPT_DELAY_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADAPT_DELAY_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADAPT_DELAY_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000504 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000504 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
        t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                            h_ptr,
                                            id,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_ADAPT_DELAY_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_ADAPT_DELAY_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADAPT_DELAY_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADAPT_DELAY_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADAPT_DELAY_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000504 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK)
                  >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADAPT_DELAY_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_ADAPT_LOOPS_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_ADAPT_LOOPS_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_LOOPS_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_ADAPT_LOOPS_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_ADAPT_LOOPS_set", id, value );

    /* (0x00000504 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_ADAPT_LOOPS_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADAPT_LOOPS_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_LOOPS_get", id, 2);
    /* (0x00000504 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADAPT_LOOPS_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_ADAPT_LOOPS_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_ADAPT_LOOPS_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADAPT_LOOPS_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADAPT_LOOPS_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADAPT_LOOPS_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADAPT_LOOPS_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000504 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000504 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
        t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                            h_ptr,
                                            id,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_ADAPT_LOOPS_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_ADAPT_LOOPS_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADAPT_LOOPS_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADAPT_LOOPS_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADAPT_LOOPS_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000504 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK)
                  >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADAPT_LOOPS_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_ADAPT_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_ADAPT_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id,
                                                UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_ENB_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_ADAPT_ENB_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_ADAPT_ENB_set", id, value );

    /* (0x00000504 bits 3) field ADAPT_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_ADAPT_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADAPT_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_ENB_get", id, 2);
    /* (0x00000504 bits 3) field ADAPT_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADAPT_ENB_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_SEL_LMS_CLIP_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_SEL_LMS_CLIP_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_SEL_LMS_CLIP_set", id, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_SEL_LMS_CLIP_set", value, 3);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_SEL_LMS_CLIP_set", id, value );

    /* (0x00000504 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_SEL_LMS_CLIP_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_SEL_LMS_CLIP_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_SEL_LMS_CLIP_get", id, 2);
    /* (0x00000504 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_SEL_LMS_CLIP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_SEL_LMS_CLIP_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_SEL_LMS_CLIP_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_SEL_LMS_CLIP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_SEL_LMS_CLIP_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_SEL_LMS_CLIP_set", stop_bit, 1 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_SEL_LMS_CLIP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000504 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_1 */
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
        /* (0x00000504 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_1 */
        t8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                            h_ptr,
                                            id,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_SEL_LMS_CLIP_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_SEL_LMS_CLIP_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_SEL_LMS_CLIP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_SEL_LMS_CLIP_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_SEL_LMS_CLIP_get", stop_bit, 1 );
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
    /* (0x00000504 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_MSK)
                  >> ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_MSK, ADSP_REG_ADAPT_CTRL_1_BIT_SEL_LMS_CLIP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_SEL_LMS_CLIP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_OBJECTS_EN_set( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_OBJECTS_EN_set( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_OBJECTS_EN_set", id, 2);
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_OBJECTS_EN_set", value, 32767);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_OBJECTS_EN_set", id, value );

    /* (0x00000508 bits 29:15) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    t8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_MSK,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_OBJECTS_EN_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_OBJECTS_EN_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_OBJECTS_EN_get", id, 2);
    /* (0x00000508 bits 29:15) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_MSK) >> ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_OBJECTS_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_OBJECTS_EN_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_OBJECTS_EN_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_OBJECTS_EN_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_OBJECTS_EN_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_OBJECTS_EN_set", stop_bit, 14 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_OBJECTS_EN_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000508 bits 29:15) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000508 bits 29:15) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
        t8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                            h_ptr,
                                            id,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_OBJECTS_EN_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_OBJECTS_EN_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_OBJECTS_EN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_OBJECTS_EN_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_OBJECTS_EN_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000508 bits 29:15) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_MSK)
                  >> ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_MSK, ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_OBJECTS_EN_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_LMS_DATA_SEL_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_LMS_DATA_SEL_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_LMS_DATA_SEL_set", id, 2);
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_LMS_DATA_SEL_set", value, 32767);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_LMS_DATA_SEL_set", id, value );

    /* (0x00000508 bits 14:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    t8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF,
                                        value);
}

static INLINE UINT32 t8_adsp_field_LMS_DATA_SEL_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_LMS_DATA_SEL_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_LMS_DATA_SEL_get", id, 2);
    /* (0x00000508 bits 14:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK) >> ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_LMS_DATA_SEL_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_LMS_DATA_SEL_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_LMS_DATA_SEL_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_LMS_DATA_SEL_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_LMS_DATA_SEL_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_LMS_DATA_SEL_set", stop_bit, 14 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_LMS_DATA_SEL_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000508 bits 14:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000508 bits 14:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
        t8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                            h_ptr,
                                            id,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_LMS_DATA_SEL_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_LMS_DATA_SEL_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_LMS_DATA_SEL_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_LMS_DATA_SEL_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_LMS_DATA_SEL_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000508 bits 14:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = t8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK)
                  >> ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK, ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_LMS_DATA_SEL_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_EXPECTED_EQ2_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_EXPECTED_EQ2_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_EXPECTED_EQ2_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_EXPECTED_EQ2_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_EXPECTED_EQ2_set", id, value );

    /* (0x00000530 bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    id,
                                    ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF,
                                    value);
}

static INLINE UINT32 t8_adsp_field_EXPECTED_EQ2_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_EXPECTED_EQ2_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_EXPECTED_EQ2_get", id, 2);
    /* (0x00000530 bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK) >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_EXPECTED_EQ2_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_EXPECTED_EQ2_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_EXPECTED_EQ2_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_EXPECTED_EQ2_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_EXPECTED_EQ2_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_EXPECTED_EQ2_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_EXPECTED_EQ2_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000530 bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
        t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_EXPECTED_EQ2_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_EXPECTED_EQ2_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_EXPECTED_EQ2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_EXPECTED_EQ2_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_EXPECTED_EQ2_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000530 bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_EXPECTED_EQ2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_EXPECTED_EQ1_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_EXPECTED_EQ1_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_EXPECTED_EQ1_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_EXPECTED_EQ1_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_EXPECTED_EQ1_set", id, value );

    /* (0x00000530 bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    id,
                                    ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF,
                                    value);
}

static INLINE UINT32 t8_adsp_field_EXPECTED_EQ1_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_EXPECTED_EQ1_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_EXPECTED_EQ1_get", id, 2);
    /* (0x00000530 bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK) >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_EXPECTED_EQ1_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_EXPECTED_EQ1_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_EXPECTED_EQ1_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_EXPECTED_EQ1_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_EXPECTED_EQ1_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_EXPECTED_EQ1_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_EXPECTED_EQ1_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000530 bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
        t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_EXPECTED_EQ1_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_EXPECTED_EQ1_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_EXPECTED_EQ1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_EXPECTED_EQ1_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_EXPECTED_EQ1_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000530 bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_EXPECTED_EQ1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_H_LIMIT_set( t8_adsp_buffer_t *b_ptr,
                                              t8_handle_t *h_ptr,
                                              UINT32 id,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_H_LIMIT_set( t8_adsp_buffer_t *b_ptr,
                                              t8_handle_t *h_ptr,
                                              UINT32 id,
                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_H_LIMIT_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_H_LIMIT_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_H_LIMIT_set", id, value );

    /* (0x00000530 bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    id,
                                    ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF,
                                    value);
}

static INLINE UINT32 t8_adsp_field_H_LIMIT_get( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_H_LIMIT_get( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_H_LIMIT_get", id, 2);
    /* (0x00000530 bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK) >> ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_H_LIMIT_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_H_LIMIT_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_H_LIMIT_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_H_LIMIT_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_H_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_H_LIMIT_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_H_LIMIT_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x00000530 bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
        t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_H_LIMIT_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_H_LIMIT_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_H_LIMIT_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_H_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_H_LIMIT_get", stop_bit, 6 );
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
    /* (0x00000530 bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK, ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_H_LIMIT_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_S_LIMIT_set( t8_adsp_buffer_t *b_ptr,
                                              t8_handle_t *h_ptr,
                                              UINT32 id,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_S_LIMIT_set( t8_adsp_buffer_t *b_ptr,
                                              t8_handle_t *h_ptr,
                                              UINT32 id,
                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_S_LIMIT_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_S_LIMIT_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_S_LIMIT_set", id, value );

    /* (0x00000530 bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    id,
                                    ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF,
                                    value);
}

static INLINE UINT32 t8_adsp_field_S_LIMIT_get( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_S_LIMIT_get( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_S_LIMIT_get", id, 2);
    /* (0x00000530 bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK) >> ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_S_LIMIT_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_S_LIMIT_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_S_LIMIT_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_S_LIMIT_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_S_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_S_LIMIT_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_S_LIMIT_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x00000530 bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
        t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_S_LIMIT_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_S_LIMIT_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_S_LIMIT_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_S_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_S_LIMIT_get", stop_bit, 6 );
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
    /* (0x00000530 bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK, ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_S_LIMIT_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_OFL_DECAY_set( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_OFL_DECAY_set( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id,
                                                UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_OFL_DECAY_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_OFL_DECAY_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_OFL_DECAY_set", id, value );

    /* (0x00000530 bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    id,
                                    ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF,
                                    value);
}

static INLINE UINT32 t8_adsp_field_OFL_DECAY_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_OFL_DECAY_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_OFL_DECAY_get", id, 2);
    /* (0x00000530 bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK) >> ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_OFL_DECAY_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_OFL_DECAY_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_OFL_DECAY_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_OFL_DECAY_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_OFL_DECAY_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_OFL_DECAY_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_OFL_DECAY_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
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
        if (stop_bit < 4) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 4;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000530 bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
        t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        id,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_OFL_DECAY_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_OFL_DECAY_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_OFL_DECAY_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_OFL_DECAY_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_OFL_DECAY_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000530 bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK, ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_OFL_DECAY_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_PATH_PATTERN_CHK_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PATH_PATTERN_CHK_ENB_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PATH_PATTERN_CHK_ENB_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PATH_PATTERN_CHK_ENB_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PATH_PATTERN_CHK_ENB_set", id, value );

    /* (0x00000530 bits 1) field PATH_PATTERN_CHK_ENB of register PMC_ADSP_REG_DSS_CTRL */
    t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    id,
                                    ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_OFF,
                                    value);
}

static INLINE UINT32 t8_adsp_field_PATH_PATTERN_CHK_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PATH_PATTERN_CHK_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PATH_PATTERN_CHK_ENB_get", id, 2);
    /* (0x00000530 bits 1) field PATH_PATTERN_CHK_ENB of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_MSK) >> ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PATH_PATTERN_CHK_ENB_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DSS_ENB_set( t8_adsp_buffer_t *b_ptr,
                                              t8_handle_t *h_ptr,
                                              UINT32 id,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DSS_ENB_set( t8_adsp_buffer_t *b_ptr,
                                              t8_handle_t *h_ptr,
                                              UINT32 id,
                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DSS_ENB_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DSS_ENB_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DSS_ENB_set", id, value );

    /* (0x00000530 bits 0) field DSS_ENB of register PMC_ADSP_REG_DSS_CTRL */
    t8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    id,
                                    ADSP_REG_DSS_CTRL_BIT_DSS_ENB_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_DSS_ENB_OFF,
                                    value);
}

static INLINE UINT32 t8_adsp_field_DSS_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DSS_ENB_get( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DSS_ENB_get", id, 2);
    /* (0x00000530 bits 0) field DSS_ENB of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = t8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_DSS_ENB_MSK) >> ADSP_REG_DSS_CTRL_BIT_DSS_ENB_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DSS_ENB_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_set", id, value );

    /* (0x00000534 bits 30) field DFE_COEF5_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_get", id, 2);
    /* (0x00000534 bits 30) field DFE_COEF5_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_set", id, value );

    /* (0x00000534 bits 29) field DFE_COEF4_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_get", id, 2);
    /* (0x00000534 bits 29) field DFE_COEF4_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_set", id, value );

    /* (0x00000534 bits 28) field DFE_COEF3_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_get", id, 2);
    /* (0x00000534 bits 28) field DFE_COEF3_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_set", id, value );

    /* (0x00000534 bits 27) field DFE_COEF2_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_get", id, 2);
    /* (0x00000534 bits 27) field DFE_COEF2_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_set", id, value );

    /* (0x00000534 bits 26) field DFE_COEF1_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_get", id, 2);
    /* (0x00000534 bits 26) field DFE_COEF1_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_set", id, value );

    /* (0x00000534 bits 25) field DFE_COEF0_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_get", id, 2);
    /* (0x00000534 bits 25) field DFE_COEF0_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_set", id, value );

    /* (0x00000534 bits 24) field TARGET_AMP_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_get", id, 2);
    /* (0x00000534 bits 24) field TARGET_AMP_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF5_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF5_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF5_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF5_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF5_TAP_set", id, value );

    /* (0x00000534 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF5_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF5_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF5_TAP_get", id, 2);
    /* (0x00000534 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF5_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF5_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF5_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF5_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF5_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF5_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF5_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000534 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000534 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF5_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF5_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF5_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF5_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF5_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000534 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF5_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF4_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF4_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF4_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF4_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF4_TAP_set", id, value );

    /* (0x00000534 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF4_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF4_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF4_TAP_get", id, 2);
    /* (0x00000534 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF4_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF4_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF4_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF4_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF4_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF4_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF4_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000534 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000534 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF4_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF4_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF4_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF4_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF4_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000534 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF4_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF3_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF3_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF3_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF3_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF3_TAP_set", id, value );

    /* (0x00000534 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF3_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF3_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF3_TAP_get", id, 2);
    /* (0x00000534 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF3_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF3_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF3_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF3_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF3_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF3_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF3_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000534 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000534 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF3_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF3_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF3_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF3_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF3_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000534 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF3_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF2_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF2_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF2_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF2_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF2_TAP_set", id, value );

    /* (0x00000534 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF2_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF2_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF2_TAP_get", id, 2);
    /* (0x00000534 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF2_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF2_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF2_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF2_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF2_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF2_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF2_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000534 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000534 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF2_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF2_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF2_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF2_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF2_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000534 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF2_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF1_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF1_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF1_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF1_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF1_TAP_set", id, value );

    /* (0x00000534 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF1_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF1_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF1_TAP_get", id, 2);
    /* (0x00000534 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF1_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF1_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF1_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF1_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF1_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF1_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF1_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000534 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000534 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF1_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF1_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF1_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF1_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF1_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000534 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF1_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF0_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF0_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF0_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF0_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF0_TAP_set", id, value );

    /* (0x00000534 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF0_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF0_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF0_TAP_get", id, 2);
    /* (0x00000534 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF0_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF0_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF0_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF0_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF0_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF0_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF0_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000534 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000534 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        t8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF0_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF0_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF0_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF0_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF0_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000534 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = t8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF0_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_GAIN_TAP_set( t8_adsp_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_GAIN_TAP_set( t8_adsp_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id,
                                               UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_GAIN_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_GAIN_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_GAIN_TAP_set", id, value );

    /* (0x00000538 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    t8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_GAIN_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_GAIN_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_GAIN_TAP_get", id, 2);
    /* (0x00000538 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = t8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK) >> ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_GAIN_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_GAIN_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_GAIN_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_GAIN_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_GAIN_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_GAIN_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_GAIN_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000538 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000538 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
        t8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_GAIN_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_GAIN_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_GAIN_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_GAIN_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_GAIN_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000538 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = t8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK, ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_GAIN_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_TAP2_set( t8_adsp_buffer_t *b_ptr,
                                           t8_handle_t *h_ptr,
                                           UINT32 id,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_TAP2_set( t8_adsp_buffer_t *b_ptr,
                                           t8_handle_t *h_ptr,
                                           UINT32 id,
                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TAP2_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_TAP2_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_TAP2_set", id, value );

    /* (0x0000053c bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_TAP2_get( t8_adsp_buffer_t *b_ptr,
                                             t8_handle_t *h_ptr,
                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TAP2_get( t8_adsp_buffer_t *b_ptr,
                                             t8_handle_t *h_ptr,
                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TAP2_get", id, 2);
    /* (0x0000053c bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TAP2_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_TAP2_set( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_TAP2_set( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TAP2_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TAP2_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TAP2_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TAP2_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000053c bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000053c bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
        t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_TAP2_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_TAP2_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TAP2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TAP2_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TAP2_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000053c bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK, ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TAP2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_set", id, value );

    /* (0x0000053c bits 26) field DP_FFE_C_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_get", id, 2);
    /* (0x0000053c bits 26) field DP_FFE_C_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_set", id, value );

    /* (0x0000053c bits 25) field DP_FFE_B_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_get", id, 2);
    /* (0x0000053c bits 25) field DP_FFE_B_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_set", id, value );

    /* (0x0000053c bits 24) field DP_FFE_A_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_get", id, 2);
    /* (0x0000053c bits 24) field DP_FFE_A_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_C_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_C_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_C_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_C_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_C_TAP_set", id, value );

    /* (0x0000053c bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_C_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_C_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_C_TAP_get", id, 2);
    /* (0x0000053c bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_C_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DP_FFE_C_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DP_FFE_C_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_C_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_C_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_C_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_C_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000053c bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000053c bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DP_FFE_C_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_C_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_C_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_C_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_C_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000053c bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_C_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_B_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_B_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_B_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_B_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_B_TAP_set", id, value );

    /* (0x0000053c bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_B_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_B_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_B_TAP_get", id, 2);
    /* (0x0000053c bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_B_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DP_FFE_B_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DP_FFE_B_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_B_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_B_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_B_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_B_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000053c bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000053c bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DP_FFE_B_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_B_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_B_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_B_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_B_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000053c bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_B_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_A_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_A_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_A_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_A_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_A_TAP_set", id, value );

    /* (0x0000053c bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_A_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_A_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_A_TAP_get", id, 2);
    /* (0x0000053c bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_A_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DP_FFE_A_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DP_FFE_A_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_A_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_A_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_A_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_A_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000053c bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000053c bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DP_FFE_A_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_A_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_A_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_A_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_A_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000053c bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_A_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_TARGET_AMP_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_TARGET_AMP_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TARGET_AMP_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_TARGET_AMP_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_TARGET_AMP_TAP_set", id, value );

    /* (0x0000053c bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_TARGET_AMP_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TARGET_AMP_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TARGET_AMP_TAP_get", id, 2);
    /* (0x0000053c bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TARGET_AMP_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_TARGET_AMP_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_TARGET_AMP_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TARGET_AMP_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TARGET_AMP_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TARGET_AMP_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TARGET_AMP_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000053c bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000053c bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_TARGET_AMP_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_TARGET_AMP_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TARGET_AMP_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TARGET_AMP_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TARGET_AMP_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000053c bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TARGET_AMP_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_ADC_TAP_set( t8_adsp_buffer_t *b_ptr,
                                              t8_handle_t *h_ptr,
                                              UINT32 id,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_ADC_TAP_set( t8_adsp_buffer_t *b_ptr,
                                              t8_handle_t *h_ptr,
                                              UINT32 id,
                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADC_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_ADC_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_ADC_TAP_set", id, value );

    /* (0x0000053c bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_ADC_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADC_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADC_TAP_get", id, 2);
    /* (0x0000053c bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADC_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_ADC_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_ADC_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADC_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADC_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADC_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADC_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000053c bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000053c bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_ADC_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_ADC_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADC_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADC_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADC_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000053c bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADC_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_OFFSET_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_OFFSET_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_OFFSET_TAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_OFFSET_TAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_OFFSET_TAP_set", id, value );

    /* (0x0000053c bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_OFFSET_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_OFFSET_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_OFFSET_TAP_get", id, 2);
    /* (0x0000053c bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_OFFSET_TAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_OFFSET_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_OFFSET_TAP_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_OFFSET_TAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_OFFSET_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_OFFSET_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_OFFSET_TAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000053c bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000053c bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        t8_adsp_reg_DSS_TAPS_3_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_OFFSET_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_OFFSET_TAP_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_OFFSET_TAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_OFFSET_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_OFFSET_TAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000053c bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = t8_adsp_reg_DSS_TAPS_3_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_OFFSET_TAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_MU_LEAK_ADC_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_MU_LEAK_ADC_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_LEAK_ADC_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_MU_LEAK_ADC_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_MU_LEAK_ADC_set", id, value );

    /* (0x00000540 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF,
                                          value);
}

static INLINE UINT32 t8_adsp_field_MU_LEAK_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_MU_LEAK_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_LEAK_ADC_get", id, 2);
    /* (0x00000540 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_MU_LEAK_ADC_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_MU_LEAK_ADC_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_MU_LEAK_ADC_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_LEAK_ADC_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_LEAK_ADC_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_LEAK_ADC_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_LEAK_ADC_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000540 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000540 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
        t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_MU_LEAK_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_MU_LEAK_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_LEAK_ADC_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_LEAK_ADC_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_LEAK_ADC_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000540 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_LEAK_ADC_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_MU_LEAK_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_MU_LEAK_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_LEAK_COEF_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_MU_LEAK_COEF_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_MU_LEAK_COEF_set", id, value );

    /* (0x00000540 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF,
                                          value);
}

static INLINE UINT32 t8_adsp_field_MU_LEAK_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_MU_LEAK_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_LEAK_COEF_get", id, 2);
    /* (0x00000540 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_MU_LEAK_COEF_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_MU_LEAK_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_MU_LEAK_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_LEAK_COEF_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_LEAK_COEF_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_LEAK_COEF_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_LEAK_COEF_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000540 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000540 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
        t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_MU_LEAK_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_MU_LEAK_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_LEAK_COEF_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_LEAK_COEF_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_LEAK_COEF_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000540 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_LEAK_COEF_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_MU_LEAK_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_MU_LEAK_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_LEAK_OFFSET_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_MU_LEAK_OFFSET_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_MU_LEAK_OFFSET_set", id, value );

    /* (0x00000540 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF,
                                          value);
}

static INLINE UINT32 t8_adsp_field_MU_LEAK_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_MU_LEAK_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_LEAK_OFFSET_get", id, 2);
    /* (0x00000540 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_MU_LEAK_OFFSET_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_MU_LEAK_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_MU_LEAK_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_LEAK_OFFSET_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_LEAK_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_LEAK_OFFSET_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_LEAK_OFFSET_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000540 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000540 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
        t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_MU_LEAK_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_MU_LEAK_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_LEAK_OFFSET_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_LEAK_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_LEAK_OFFSET_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000540 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_LEAK_OFFSET_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_MU_LEAK_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_MU_LEAK_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_LEAK_GAIN_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_MU_LEAK_GAIN_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_MU_LEAK_GAIN_set", id, value );

    /* (0x00000540 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF,
                                          value);
}

static INLINE UINT32 t8_adsp_field_MU_LEAK_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_MU_LEAK_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_LEAK_GAIN_get", id, 2);
    /* (0x00000540 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_MU_LEAK_GAIN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_MU_LEAK_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_MU_LEAK_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_LEAK_GAIN_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_LEAK_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_LEAK_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_LEAK_GAIN_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000540 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000540 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
        t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_MU_LEAK_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_MU_LEAK_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_LEAK_GAIN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_LEAK_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_LEAK_GAIN_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000540 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_LEAK_GAIN_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_MU_STEP_ADC_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_MU_STEP_ADC_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_STEP_ADC_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_MU_STEP_ADC_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_MU_STEP_ADC_set", id, value );

    /* (0x00000540 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF,
                                          value);
}

static INLINE UINT32 t8_adsp_field_MU_STEP_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_MU_STEP_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_STEP_ADC_get", id, 2);
    /* (0x00000540 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_MU_STEP_ADC_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_MU_STEP_ADC_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_MU_STEP_ADC_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_STEP_ADC_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_STEP_ADC_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_STEP_ADC_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_STEP_ADC_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000540 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000540 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
        t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_MU_STEP_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_MU_STEP_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_STEP_ADC_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_STEP_ADC_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_STEP_ADC_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000540 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_STEP_ADC_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_MU_STEP_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_MU_STEP_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_STEP_COEF_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_MU_STEP_COEF_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_MU_STEP_COEF_set", id, value );

    /* (0x00000540 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF,
                                          value);
}

static INLINE UINT32 t8_adsp_field_MU_STEP_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_MU_STEP_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_STEP_COEF_get", id, 2);
    /* (0x00000540 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_MU_STEP_COEF_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_MU_STEP_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_MU_STEP_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_STEP_COEF_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_STEP_COEF_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_STEP_COEF_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_STEP_COEF_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000540 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000540 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
        t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_MU_STEP_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_MU_STEP_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_STEP_COEF_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_STEP_COEF_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_STEP_COEF_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000540 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_STEP_COEF_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_MU_STEP_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_MU_STEP_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_STEP_OFFSET_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_MU_STEP_OFFSET_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_MU_STEP_OFFSET_set", id, value );

    /* (0x00000540 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF,
                                          value);
}

static INLINE UINT32 t8_adsp_field_MU_STEP_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_MU_STEP_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_STEP_OFFSET_get", id, 2);
    /* (0x00000540 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_MU_STEP_OFFSET_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_MU_STEP_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_MU_STEP_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_STEP_OFFSET_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_STEP_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_STEP_OFFSET_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_STEP_OFFSET_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000540 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000540 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
        t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_MU_STEP_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_MU_STEP_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_STEP_OFFSET_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_STEP_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_STEP_OFFSET_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000540 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_STEP_OFFSET_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_MU_STEP_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_MU_STEP_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_STEP_GAIN_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_MU_STEP_GAIN_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_MU_STEP_GAIN_set", id, value );

    /* (0x00000540 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          id,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF,
                                          value);
}

static INLINE UINT32 t8_adsp_field_MU_STEP_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_MU_STEP_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_MU_STEP_GAIN_get", id, 2);
    /* (0x00000540 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_MU_STEP_GAIN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_MU_STEP_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_MU_STEP_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_STEP_GAIN_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_STEP_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_STEP_GAIN_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_STEP_GAIN_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000540 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000540 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
        t8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_MU_STEP_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_MU_STEP_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_MU_STEP_GAIN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_MU_STEP_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_MU_STEP_GAIN_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000540 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = t8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_MU_STEP_GAIN_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_RR_PERIOD_DSS_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_RR_PERIOD_DSS_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_DSS_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_RR_PERIOD_DSS_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_DSS_set", id, value );

    /* (0x00000544 bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               id,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF,
                                               value);
}

static INLINE UINT32 t8_adsp_field_RR_PERIOD_DSS_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_RR_PERIOD_DSS_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_DSS_get", id, 2);
    /* (0x00000544 bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_DSS_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_RR_PERIOD_DSS_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_RR_PERIOD_DSS_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_DSS_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_DSS_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_DSS_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_DSS_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000544 bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000544 bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   id,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_DSS_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_DSS_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_DSS_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_DSS_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_DSS_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000544 bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_DSS_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_RR_PERIOD_ADC_MAP_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_RR_PERIOD_ADC_MAP_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_ADC_MAP_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_RR_PERIOD_ADC_MAP_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_ADC_MAP_set", id, value );

    /* (0x00000544 bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               id,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF,
                                               value);
}

static INLINE UINT32 t8_adsp_field_RR_PERIOD_ADC_MAP_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_RR_PERIOD_ADC_MAP_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_ADC_MAP_get", id, 2);
    /* (0x00000544 bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_ADC_MAP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_RR_PERIOD_ADC_MAP_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_RR_PERIOD_ADC_MAP_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_ADC_MAP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_ADC_MAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_ADC_MAP_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_ADC_MAP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000544 bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000544 bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   id,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_ADC_MAP_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_ADC_MAP_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_ADC_MAP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_ADC_MAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_ADC_MAP_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000544 bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_ADC_MAP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_RR_PERIOD_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_RR_PERIOD_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_OFFSET_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_RR_PERIOD_OFFSET_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_OFFSET_set", id, value );

    /* (0x00000544 bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               id,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF,
                                               value);
}

static INLINE UINT32 t8_adsp_field_RR_PERIOD_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_RR_PERIOD_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_OFFSET_get", id, 2);
    /* (0x00000544 bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_OFFSET_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_RR_PERIOD_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_RR_PERIOD_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_OFFSET_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_OFFSET_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_OFFSET_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000544 bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000544 bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   id,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_OFFSET_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_OFFSET_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000544 bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_OFFSET_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_RR_PERIOD_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_RR_PERIOD_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_GAIN_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_RR_PERIOD_GAIN_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_GAIN_set", id, value );

    /* (0x00000544 bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               id,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF,
                                               value);
}

static INLINE UINT32 t8_adsp_field_RR_PERIOD_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_RR_PERIOD_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_GAIN_get", id, 2);
    /* (0x00000544 bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_GAIN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_RR_PERIOD_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_RR_PERIOD_GAIN_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_GAIN_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_GAIN_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000544 bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000544 bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   id,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_GAIN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_GAIN_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000544 bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_GAIN_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_RR_PERIOD_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_RR_PERIOD_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_COEF_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_RR_PERIOD_COEF_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_COEF_set", id, value );

    /* (0x00000544 bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               id,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF,
                                               value);
}

static INLINE UINT32 t8_adsp_field_RR_PERIOD_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_RR_PERIOD_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_RR_PERIOD_COEF_get", id, 2);
    /* (0x00000544 bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_RR_PERIOD_COEF_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_RR_PERIOD_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_RR_PERIOD_COEF_set( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_COEF_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_COEF_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_COEF_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_COEF_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000544 bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000544 bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   id,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_RR_PERIOD_COEF_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_RR_PERIOD_COEF_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_RR_PERIOD_COEF_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_RR_PERIOD_COEF_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000544 bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = t8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      id);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_RR_PERIOD_COEF_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_TR_FFE_B_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_TR_FFE_B_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TR_FFE_B_PRELOAD_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_TR_FFE_B_PRELOAD_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_TR_FFE_B_PRELOAD_set", id, value );

    /* (0x00000548 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_TR_FFE_B_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TR_FFE_B_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TR_FFE_B_PRELOAD_get", id, 2);
    /* (0x00000548 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TR_FFE_B_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_TR_FFE_B_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_TR_FFE_B_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TR_FFE_B_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TR_FFE_B_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TR_FFE_B_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TR_FFE_B_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000548 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        if (stop_bit < 4) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 4;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000548 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_TR_FFE_B_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_TR_FFE_B_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TR_FFE_B_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TR_FFE_B_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TR_FFE_B_PRELOAD_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000548 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TR_FFE_B_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_TR_FFE_A_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_TR_FFE_A_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TR_FFE_A_PRELOAD_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_TR_FFE_A_PRELOAD_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_TR_FFE_A_PRELOAD_set", id, value );

    /* (0x00000548 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_TR_FFE_A_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TR_FFE_A_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TR_FFE_A_PRELOAD_get", id, 2);
    /* (0x00000548 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TR_FFE_A_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_TR_FFE_A_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_TR_FFE_A_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TR_FFE_A_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TR_FFE_A_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TR_FFE_A_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TR_FFE_A_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000548 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        if (stop_bit < 4) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 4;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000548 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_TR_FFE_A_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_TR_FFE_A_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TR_FFE_A_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TR_FFE_A_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TR_FFE_A_PRELOAD_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000548 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TR_FFE_A_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_C_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_C_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_C_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_C_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_C_PRELOAD_set", id, value );

    /* (0x00000548 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_C_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_C_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_C_PRELOAD_get", id, 2);
    /* (0x00000548 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_C_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DP_FFE_C_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DP_FFE_C_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_C_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_C_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_C_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_C_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000548 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000548 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DP_FFE_C_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_C_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_C_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_C_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_C_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000548 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_C_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_B_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_B_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_B_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_B_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_B_PRELOAD_set", id, value );

    /* (0x00000548 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_B_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_B_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_B_PRELOAD_get", id, 2);
    /* (0x00000548 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_B_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DP_FFE_B_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DP_FFE_B_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_B_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_B_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_B_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_B_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000548 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000548 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DP_FFE_B_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_B_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_B_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_B_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_B_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000548 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_B_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DP_FFE_A_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DP_FFE_A_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_A_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DP_FFE_A_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DP_FFE_A_PRELOAD_set", id, value );

    /* (0x00000548 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DP_FFE_A_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_A_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_A_PRELOAD_get", id, 2);
    /* (0x00000548 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_A_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DP_FFE_A_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DP_FFE_A_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_A_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_A_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_A_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_A_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000548 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000548 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DP_FFE_A_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_A_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_A_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_A_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_A_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000548 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_A_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF3_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF3_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF3_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF3_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF3_PRELOAD_set", id, value );

    /* (0x0000054c bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF3_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF3_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF3_PRELOAD_get", id, 2);
    /* (0x0000054c bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF3_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF3_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF3_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF3_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF3_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF3_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF3_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000054c bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x0000054c bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF3_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF3_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF3_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF3_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF3_PRELOAD_get", stop_bit, 6 );
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
    /* (0x0000054c bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF3_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF2_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF2_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF2_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF2_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF2_PRELOAD_set", id, value );

    /* (0x0000054c bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF2_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF2_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF2_PRELOAD_get", id, 2);
    /* (0x0000054c bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF2_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF2_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF2_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF2_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF2_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF2_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF2_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000054c bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x0000054c bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF2_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF2_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF2_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF2_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF2_PRELOAD_get", stop_bit, 6 );
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
    /* (0x0000054c bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF2_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF1_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF1_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF1_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF1_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF1_PRELOAD_set", id, value );

    /* (0x0000054c bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF1_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF1_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF1_PRELOAD_get", id, 2);
    /* (0x0000054c bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF1_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF1_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF1_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF1_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF1_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF1_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF1_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000054c bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x0000054c bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF1_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF1_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF1_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF1_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF1_PRELOAD_get", stop_bit, 6 );
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
    /* (0x0000054c bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF1_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF0_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF0_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF0_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF0_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF0_PRELOAD_set", id, value );

    /* (0x0000054c bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF0_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF0_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF0_PRELOAD_get", id, 2);
    /* (0x0000054c bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF0_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF0_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF0_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF0_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF0_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF0_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF0_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000054c bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x0000054c bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF0_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF0_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF0_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF0_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF0_PRELOAD_get", stop_bit, 6 );
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
    /* (0x0000054c bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF0_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF5_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF5_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF5_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF5_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF5_PRELOAD_set", id, value );

    /* (0x00000550 bits 13:7) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF5_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF5_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF5_PRELOAD_get", id, 2);
    /* (0x00000550 bits 13:7) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF5_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF5_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF5_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF5_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF5_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF5_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF5_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000550 bits 13:7) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000550 bits 13:7) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF5_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF5_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF5_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF5_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF5_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000550 bits 13:7) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF5_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_DFE_COEF4_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_DFE_COEF4_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF4_PRELOAD_set", id, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_DFE_COEF4_PRELOAD_set", value, 127);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_DFE_COEF4_PRELOAD_set", id, value );

    /* (0x00000550 bits 6:0) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_DFE_COEF4_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF4_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF4_PRELOAD_get", id, 2);
    /* (0x00000550 bits 6:0) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF4_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_DFE_COEF4_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_DFE_COEF4_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF4_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF4_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF4_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF4_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000550 bits 6:0) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000550 bits 6:0) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_DFE_COEF4_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF4_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF4_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF4_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF4_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000550 bits 6:0) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF4_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_ADC_MAP_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_ADC_MAP_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADC_MAP_PRELOAD_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_ADC_MAP_PRELOAD_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_ADC_MAP_PRELOAD_set", id, value );

    /* (0x00000554 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_ADC_MAP_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADC_MAP_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADC_MAP_PRELOAD_get", id, 2);
    /* (0x00000554 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADC_MAP_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_ADC_MAP_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_ADC_MAP_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADC_MAP_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADC_MAP_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADC_MAP_PRELOAD_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADC_MAP_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000554 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000554 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_ADC_MAP_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_ADC_MAP_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADC_MAP_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADC_MAP_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADC_MAP_PRELOAD_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000554 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADC_MAP_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_TARGET_AMP_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_TARGET_AMP_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TARGET_AMP_PRELOAD_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_TARGET_AMP_PRELOAD_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_TARGET_AMP_PRELOAD_set", id, value );

    /* (0x00000554 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_TARGET_AMP_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TARGET_AMP_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TARGET_AMP_PRELOAD_get", id, 2);
    /* (0x00000554 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TARGET_AMP_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_TARGET_AMP_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_TARGET_AMP_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TARGET_AMP_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TARGET_AMP_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TARGET_AMP_PRELOAD_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TARGET_AMP_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000554 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000554 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_TARGET_AMP_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_TARGET_AMP_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TARGET_AMP_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TARGET_AMP_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TARGET_AMP_PRELOAD_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000554 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TARGET_AMP_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_PGA_OFFSET_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PGA_OFFSET_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PGA_OFFSET_PRELOAD_set", id, 2);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PGA_OFFSET_PRELOAD_set", value, 255);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PGA_OFFSET_PRELOAD_set", id, value );

    /* (0x00000554 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_PGA_OFFSET_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PGA_OFFSET_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PGA_OFFSET_PRELOAD_get", id, 2);
    /* (0x00000554 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PGA_OFFSET_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_PGA_OFFSET_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_PGA_OFFSET_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_PGA_OFFSET_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_PGA_OFFSET_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_PGA_OFFSET_PRELOAD_set", stop_bit, 7 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_PGA_OFFSET_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000554 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000554 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_PGA_OFFSET_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_PGA_OFFSET_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_PGA_OFFSET_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_PGA_OFFSET_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_PGA_OFFSET_PRELOAD_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000554 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_PGA_OFFSET_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_PGA_GAIN_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PGA_GAIN_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PGA_GAIN_PRELOAD_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PGA_GAIN_PRELOAD_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PGA_GAIN_PRELOAD_set", id, value );

    /* (0x00000554 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_PGA_GAIN_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PGA_GAIN_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PGA_GAIN_PRELOAD_get", id, 2);
    /* (0x00000554 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PGA_GAIN_PRELOAD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_PGA_GAIN_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_PGA_GAIN_PRELOAD_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_PGA_GAIN_PRELOAD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_PGA_GAIN_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_PGA_GAIN_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_PGA_GAIN_PRELOAD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000554 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
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
        if (stop_bit < 4) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 4;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000554 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
        t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_PGA_GAIN_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_PGA_GAIN_PRELOAD_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_PGA_GAIN_PRELOAD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_PGA_GAIN_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_PGA_GAIN_PRELOAD_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000554 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_PGA_GAIN_PRELOAD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_PROG_REF_OVR_EN_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PROG_REF_OVR_EN_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_OVR_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PROG_REF_OVR_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PROG_REF_OVR_EN_set", id, value );

    /* (0x00000558 bits 19) field PROG_REF_OVR_EN of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_PROG_REF_OVR_EN_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_OVR_EN_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_OVR_EN_get", id, 2);
    /* (0x00000558 bits 19) field PROG_REF_OVR_EN of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_OVR_EN_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_PROG_REF_OVR_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PROG_REF_OVR_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_OVR_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PROG_REF_OVR_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PROG_REF_OVR_set", id, value );

    /* (0x00000558 bits 18) field PROG_REF_OVR of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_PROG_REF_OVR_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_OVR_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_OVR_get", id, 2);
    /* (0x00000558 bits 18) field PROG_REF_OVR of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_OVR_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set", id, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set", value, 15);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set", id, value );

    /* (0x00000558 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get", id, 2);
    /* (0x00000558 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set", stop_bit, 3 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000558 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000558 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get( t8_adsp_buffer_t *b_ptr,
                                                                      t8_handle_t *h_ptr,
                                                                      UINT32 id,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get( t8_adsp_buffer_t *b_ptr,
                                                                      t8_handle_t *h_ptr,
                                                                      UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000558 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_CLIP_RATE_THRESH_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_CLIP_RATE_THRESH_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CLIP_RATE_THRESH_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_CLIP_RATE_THRESH_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_CLIP_RATE_THRESH_set", id, value );

    /* (0x00000558 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_CLIP_RATE_THRESH_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_CLIP_RATE_THRESH_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CLIP_RATE_THRESH_get", id, 2);
    /* (0x00000558 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_CLIP_RATE_THRESH_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_CLIP_RATE_THRESH_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_CLIP_RATE_THRESH_set( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_CLIP_RATE_THRESH_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_CLIP_RATE_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_CLIP_RATE_THRESH_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_CLIP_RATE_THRESH_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000558 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000558 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_CLIP_RATE_THRESH_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_CLIP_RATE_THRESH_get( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_CLIP_RATE_THRESH_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_CLIP_RATE_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_CLIP_RATE_THRESH_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000558 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_CLIP_RATE_THRESH_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_CLIP_RATE_AMP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_CLIP_RATE_AMP_set( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id,
                                                    UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CLIP_RATE_AMP_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_CLIP_RATE_AMP_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_CLIP_RATE_AMP_set", id, value );

    /* (0x00000558 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_CLIP_RATE_AMP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_CLIP_RATE_AMP_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CLIP_RATE_AMP_get", id, 2);
    /* (0x00000558 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_CLIP_RATE_AMP_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_CLIP_RATE_AMP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_CLIP_RATE_AMP_set( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_CLIP_RATE_AMP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_CLIP_RATE_AMP_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_CLIP_RATE_AMP_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_CLIP_RATE_AMP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000558 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000558 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_CLIP_RATE_AMP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_CLIP_RATE_AMP_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_CLIP_RATE_AMP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_CLIP_RATE_AMP_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_CLIP_RATE_AMP_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000558 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_CLIP_RATE_AMP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_GAIN_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_GAIN_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_GAIN_OFFSET_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_GAIN_OFFSET_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_GAIN_OFFSET_set", id, value );

    /* (0x00000558 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_GAIN_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_GAIN_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_GAIN_OFFSET_get", id, 2);
    /* (0x00000558 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_GAIN_OFFSET_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_GAIN_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_GAIN_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_GAIN_OFFSET_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_GAIN_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_GAIN_OFFSET_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_GAIN_OFFSET_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000558 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000558 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_GAIN_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_GAIN_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_GAIN_OFFSET_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_GAIN_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_GAIN_OFFSET_get", stop_bit, 2 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 2) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 2;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000558 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_GAIN_OFFSET_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_adsp_field_ADC_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_ADC_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADC_OFFSET_set", id, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_ADC_OFFSET_set", value, 3);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_ADC_OFFSET_set", id, value );

    /* (0x00000558 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                id,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF,
                                                value);
}

static INLINE UINT32 t8_adsp_field_ADC_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADC_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADC_OFFSET_get", id, 2);
    /* (0x00000558 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADC_OFFSET_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_range_ADC_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_range_ADC_OFFSET_set( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADC_OFFSET_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADC_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADC_OFFSET_set", stop_bit, 1 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADC_OFFSET_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000558 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x00000558 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    id,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 t8_adsp_field_range_ADC_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_ADC_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADC_OFFSET_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADC_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADC_OFFSET_get", stop_bit, 1 );
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
    /* (0x00000558 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       id);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADC_OFFSET_get", id, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set( t8_adsp_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set( t8_adsp_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set", id, value );

    /* (0x0000050c bits 0) field CAPTURE_ADAPT_STATUS_REQUEST of register PMC_ADSP_REG_ADAPT_STAT */
    t8_adsp_reg_ADAPT_STAT_field_set( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_MSK,
                                      ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_OFF,
                                      value);
}

static INLINE UINT32 t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get( t8_adsp_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get( t8_adsp_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get", id, 2);
    /* (0x0000050c bits 0) field CAPTURE_ADAPT_STATUS_REQUEST of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = t8_adsp_reg_ADAPT_STAT_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_MSK) >> ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_CAPTURE_RESULTS_REQUEST_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_CAPTURE_RESULTS_REQUEST_set( t8_adsp_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CAPTURE_RESULTS_REQUEST_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_CAPTURE_RESULTS_REQUEST_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_CAPTURE_RESULTS_REQUEST_set", id, value );

    /* (0x00000510 bits 0) field CAPTURE_RESULTS_REQUEST of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST */
    t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( b_ptr,
                                                         h_ptr,
                                                         id,
                                                         ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_MSK,
                                                         ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_OFF,
                                                         value);
}

static INLINE UINT32 t8_adsp_field_CAPTURE_RESULTS_REQUEST_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_CAPTURE_RESULTS_REQUEST_get( t8_adsp_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CAPTURE_RESULTS_REQUEST_get", id, 2);
    /* (0x00000510 bits 0) field CAPTURE_RESULTS_REQUEST of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( b_ptr,
                                                                h_ptr,
                                                                id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_CAPTURE_RESULTS_REQUEST_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set( t8_adsp_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set( t8_adsp_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", id, value );

    /* (0x0000055c bits 9) field CAPTURE_GAIN_CLIP_FLAG_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( b_ptr,
                                                     h_ptr,
                                                     id,
                                                     ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_MSK,
                                                     ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_OFF,
                                                     value);
}

static INLINE UINT32 t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get( t8_adsp_buffer_t *b_ptr,
                                                                       t8_handle_t *h_ptr,
                                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get( t8_adsp_buffer_t *b_ptr,
                                                                       t8_handle_t *h_ptr,
                                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get", id, 2);
    /* (0x0000055c bits 9) field CAPTURE_GAIN_CLIP_FLAG_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set( t8_adsp_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set( t8_adsp_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", id, value );

    /* (0x0000055c bits 0) field CAPTURE_ADC_CLIP_FLAGS_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( b_ptr,
                                                     h_ptr,
                                                     id,
                                                     ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_MSK,
                                                     ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_OFF,
                                                     value);
}

static INLINE UINT32 t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get( t8_adsp_buffer_t *b_ptr,
                                                                       t8_handle_t *h_ptr,
                                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get( t8_adsp_buffer_t *b_ptr,
                                                                       t8_handle_t *h_ptr,
                                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get", id, 2);
    /* (0x0000055c bits 0) field CAPTURE_ADC_CLIP_FLAGS_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get", id, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void t8_adsp_field_PROG_REF_DONE_I_set_to_clear( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PROG_REF_DONE_I_set_to_clear( t8_adsp_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_DONE_I_set_to_clear", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PROG_REF_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PROG_REF_DONE_I_set_to_clear", id, value );

    /* (0x00000528 bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    t8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          id,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF,
                                                          value);
}

static INLINE UINT32 t8_adsp_field_PROG_REF_DONE_I_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_DONE_I_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_DONE_I_get", id, 2);
    /* (0x00000528 bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = t8_adsp_reg_STAT_INT_EVENT_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_DONE_I_get", id, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_DONE_I_poll( t8_adsp_buffer_t *b_ptr,
                                                                       t8_handle_t *h_ptr,
                                                                       UINT32 id,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_DONE_I_poll( t8_adsp_buffer_t *b_ptr,
                                                                       t8_handle_t *h_ptr,
                                                                       UINT32 id,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  id=%d 0x%08x", "t8_adsp_field_PROG_REF_DONE_I_poll", id, value );

    /* (0x00000528 bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return t8_adsp_reg_STAT_INT_EVENT_poll( b_ptr,
                                            h_ptr,
                                            id,
                                            ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK,
                                            (value<<ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void t8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear( t8_adsp_buffer_t *b_ptr,
                                                                  t8_handle_t *h_ptr,
                                                                  UINT32 id,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear( t8_adsp_buffer_t *b_ptr,
                                                                  t8_handle_t *h_ptr,
                                                                  UINT32 id,
                                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear", id, value );

    /* (0x00000528 bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    t8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          id,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF,
                                                          value);
}

static INLINE UINT32 t8_adsp_field_PROG_REF_TIMEOUT_I_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_TIMEOUT_I_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_TIMEOUT_I_get", id, 2);
    /* (0x00000528 bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = t8_adsp_reg_STAT_INT_EVENT_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_I_get", id, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_TIMEOUT_I_poll( t8_adsp_buffer_t *b_ptr,
                                                                          t8_handle_t *h_ptr,
                                                                          UINT32 id,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_TIMEOUT_I_poll( t8_adsp_buffer_t *b_ptr,
                                                                          t8_handle_t *h_ptr,
                                                                          UINT32 id,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_I_poll", id, value );

    /* (0x00000528 bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return t8_adsp_reg_STAT_INT_EVENT_poll( b_ptr,
                                            h_ptr,
                                            id,
                                            ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK,
                                            (value<<ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void t8_adsp_field_STOP_ADAPT_I_set_to_clear( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_STOP_ADAPT_I_set_to_clear( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_STOP_ADAPT_I_set_to_clear", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_STOP_ADAPT_I_set_to_clear", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_STOP_ADAPT_I_set_to_clear", id, value );

    /* (0x00000528 bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    t8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          id,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF,
                                                          value);
}

static INLINE UINT32 t8_adsp_field_STOP_ADAPT_I_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_STOP_ADAPT_I_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_STOP_ADAPT_I_get", id, 2);
    /* (0x00000528 bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = t8_adsp_reg_STAT_INT_EVENT_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_STOP_ADAPT_I_get", id, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_STOP_ADAPT_I_poll( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_STOP_ADAPT_I_poll( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  id=%d 0x%08x", "t8_adsp_field_STOP_ADAPT_I_poll", id, value );

    /* (0x00000528 bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return t8_adsp_reg_STAT_INT_EVENT_poll( b_ptr,
                                            h_ptr,
                                            id,
                                            ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK,
                                            (value<<ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF),
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
static INLINE void t8_adsp_field_PROG_REF_DONE_E_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PROG_REF_DONE_E_set( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_DONE_E_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PROG_REF_DONE_E_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PROG_REF_DONE_E_set", id, value );

    /* (0x00000524 bits 9) field PROG_REF_DONE_E of register PMC_ADSP_REG_STAT_INT_EN */
    t8_adsp_reg_STAT_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       id,
                                       ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_MSK,
                                       ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_OFF,
                                       value);
}

static INLINE UINT32 t8_adsp_field_PROG_REF_DONE_E_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_DONE_E_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_DONE_E_get", id, 2);
    /* (0x00000524 bits 9) field PROG_REF_DONE_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = t8_adsp_reg_STAT_INT_EN_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_DONE_E_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_PROG_REF_TIMEOUT_E_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_PROG_REF_TIMEOUT_E_set( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_TIMEOUT_E_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_E_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_E_set", id, value );

    /* (0x00000524 bits 8) field PROG_REF_TIMEOUT_E of register PMC_ADSP_REG_STAT_INT_EN */
    t8_adsp_reg_STAT_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       id,
                                       ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_MSK,
                                       ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_OFF,
                                       value);
}

static INLINE UINT32 t8_adsp_field_PROG_REF_TIMEOUT_E_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_TIMEOUT_E_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_TIMEOUT_E_get", id, 2);
    /* (0x00000524 bits 8) field PROG_REF_TIMEOUT_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = t8_adsp_reg_STAT_INT_EN_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_E_get", id, value );

    return value;
}
static INLINE void t8_adsp_field_STOP_ADAPT_E_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_adsp_field_STOP_ADAPT_E_set( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_STOP_ADAPT_E_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_adsp_field_STOP_ADAPT_E_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_adsp_field_STOP_ADAPT_E_set", id, value );

    /* (0x00000524 bits 0) field STOP_ADAPT_E of register PMC_ADSP_REG_STAT_INT_EN */
    t8_adsp_reg_STAT_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       id,
                                       ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_MSK,
                                       ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_OFF,
                                       value);
}

static INLINE UINT32 t8_adsp_field_STOP_ADAPT_E_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_STOP_ADAPT_E_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_STOP_ADAPT_E_get", id, 2);
    /* (0x00000524 bits 0) field STOP_ADAPT_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = t8_adsp_reg_STAT_INT_EN_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_STOP_ADAPT_E_get", id, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 t8_adsp_field_PROG_REF_DONE_V_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_DONE_V_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_DONE_V_get", id, 2);
    /* (0x0000052c bits 9) field PROG_REF_DONE_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = t8_adsp_reg_STAT_INT_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_MSK) >> ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_DONE_V_get", id, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_DONE_V_poll( t8_adsp_buffer_t *b_ptr,
                                                                       t8_handle_t *h_ptr,
                                                                       UINT32 id,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_DONE_V_poll( t8_adsp_buffer_t *b_ptr,
                                                                       t8_handle_t *h_ptr,
                                                                       UINT32 id,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  id=%d 0x%08x", "t8_adsp_field_PROG_REF_DONE_V_poll", id, value );

    /* (0x0000052c bits 9) field PROG_REF_DONE_V of register PMC_ADSP_REG_STAT_INT */
    return t8_adsp_reg_STAT_INT_poll( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_MSK,
                                      (value<<ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 t8_adsp_field_PROG_REF_TIMEOUT_V_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PROG_REF_TIMEOUT_V_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PROG_REF_TIMEOUT_V_get", id, 2);
    /* (0x0000052c bits 8) field PROG_REF_TIMEOUT_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = t8_adsp_reg_STAT_INT_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_MSK) >> ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_V_get", id, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_TIMEOUT_V_poll( t8_adsp_buffer_t *b_ptr,
                                                                          t8_handle_t *h_ptr,
                                                                          UINT32 id,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_TIMEOUT_V_poll( t8_adsp_buffer_t *b_ptr,
                                                                          t8_handle_t *h_ptr,
                                                                          UINT32 id,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  id=%d 0x%08x", "t8_adsp_field_PROG_REF_TIMEOUT_V_poll", id, value );

    /* (0x0000052c bits 8) field PROG_REF_TIMEOUT_V of register PMC_ADSP_REG_STAT_INT */
    return t8_adsp_reg_STAT_INT_poll( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_MSK,
                                      (value<<ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 t8_adsp_field_STOP_ADAPT_V_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_STOP_ADAPT_V_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_STOP_ADAPT_V_get", id, 2);
    /* (0x0000052c bits 0) field STOP_ADAPT_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = t8_adsp_reg_STAT_INT_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_MSK) >> ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_STOP_ADAPT_V_get", id, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_STOP_ADAPT_V_poll( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE t8_adsp_field_STOP_ADAPT_V_poll( t8_adsp_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  id=%d 0x%08x", "t8_adsp_field_STOP_ADAPT_V_poll", id, value );

    /* (0x0000052c bits 0) field STOP_ADAPT_V of register PMC_ADSP_REG_STAT_INT */
    return t8_adsp_reg_STAT_INT_poll( b_ptr,
                                      h_ptr,
                                      id,
                                      ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_MSK,
                                      (value<<ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 t8_adsp_field_ADAPT_STATUS_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADAPT_STATUS_get( t8_adsp_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADAPT_STATUS_get", id, 2);
    /* (0x0000050c bits 11:1) field ADAPT_STATUS of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = t8_adsp_reg_ADAPT_STAT_read(  b_ptr, h_ptr, id);
    value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK) >> ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADAPT_STATUS_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_ADAPT_STATUS_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_ADAPT_STATUS_get( t8_adsp_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADAPT_STATUS_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADAPT_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADAPT_STATUS_get", stop_bit, 10 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 10) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 10;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000050c bits 11:1) field ADAPT_STATUS of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = t8_adsp_reg_ADAPT_STAT_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK)
                  >> ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK, ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADAPT_STATUS_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_TR_FFE_B_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TR_FFE_B_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TR_FFE_B_get", id, 2);
    /* (0x00000514 bits 30:26) field TR_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TR_FFE_B_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_TR_FFE_B_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_TR_FFE_B_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TR_FFE_B_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TR_FFE_B_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TR_FFE_B_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000514 bits 30:26) field TR_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TR_FFE_B_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_TR_FFE_A_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TR_FFE_A_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TR_FFE_A_get", id, 2);
    /* (0x00000514 bits 25:21) field TR_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TR_FFE_A_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_TR_FFE_A_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_TR_FFE_A_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TR_FFE_A_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TR_FFE_A_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TR_FFE_A_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000514 bits 25:21) field TR_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TR_FFE_A_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DP_FFE_C_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_C_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_C_get", id, 2);
    /* (0x00000514 bits 20:14) field DP_FFE_C of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_C_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DP_FFE_C_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_C_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_C_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_C_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_C_get", stop_bit, 6 );
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
    /* (0x00000514 bits 20:14) field DP_FFE_C of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_C_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DP_FFE_B_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_B_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_B_get", id, 2);
    /* (0x00000514 bits 13:7) field DP_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_B_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DP_FFE_B_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_B_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_B_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_B_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_B_get", stop_bit, 6 );
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
    /* (0x00000514 bits 13:7) field DP_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_B_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DP_FFE_A_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DP_FFE_A_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DP_FFE_A_get", id, 2);
    /* (0x00000514 bits 6:0) field DP_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DP_FFE_A_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DP_FFE_A_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DP_FFE_A_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DP_FFE_A_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DP_FFE_A_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DP_FFE_A_get", stop_bit, 6 );
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
    /* (0x00000514 bits 6:0) field DP_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DP_FFE_A_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DFE_COEF3_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF3_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF3_get", id, 2);
    /* (0x00000518 bits 27:21) field DFE_COEF3 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF3_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DFE_COEF3_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF3_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF3_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF3_get", stop_bit, 6 );
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
    /* (0x00000518 bits 27:21) field DFE_COEF3 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DFE_COEF2_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF2_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF2_get", id, 2);
    /* (0x00000518 bits 20:14) field DFE_COEF2 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF2_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DFE_COEF2_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF2_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF2_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF2_get", stop_bit, 6 );
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
    /* (0x00000518 bits 20:14) field DFE_COEF2 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DFE_COEF1_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF1_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF1_get", id, 2);
    /* (0x00000518 bits 13:7) field DFE_COEF1 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF1_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DFE_COEF1_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF1_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF1_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF1_get", stop_bit, 6 );
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
    /* (0x00000518 bits 13:7) field DFE_COEF1 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DFE_COEF0_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF0_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF0_get", id, 2);
    /* (0x00000518 bits 6:0) field DFE_COEF0 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF0_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DFE_COEF0_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF0_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF0_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF0_get", stop_bit, 6 );
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
    /* (0x00000518 bits 6:0) field DFE_COEF0 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DFE_COEF5_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF5_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF5_get", id, 2);
    /* (0x0000051c bits 13:7) field DFE_COEF5 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF5_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DFE_COEF5_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF5_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF5_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF5_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF5_get", stop_bit, 6 );
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
    /* (0x0000051c bits 13:7) field DFE_COEF5 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF5_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_DFE_COEF4_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_DFE_COEF4_get( t8_adsp_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_DFE_COEF4_get", id, 2);
    /* (0x0000051c bits 6:0) field DFE_COEF4 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_DFE_COEF4_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_DFE_COEF4_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_DFE_COEF4_get( t8_adsp_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_DFE_COEF4_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_DFE_COEF4_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_DFE_COEF4_get", stop_bit, 6 );
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
    /* (0x0000051c bits 6:0) field DFE_COEF4 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_DFE_COEF4_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_ADC_MAP_SCALE_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_ADC_MAP_SCALE_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_ADC_MAP_SCALE_get", id, 2);
    /* (0x00000520 bits 22:19) field ADC_MAP_SCALE of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_ADC_MAP_SCALE_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_ADC_MAP_SCALE_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_ADC_MAP_SCALE_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_ADC_MAP_SCALE_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_ADC_MAP_SCALE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_ADC_MAP_SCALE_get", stop_bit, 3 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 3) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 3;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000520 bits 22:19) field ADC_MAP_SCALE of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_ADC_MAP_SCALE_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_TARGET_AMP_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_TARGET_AMP_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_TARGET_AMP_get", id, 2);
    /* (0x00000520 bits 18:13) field TARGET_AMP of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_TARGET_AMP_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_TARGET_AMP_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_TARGET_AMP_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_TARGET_AMP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_TARGET_AMP_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_TARGET_AMP_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000520 bits 18:13) field TARGET_AMP of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_TARGET_AMP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_PGA_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PGA_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PGA_OFFSET_get", id, 2);
    /* (0x00000520 bits 12:5) field PGA_OFFSET of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PGA_OFFSET_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_PGA_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_PGA_OFFSET_get( t8_adsp_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_PGA_OFFSET_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_PGA_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_PGA_OFFSET_get", stop_bit, 7 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 7) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 7;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000520 bits 12:5) field PGA_OFFSET of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_PGA_OFFSET_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_PGA_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_PGA_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_PGA_GAIN_get", id, 2);
    /* (0x00000520 bits 4:0) field PGA_GAIN of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                          h_ptr,
                                                          id);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_PGA_GAIN_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_PGA_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_PGA_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_PGA_GAIN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_PGA_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_PGA_GAIN_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000520 bits 4:0) field PGA_GAIN of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                          h_ptr,
                                                          id);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_PGA_GAIN_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_CLIP_FLAG_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_CLIP_FLAG_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CLIP_FLAG_ADC_get", id, 2);
    /* (0x0000055c bits 7:2) field CLIP_FLAG_ADC of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_CLIP_FLAG_ADC_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_CLIP_FLAG_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_CLIP_FLAG_ADC_get( t8_adsp_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_CLIP_FLAG_ADC_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_CLIP_FLAG_ADC_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_CLIP_FLAG_ADC_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000055c bits 7:2) field CLIP_FLAG_ADC of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK, ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_CLIP_FLAG_ADC_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_CLIP_FLAG_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_CLIP_FLAG_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_CLIP_FLAG_GAIN_get", id, 2);
    /* (0x00000560 bits 31:0) field CLIP_FLAG_GAIN of register PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( b_ptr,
                                                                 h_ptr,
                                                                 id);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK) >> ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_adsp_field_CLIP_FLAG_GAIN_get", id, value );

    return value;
}
static INLINE UINT32 t8_adsp_field_range_CLIP_FLAG_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_adsp_field_range_CLIP_FLAG_GAIN_get( t8_adsp_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
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

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_adsp_field_range_CLIP_FLAG_GAIN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_adsp_field_range_CLIP_FLAG_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_adsp_field_range_CLIP_FLAG_GAIN_get", stop_bit, 31 );
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
    /* (0x00000560 bits 31:0) field CLIP_FLAG_GAIN of register PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS */
    reg_value = t8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( b_ptr,
                                                                 h_ptr,
                                                                 id);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK, ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_adsp_field_range_CLIP_FLAG_GAIN_get", id, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _T8_ADSP_IO_INLINE_H */
