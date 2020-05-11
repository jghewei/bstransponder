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
 *     and register accessor functions for the t8_40n block
 *****************************************************************************/
#ifndef _T8_40N_IO_INLINE_H
#define _T8_40N_IO_INLINE_H

#include "t8.h"
#include "t8_40n_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define T8_40N_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for t8_40n
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
} t8_40n_buffer_t;
static INLINE void t8_40n_buffer_init( t8_40n_buffer_t *b_ptr,
                                       t8_handle_t *h_ptr,
                                       UINT32 id ) ALWAYS_INLINE;
static INLINE void t8_40n_buffer_init( t8_40n_buffer_t *b_ptr,
                                       t8_handle_t *h_ptr,
                                       UINT32 id )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->id                       = id;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x600) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s id=%d", "t8_40n_buffer_init", id);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void t8_40n_buffer_flush( t8_40n_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void t8_40n_buffer_flush( t8_40n_buffer_t *b_ptr )
{
    IOLOG( "t8_40n_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 t8_40n_reg_read( t8_40n_buffer_t *b_ptr,
                                      t8_handle_t *h_ptr,
                                      UINT32 id,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_read( t8_40n_buffer_t *b_ptr,
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
static INLINE void t8_40n_reg_write( t8_40n_buffer_t *b_ptr,
                                     t8_handle_t *h_ptr,
                                     UINT32 id,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_write( t8_40n_buffer_t *b_ptr,
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

static INLINE void t8_40n_field_set( t8_40n_buffer_t *b_ptr,
                                     t8_handle_t *h_ptr,
                                     UINT32 id,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_set( t8_40n_buffer_t *b_ptr,
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

static INLINE void t8_40n_action_on_write_field_set( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_action_on_write_field_set( t8_40n_buffer_t *b_ptr,
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

static INLINE void t8_40n_burst_read( t8_40n_buffer_t *b_ptr,
                                      t8_handle_t *h_ptr,
                                      UINT32 id,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void t8_40n_burst_read( t8_40n_buffer_t *b_ptr,
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

static INLINE void t8_40n_burst_write( t8_40n_buffer_t *b_ptr,
                                       t8_handle_t *h_ptr,
                                       UINT32 id,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void t8_40n_burst_write( t8_40n_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE t8_40n_poll( t8_40n_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE t8_40n_poll( t8_40n_buffer_t *b_ptr,
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
 *  register access functions for t8_40n
 * ==================================================================================
 */

static INLINE void t8_40n_reg_CONFIG_1_LW20_80_15_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_1_LW20_80_15_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_40n_reg_CONFIG_1_LW20_80_15_write", value );
    t8_40n_reg_write( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15,
                      value);
}

static INLINE void t8_40n_reg_CONFIG_1_LW20_80_15_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_1_LW20_80_15_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_40n_reg_CONFIG_1_LW20_80_15_field_set", id, mask, ofs, value );
    t8_40n_field_set( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15,
                      mask,
                      PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 t8_40n_reg_CONFIG_1_LW20_80_15_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_CONFIG_1_LW20_80_15_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_CONFIG,
                                 PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_CONFIG_1_LW20_80_15_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_40n_reg_CONFIG_2_LW20_80_15_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_2_LW20_80_15_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_40n_reg_CONFIG_2_LW20_80_15_write", value );
    t8_40n_reg_write( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15,
                      value);
}

static INLINE void t8_40n_reg_CONFIG_2_LW20_80_15_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_2_LW20_80_15_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_40n_reg_CONFIG_2_LW20_80_15_field_set", id, mask, ofs, value );
    t8_40n_field_set( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15,
                      mask,
                      PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 t8_40n_reg_CONFIG_2_LW20_80_15_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_CONFIG_2_LW20_80_15_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_CONFIG,
                                 PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_CONFIG_2_LW20_80_15_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_write( t8_40n_buffer_t *b_ptr,
                                                                                            t8_handle_t *h_ptr,
                                                                                            UINT32 id,
                                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_write( t8_40n_buffer_t *b_ptr,
                                                                                            t8_handle_t *h_ptr,
                                                                                            UINT32 id,
                                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_write", value );
    t8_40n_reg_write( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB,
                      value);
}

static INLINE void t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( t8_40n_buffer_t *b_ptr,
                                                                                                t8_handle_t *h_ptr,
                                                                                                UINT32 id,
                                                                                                UINT32 mask,
                                                                                                UINT32 ofs,
                                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( t8_40n_buffer_t *b_ptr,
                                                                                                t8_handle_t *h_ptr,
                                                                                                UINT32 id,
                                                                                                UINT32 mask,
                                                                                                UINT32 ofs,
                                                                                                UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set", id, mask, ofs, value );
    t8_40n_field_set( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB,
                      mask,
                      PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( t8_40n_buffer_t *b_ptr,
                                                                                             t8_handle_t *h_ptr,
                                                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( t8_40n_buffer_t *b_ptr,
                                                                                             t8_handle_t *h_ptr,
                                                                                             UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_CONFIG,
                                 PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_40n_reg_CONFIG_1_LW20_80_36_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_1_LW20_80_36_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_40n_reg_CONFIG_1_LW20_80_36_write", value );
    t8_40n_reg_write( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36,
                      value);
}

static INLINE void t8_40n_reg_CONFIG_1_LW20_80_36_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_1_LW20_80_36_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_40n_reg_CONFIG_1_LW20_80_36_field_set", id, mask, ofs, value );
    t8_40n_field_set( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36,
                      mask,
                      PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 t8_40n_reg_CONFIG_1_LW20_80_36_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_CONFIG_1_LW20_80_36_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_CONFIG,
                                 PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_CONFIG_1_LW20_80_36_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_40n_reg_CONFIG_2_LW20_80_36_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_2_LW20_80_36_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_40n_reg_CONFIG_2_LW20_80_36_write", value );
    t8_40n_reg_write( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36,
                      value);
}

static INLINE void t8_40n_reg_CONFIG_2_LW20_80_36_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_2_LW20_80_36_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_40n_reg_CONFIG_2_LW20_80_36_field_set", id, mask, ofs, value );
    t8_40n_field_set( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36,
                      mask,
                      PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 t8_40n_reg_CONFIG_2_LW20_80_36_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_CONFIG_2_LW20_80_36_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_CONFIG,
                                 PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_CONFIG_2_LW20_80_36_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_40n_reg_CONFIG_3_LW20_80_36_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_3_LW20_80_36_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_40n_reg_CONFIG_3_LW20_80_36_write", value );
    t8_40n_reg_write( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36,
                      value);
}

static INLINE void t8_40n_reg_CONFIG_3_LW20_80_36_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_3_LW20_80_36_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_40n_reg_CONFIG_3_LW20_80_36_field_set", id, mask, ofs, value );
    t8_40n_field_set( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36,
                      mask,
                      PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 t8_40n_reg_CONFIG_3_LW20_80_36_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_CONFIG_3_LW20_80_36_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_CONFIG,
                                 PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_CONFIG_3_LW20_80_36_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_40n_reg_CONFIG_4_LW20_80_36_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_4_LW20_80_36_write( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_40n_reg_CONFIG_4_LW20_80_36_write", value );
    t8_40n_reg_write( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36,
                      value);
}

static INLINE void t8_40n_reg_CONFIG_4_LW20_80_36_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_CONFIG_4_LW20_80_36_field_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_40n_reg_CONFIG_4_LW20_80_36_field_set", id, mask, ofs, value );
    t8_40n_field_set( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36,
                      mask,
                      PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 t8_40n_reg_CONFIG_4_LW20_80_36_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_CONFIG_4_LW20_80_36_read( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_CONFIG,
                                 PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_CONFIG_4_LW20_80_36_read", reg_value, id);
    return reg_value;
}

static INLINE void t8_40n_reg_ADD_DROP_BLOCKCNT_write( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_ADD_DROP_BLOCKCNT_write( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "t8_40n_reg_ADD_DROP_BLOCKCNT_write", value );
    t8_40n_reg_write( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT,
                      value);
}

static INLINE void t8_40n_reg_ADD_DROP_BLOCKCNT_field_set( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_reg_ADD_DROP_BLOCKCNT_field_set( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- id=%d mask=0x%08x ofs=%d value=0x%08x", "t8_40n_reg_ADD_DROP_BLOCKCNT_field_set", id, mask, ofs, value );
    t8_40n_field_set( b_ptr,
                      h_ptr,
                      id,
                      MEM_TYPE_CONFIG,
                      PMC_MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT,
                      mask,
                      PMC_MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 t8_40n_reg_ADD_DROP_BLOCKCNT_read( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_ADD_DROP_BLOCKCNT_read( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_CONFIG,
                                 PMC_MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_ADD_DROP_BLOCKCNT_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_A_TIMING_CTRL_read( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_A_TIMING_CTRL_read( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_A_TIMING_CTRL);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_A_TIMING_CTRL_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_B_TIMING_CTRL_read( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_B_TIMING_CTRL_read( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_B_TIMING_CTRL);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_B_TIMING_CTRL_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_C_TIMING_CTRL_read( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_C_TIMING_CTRL_read( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_C_TIMING_CTRL);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_C_TIMING_CTRL_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_DFE_STATE_read( t8_40n_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_DFE_STATE_read( t8_40n_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_DFE_STATE);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_DFE_STATE_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_TCB_STATE_read( t8_40n_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_TCB_STATE_read( t8_40n_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_TCB_STATE);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_TCB_STATE_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_ADD_DROP_ADDCNT_read( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_ADD_DROP_ADDCNT_read( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_ADD_DROP_ADDCNT);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_ADD_DROP_ADDCNT_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_ADD_DROP_DROPCNT_read( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_ADD_DROP_DROPCNT_read( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_ADD_DROP_DROPCNT);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_ADD_DROP_DROPCNT_read", reg_value, id);
    return reg_value;
}

static INLINE UINT32 t8_40n_reg_ADD_DROP_DIFFCNT_read( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_reg_ADD_DROP_DIFFCNT_read( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 reg_value;

    reg_value = t8_40n_reg_read( b_ptr,
                                 h_ptr,
                                 id,
                                 MEM_TYPE_STATUS,
                                 PMC_MDSP_T8_40N_REG_ADD_DROP_DIFFCNT);

    IOLOG( "%s -> 0x%08x; id=%d", "t8_40n_reg_ADD_DROP_DIFFCNT_read", reg_value, id);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void t8_40n_field_ADC_LEVELS_VEC_29_TO_24_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_29_TO_24_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_29_TO_24_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_29_TO_24_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_29_TO_24_set", id, value );

    /* (0x00000400 bits 29:24) field ADC_LEVELS_VEC_29_TO_24 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_29_TO_24_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_29_TO_24_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_29_TO_24_get", id, 2);
    /* (0x00000400 bits 29:24) field ADC_LEVELS_VEC_29_TO_24 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_29_TO_24_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000400 bits 29:24) field ADC_LEVELS_VEC_29_TO_24 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
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
        /* (0x00000400 bits 29:24) field ADC_LEVELS_VEC_29_TO_24 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
        t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_get", stop_bit, 5 );
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
    /* (0x00000400 bits 29:24) field ADC_LEVELS_VEC_29_TO_24 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_29_TO_24_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_23_TO_18_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_23_TO_18_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_23_TO_18_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_23_TO_18_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_23_TO_18_set", id, value );

    /* (0x00000400 bits 23:18) field ADC_LEVELS_VEC_23_TO_18 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_23_TO_18_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_23_TO_18_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_23_TO_18_get", id, 2);
    /* (0x00000400 bits 23:18) field ADC_LEVELS_VEC_23_TO_18 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_23_TO_18_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000400 bits 23:18) field ADC_LEVELS_VEC_23_TO_18 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
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
        /* (0x00000400 bits 23:18) field ADC_LEVELS_VEC_23_TO_18 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
        t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_get", stop_bit, 5 );
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
    /* (0x00000400 bits 23:18) field ADC_LEVELS_VEC_23_TO_18 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_23_TO_18_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_17_TO_12_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_17_TO_12_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_17_TO_12_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_17_TO_12_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_17_TO_12_set", id, value );

    /* (0x00000400 bits 17:12) field ADC_LEVELS_VEC_17_TO_12 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_17_TO_12_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_17_TO_12_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_17_TO_12_get", id, 2);
    /* (0x00000400 bits 17:12) field ADC_LEVELS_VEC_17_TO_12 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_17_TO_12_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000400 bits 17:12) field ADC_LEVELS_VEC_17_TO_12 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
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
        /* (0x00000400 bits 17:12) field ADC_LEVELS_VEC_17_TO_12 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
        t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_get", stop_bit, 5 );
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
    /* (0x00000400 bits 17:12) field ADC_LEVELS_VEC_17_TO_12 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_17_TO_12_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_11_TO_6_set( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_11_TO_6_set( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_11_TO_6_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_11_TO_6_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_11_TO_6_set", id, value );

    /* (0x00000400 bits 11:6) field ADC_LEVELS_VEC_11_TO_6 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_11_TO_6_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_11_TO_6_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_11_TO_6_get", id, 2);
    /* (0x00000400 bits 11:6) field ADC_LEVELS_VEC_11_TO_6 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_11_TO_6_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_set( t8_40n_buffer_t *b_ptr,
                                                                  t8_handle_t *h_ptr,
                                                                  UINT32 id,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_set( t8_40n_buffer_t *b_ptr,
                                                                  t8_handle_t *h_ptr,
                                                                  UINT32 id,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000400 bits 11:6) field ADC_LEVELS_VEC_11_TO_6 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
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
        /* (0x00000400 bits 11:6) field ADC_LEVELS_VEC_11_TO_6 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
        t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_get", stop_bit, 5 );
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
    /* (0x00000400 bits 11:6) field ADC_LEVELS_VEC_11_TO_6 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_11_TO_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_5_TO_0_set( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_5_TO_0_set( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_5_TO_0_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_5_TO_0_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_5_TO_0_set", id, value );

    /* (0x00000400 bits 5:0) field ADC_LEVELS_VEC_5_TO_0 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_5_TO_0_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_5_TO_0_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_5_TO_0_get", id, 2);
    /* (0x00000400 bits 5:0) field ADC_LEVELS_VEC_5_TO_0 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_5_TO_0_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_set( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_set( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000400 bits 5:0) field ADC_LEVELS_VEC_5_TO_0 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
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
        /* (0x00000400 bits 5:0) field ADC_LEVELS_VEC_5_TO_0 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
        t8_40n_reg_CONFIG_1_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_get", stop_bit, 5 );
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
    /* (0x00000400 bits 5:0) field ADC_LEVELS_VEC_5_TO_0 of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_15_BIT_ADC_LEVELS_VEC_5_TO_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_59_TO_54_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_59_TO_54_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_59_TO_54_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_59_TO_54_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_59_TO_54_set", id, value );

    /* (0x00000404 bits 29:24) field ADC_LEVELS_VEC_59_TO_54 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_59_TO_54_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_59_TO_54_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_59_TO_54_get", id, 2);
    /* (0x00000404 bits 29:24) field ADC_LEVELS_VEC_59_TO_54 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_59_TO_54_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000404 bits 29:24) field ADC_LEVELS_VEC_59_TO_54 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
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
        /* (0x00000404 bits 29:24) field ADC_LEVELS_VEC_59_TO_54 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
        t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_get", stop_bit, 5 );
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
    /* (0x00000404 bits 29:24) field ADC_LEVELS_VEC_59_TO_54 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_MSK, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_59_TO_54_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_53_TO_48_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_53_TO_48_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_53_TO_48_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_53_TO_48_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_53_TO_48_set", id, value );

    /* (0x00000404 bits 23:18) field ADC_LEVELS_VEC_53_TO_48 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_53_TO_48_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_53_TO_48_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_53_TO_48_get", id, 2);
    /* (0x00000404 bits 23:18) field ADC_LEVELS_VEC_53_TO_48 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_53_TO_48_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000404 bits 23:18) field ADC_LEVELS_VEC_53_TO_48 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
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
        /* (0x00000404 bits 23:18) field ADC_LEVELS_VEC_53_TO_48 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
        t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_get", stop_bit, 5 );
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
    /* (0x00000404 bits 23:18) field ADC_LEVELS_VEC_53_TO_48 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_MSK, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_53_TO_48_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_47_TO_42_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_47_TO_42_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_47_TO_42_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_47_TO_42_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_47_TO_42_set", id, value );

    /* (0x00000404 bits 17:12) field ADC_LEVELS_VEC_47_TO_42 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_47_TO_42_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_47_TO_42_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_47_TO_42_get", id, 2);
    /* (0x00000404 bits 17:12) field ADC_LEVELS_VEC_47_TO_42 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_47_TO_42_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000404 bits 17:12) field ADC_LEVELS_VEC_47_TO_42 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
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
        /* (0x00000404 bits 17:12) field ADC_LEVELS_VEC_47_TO_42 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
        t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_get", stop_bit, 5 );
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
    /* (0x00000404 bits 17:12) field ADC_LEVELS_VEC_47_TO_42 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_MSK, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_47_TO_42_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_41_TO_36_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_41_TO_36_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_41_TO_36_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_41_TO_36_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_41_TO_36_set", id, value );

    /* (0x00000404 bits 11:6) field ADC_LEVELS_VEC_41_TO_36 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_41_TO_36_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_41_TO_36_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_41_TO_36_get", id, 2);
    /* (0x00000404 bits 11:6) field ADC_LEVELS_VEC_41_TO_36 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_41_TO_36_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000404 bits 11:6) field ADC_LEVELS_VEC_41_TO_36 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
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
        /* (0x00000404 bits 11:6) field ADC_LEVELS_VEC_41_TO_36 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
        t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_get", stop_bit, 5 );
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
    /* (0x00000404 bits 11:6) field ADC_LEVELS_VEC_41_TO_36 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_MSK, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_41_TO_36_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_35_TO_30_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_35_TO_30_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_35_TO_30_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_35_TO_30_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_35_TO_30_set", id, value );

    /* (0x00000404 bits 5:0) field ADC_LEVELS_VEC_35_TO_30 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_35_TO_30_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_35_TO_30_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_35_TO_30_get", id, 2);
    /* (0x00000404 bits 5:0) field ADC_LEVELS_VEC_35_TO_30 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_35_TO_30_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000404 bits 5:0) field ADC_LEVELS_VEC_35_TO_30 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
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
        /* (0x00000404 bits 5:0) field ADC_LEVELS_VEC_35_TO_30 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
        t8_40n_reg_CONFIG_2_LW20_80_15_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_get", stop_bit, 5 );
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
    /* (0x00000404 bits 5:0) field ADC_LEVELS_VEC_35_TO_30 of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_15 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_15_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_MSK, MDSP_T8_40N_REG_CONFIG_2_LW20_80_15_BIT_ADC_LEVELS_VEC_35_TO_30_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_FIFO_HW_RESET_EN_set( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_FIFO_HW_RESET_EN_set( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_FIFO_HW_RESET_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_FIFO_HW_RESET_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_FIFO_HW_RESET_EN_set", id, value );

    /* (0x00000408 bits 31) field FIFO_HW_RESET_EN of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_FIFO_HW_RESET_EN_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_FIFO_HW_RESET_EN_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_FIFO_HW_RESET_EN_get( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_FIFO_HW_RESET_EN_get( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_FIFO_HW_RESET_EN_get", id, 2);
    /* (0x00000408 bits 31) field FIFO_HW_RESET_EN of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_FIFO_HW_RESET_EN_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_FIFO_HW_RESET_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_FIFO_HW_RESET_EN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_FIFO_RESET_FROM_PCBI_set( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_FIFO_RESET_FROM_PCBI_set( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_FIFO_RESET_FROM_PCBI_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_FIFO_RESET_FROM_PCBI_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_FIFO_RESET_FROM_PCBI_set", id, value );

    /* (0x00000408 bits 30) field FIFO_RESET_FROM_PCBI of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_FIFO_RESET_FROM_PCBI_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_FIFO_RESET_FROM_PCBI_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_FIFO_RESET_FROM_PCBI_get( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_FIFO_RESET_FROM_PCBI_get( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_FIFO_RESET_FROM_PCBI_get", id, 2);
    /* (0x00000408 bits 30) field FIFO_RESET_FROM_PCBI of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_FIFO_RESET_FROM_PCBI_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_FIFO_RESET_FROM_PCBI_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_FIFO_RESET_FROM_PCBI_get", id, value );

    return value;
}
static INLINE void t8_40n_field_RD_PTR_RCNTR_set( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_RD_PTR_RCNTR_set( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_RD_PTR_RCNTR_set", id, 2);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_RD_PTR_RCNTR_set", value, 7);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_RD_PTR_RCNTR_set", id, value );

    /* (0x00000408 bits 29:27) field RD_PTR_RCNTR of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_RD_PTR_RCNTR_get( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_RD_PTR_RCNTR_get( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_RD_PTR_RCNTR_get", id, 2);
    /* (0x00000408 bits 29:27) field RD_PTR_RCNTR of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_RD_PTR_RCNTR_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_RD_PTR_RCNTR_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_RD_PTR_RCNTR_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_RD_PTR_RCNTR_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_RD_PTR_RCNTR_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_RD_PTR_RCNTR_set", stop_bit, 2 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_RD_PTR_RCNTR_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000408 bits 29:27) field RD_PTR_RCNTR of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
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
        /* (0x00000408 bits 29:27) field RD_PTR_RCNTR of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
        t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                     h_ptr,
                                                                                     id,
                                                                                     subfield_mask << (MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_OFF + subfield_offset),
                                                                                     MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_OFF + subfield_offset,
                                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_RD_PTR_RCNTR_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_RD_PTR_RCNTR_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_RD_PTR_RCNTR_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_RD_PTR_RCNTR_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_RD_PTR_RCNTR_get", stop_bit, 2 );
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
    /* (0x00000408 bits 29:27) field RD_PTR_RCNTR of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_MSK, MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_RD_PTR_RCNTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_RD_PTR_RCNTR_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_BUS_WIDTH_set( t8_40n_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_BUS_WIDTH_set( t8_40n_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id,
                                               UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_BUS_WIDTH_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_BUS_WIDTH_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_BUS_WIDTH_set", id, value );

    /* (0x00000408 bits 26) field BUS_WIDTH of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_BUS_WIDTH_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_BUS_WIDTH_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_BUS_WIDTH_get( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_BUS_WIDTH_get( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_BUS_WIDTH_get", id, 2);
    /* (0x00000408 bits 26) field BUS_WIDTH of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_BUS_WIDTH_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_BUS_WIDTH_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_BUS_WIDTH_get", id, value );

    return value;
}
static INLINE void t8_40n_field_ADD_DROP_HOLD_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADD_DROP_HOLD_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_HOLD_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADD_DROP_HOLD_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADD_DROP_HOLD_set", id, value );

    /* (0x00000408 bits 18) field ADD_DROP_HOLD of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADD_DROP_HOLD_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADD_DROP_HOLD_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_ADD_DROP_HOLD_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_HOLD_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_HOLD_get", id, 2);
    /* (0x00000408 bits 18) field ADD_DROP_HOLD of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADD_DROP_HOLD_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADD_DROP_HOLD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_HOLD_get", id, value );

    return value;
}
static INLINE void t8_40n_field_DLB_ENB_set( t8_40n_buffer_t *b_ptr,
                                             t8_handle_t *h_ptr,
                                             UINT32 id,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_DLB_ENB_set( t8_40n_buffer_t *b_ptr,
                                             t8_handle_t *h_ptr,
                                             UINT32 id,
                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_DLB_ENB_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_DLB_ENB_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_DLB_ENB_set", id, value );

    /* (0x00000408 bits 17) field DLB_ENB of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_DLB_ENB_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_DLB_ENB_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_DLB_ENB_get( t8_40n_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_DLB_ENB_get( t8_40n_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_DLB_ENB_get", id, 2);
    /* (0x00000408 bits 17) field DLB_ENB of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_DLB_ENB_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_DLB_ENB_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_DLB_ENB_get", id, value );

    return value;
}
static INLINE void t8_40n_field_CFG_FROM_PCBI_EN_set( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_CFG_FROM_PCBI_EN_set( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_CFG_FROM_PCBI_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_CFG_FROM_PCBI_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_CFG_FROM_PCBI_EN_set", id, value );

    /* (0x00000408 bits 16) field CFG_FROM_PCBI_EN of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_CFG_FROM_PCBI_EN_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_CFG_FROM_PCBI_EN_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_CFG_FROM_PCBI_EN_get( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_CFG_FROM_PCBI_EN_get( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_CFG_FROM_PCBI_EN_get", id, 2);
    /* (0x00000408 bits 16) field CFG_FROM_PCBI_EN of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_CFG_FROM_PCBI_EN_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_CFG_FROM_PCBI_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_CFG_FROM_PCBI_EN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_PCBI_PARAM_EN_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_PCBI_PARAM_EN_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PCBI_PARAM_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_PCBI_PARAM_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_PCBI_PARAM_EN_set", id, value );

    /* (0x00000408 bits 15) field PCBI_PARAM_EN of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_PCBI_PARAM_EN_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_PCBI_PARAM_EN_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_PCBI_PARAM_EN_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_PCBI_PARAM_EN_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PCBI_PARAM_EN_get", id, 2);
    /* (0x00000408 bits 15) field PCBI_PARAM_EN of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_PCBI_PARAM_EN_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_PCBI_PARAM_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_PCBI_PARAM_EN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_DFE_BYPASS_set( t8_40n_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_DFE_BYPASS_set( t8_40n_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id,
                                                UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_DFE_BYPASS_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_DFE_BYPASS_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_DFE_BYPASS_set", id, value );

    /* (0x00000408 bits 11) field DFE_BYPASS of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_DFE_BYPASS_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_DFE_BYPASS_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_DFE_BYPASS_get( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_DFE_BYPASS_get( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_DFE_BYPASS_get", id, 2);
    /* (0x00000408 bits 11) field DFE_BYPASS of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_DFE_BYPASS_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_DFE_BYPASS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_DFE_BYPASS_get", id, value );

    return value;
}
static INLINE void t8_40n_field_INV_DFE_OUT_set( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_INV_DFE_OUT_set( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INV_DFE_OUT_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_INV_DFE_OUT_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_INV_DFE_OUT_set", id, value );

    /* (0x00000408 bits 10) field INV_DFE_OUT of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DFE_OUT_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DFE_OUT_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_INV_DFE_OUT_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INV_DFE_OUT_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INV_DFE_OUT_get", id, 2);
    /* (0x00000408 bits 10) field INV_DFE_OUT of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DFE_OUT_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DFE_OUT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INV_DFE_OUT_get", id, value );

    return value;
}
static INLINE void t8_40n_field_INV_DFE_ERROR_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_INV_DFE_ERROR_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INV_DFE_ERROR_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_INV_DFE_ERROR_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_INV_DFE_ERROR_set", id, value );

    /* (0x00000408 bits 9) field INV_DFE_ERROR of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DFE_ERROR_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DFE_ERROR_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_INV_DFE_ERROR_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INV_DFE_ERROR_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INV_DFE_ERROR_get", id, 2);
    /* (0x00000408 bits 9) field INV_DFE_ERROR of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DFE_ERROR_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DFE_ERROR_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INV_DFE_ERROR_get", id, value );

    return value;
}
static INLINE void t8_40n_field_INV_DP_FFE_IN_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_INV_DP_FFE_IN_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INV_DP_FFE_IN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_INV_DP_FFE_IN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_INV_DP_FFE_IN_set", id, value );

    /* (0x00000408 bits 8) field INV_DP_FFE_IN of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DP_FFE_IN_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DP_FFE_IN_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_INV_DP_FFE_IN_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INV_DP_FFE_IN_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INV_DP_FFE_IN_get", id, 2);
    /* (0x00000408 bits 8) field INV_DP_FFE_IN of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DP_FFE_IN_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_INV_DP_FFE_IN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INV_DP_FFE_IN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_ADC_LEVELS_VEC_65_TO_60_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADC_LEVELS_VEC_65_TO_60_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_65_TO_60_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_65_TO_60_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_65_TO_60_set", id, value );

    /* (0x00000408 bits 5:0) field ADC_LEVELS_VEC_65_TO_60 of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_65_TO_60_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADC_LEVELS_VEC_65_TO_60_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADC_LEVELS_VEC_65_TO_60_get", id, 2);
    /* (0x00000408 bits 5:0) field ADC_LEVELS_VEC_65_TO_60 of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADC_LEVELS_VEC_65_TO_60_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_set( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000408 bits 5:0) field ADC_LEVELS_VEC_65_TO_60 of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
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
        /* (0x00000408 bits 5:0) field ADC_LEVELS_VEC_65_TO_60 of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
        t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                     h_ptr,
                                                                                     id,
                                                                                     subfield_mask << (MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_OFF + subfield_offset),
                                                                                     MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_OFF + subfield_offset,
                                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_get( t8_40n_buffer_t *b_ptr,
                                                                     t8_handle_t *h_ptr,
                                                                     UINT32 id,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_get", stop_bit, 5 );
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
    /* (0x00000408 bits 5:0) field ADC_LEVELS_VEC_65_TO_60 of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_MSK, MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_ADC_LEVELS_VEC_65_TO_60_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_TR_FFE_B_TIMING_DIS_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_TR_FFE_B_TIMING_DIS_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TR_FFE_B_TIMING_DIS_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_TR_FFE_B_TIMING_DIS_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_TR_FFE_B_TIMING_DIS_set", id, value );

    /* (0x0000040c bits 26) field TR_FFE_B_TIMING_DIS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_B_TIMING_DIS_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_B_TIMING_DIS_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_TR_FFE_B_TIMING_DIS_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_TR_FFE_B_TIMING_DIS_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TR_FFE_B_TIMING_DIS_get", id, 2);
    /* (0x0000040c bits 26) field TR_FFE_B_TIMING_DIS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_B_TIMING_DIS_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_B_TIMING_DIS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_TR_FFE_B_TIMING_DIS_get", id, value );

    return value;
}
static INLINE void t8_40n_field_TR_FFE_A_TIMING_DIS_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_TR_FFE_A_TIMING_DIS_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TR_FFE_A_TIMING_DIS_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_TR_FFE_A_TIMING_DIS_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_TR_FFE_A_TIMING_DIS_set", id, value );

    /* (0x0000040c bits 25) field TR_FFE_A_TIMING_DIS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_A_TIMING_DIS_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_A_TIMING_DIS_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_TR_FFE_A_TIMING_DIS_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_TR_FFE_A_TIMING_DIS_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TR_FFE_A_TIMING_DIS_get", id, 2);
    /* (0x0000040c bits 25) field TR_FFE_A_TIMING_DIS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_A_TIMING_DIS_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_A_TIMING_DIS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_TR_FFE_A_TIMING_DIS_get", id, value );

    return value;
}
static INLINE void t8_40n_field_TR_FFE_B_DATA_DIS_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_TR_FFE_B_DATA_DIS_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TR_FFE_B_DATA_DIS_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_TR_FFE_B_DATA_DIS_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_TR_FFE_B_DATA_DIS_set", id, value );

    /* (0x0000040c bits 24) field TR_FFE_B_DATA_DIS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_B_DATA_DIS_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_B_DATA_DIS_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_TR_FFE_B_DATA_DIS_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_TR_FFE_B_DATA_DIS_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TR_FFE_B_DATA_DIS_get", id, 2);
    /* (0x0000040c bits 24) field TR_FFE_B_DATA_DIS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_B_DATA_DIS_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_B_DATA_DIS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_TR_FFE_B_DATA_DIS_get", id, value );

    return value;
}
static INLINE void t8_40n_field_TR_FFE_A_DATA_DIS_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_TR_FFE_A_DATA_DIS_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TR_FFE_A_DATA_DIS_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_TR_FFE_A_DATA_DIS_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_TR_FFE_A_DATA_DIS_set", id, value );

    /* (0x0000040c bits 23) field TR_FFE_A_DATA_DIS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_A_DATA_DIS_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_A_DATA_DIS_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_TR_FFE_A_DATA_DIS_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_TR_FFE_A_DATA_DIS_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TR_FFE_A_DATA_DIS_get", id, 2);
    /* (0x0000040c bits 23) field TR_FFE_A_DATA_DIS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_A_DATA_DIS_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TR_FFE_A_DATA_DIS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_TR_FFE_A_DATA_DIS_get", id, value );

    return value;
}
static INLINE void t8_40n_field_LF_INTEG_LEFT_SHIFT_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_INTEG_LEFT_SHIFT_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEG_LEFT_SHIFT_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_INTEG_LEFT_SHIFT_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_INTEG_LEFT_SHIFT_set", id, value );

    /* (0x0000040c bits 21) field LF_INTEG_LEFT_SHIFT of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_LEFT_SHIFT_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_LEFT_SHIFT_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_INTEG_LEFT_SHIFT_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_INTEG_LEFT_SHIFT_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEG_LEFT_SHIFT_get", id, 2);
    /* (0x0000040c bits 21) field LF_INTEG_LEFT_SHIFT of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_LEFT_SHIFT_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_LEFT_SHIFT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_INTEG_LEFT_SHIFT_get", id, value );

    return value;
}
static INLINE void t8_40n_field_LF_INTEG_CTRL_GAIN_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_INTEG_CTRL_GAIN_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEG_CTRL_GAIN_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_INTEG_CTRL_GAIN_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_INTEG_CTRL_GAIN_set", id, value );

    /* (0x0000040c bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_INTEG_CTRL_GAIN_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_INTEG_CTRL_GAIN_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEG_CTRL_GAIN_get", id, 2);
    /* (0x0000040c bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_INTEG_CTRL_GAIN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_LF_INTEG_CTRL_GAIN_set( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_LF_INTEG_CTRL_GAIN_set( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_INTEG_CTRL_GAIN_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_INTEG_CTRL_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_INTEG_CTRL_GAIN_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_INTEG_CTRL_GAIN_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000040c bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
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
        /* (0x0000040c bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
        t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_LF_INTEG_CTRL_GAIN_get( t8_40n_buffer_t *b_ptr,
                                                                t8_handle_t *h_ptr,
                                                                UINT32 id,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_LF_INTEG_CTRL_GAIN_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_INTEG_CTRL_GAIN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_INTEG_CTRL_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_INTEG_CTRL_GAIN_get", stop_bit, 4 );
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
    /* (0x0000040c bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_INTEG_CTRL_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_INTEG_CTRL_GAIN_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_LF_PROP_LEFT_SHIFT_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_PROP_LEFT_SHIFT_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_PROP_LEFT_SHIFT_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_PROP_LEFT_SHIFT_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_PROP_LEFT_SHIFT_set", id, value );

    /* (0x0000040c bits 14) field LF_PROP_LEFT_SHIFT of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_LEFT_SHIFT_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_LEFT_SHIFT_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_PROP_LEFT_SHIFT_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_PROP_LEFT_SHIFT_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_PROP_LEFT_SHIFT_get", id, 2);
    /* (0x0000040c bits 14) field LF_PROP_LEFT_SHIFT of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_LEFT_SHIFT_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_LEFT_SHIFT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_PROP_LEFT_SHIFT_get", id, value );

    return value;
}
static INLINE void t8_40n_field_LF_PROP_CTRL_GAIN_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_PROP_CTRL_GAIN_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_PROP_CTRL_GAIN_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_PROP_CTRL_GAIN_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_PROP_CTRL_GAIN_set", id, value );

    /* (0x0000040c bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_PROP_CTRL_GAIN_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_PROP_CTRL_GAIN_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_PROP_CTRL_GAIN_get", id, 2);
    /* (0x0000040c bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_PROP_CTRL_GAIN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_LF_PROP_CTRL_GAIN_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_LF_PROP_CTRL_GAIN_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_PROP_CTRL_GAIN_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_PROP_CTRL_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_PROP_CTRL_GAIN_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_PROP_CTRL_GAIN_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000040c bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
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
        /* (0x0000040c bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
        t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_LF_PROP_CTRL_GAIN_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_LF_PROP_CTRL_GAIN_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_PROP_CTRL_GAIN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_PROP_CTRL_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_PROP_CTRL_GAIN_get", stop_bit, 4 );
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
    /* (0x0000040c bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_PROP_CTRL_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_PROP_CTRL_GAIN_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_TED_HOLD_OUTPUT_EN_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_TED_HOLD_OUTPUT_EN_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TED_HOLD_OUTPUT_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_TED_HOLD_OUTPUT_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_TED_HOLD_OUTPUT_EN_set", id, value );

    /* (0x0000040c bits 8) field TED_HOLD_OUTPUT_EN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_HOLD_OUTPUT_EN_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_HOLD_OUTPUT_EN_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_TED_HOLD_OUTPUT_EN_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_TED_HOLD_OUTPUT_EN_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TED_HOLD_OUTPUT_EN_get", id, 2);
    /* (0x0000040c bits 8) field TED_HOLD_OUTPUT_EN of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_HOLD_OUTPUT_EN_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_HOLD_OUTPUT_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_TED_HOLD_OUTPUT_EN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_TED_MODE_SEL_set( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_TED_MODE_SEL_set( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TED_MODE_SEL_set", id, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_TED_MODE_SEL_set", value, 3);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_TED_MODE_SEL_set", id, value );

    /* (0x0000040c bits 7:6) field TED_MODE_SEL of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_TED_MODE_SEL_get( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_TED_MODE_SEL_get( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_TED_MODE_SEL_get", id, 2);
    /* (0x0000040c bits 7:6) field TED_MODE_SEL of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_TED_MODE_SEL_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_TED_MODE_SEL_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_TED_MODE_SEL_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_TED_MODE_SEL_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_TED_MODE_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_TED_MODE_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_TED_MODE_SEL_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000040c bits 7:6) field TED_MODE_SEL of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
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
        /* (0x0000040c bits 7:6) field TED_MODE_SEL of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
        t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_TED_MODE_SEL_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_TED_MODE_SEL_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_TED_MODE_SEL_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_TED_MODE_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_TED_MODE_SEL_get", stop_bit, 1 );
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
    /* (0x0000040c bits 7:6) field TED_MODE_SEL of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_TED_MODE_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_TED_MODE_SEL_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_LF_BIAS_set( t8_40n_buffer_t *b_ptr,
                                             t8_handle_t *h_ptr,
                                             UINT32 id,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_BIAS_set( t8_40n_buffer_t *b_ptr,
                                             t8_handle_t *h_ptr,
                                             UINT32 id,
                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_BIAS_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_BIAS_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_BIAS_set", id, value );

    /* (0x0000040c bits 4:0) field LF_BIAS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_MSK,
                                              MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_BIAS_get( t8_40n_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_BIAS_get( t8_40n_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_BIAS_get", id, 2);
    /* (0x0000040c bits 4:0) field LF_BIAS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_MSK) >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_BIAS_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_LF_BIAS_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_LF_BIAS_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_BIAS_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_BIAS_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_BIAS_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_BIAS_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000040c bits 4:0) field LF_BIAS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
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
        /* (0x0000040c bits 4:0) field LF_BIAS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
        t8_40n_reg_CONFIG_1_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_LF_BIAS_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_LF_BIAS_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_BIAS_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_BIAS_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_BIAS_get", stop_bit, 4 );
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
    /* (0x0000040c bits 4:0) field LF_BIAS of register PMC_MDSP_T8_40N_REG_CONFIG_1_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_1_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_MSK, MDSP_T8_40N_REG_CONFIG_1_LW20_80_36_BIT_LF_BIAS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_BIAS_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_PC_INTERP_IDX_OVR_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_PC_INTERP_IDX_OVR_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_INTERP_IDX_OVR_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_PC_INTERP_IDX_OVR_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_PC_INTERP_IDX_OVR_set", id, value );

    /* (0x00000410 bits 25:21) field PC_INTERP_IDX_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    t8_40n_reg_CONFIG_2_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_PC_INTERP_IDX_OVR_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_PC_INTERP_IDX_OVR_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_INTERP_IDX_OVR_get", id, 2);
    /* (0x00000410 bits 25:21) field PC_INTERP_IDX_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_PC_INTERP_IDX_OVR_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_PC_INTERP_IDX_OVR_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_PC_INTERP_IDX_OVR_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_INTERP_IDX_OVR_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_INTERP_IDX_OVR_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_INTERP_IDX_OVR_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_INTERP_IDX_OVR_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000410 bits 25:21) field PC_INTERP_IDX_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
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
        /* (0x00000410 bits 25:21) field PC_INTERP_IDX_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
        t8_40n_reg_CONFIG_2_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_PC_INTERP_IDX_OVR_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_PC_INTERP_IDX_OVR_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_INTERP_IDX_OVR_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_INTERP_IDX_OVR_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_INTERP_IDX_OVR_get", stop_bit, 4 );
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
    /* (0x00000410 bits 25:21) field PC_INTERP_IDX_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_MSK, MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_INTERP_IDX_OVR_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_PC_INTERP_IDX_OVR_EN_set( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_PC_INTERP_IDX_OVR_EN_set( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_INTERP_IDX_OVR_EN_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_PC_INTERP_IDX_OVR_EN_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_PC_INTERP_IDX_OVR_EN_set", id, value );

    /* (0x00000410 bits 20) field PC_INTERP_IDX_OVR_EN of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    t8_40n_reg_CONFIG_2_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_EN_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_EN_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_PC_INTERP_IDX_OVR_EN_get( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_PC_INTERP_IDX_OVR_EN_get( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_INTERP_IDX_OVR_EN_get", id, 2);
    /* (0x00000410 bits 20) field PC_INTERP_IDX_OVR_EN of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_EN_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_PC_INTERP_IDX_OVR_EN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_PC_INTERP_IDX_OVR_EN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_LF_INTEG_OUT_MODE_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_INTEG_OUT_MODE_set( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEG_OUT_MODE_set", id, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_INTEG_OUT_MODE_set", value, 3);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_INTEG_OUT_MODE_set", id, value );

    /* (0x00000410 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    t8_40n_reg_CONFIG_2_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_INTEG_OUT_MODE_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_INTEG_OUT_MODE_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEG_OUT_MODE_get", id, 2);
    /* (0x00000410 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_INTEG_OUT_MODE_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_LF_INTEG_OUT_MODE_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_LF_INTEG_OUT_MODE_set( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_INTEG_OUT_MODE_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_INTEG_OUT_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_INTEG_OUT_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_INTEG_OUT_MODE_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000410 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
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
        /* (0x00000410 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
        t8_40n_reg_CONFIG_2_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_LF_INTEG_OUT_MODE_get( t8_40n_buffer_t *b_ptr,
                                                               t8_handle_t *h_ptr,
                                                               UINT32 id,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_LF_INTEG_OUT_MODE_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_INTEG_OUT_MODE_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_INTEG_OUT_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_INTEG_OUT_MODE_get", stop_bit, 1 );
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
    /* (0x00000410 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_MSK, MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_INTEG_OUT_MODE_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_LF_INTEG_OUT_OVR_set( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_INTEG_OUT_OVR_set( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEG_OUT_OVR_set", id, 2);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_INTEG_OUT_OVR_set", value, 255);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_INTEG_OUT_OVR_set", id, value );

    /* (0x00000410 bits 7:0) field LF_INTEG_OUT_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    t8_40n_reg_CONFIG_2_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_MSK,
                                              MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_INTEG_OUT_OVR_get( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_INTEG_OUT_OVR_get( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEG_OUT_OVR_get", id, 2);
    /* (0x00000410 bits 7:0) field LF_INTEG_OUT_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_MSK) >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_INTEG_OUT_OVR_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_LF_INTEG_OUT_OVR_set( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_LF_INTEG_OUT_OVR_set( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_INTEG_OUT_OVR_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_INTEG_OUT_OVR_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_INTEG_OUT_OVR_set", stop_bit, 7 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_INTEG_OUT_OVR_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000410 bits 7:0) field LF_INTEG_OUT_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
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
        /* (0x00000410 bits 7:0) field LF_INTEG_OUT_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
        t8_40n_reg_CONFIG_2_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_LF_INTEG_OUT_OVR_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_LF_INTEG_OUT_OVR_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_INTEG_OUT_OVR_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_INTEG_OUT_OVR_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_INTEG_OUT_OVR_get", stop_bit, 7 );
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
    /* (0x00000410 bits 7:0) field LF_INTEG_OUT_OVR of register PMC_MDSP_T8_40N_REG_CONFIG_2_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_2_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_MSK, MDSP_T8_40N_REG_CONFIG_2_LW20_80_36_BIT_LF_INTEG_OUT_OVR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_INTEG_OUT_OVR_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_PC_NEG_OFFSET_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_PC_NEG_OFFSET_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_NEG_OFFSET_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_PC_NEG_OFFSET_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_PC_NEG_OFFSET_set", id, value );

    /* (0x00000414 bits 29:24) field PC_NEG_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    t8_40n_reg_CONFIG_3_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_MSK,
                                              MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_PC_NEG_OFFSET_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_PC_NEG_OFFSET_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_NEG_OFFSET_get", id, 2);
    /* (0x00000414 bits 29:24) field PC_NEG_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_3_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_MSK) >> MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_PC_NEG_OFFSET_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_PC_NEG_OFFSET_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_PC_NEG_OFFSET_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_NEG_OFFSET_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_NEG_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_NEG_OFFSET_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_NEG_OFFSET_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000414 bits 29:24) field PC_NEG_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
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
        /* (0x00000414 bits 29:24) field PC_NEG_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
        t8_40n_reg_CONFIG_3_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_PC_NEG_OFFSET_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_PC_NEG_OFFSET_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_NEG_OFFSET_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_NEG_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_NEG_OFFSET_get", stop_bit, 5 );
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
    /* (0x00000414 bits 29:24) field PC_NEG_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_3_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_MSK, MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_NEG_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_NEG_OFFSET_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_PC_POS_OFFSET_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_PC_POS_OFFSET_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_POS_OFFSET_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_PC_POS_OFFSET_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_PC_POS_OFFSET_set", id, value );

    /* (0x00000414 bits 23:18) field PC_POS_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    t8_40n_reg_CONFIG_3_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_MSK,
                                              MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_PC_POS_OFFSET_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_PC_POS_OFFSET_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_POS_OFFSET_get", id, 2);
    /* (0x00000414 bits 23:18) field PC_POS_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_3_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_MSK) >> MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_PC_POS_OFFSET_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_PC_POS_OFFSET_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_PC_POS_OFFSET_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_POS_OFFSET_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_POS_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_POS_OFFSET_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_POS_OFFSET_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000414 bits 23:18) field PC_POS_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
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
        /* (0x00000414 bits 23:18) field PC_POS_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
        t8_40n_reg_CONFIG_3_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_PC_POS_OFFSET_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_PC_POS_OFFSET_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_POS_OFFSET_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_POS_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_POS_OFFSET_get", stop_bit, 5 );
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
    /* (0x00000414 bits 23:18) field PC_POS_OFFSET of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_3_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_MSK, MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_PC_POS_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_POS_OFFSET_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_LF_CLAMP_MAX_set( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_CLAMP_MAX_set( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_CLAMP_MAX_set", id, 2);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_CLAMP_MAX_set", value, 255);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_CLAMP_MAX_set", id, value );

    /* (0x00000414 bits 7:0) field LF_CLAMP_MAX of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    t8_40n_reg_CONFIG_3_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_MSK,
                                              MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_CLAMP_MAX_get( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_CLAMP_MAX_get( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_CLAMP_MAX_get", id, 2);
    /* (0x00000414 bits 7:0) field LF_CLAMP_MAX of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_3_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_MSK) >> MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_CLAMP_MAX_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_LF_CLAMP_MAX_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_LF_CLAMP_MAX_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_CLAMP_MAX_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_CLAMP_MAX_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_CLAMP_MAX_set", stop_bit, 7 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_CLAMP_MAX_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000414 bits 7:0) field LF_CLAMP_MAX of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
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
        /* (0x00000414 bits 7:0) field LF_CLAMP_MAX of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
        t8_40n_reg_CONFIG_3_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_LF_CLAMP_MAX_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_LF_CLAMP_MAX_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_CLAMP_MAX_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_CLAMP_MAX_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_CLAMP_MAX_get", stop_bit, 7 );
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
    /* (0x00000414 bits 7:0) field LF_CLAMP_MAX of register PMC_MDSP_T8_40N_REG_CONFIG_3_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_3_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_MSK, MDSP_T8_40N_REG_CONFIG_3_LW20_80_36_BIT_LF_CLAMP_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_CLAMP_MAX_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_PC_TH_DROP_set( t8_40n_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_PC_TH_DROP_set( t8_40n_buffer_t *b_ptr,
                                                t8_handle_t *h_ptr,
                                                UINT32 id,
                                                UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_TH_DROP_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_PC_TH_DROP_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_PC_TH_DROP_set", id, value );

    /* (0x00000418 bits 27:23) field PC_TH_DROP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    t8_40n_reg_CONFIG_4_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_MSK,
                                              MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_PC_TH_DROP_get( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_PC_TH_DROP_get( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_TH_DROP_get", id, 2);
    /* (0x00000418 bits 27:23) field PC_TH_DROP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_4_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_MSK) >> MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_PC_TH_DROP_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_PC_TH_DROP_set( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_PC_TH_DROP_set( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_TH_DROP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_TH_DROP_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_TH_DROP_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_TH_DROP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000418 bits 27:23) field PC_TH_DROP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
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
        /* (0x00000418 bits 27:23) field PC_TH_DROP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
        t8_40n_reg_CONFIG_4_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_PC_TH_DROP_get( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_PC_TH_DROP_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_TH_DROP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_TH_DROP_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_TH_DROP_get", stop_bit, 4 );
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
    /* (0x00000418 bits 27:23) field PC_TH_DROP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_4_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_MSK, MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_DROP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_TH_DROP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_PC_TH_ADD_set( t8_40n_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_PC_TH_ADD_set( t8_40n_buffer_t *b_ptr,
                                               t8_handle_t *h_ptr,
                                               UINT32 id,
                                               UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_TH_ADD_set", id, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_PC_TH_ADD_set", value, 31);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_PC_TH_ADD_set", id, value );

    /* (0x00000418 bits 22:18) field PC_TH_ADD of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    t8_40n_reg_CONFIG_4_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_MSK,
                                              MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_PC_TH_ADD_get( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_PC_TH_ADD_get( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_TH_ADD_get", id, 2);
    /* (0x00000418 bits 22:18) field PC_TH_ADD of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_4_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_MSK) >> MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_PC_TH_ADD_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_PC_TH_ADD_set( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_PC_TH_ADD_set( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_TH_ADD_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_TH_ADD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_TH_ADD_set", stop_bit, 4 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_TH_ADD_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000418 bits 22:18) field PC_TH_ADD of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
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
        /* (0x00000418 bits 22:18) field PC_TH_ADD of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
        t8_40n_reg_CONFIG_4_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_PC_TH_ADD_get( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_PC_TH_ADD_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_TH_ADD_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_TH_ADD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_TH_ADD_get", stop_bit, 4 );
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
    /* (0x00000418 bits 22:18) field PC_TH_ADD of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_4_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_MSK, MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_PC_TH_ADD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_TH_ADD_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_EQ_TARGET_AMP_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_EQ_TARGET_AMP_set( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id,
                                                   UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_EQ_TARGET_AMP_set", id, 2);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_EQ_TARGET_AMP_set", value, 63);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_EQ_TARGET_AMP_set", id, value );

    /* (0x00000418 bits 17:12) field EQ_TARGET_AMP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    t8_40n_reg_CONFIG_4_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_MSK,
                                              MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_EQ_TARGET_AMP_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_EQ_TARGET_AMP_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_EQ_TARGET_AMP_get", id, 2);
    /* (0x00000418 bits 17:12) field EQ_TARGET_AMP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_4_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_MSK) >> MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_EQ_TARGET_AMP_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_EQ_TARGET_AMP_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_EQ_TARGET_AMP_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_EQ_TARGET_AMP_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_EQ_TARGET_AMP_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_EQ_TARGET_AMP_set", stop_bit, 5 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_EQ_TARGET_AMP_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000418 bits 17:12) field EQ_TARGET_AMP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
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
        /* (0x00000418 bits 17:12) field EQ_TARGET_AMP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
        t8_40n_reg_CONFIG_4_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_EQ_TARGET_AMP_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_EQ_TARGET_AMP_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_EQ_TARGET_AMP_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_EQ_TARGET_AMP_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_EQ_TARGET_AMP_get", stop_bit, 5 );
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
    /* (0x00000418 bits 17:12) field EQ_TARGET_AMP of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_4_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_MSK, MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_EQ_TARGET_AMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_EQ_TARGET_AMP_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_LF_CLAMP_MIN_set( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_LF_CLAMP_MIN_set( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id,
                                                  UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_CLAMP_MIN_set", id, 2);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_LF_CLAMP_MIN_set", value, 255);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_LF_CLAMP_MIN_set", id, value );

    /* (0x00000418 bits 7:0) field LF_CLAMP_MIN of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    t8_40n_reg_CONFIG_4_LW20_80_36_field_set( b_ptr,
                                              h_ptr,
                                              id,
                                              MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_MSK,
                                              MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_OFF,
                                              value);
}

static INLINE UINT32 t8_40n_field_LF_CLAMP_MIN_get( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_CLAMP_MIN_get( t8_40n_buffer_t *b_ptr,
                                                    t8_handle_t *h_ptr,
                                                    UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_CLAMP_MIN_get", id, 2);
    /* (0x00000418 bits 7:0) field LF_CLAMP_MIN of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_4_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_MSK) >> MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_CLAMP_MIN_get", id, value );

    return value;
}
static INLINE void t8_40n_field_range_LF_CLAMP_MIN_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_range_LF_CLAMP_MIN_set( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_CLAMP_MIN_set", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_CLAMP_MIN_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_CLAMP_MIN_set", stop_bit, 7 );
    IOLOG( "%s <= id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_CLAMP_MIN_set", id, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000418 bits 7:0) field LF_CLAMP_MIN of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
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
        /* (0x00000418 bits 7:0) field LF_CLAMP_MIN of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
        t8_40n_reg_CONFIG_4_LW20_80_36_field_set( b_ptr,
                                                  h_ptr,
                                                  id,
                                                  subfield_mask << (MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_OFF + subfield_offset),
                                                  MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 t8_40n_field_range_LF_CLAMP_MIN_get( t8_40n_buffer_t *b_ptr,
                                                          t8_handle_t *h_ptr,
                                                          UINT32 id,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_LF_CLAMP_MIN_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_CLAMP_MIN_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_CLAMP_MIN_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_CLAMP_MIN_get", stop_bit, 7 );
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
    /* (0x00000418 bits 7:0) field LF_CLAMP_MIN of register PMC_MDSP_T8_40N_REG_CONFIG_4_LW20_80_36 */
    reg_value = t8_40n_reg_CONFIG_4_LW20_80_36_read( b_ptr,
                                                     h_ptr,
                                                     id);
    field_value = (reg_value & MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_MSK)
                  >> MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_MSK, MDSP_T8_40N_REG_CONFIG_4_LW20_80_36_BIT_LF_CLAMP_MIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_CLAMP_MIN_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE void t8_40n_field_ADD_DROP_UPDATE_REQ_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_ADD_DROP_UPDATE_REQ_set( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_UPDATE_REQ_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_ADD_DROP_UPDATE_REQ_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_ADD_DROP_UPDATE_REQ_set", id, value );

    /* (0x00000478 bits 31) field ADD_DROP_UPDATE_REQ of register PMC_MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT */
    t8_40n_reg_ADD_DROP_BLOCKCNT_field_set( b_ptr,
                                            h_ptr,
                                            id,
                                            MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_UPDATE_REQ_MSK,
                                            MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_UPDATE_REQ_OFF,
                                            value);
}

static INLINE UINT32 t8_40n_field_ADD_DROP_UPDATE_REQ_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_UPDATE_REQ_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_UPDATE_REQ_get", id, 2);
    /* (0x00000478 bits 31) field ADD_DROP_UPDATE_REQ of register PMC_MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT */
    reg_value = t8_40n_reg_ADD_DROP_BLOCKCNT_read( b_ptr,
                                                   h_ptr,
                                                   id);
    value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_UPDATE_REQ_MSK) >> MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_UPDATE_REQ_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_UPDATE_REQ_get", id, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void t8_40n_field_CAPTURE_REQ_set( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void t8_40n_field_CAPTURE_REQ_set( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id,
                                                 UINT32 value )
{
    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_CAPTURE_REQ_set", id, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "t8_40n_field_CAPTURE_REQ_set", value, 1);
    IOLOG( "%s <= id=%d 0x%08x", "t8_40n_field_CAPTURE_REQ_set", id, value );

    /* (0x00000408 bits 14) field CAPTURE_REQ of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set( b_ptr,
                                                                                 h_ptr,
                                                                                 id,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_CAPTURE_REQ_MSK,
                                                                                 MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_CAPTURE_REQ_OFF,
                                                                                 value);
}

static INLINE UINT32 t8_40n_field_CAPTURE_REQ_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_CAPTURE_REQ_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_CAPTURE_REQ_get", id, 2);
    /* (0x00000408 bits 14) field CAPTURE_REQ of register PMC_MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB */
    reg_value = t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read( b_ptr,
                                                                                        h_ptr,
                                                                                        id);
    value = (reg_value & MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_CAPTURE_REQ_MSK) >> MDSP_T8_40N_REG_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_BIT_CAPTURE_REQ_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_CAPTURE_REQ_get", id, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_4_get", id, 2);
    /* (0x0000041c bits 29:24) field A_INTERP_DATAIN_SMP_4 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_4_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_4_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_4_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_4_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_4_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_4_get", stop_bit, 5 );
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
    /* (0x0000041c bits 29:24) field A_INTERP_DATAIN_SMP_4 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_4_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_4_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_4_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_3_get", id, 2);
    /* (0x0000041c bits 23:18) field A_INTERP_DATAIN_SMP_3 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_3_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_3_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_3_get", stop_bit, 5 );
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
    /* (0x0000041c bits 23:18) field A_INTERP_DATAIN_SMP_3 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_3_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_3_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_2_get", id, 2);
    /* (0x0000041c bits 17:12) field A_INTERP_DATAIN_SMP_2 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_2_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_2_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_2_get", stop_bit, 5 );
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
    /* (0x0000041c bits 17:12) field A_INTERP_DATAIN_SMP_2 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_2_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_2_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_1_get", id, 2);
    /* (0x0000041c bits 11:6) field A_INTERP_DATAIN_SMP_1 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_1_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_1_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_1_get", stop_bit, 5 );
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
    /* (0x0000041c bits 11:6) field A_INTERP_DATAIN_SMP_1 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_1_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_1_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_0_get", id, 2);
    /* (0x0000041c bits 5:0) field A_INTERP_DATAIN_SMP_0 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_0_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_0_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_0_get", stop_bit, 5 );
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
    /* (0x0000041c bits 5:0) field A_INTERP_DATAIN_SMP_0 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_0_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_0_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_0_TO_5_BIT_A_INTERP_DATAIN_SMP_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_9_get", id, 2);
    /* (0x00000420 bits 29:24) field A_INTERP_DATAIN_SMP_9 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_9_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_9_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_9_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_9_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_9_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_9_get", stop_bit, 5 );
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
    /* (0x00000420 bits 29:24) field A_INTERP_DATAIN_SMP_9 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_9_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_9_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_9_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_8_get", id, 2);
    /* (0x00000420 bits 23:18) field A_INTERP_DATAIN_SMP_8 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_8_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_8_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_8_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_8_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_8_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_8_get", stop_bit, 5 );
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
    /* (0x00000420 bits 23:18) field A_INTERP_DATAIN_SMP_8 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_8_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_8_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_8_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_7_get", id, 2);
    /* (0x00000420 bits 17:12) field A_INTERP_DATAIN_SMP_7 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_7_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_7_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_7_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_7_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_7_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_7_get", stop_bit, 5 );
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
    /* (0x00000420 bits 17:12) field A_INTERP_DATAIN_SMP_7 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_7_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_7_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_7_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_6_get", id, 2);
    /* (0x00000420 bits 11:6) field A_INTERP_DATAIN_SMP_6 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_6_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_6_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_6_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_6_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_6_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_6_get", stop_bit, 5 );
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
    /* (0x00000420 bits 11:6) field A_INTERP_DATAIN_SMP_6 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_6_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_6_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_6_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_5_get", id, 2);
    /* (0x00000420 bits 5:0) field A_INTERP_DATAIN_SMP_5 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_5_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_5_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_5_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_5_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_5_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_5_get", stop_bit, 5 );
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
    /* (0x00000420 bits 5:0) field A_INTERP_DATAIN_SMP_5 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_5_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_5_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_5_TO_9_BIT_A_INTERP_DATAIN_SMP_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_5_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_14_get", id, 2);
    /* (0x00000424 bits 29:24) field A_INTERP_DATAIN_SMP_14 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_14_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_14_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_14_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_14_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_14_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_14_get", stop_bit, 5 );
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
    /* (0x00000424 bits 29:24) field A_INTERP_DATAIN_SMP_14 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_14_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_14_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_14_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_14_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_14_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_13_get", id, 2);
    /* (0x00000424 bits 23:18) field A_INTERP_DATAIN_SMP_13 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_13_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_13_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_13_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_13_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_13_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_13_get", stop_bit, 5 );
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
    /* (0x00000424 bits 23:18) field A_INTERP_DATAIN_SMP_13 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_13_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_13_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_13_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_13_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_13_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_12_get", id, 2);
    /* (0x00000424 bits 17:12) field A_INTERP_DATAIN_SMP_12 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_12_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_12_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_12_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_12_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_12_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_12_get", stop_bit, 5 );
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
    /* (0x00000424 bits 17:12) field A_INTERP_DATAIN_SMP_12 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_12_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_12_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_12_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_12_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_12_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_11_get", id, 2);
    /* (0x00000424 bits 11:6) field A_INTERP_DATAIN_SMP_11 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_11_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_11_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_11_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_11_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_11_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_11_get", stop_bit, 5 );
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
    /* (0x00000424 bits 11:6) field A_INTERP_DATAIN_SMP_11 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_11_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_11_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_11_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_10_get", id, 2);
    /* (0x00000424 bits 5:0) field A_INTERP_DATAIN_SMP_10 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_10_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_10_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_10_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_10_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_10_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_10_get", stop_bit, 5 );
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
    /* (0x00000424 bits 5:0) field A_INTERP_DATAIN_SMP_10 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_10_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_10_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_10_TO_37_BIT_A_INTERP_DATAIN_SMP_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_10_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_19_get", id, 2);
    /* (0x00000428 bits 29:24) field A_INTERP_DATAIN_SMP_19 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_19_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_19_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_19_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_19_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_19_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_19_get", stop_bit, 5 );
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
    /* (0x00000428 bits 29:24) field A_INTERP_DATAIN_SMP_19 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_19_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_19_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_19_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_19_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_19_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_18_get", id, 2);
    /* (0x00000428 bits 23:18) field A_INTERP_DATAIN_SMP_18 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_18_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_18_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_18_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_18_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_18_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_18_get", stop_bit, 5 );
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
    /* (0x00000428 bits 23:18) field A_INTERP_DATAIN_SMP_18 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_18_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_18_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_18_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_18_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_18_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_17_get", id, 2);
    /* (0x00000428 bits 17:12) field A_INTERP_DATAIN_SMP_17 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_17_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_17_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_17_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_17_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_17_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_17_get", stop_bit, 5 );
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
    /* (0x00000428 bits 17:12) field A_INTERP_DATAIN_SMP_17 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_17_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_17_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_17_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_17_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_17_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_16_get", id, 2);
    /* (0x00000428 bits 11:6) field A_INTERP_DATAIN_SMP_16 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_16_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_16_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_16_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_16_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_16_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_16_get", stop_bit, 5 );
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
    /* (0x00000428 bits 11:6) field A_INTERP_DATAIN_SMP_16 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_16_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_16_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_16_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_15_get", id, 2);
    /* (0x00000428 bits 5:0) field A_INTERP_DATAIN_SMP_15 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_15_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_15_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_15_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_15_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_15_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_15_get", stop_bit, 5 );
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
    /* (0x00000428 bits 5:0) field A_INTERP_DATAIN_SMP_15 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_15_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_15_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_15_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_15_TO_19_BIT_A_INTERP_DATAIN_SMP_15_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_15_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_23_get", id, 2);
    /* (0x0000042c bits 23:18) field A_INTERP_DATAIN_SMP_23 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_23_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_23_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_23_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_23_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_23_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_23_get", stop_bit, 5 );
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
    /* (0x0000042c bits 23:18) field A_INTERP_DATAIN_SMP_23 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_23_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_23_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_23_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_23_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_23_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_22_get", id, 2);
    /* (0x0000042c bits 17:12) field A_INTERP_DATAIN_SMP_22 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_22_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_22_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_22_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_22_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_22_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_22_get", stop_bit, 5 );
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
    /* (0x0000042c bits 17:12) field A_INTERP_DATAIN_SMP_22 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_22_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_22_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_22_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_22_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_22_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_21_get", id, 2);
    /* (0x0000042c bits 11:6) field A_INTERP_DATAIN_SMP_21 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_21_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_21_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_21_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_21_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_21_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_21_get", stop_bit, 5 );
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
    /* (0x0000042c bits 11:6) field A_INTERP_DATAIN_SMP_21 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_21_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_21_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_21_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_21_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_21_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_A_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_A_INTERP_DATAIN_SMP_20_get", id, 2);
    /* (0x0000042c bits 5:0) field A_INTERP_DATAIN_SMP_20 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_20_MSK) >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_20_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_A_INTERP_DATAIN_SMP_20_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_A_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_20_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_20_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_A_INTERP_DATAIN_SMP_20_get", stop_bit, 5 );
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
    /* (0x0000042c bits 5:0) field A_INTERP_DATAIN_SMP_20 of register PMC_MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_20_MSK)
                  >> MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_20_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_20_MSK, MDSP_T8_40N_REG_A_INTERP_DATAIN_SMP_20_TO_23_BIT_A_INTERP_DATAIN_SMP_20_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_A_INTERP_DATAIN_SMP_20_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_4_get", id, 2);
    /* (0x00000430 bits 29:24) field B_INTERP_DATAIN_SMP_4 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_4_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_4_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_4_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_4_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_4_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_4_get", stop_bit, 5 );
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
    /* (0x00000430 bits 29:24) field B_INTERP_DATAIN_SMP_4 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_4_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_4_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_4_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_3_get", id, 2);
    /* (0x00000430 bits 23:18) field B_INTERP_DATAIN_SMP_3 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_3_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_3_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_3_get", stop_bit, 5 );
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
    /* (0x00000430 bits 23:18) field B_INTERP_DATAIN_SMP_3 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_3_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_3_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_2_get", id, 2);
    /* (0x00000430 bits 17:12) field B_INTERP_DATAIN_SMP_2 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_2_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_2_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_2_get", stop_bit, 5 );
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
    /* (0x00000430 bits 17:12) field B_INTERP_DATAIN_SMP_2 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_2_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_2_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_1_get", id, 2);
    /* (0x00000430 bits 11:6) field B_INTERP_DATAIN_SMP_1 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_1_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_1_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_1_get", stop_bit, 5 );
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
    /* (0x00000430 bits 11:6) field B_INTERP_DATAIN_SMP_1 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_1_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_1_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_0_get", id, 2);
    /* (0x00000430 bits 5:0) field B_INTERP_DATAIN_SMP_0 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_0_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_0_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_0_get", stop_bit, 5 );
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
    /* (0x00000430 bits 5:0) field B_INTERP_DATAIN_SMP_0 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_0_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_0_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_0_TO_5_BIT_B_INTERP_DATAIN_SMP_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_9_get", id, 2);
    /* (0x00000434 bits 29:24) field B_INTERP_DATAIN_SMP_9 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_9_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_9_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_9_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_9_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_9_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_9_get", stop_bit, 5 );
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
    /* (0x00000434 bits 29:24) field B_INTERP_DATAIN_SMP_9 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_9_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_9_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_9_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_8_get", id, 2);
    /* (0x00000434 bits 23:18) field B_INTERP_DATAIN_SMP_8 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_8_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_8_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_8_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_8_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_8_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_8_get", stop_bit, 5 );
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
    /* (0x00000434 bits 23:18) field B_INTERP_DATAIN_SMP_8 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_8_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_8_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_8_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_7_get", id, 2);
    /* (0x00000434 bits 17:12) field B_INTERP_DATAIN_SMP_7 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_7_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_7_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_7_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_7_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_7_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_7_get", stop_bit, 5 );
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
    /* (0x00000434 bits 17:12) field B_INTERP_DATAIN_SMP_7 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_7_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_7_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_7_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_6_get", id, 2);
    /* (0x00000434 bits 11:6) field B_INTERP_DATAIN_SMP_6 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_6_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_6_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_6_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_6_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_6_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_6_get", stop_bit, 5 );
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
    /* (0x00000434 bits 11:6) field B_INTERP_DATAIN_SMP_6 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_6_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_6_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_6_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_5_get", id, 2);
    /* (0x00000434 bits 5:0) field B_INTERP_DATAIN_SMP_5 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_5_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_5_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_5_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_5_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_5_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_5_get", stop_bit, 5 );
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
    /* (0x00000434 bits 5:0) field B_INTERP_DATAIN_SMP_5 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_5_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_5_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_5_TO_9_BIT_B_INTERP_DATAIN_SMP_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_5_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_14_get", id, 2);
    /* (0x00000438 bits 29:24) field B_INTERP_DATAIN_SMP_14 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_14_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_14_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_14_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_14_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_14_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_14_get", stop_bit, 5 );
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
    /* (0x00000438 bits 29:24) field B_INTERP_DATAIN_SMP_14 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_14_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_14_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_14_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_14_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_14_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_13_get", id, 2);
    /* (0x00000438 bits 23:18) field B_INTERP_DATAIN_SMP_13 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_13_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_13_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_13_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_13_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_13_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_13_get", stop_bit, 5 );
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
    /* (0x00000438 bits 23:18) field B_INTERP_DATAIN_SMP_13 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_13_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_13_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_13_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_13_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_13_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_12_get", id, 2);
    /* (0x00000438 bits 17:12) field B_INTERP_DATAIN_SMP_12 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_12_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_12_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_12_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_12_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_12_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_12_get", stop_bit, 5 );
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
    /* (0x00000438 bits 17:12) field B_INTERP_DATAIN_SMP_12 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_12_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_12_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_12_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_12_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_12_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_11_get", id, 2);
    /* (0x00000438 bits 11:6) field B_INTERP_DATAIN_SMP_11 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_11_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_11_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_11_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_11_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_11_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_11_get", stop_bit, 5 );
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
    /* (0x00000438 bits 11:6) field B_INTERP_DATAIN_SMP_11 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_11_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_11_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_11_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_10_get", id, 2);
    /* (0x00000438 bits 5:0) field B_INTERP_DATAIN_SMP_10 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_10_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_10_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_10_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_10_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_10_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_10_get", stop_bit, 5 );
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
    /* (0x00000438 bits 5:0) field B_INTERP_DATAIN_SMP_10 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_10_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_10_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_10_TO_37_BIT_B_INTERP_DATAIN_SMP_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_10_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_19_get", id, 2);
    /* (0x0000043c bits 29:24) field B_INTERP_DATAIN_SMP_19 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_19_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_19_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_19_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_19_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_19_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_19_get", stop_bit, 5 );
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
    /* (0x0000043c bits 29:24) field B_INTERP_DATAIN_SMP_19 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_19_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_19_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_19_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_19_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_19_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_18_get", id, 2);
    /* (0x0000043c bits 23:18) field B_INTERP_DATAIN_SMP_18 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_18_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_18_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_18_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_18_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_18_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_18_get", stop_bit, 5 );
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
    /* (0x0000043c bits 23:18) field B_INTERP_DATAIN_SMP_18 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_18_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_18_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_18_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_18_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_18_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_17_get", id, 2);
    /* (0x0000043c bits 17:12) field B_INTERP_DATAIN_SMP_17 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_17_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_17_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_17_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_17_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_17_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_17_get", stop_bit, 5 );
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
    /* (0x0000043c bits 17:12) field B_INTERP_DATAIN_SMP_17 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_17_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_17_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_17_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_17_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_17_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_16_get", id, 2);
    /* (0x0000043c bits 11:6) field B_INTERP_DATAIN_SMP_16 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_16_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_16_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_16_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_16_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_16_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_16_get", stop_bit, 5 );
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
    /* (0x0000043c bits 11:6) field B_INTERP_DATAIN_SMP_16 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_16_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_16_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_16_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_15_get", id, 2);
    /* (0x0000043c bits 5:0) field B_INTERP_DATAIN_SMP_15 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_15_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_15_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_15_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_15_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_15_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_15_get", stop_bit, 5 );
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
    /* (0x0000043c bits 5:0) field B_INTERP_DATAIN_SMP_15 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_15_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_15_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_15_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_15_TO_19_BIT_B_INTERP_DATAIN_SMP_15_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_15_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_23_get", id, 2);
    /* (0x00000440 bits 23:18) field B_INTERP_DATAIN_SMP_23 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_23_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_23_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_23_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_23_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_23_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_23_get", stop_bit, 5 );
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
    /* (0x00000440 bits 23:18) field B_INTERP_DATAIN_SMP_23 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_23_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_23_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_23_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_23_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_23_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_22_get", id, 2);
    /* (0x00000440 bits 17:12) field B_INTERP_DATAIN_SMP_22 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_22_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_22_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_22_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_22_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_22_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_22_get", stop_bit, 5 );
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
    /* (0x00000440 bits 17:12) field B_INTERP_DATAIN_SMP_22 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_22_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_22_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_22_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_22_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_22_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_21_get", id, 2);
    /* (0x00000440 bits 11:6) field B_INTERP_DATAIN_SMP_21 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_21_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_21_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_21_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_21_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_21_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_21_get", stop_bit, 5 );
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
    /* (0x00000440 bits 11:6) field B_INTERP_DATAIN_SMP_21 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_21_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_21_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_21_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_21_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_21_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_B_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_B_INTERP_DATAIN_SMP_20_get", id, 2);
    /* (0x00000440 bits 5:0) field B_INTERP_DATAIN_SMP_20 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_20_MSK) >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_20_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_B_INTERP_DATAIN_SMP_20_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_B_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_20_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_20_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_B_INTERP_DATAIN_SMP_20_get", stop_bit, 5 );
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
    /* (0x00000440 bits 5:0) field B_INTERP_DATAIN_SMP_20 of register PMC_MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_20_MSK)
                  >> MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_20_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_20_MSK, MDSP_T8_40N_REG_B_INTERP_DATAIN_SMP_20_TO_23_BIT_B_INTERP_DATAIN_SMP_20_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_B_INTERP_DATAIN_SMP_20_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_4_get", id, 2);
    /* (0x00000444 bits 29:24) field C_INTERP_DATAIN_SMP_4 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_4_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_4_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_4_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_4_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_4_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_4_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_4_get", stop_bit, 5 );
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
    /* (0x00000444 bits 29:24) field C_INTERP_DATAIN_SMP_4 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_4_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_4_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_4_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_3_get", id, 2);
    /* (0x00000444 bits 23:18) field C_INTERP_DATAIN_SMP_3 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_3_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_3_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_3_get", stop_bit, 5 );
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
    /* (0x00000444 bits 23:18) field C_INTERP_DATAIN_SMP_3 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_3_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_3_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_2_get", id, 2);
    /* (0x00000444 bits 17:12) field C_INTERP_DATAIN_SMP_2 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_2_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_2_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_2_get", stop_bit, 5 );
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
    /* (0x00000444 bits 17:12) field C_INTERP_DATAIN_SMP_2 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_2_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_2_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_1_get", id, 2);
    /* (0x00000444 bits 11:6) field C_INTERP_DATAIN_SMP_1 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_1_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_1_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_1_get", stop_bit, 5 );
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
    /* (0x00000444 bits 11:6) field C_INTERP_DATAIN_SMP_1 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_1_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_1_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_0_get", id, 2);
    /* (0x00000444 bits 5:0) field C_INTERP_DATAIN_SMP_0 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_0_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_0_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_0_get", stop_bit, 5 );
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
    /* (0x00000444 bits 5:0) field C_INTERP_DATAIN_SMP_0 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_0_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_0_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_0_TO_5_BIT_C_INTERP_DATAIN_SMP_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_9_get", id, 2);
    /* (0x00000448 bits 29:24) field C_INTERP_DATAIN_SMP_9 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_9_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_9_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_9_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_9_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_9_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_9_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_9_get", stop_bit, 5 );
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
    /* (0x00000448 bits 29:24) field C_INTERP_DATAIN_SMP_9 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_9_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_9_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_9_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_8_get", id, 2);
    /* (0x00000448 bits 23:18) field C_INTERP_DATAIN_SMP_8 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_8_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_8_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_8_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_8_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_8_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_8_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_8_get", stop_bit, 5 );
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
    /* (0x00000448 bits 23:18) field C_INTERP_DATAIN_SMP_8 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_8_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_8_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_8_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_7_get", id, 2);
    /* (0x00000448 bits 17:12) field C_INTERP_DATAIN_SMP_7 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_7_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_7_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_7_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_7_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_7_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_7_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_7_get", stop_bit, 5 );
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
    /* (0x00000448 bits 17:12) field C_INTERP_DATAIN_SMP_7 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_7_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_7_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_7_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_6_get", id, 2);
    /* (0x00000448 bits 11:6) field C_INTERP_DATAIN_SMP_6 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_6_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_6_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_6_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_6_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_6_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_6_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_6_get", stop_bit, 5 );
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
    /* (0x00000448 bits 11:6) field C_INTERP_DATAIN_SMP_6 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_6_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_6_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_6_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_5_get", id, 2);
    /* (0x00000448 bits 5:0) field C_INTERP_DATAIN_SMP_5 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_5_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_5_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_5_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
                                                                   t8_handle_t *h_ptr,
                                                                   UINT32 id,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_5_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_5_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_5_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_5_get", stop_bit, 5 );
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
    /* (0x00000448 bits 5:0) field C_INTERP_DATAIN_SMP_5 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read( b_ptr,
                                                            h_ptr,
                                                            id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_5_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_5_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_5_TO_9_BIT_C_INTERP_DATAIN_SMP_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_5_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_14_get", id, 2);
    /* (0x0000044c bits 29:24) field C_INTERP_DATAIN_SMP_14 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_14_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_14_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_14_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_14_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_14_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_14_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_14_get", stop_bit, 5 );
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
    /* (0x0000044c bits 29:24) field C_INTERP_DATAIN_SMP_14 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_14_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_14_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_14_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_14_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_14_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_13_get", id, 2);
    /* (0x0000044c bits 23:18) field C_INTERP_DATAIN_SMP_13 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_13_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_13_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_13_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_13_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_13_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_13_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_13_get", stop_bit, 5 );
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
    /* (0x0000044c bits 23:18) field C_INTERP_DATAIN_SMP_13 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_13_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_13_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_13_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_13_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_13_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_12_get", id, 2);
    /* (0x0000044c bits 17:12) field C_INTERP_DATAIN_SMP_12 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_12_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_12_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_12_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_12_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_12_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_12_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_12_get", stop_bit, 5 );
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
    /* (0x0000044c bits 17:12) field C_INTERP_DATAIN_SMP_12 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_12_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_12_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_12_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_12_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_12_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_11_get", id, 2);
    /* (0x0000044c bits 11:6) field C_INTERP_DATAIN_SMP_11 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_11_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_11_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_11_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_11_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_11_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_11_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_11_get", stop_bit, 5 );
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
    /* (0x0000044c bits 11:6) field C_INTERP_DATAIN_SMP_11 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_11_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_11_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_11_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_10_get", id, 2);
    /* (0x0000044c bits 5:0) field C_INTERP_DATAIN_SMP_10 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_10_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_10_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_10_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_10_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_10_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_10_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_10_get", stop_bit, 5 );
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
    /* (0x0000044c bits 5:0) field C_INTERP_DATAIN_SMP_10 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_10_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_10_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_10_TO_37_BIT_C_INTERP_DATAIN_SMP_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_10_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_19_get", id, 2);
    /* (0x00000450 bits 29:24) field C_INTERP_DATAIN_SMP_19 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_19_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_19_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_19_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_19_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_19_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_19_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_19_get", stop_bit, 5 );
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
    /* (0x00000450 bits 29:24) field C_INTERP_DATAIN_SMP_19 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_19_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_19_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_19_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_19_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_19_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_18_get", id, 2);
    /* (0x00000450 bits 23:18) field C_INTERP_DATAIN_SMP_18 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_18_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_18_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_18_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_18_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_18_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_18_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_18_get", stop_bit, 5 );
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
    /* (0x00000450 bits 23:18) field C_INTERP_DATAIN_SMP_18 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_18_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_18_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_18_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_18_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_18_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_17_get", id, 2);
    /* (0x00000450 bits 17:12) field C_INTERP_DATAIN_SMP_17 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_17_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_17_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_17_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_17_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_17_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_17_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_17_get", stop_bit, 5 );
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
    /* (0x00000450 bits 17:12) field C_INTERP_DATAIN_SMP_17 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_17_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_17_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_17_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_17_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_17_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_16_get", id, 2);
    /* (0x00000450 bits 11:6) field C_INTERP_DATAIN_SMP_16 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_16_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_16_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_16_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_16_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_16_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_16_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_16_get", stop_bit, 5 );
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
    /* (0x00000450 bits 11:6) field C_INTERP_DATAIN_SMP_16 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_16_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_16_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_16_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_15_get", id, 2);
    /* (0x00000450 bits 5:0) field C_INTERP_DATAIN_SMP_15 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_15_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_15_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_15_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_15_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_15_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_15_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_15_get", stop_bit, 5 );
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
    /* (0x00000450 bits 5:0) field C_INTERP_DATAIN_SMP_15 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_15_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_15_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_15_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_15_TO_19_BIT_C_INTERP_DATAIN_SMP_15_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_15_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_23_get", id, 2);
    /* (0x00000454 bits 23:18) field C_INTERP_DATAIN_SMP_23 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_23_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_23_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_23_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_23_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_23_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_23_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_23_get", stop_bit, 5 );
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
    /* (0x00000454 bits 23:18) field C_INTERP_DATAIN_SMP_23 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_23_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_23_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_23_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_23_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_23_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_22_get", id, 2);
    /* (0x00000454 bits 17:12) field C_INTERP_DATAIN_SMP_22 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_22_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_22_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_22_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_22_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_22_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_22_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_22_get", stop_bit, 5 );
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
    /* (0x00000454 bits 17:12) field C_INTERP_DATAIN_SMP_22 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_22_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_22_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_22_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_22_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_22_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_21_get", id, 2);
    /* (0x00000454 bits 11:6) field C_INTERP_DATAIN_SMP_21 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_21_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_21_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_21_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_21_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_21_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_21_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_21_get", stop_bit, 5 );
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
    /* (0x00000454 bits 11:6) field C_INTERP_DATAIN_SMP_21 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_21_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_21_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_21_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_21_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_21_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_C_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                              t8_handle_t *h_ptr,
                                                              UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_C_INTERP_DATAIN_SMP_20_get", id, 2);
    /* (0x00000454 bits 5:0) field C_INTERP_DATAIN_SMP_20 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_20_MSK) >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_20_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_C_INTERP_DATAIN_SMP_20_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
                                                                    t8_handle_t *h_ptr,
                                                                    UINT32 id,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_C_INTERP_DATAIN_SMP_20_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_20_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_20_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_C_INTERP_DATAIN_SMP_20_get", stop_bit, 5 );
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
    /* (0x00000454 bits 5:0) field C_INTERP_DATAIN_SMP_20 of register PMC_MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23 */
    reg_value = t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read( b_ptr,
                                                              h_ptr,
                                                              id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_20_MSK)
                  >> MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_20_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_20_MSK, MDSP_T8_40N_REG_C_INTERP_DATAIN_SMP_20_TO_23_BIT_C_INTERP_DATAIN_SMP_20_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_C_INTERP_DATAIN_SMP_20_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_A3_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_A3_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_A3_get", id, 2);
    /* (0x00000458 bits 27:26) field ADD_DROP_A3 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A3_MSK) >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_A3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_A3_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_A3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_A3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_A3_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_A3_get", stop_bit, 1 );
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
    /* (0x00000458 bits 27:26) field ADD_DROP_A3 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A3_MSK)
                  >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A3_MSK, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_A3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_A2_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_A2_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_A2_get", id, 2);
    /* (0x00000458 bits 25:24) field ADD_DROP_A2 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A2_MSK) >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_A2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_A2_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_A2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_A2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_A2_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_A2_get", stop_bit, 1 );
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
    /* (0x00000458 bits 25:24) field ADD_DROP_A2 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A2_MSK)
                  >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A2_MSK, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_A2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_A1_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_A1_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_A1_get", id, 2);
    /* (0x00000458 bits 23:22) field ADD_DROP_A1 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A1_MSK) >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_A1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_A1_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_A1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_A1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_A1_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_A1_get", stop_bit, 1 );
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
    /* (0x00000458 bits 23:22) field ADD_DROP_A1 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A1_MSK)
                  >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A1_MSK, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_A1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_A0_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_A0_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_A0_get", id, 2);
    /* (0x00000458 bits 21:20) field ADD_DROP_A0 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A0_MSK) >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_A0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_A0_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_A0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_A0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_A0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_A0_get", stop_bit, 1 );
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
    /* (0x00000458 bits 21:20) field ADD_DROP_A0 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A0_MSK)
                  >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A0_MSK, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_ADD_DROP_A0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_A0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_A3_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_A3_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_A3_get", id, 2);
    /* (0x00000458 bits 19:15) field INTERP_IDX_A3 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A3_MSK) >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_A3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_A3_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_A3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_A3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_A3_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_A3_get", stop_bit, 4 );
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
    /* (0x00000458 bits 19:15) field INTERP_IDX_A3 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A3_MSK)
                  >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A3_MSK, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_A3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_A2_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_A2_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_A2_get", id, 2);
    /* (0x00000458 bits 14:10) field INTERP_IDX_A2 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A2_MSK) >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_A2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_A2_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_A2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_A2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_A2_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_A2_get", stop_bit, 4 );
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
    /* (0x00000458 bits 14:10) field INTERP_IDX_A2 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A2_MSK)
                  >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A2_MSK, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_A2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_A1_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_A1_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_A1_get", id, 2);
    /* (0x00000458 bits 9:5) field INTERP_IDX_A1 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A1_MSK) >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_A1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_A1_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_A1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_A1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_A1_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_A1_get", stop_bit, 4 );
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
    /* (0x00000458 bits 9:5) field INTERP_IDX_A1 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A1_MSK)
                  >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A1_MSK, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_A1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_A0_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_A0_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_A0_get", id, 2);
    /* (0x00000458 bits 4:0) field INTERP_IDX_A0 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A0_MSK) >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_A0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_A0_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_A0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_A0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_A0_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_A0_get", stop_bit, 4 );
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
    /* (0x00000458 bits 4:0) field INTERP_IDX_A0 of register PMC_MDSP_T8_40N_REG_A_TIMING_CTRL */
    reg_value = t8_40n_reg_A_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A0_MSK)
                  >> MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A0_MSK, MDSP_T8_40N_REG_A_TIMING_CTRL_BIT_INTERP_IDX_A0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_A0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_B3_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_B3_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_B3_get", id, 2);
    /* (0x0000045c bits 27:26) field ADD_DROP_B3 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B3_MSK) >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_B3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_B3_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_B3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_B3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_B3_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_B3_get", stop_bit, 1 );
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
    /* (0x0000045c bits 27:26) field ADD_DROP_B3 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B3_MSK)
                  >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B3_MSK, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_B3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_B2_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_B2_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_B2_get", id, 2);
    /* (0x0000045c bits 25:24) field ADD_DROP_B2 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B2_MSK) >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_B2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_B2_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_B2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_B2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_B2_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_B2_get", stop_bit, 1 );
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
    /* (0x0000045c bits 25:24) field ADD_DROP_B2 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B2_MSK)
                  >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B2_MSK, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_B2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_B1_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_B1_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_B1_get", id, 2);
    /* (0x0000045c bits 23:22) field ADD_DROP_B1 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B1_MSK) >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_B1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_B1_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_B1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_B1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_B1_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_B1_get", stop_bit, 1 );
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
    /* (0x0000045c bits 23:22) field ADD_DROP_B1 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B1_MSK)
                  >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B1_MSK, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_B1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_B0_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_B0_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_B0_get", id, 2);
    /* (0x0000045c bits 21:20) field ADD_DROP_B0 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B0_MSK) >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_B0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_B0_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_B0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_B0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_B0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_B0_get", stop_bit, 1 );
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
    /* (0x0000045c bits 21:20) field ADD_DROP_B0 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B0_MSK)
                  >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B0_MSK, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_ADD_DROP_B0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_B0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_B3_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_B3_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_B3_get", id, 2);
    /* (0x0000045c bits 19:15) field INTERP_IDX_B3 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B3_MSK) >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_B3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_B3_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_B3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_B3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_B3_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_B3_get", stop_bit, 4 );
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
    /* (0x0000045c bits 19:15) field INTERP_IDX_B3 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B3_MSK)
                  >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B3_MSK, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_B3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_B2_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_B2_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_B2_get", id, 2);
    /* (0x0000045c bits 14:10) field INTERP_IDX_B2 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B2_MSK) >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_B2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_B2_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_B2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_B2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_B2_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_B2_get", stop_bit, 4 );
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
    /* (0x0000045c bits 14:10) field INTERP_IDX_B2 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B2_MSK)
                  >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B2_MSK, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_B2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_B1_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_B1_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_B1_get", id, 2);
    /* (0x0000045c bits 9:5) field INTERP_IDX_B1 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B1_MSK) >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_B1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_B1_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_B1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_B1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_B1_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_B1_get", stop_bit, 4 );
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
    /* (0x0000045c bits 9:5) field INTERP_IDX_B1 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B1_MSK)
                  >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B1_MSK, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_B1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_B0_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_B0_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_B0_get", id, 2);
    /* (0x0000045c bits 4:0) field INTERP_IDX_B0 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B0_MSK) >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_B0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_B0_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_B0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_B0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_B0_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_B0_get", stop_bit, 4 );
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
    /* (0x0000045c bits 4:0) field INTERP_IDX_B0 of register PMC_MDSP_T8_40N_REG_B_TIMING_CTRL */
    reg_value = t8_40n_reg_B_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B0_MSK)
                  >> MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B0_MSK, MDSP_T8_40N_REG_B_TIMING_CTRL_BIT_INTERP_IDX_B0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_B0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_C3_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_C3_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_C3_get", id, 2);
    /* (0x00000460 bits 27:26) field ADD_DROP_C3 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C3_MSK) >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_C3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_C3_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_C3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_C3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_C3_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_C3_get", stop_bit, 1 );
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
    /* (0x00000460 bits 27:26) field ADD_DROP_C3 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C3_MSK)
                  >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C3_MSK, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_C3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_C2_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_C2_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_C2_get", id, 2);
    /* (0x00000460 bits 25:24) field ADD_DROP_C2 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C2_MSK) >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_C2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_C2_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_C2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_C2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_C2_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_C2_get", stop_bit, 1 );
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
    /* (0x00000460 bits 25:24) field ADD_DROP_C2 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C2_MSK)
                  >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C2_MSK, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_C2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_C1_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_C1_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_C1_get", id, 2);
    /* (0x00000460 bits 23:22) field ADD_DROP_C1 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C1_MSK) >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_C1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_C1_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_C1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_C1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_C1_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_C1_get", stop_bit, 1 );
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
    /* (0x00000460 bits 23:22) field ADD_DROP_C1 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C1_MSK)
                  >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C1_MSK, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_C1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_C0_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_C0_get( t8_40n_buffer_t *b_ptr,
                                                   t8_handle_t *h_ptr,
                                                   UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_C0_get", id, 2);
    /* (0x00000460 bits 21:20) field ADD_DROP_C0 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C0_MSK) >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_C0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_C0_get( t8_40n_buffer_t *b_ptr,
                                                         t8_handle_t *h_ptr,
                                                         UINT32 id,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_C0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_C0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_C0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_C0_get", stop_bit, 1 );
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
    /* (0x00000460 bits 21:20) field ADD_DROP_C0 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C0_MSK)
                  >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C0_MSK, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_ADD_DROP_C0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_C0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_C3_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_C3_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_C3_get", id, 2);
    /* (0x00000460 bits 19:15) field INTERP_IDX_C3 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C3_MSK) >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C3_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_C3_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_C3_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_C3_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_C3_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_C3_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_C3_get", stop_bit, 4 );
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
    /* (0x00000460 bits 19:15) field INTERP_IDX_C3 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C3_MSK)
                  >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C3_MSK, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_C3_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_C2_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_C2_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_C2_get", id, 2);
    /* (0x00000460 bits 14:10) field INTERP_IDX_C2 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C2_MSK) >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C2_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_C2_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_C2_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_C2_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_C2_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_C2_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_C2_get", stop_bit, 4 );
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
    /* (0x00000460 bits 14:10) field INTERP_IDX_C2 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C2_MSK)
                  >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C2_MSK, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_C2_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_C1_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_C1_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_C1_get", id, 2);
    /* (0x00000460 bits 9:5) field INTERP_IDX_C1 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C1_MSK) >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C1_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_C1_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_C1_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_C1_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_C1_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_C1_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_C1_get", stop_bit, 4 );
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
    /* (0x00000460 bits 9:5) field INTERP_IDX_C1 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C1_MSK)
                  >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C1_MSK, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_C1_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_INTERP_IDX_C0_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_INTERP_IDX_C0_get( t8_40n_buffer_t *b_ptr,
                                                     t8_handle_t *h_ptr,
                                                     UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_INTERP_IDX_C0_get", id, 2);
    /* (0x00000460 bits 4:0) field INTERP_IDX_C0 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C0_MSK) >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C0_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_INTERP_IDX_C0_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_C0_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_INTERP_IDX_C0_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_INTERP_IDX_C0_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_INTERP_IDX_C0_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_INTERP_IDX_C0_get", stop_bit, 4 );
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
    /* (0x00000460 bits 4:0) field INTERP_IDX_C0 of register PMC_MDSP_T8_40N_REG_C_TIMING_CTRL */
    reg_value = t8_40n_reg_C_TIMING_CTRL_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C0_MSK)
                  >> MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C0_MSK, MDSP_T8_40N_REG_C_TIMING_CTRL_BIT_INTERP_IDX_C0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_INTERP_IDX_C0_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_DFE_STATE_CAPT_get( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_DFE_STATE_CAPT_get( t8_40n_buffer_t *b_ptr,
                                                      t8_handle_t *h_ptr,
                                                      UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_DFE_STATE_CAPT_get", id, 2);
    /* (0x00000464 bits 4:0) field DFE_STATE_CAPT of register PMC_MDSP_T8_40N_REG_DFE_STATE */
    reg_value = t8_40n_reg_DFE_STATE_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_DFE_STATE_BIT_DFE_STATE_CAPT_MSK) >> MDSP_T8_40N_REG_DFE_STATE_BIT_DFE_STATE_CAPT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_DFE_STATE_CAPT_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_DFE_STATE_CAPT_get( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_DFE_STATE_CAPT_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_DFE_STATE_CAPT_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_DFE_STATE_CAPT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_DFE_STATE_CAPT_get", stop_bit, 4 );
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
    /* (0x00000464 bits 4:0) field DFE_STATE_CAPT of register PMC_MDSP_T8_40N_REG_DFE_STATE */
    reg_value = t8_40n_reg_DFE_STATE_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_DFE_STATE_BIT_DFE_STATE_CAPT_MSK)
                  >> MDSP_T8_40N_REG_DFE_STATE_BIT_DFE_STATE_CAPT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_DFE_STATE_BIT_DFE_STATE_CAPT_MSK, MDSP_T8_40N_REG_DFE_STATE_BIT_DFE_STATE_CAPT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_DFE_STATE_CAPT_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_PC_INTEGRAL_ACC_get( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_PC_INTEGRAL_ACC_get( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_PC_INTEGRAL_ACC_get", id, 2);
    /* (0x00000468 bits 29:18) field PC_INTEGRAL_ACC of register PMC_MDSP_T8_40N_REG_TCB_STATE */
    reg_value = t8_40n_reg_TCB_STATE_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_TCB_STATE_BIT_PC_INTEGRAL_ACC_MSK) >> MDSP_T8_40N_REG_TCB_STATE_BIT_PC_INTEGRAL_ACC_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_PC_INTEGRAL_ACC_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_PC_INTEGRAL_ACC_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_PC_INTEGRAL_ACC_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_PC_INTEGRAL_ACC_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_PC_INTEGRAL_ACC_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_PC_INTEGRAL_ACC_get", stop_bit, 11 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 11) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 11;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000468 bits 29:18) field PC_INTEGRAL_ACC of register PMC_MDSP_T8_40N_REG_TCB_STATE */
    reg_value = t8_40n_reg_TCB_STATE_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_TCB_STATE_BIT_PC_INTEGRAL_ACC_MSK)
                  >> MDSP_T8_40N_REG_TCB_STATE_BIT_PC_INTEGRAL_ACC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_TCB_STATE_BIT_PC_INTEGRAL_ACC_MSK, MDSP_T8_40N_REG_TCB_STATE_BIT_PC_INTEGRAL_ACC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_PC_INTEGRAL_ACC_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_LF_INTEGRAL_ACC_get( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_LF_INTEGRAL_ACC_get( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_LF_INTEGRAL_ACC_get", id, 2);
    /* (0x00000468 bits 13:0) field LF_INTEGRAL_ACC of register PMC_MDSP_T8_40N_REG_TCB_STATE */
    reg_value = t8_40n_reg_TCB_STATE_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_TCB_STATE_BIT_LF_INTEGRAL_ACC_MSK) >> MDSP_T8_40N_REG_TCB_STATE_BIT_LF_INTEGRAL_ACC_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_LF_INTEGRAL_ACC_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_LF_INTEGRAL_ACC_get( t8_40n_buffer_t *b_ptr,
                                                             t8_handle_t *h_ptr,
                                                             UINT32 id,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_LF_INTEGRAL_ACC_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_LF_INTEGRAL_ACC_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_LF_INTEGRAL_ACC_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_LF_INTEGRAL_ACC_get", stop_bit, 13 );
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
    /* (0x00000468 bits 13:0) field LF_INTEGRAL_ACC of register PMC_MDSP_T8_40N_REG_TCB_STATE */
    reg_value = t8_40n_reg_TCB_STATE_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_TCB_STATE_BIT_LF_INTEGRAL_ACC_MSK)
                  >> MDSP_T8_40N_REG_TCB_STATE_BIT_LF_INTEGRAL_ACC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_TCB_STATE_BIT_LF_INTEGRAL_ACC_MSK, MDSP_T8_40N_REG_TCB_STATE_BIT_LF_INTEGRAL_ACC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_LF_INTEGRAL_ACC_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                 t8_handle_t *h_ptr,
                                                 UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_COUNT_get", id, 2);
    /* (0x0000046c bits 19:0) field ADD_COUNT of register PMC_MDSP_T8_40N_REG_ADD_DROP_ADDCNT */
    reg_value = t8_40n_reg_ADD_DROP_ADDCNT_read(  b_ptr, h_ptr, id);
    value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_ADDCNT_BIT_ADD_COUNT_MSK) >> MDSP_T8_40N_REG_ADD_DROP_ADDCNT_BIT_ADD_COUNT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_COUNT_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                       t8_handle_t *h_ptr,
                                                       UINT32 id,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_COUNT_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_COUNT_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_COUNT_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000046c bits 19:0) field ADD_COUNT of register PMC_MDSP_T8_40N_REG_ADD_DROP_ADDCNT */
    reg_value = t8_40n_reg_ADD_DROP_ADDCNT_read(  b_ptr, h_ptr, id);
    field_value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_ADDCNT_BIT_ADD_COUNT_MSK)
                  >> MDSP_T8_40N_REG_ADD_DROP_ADDCNT_BIT_ADD_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_ADD_DROP_ADDCNT_BIT_ADD_COUNT_MSK, MDSP_T8_40N_REG_ADD_DROP_ADDCNT_BIT_ADD_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_COUNT_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_DROP_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_DROP_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                  t8_handle_t *h_ptr,
                                                  UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_DROP_COUNT_get", id, 2);
    /* (0x00000470 bits 19:0) field DROP_COUNT of register PMC_MDSP_T8_40N_REG_ADD_DROP_DROPCNT */
    reg_value = t8_40n_reg_ADD_DROP_DROPCNT_read( b_ptr,
                                                  h_ptr,
                                                  id);
    value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_DROPCNT_BIT_DROP_COUNT_MSK) >> MDSP_T8_40N_REG_ADD_DROP_DROPCNT_BIT_DROP_COUNT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_DROP_COUNT_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_DROP_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                        t8_handle_t *h_ptr,
                                                        UINT32 id,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_DROP_COUNT_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_DROP_COUNT_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_DROP_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_DROP_COUNT_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000470 bits 19:0) field DROP_COUNT of register PMC_MDSP_T8_40N_REG_ADD_DROP_DROPCNT */
    reg_value = t8_40n_reg_ADD_DROP_DROPCNT_read( b_ptr,
                                                  h_ptr,
                                                  id);
    field_value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_DROPCNT_BIT_DROP_COUNT_MSK)
                  >> MDSP_T8_40N_REG_ADD_DROP_DROPCNT_BIT_DROP_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_ADD_DROP_DROPCNT_BIT_DROP_COUNT_MSK, MDSP_T8_40N_REG_ADD_DROP_DROPCNT_BIT_DROP_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_DROP_COUNT_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_DIFF_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_DIFF_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                           t8_handle_t *h_ptr,
                                                           UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_DIFF_COUNT_get", id, 2);
    /* (0x00000474 bits 19:0) field ADD_DROP_DIFF_COUNT of register PMC_MDSP_T8_40N_REG_ADD_DROP_DIFFCNT */
    reg_value = t8_40n_reg_ADD_DROP_DIFFCNT_read( b_ptr,
                                                  h_ptr,
                                                  id);
    value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_DIFFCNT_BIT_ADD_DROP_DIFF_COUNT_MSK) >> MDSP_T8_40N_REG_ADD_DROP_DIFFCNT_BIT_ADD_DROP_DIFF_COUNT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_DIFF_COUNT_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_DIFF_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                                 t8_handle_t *h_ptr,
                                                                 UINT32 id,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_DIFF_COUNT_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_DIFF_COUNT_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_DIFF_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_DIFF_COUNT_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000474 bits 19:0) field ADD_DROP_DIFF_COUNT of register PMC_MDSP_T8_40N_REG_ADD_DROP_DIFFCNT */
    reg_value = t8_40n_reg_ADD_DROP_DIFFCNT_read( b_ptr,
                                                  h_ptr,
                                                  id);
    field_value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_DIFFCNT_BIT_ADD_DROP_DIFF_COUNT_MSK)
                  >> MDSP_T8_40N_REG_ADD_DROP_DIFFCNT_BIT_ADD_DROP_DIFF_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_ADD_DROP_DIFFCNT_BIT_ADD_DROP_DIFF_COUNT_MSK, MDSP_T8_40N_REG_ADD_DROP_DIFFCNT_BIT_ADD_DROP_DIFF_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_DIFF_COUNT_get", id, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 t8_40n_field_ADD_DROP_BLOCK_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_ADD_DROP_BLOCK_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                            t8_handle_t *h_ptr,
                                                            UINT32 id )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (id > 2)
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_ADD_DROP_BLOCK_COUNT_get", id, 2);
    /* (0x00000478 bits 19:0) field ADD_DROP_BLOCK_COUNT of register PMC_MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT */
    reg_value = t8_40n_reg_ADD_DROP_BLOCKCNT_read( b_ptr,
                                                   h_ptr,
                                                   id);
    value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_BLOCK_COUNT_MSK) >> MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_BLOCK_COUNT_OFF;
    IOLOG( "%s -> id=%d 0x%08x", "t8_40n_field_ADD_DROP_BLOCK_COUNT_get", id, value );

    return value;
}
static INLINE UINT32 t8_40n_field_range_ADD_DROP_BLOCK_COUNT_get( t8_40n_buffer_t *b_ptr,
                                                                  t8_handle_t *h_ptr,
                                                                  UINT32 id,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 t8_40n_field_range_ADD_DROP_BLOCK_COUNT_get( t8_40n_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s id is %d but max is %d", "t8_40n_field_range_ADD_DROP_BLOCK_COUNT_get", id, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "t8_40n_field_range_ADD_DROP_BLOCK_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "t8_40n_field_range_ADD_DROP_BLOCK_COUNT_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000478 bits 19:0) field ADD_DROP_BLOCK_COUNT of register PMC_MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT */
    reg_value = t8_40n_reg_ADD_DROP_BLOCKCNT_read( b_ptr,
                                                   h_ptr,
                                                   id);
    field_value = (reg_value & MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_BLOCK_COUNT_MSK)
                  >> MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_BLOCK_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_BLOCK_COUNT_MSK, MDSP_T8_40N_REG_ADD_DROP_BLOCKCNT_BIT_ADD_DROP_BLOCK_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> id=%d start_bit=%d stop_bit=%d 0x%08x", "t8_40n_field_range_ADD_DROP_BLOCK_COUNT_get", id, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _T8_40N_IO_INLINE_H */
