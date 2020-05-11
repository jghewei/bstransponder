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
 *     and register accessor functions for the s16_titan_br2_mdsp block
 *****************************************************************************/
#ifndef _S16_TITAN_BR2_MDSP_IO_INLINE_H
#define _S16_TITAN_BR2_MDSP_IO_INLINE_H

#include "s16_loc.h"
#include "s16_titan_br2_mdsp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S16_TITAN_BR2_MDSP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for s16_titan_br2_mdsp
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
    s16_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} s16_titan_br2_mdsp_buffer_t;
static INLINE void s16_titan_br2_mdsp_buffer_init( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                   s16_handle_t *h_ptr,
                                                   UINT32 A ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_buffer_init( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                   s16_handle_t *h_ptr,
                                                   UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "s16_titan_br2_mdsp_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void s16_titan_br2_mdsp_buffer_flush( s16_titan_br2_mdsp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_buffer_flush( s16_titan_br2_mdsp_buffer_t *b_ptr )
{
    IOLOG( "s16_titan_br2_mdsp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 s16_titan_br2_mdsp_reg_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                  s16_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mem_type,
                                                  UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                  s16_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mem_type,
                                                  UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x1000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg);
    return value;
}
static INLINE void s16_titan_br2_mdsp_reg_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                 s16_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                 s16_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value);
}

static INLINE void s16_titan_br2_mdsp_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                 s16_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 unused_mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                 s16_handle_t *h_ptr,
                                                 UINT32 A,
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
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void s16_titan_br2_mdsp_action_on_write_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mem_type,
                                                                 UINT32 reg,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_action_on_write_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mem_type,
                                                                 UINT32 reg,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value<<ofs);
}

static INLINE void s16_titan_br2_mdsp_burst_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                  s16_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 len,
                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_burst_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                  s16_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 len,
                                                  UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                          len,
                          value);
}

static INLINE void s16_titan_br2_mdsp_burst_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                   s16_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 len,
                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_burst_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                   s16_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 len,
                                                   UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE s16_titan_br2_mdsp_poll( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mem_type,
                                                            UINT32 reg,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_titan_br2_mdsp_poll( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
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
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for s16_titan_br2_mdsp
 * ==================================================================================
 */

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                                              s16_handle_t *h_ptr,
                                                                                                              UINT32 A,
                                                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                                              s16_handle_t *h_ptr,
                                                                                                              UINT32 A,
                                                                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                                                  s16_handle_t *h_ptr,
                                                                                                                  UINT32 A,
                                                                                                                  UINT32 mask,
                                                                                                                  UINT32 ofs,
                                                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                                                  s16_handle_t *h_ptr,
                                                                                                                  UINT32 A,
                                                                                                                  UINT32 mask,
                                                                                                                  UINT32 ofs,
                                                                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                                               s16_handle_t *h_ptr,
                                                                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                                               s16_handle_t *h_ptr,
                                                                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_titan_br2_mdsp_reg_DIAG_CONTROL_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_DIAG_CONTROL_write( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_titan_br2_mdsp_reg_DIAG_CONTROL_write", value );
    s16_titan_br2_mdsp_reg_write( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL,
                                  value);
}

static INLINE void s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set", A, mask, ofs, value );
    s16_titan_br2_mdsp_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL,
                                  mask,
                                  PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL_UNUSED_MASK,
                                  ofs,
                                  value);

}

static INLINE UINT32 s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_CONFIG,
                                             PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_DIAG_CONTROL_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_STATUS,
                                             PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_titan_br2_mdsp_reg_DFE_STATE_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_DFE_STATE_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_STATUS,
                                             PMC_TITAN_BR2_DIGI_REG_DFE_STATE);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_DFE_STATE_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_titan_br2_mdsp_reg_TCB_STATE_0_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_reg_TCB_STATE_0_read( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_titan_br2_mdsp_reg_read( b_ptr,
                                             h_ptr,
                                             A,
                                             MEM_TYPE_STATUS,
                                             PMC_TITAN_BR2_DIGI_REG_TCB_STATE_0);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_titan_br2_mdsp_reg_TCB_STATE_0_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_4_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_4_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_4_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_4_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_4_set", A, value );

    /* (0x00000200 bits 29:24) field ADC_D_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_4_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_4_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_4_get", A, 15);
    /* (0x00000200 bits 29:24) field ADC_D_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_4_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000200 bits 29:24) field ADC_D_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
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
        /* (0x00000200 bits 29:24) field ADC_D_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_get", stop_bit, 5 );
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
    /* (0x00000200 bits 29:24) field ADC_D_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_3_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_3_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_3_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_3_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_3_set", A, value );

    /* (0x00000200 bits 23:18) field ADC_D_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_3_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_3_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_3_get", A, 15);
    /* (0x00000200 bits 23:18) field ADC_D_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_3_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000200 bits 23:18) field ADC_D_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
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
        /* (0x00000200 bits 23:18) field ADC_D_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_get", stop_bit, 5 );
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
    /* (0x00000200 bits 23:18) field ADC_D_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_2_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_2_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_2_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_2_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_2_set", A, value );

    /* (0x00000200 bits 17:12) field ADC_D_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_2_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_2_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_2_get", A, 15);
    /* (0x00000200 bits 17:12) field ADC_D_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_2_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000200 bits 17:12) field ADC_D_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
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
        /* (0x00000200 bits 17:12) field ADC_D_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_get", stop_bit, 5 );
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
    /* (0x00000200 bits 17:12) field ADC_D_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_1_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_1_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_1_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_1_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_1_set", A, value );

    /* (0x00000200 bits 11:6) field ADC_D_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_1_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_1_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_1_get", A, 15);
    /* (0x00000200 bits 11:6) field ADC_D_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_1_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000200 bits 11:6) field ADC_D_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
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
        /* (0x00000200 bits 11:6) field ADC_D_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_get", stop_bit, 5 );
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
    /* (0x00000200 bits 11:6) field ADC_D_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_0_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_0_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_0_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_0_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_0_set", A, value );

    /* (0x00000200 bits 5:0) field ADC_D_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_0_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_0_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_0_get", A, 15);
    /* (0x00000200 bits 5:0) field ADC_D_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_0_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000200 bits 5:0) field ADC_D_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
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
        /* (0x00000200 bits 5:0) field ADC_D_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_get", stop_bit, 5 );
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
    /* (0x00000200 bits 5:0) field ADC_D_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_9_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_9_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_9_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_9_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_9_set", A, value );

    /* (0x00000204 bits 29:24) field ADC_D_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_9_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_9_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_9_get", A, 15);
    /* (0x00000204 bits 29:24) field ADC_D_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_9_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000204 bits 29:24) field ADC_D_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
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
        /* (0x00000204 bits 29:24) field ADC_D_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_get", stop_bit, 5 );
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
    /* (0x00000204 bits 29:24) field ADC_D_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_MSK, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_8_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_8_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_8_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_8_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_8_set", A, value );

    /* (0x00000204 bits 23:18) field ADC_D_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_8_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_8_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_8_get", A, 15);
    /* (0x00000204 bits 23:18) field ADC_D_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_8_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000204 bits 23:18) field ADC_D_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
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
        /* (0x00000204 bits 23:18) field ADC_D_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_get", stop_bit, 5 );
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
    /* (0x00000204 bits 23:18) field ADC_D_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_MSK, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_7_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_7_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_7_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_7_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_7_set", A, value );

    /* (0x00000204 bits 17:12) field ADC_D_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_7_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_7_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_7_get", A, 15);
    /* (0x00000204 bits 17:12) field ADC_D_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_7_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000204 bits 17:12) field ADC_D_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
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
        /* (0x00000204 bits 17:12) field ADC_D_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_get", stop_bit, 5 );
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
    /* (0x00000204 bits 17:12) field ADC_D_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_MSK, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_6_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_6_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_6_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_6_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_6_set", A, value );

    /* (0x00000204 bits 11:6) field ADC_D_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_6_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_6_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_6_get", A, 15);
    /* (0x00000204 bits 11:6) field ADC_D_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_6_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000204 bits 11:6) field ADC_D_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
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
        /* (0x00000204 bits 11:6) field ADC_D_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_get", stop_bit, 5 );
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
    /* (0x00000204 bits 11:6) field ADC_D_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_MSK, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_5_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_5_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_5_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_5_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_5_set", A, value );

    /* (0x00000204 bits 5:0) field ADC_D_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_5_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_5_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_5_get", A, 15);
    /* (0x00000204 bits 5:0) field ADC_D_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_5_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000204 bits 5:0) field ADC_D_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
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
        /* (0x00000204 bits 5:0) field ADC_D_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_get", stop_bit, 5 );
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
    /* (0x00000204 bits 5:0) field ADC_D_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_MSK, TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_14_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_14_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_14_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_14_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_14_set", A, value );

    /* (0x00000208 bits 29:24) field ADC_D_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_14_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_14_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_14_get", A, 15);
    /* (0x00000208 bits 29:24) field ADC_D_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_14_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000208 bits 29:24) field ADC_D_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
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
        /* (0x00000208 bits 29:24) field ADC_D_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_get", stop_bit, 5 );
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
    /* (0x00000208 bits 29:24) field ADC_D_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_MSK, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_13_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_13_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_13_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_13_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_13_set", A, value );

    /* (0x00000208 bits 23:18) field ADC_D_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_13_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_13_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_13_get", A, 15);
    /* (0x00000208 bits 23:18) field ADC_D_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_13_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000208 bits 23:18) field ADC_D_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
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
        /* (0x00000208 bits 23:18) field ADC_D_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_get", stop_bit, 5 );
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
    /* (0x00000208 bits 23:18) field ADC_D_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_MSK, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_12_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_12_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_12_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_12_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_12_set", A, value );

    /* (0x00000208 bits 17:12) field ADC_D_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_12_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_12_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_12_get", A, 15);
    /* (0x00000208 bits 17:12) field ADC_D_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_12_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000208 bits 17:12) field ADC_D_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
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
        /* (0x00000208 bits 17:12) field ADC_D_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_get", stop_bit, 5 );
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
    /* (0x00000208 bits 17:12) field ADC_D_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_MSK, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_11_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_11_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_11_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_11_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_11_set", A, value );

    /* (0x00000208 bits 11:6) field ADC_D_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_11_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_11_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_11_get", A, 15);
    /* (0x00000208 bits 11:6) field ADC_D_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_11_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000208 bits 11:6) field ADC_D_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
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
        /* (0x00000208 bits 11:6) field ADC_D_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_get", stop_bit, 5 );
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
    /* (0x00000208 bits 11:6) field ADC_D_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_MSK, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_10_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_10_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_10_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_10_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_10_set", A, value );

    /* (0x00000208 bits 5:0) field ADC_D_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_10_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_10_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_10_get", A, 15);
    /* (0x00000208 bits 5:0) field ADC_D_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_10_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000208 bits 5:0) field ADC_D_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
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
        /* (0x00000208 bits 5:0) field ADC_D_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_get", stop_bit, 5 );
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
    /* (0x00000208 bits 5:0) field ADC_D_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_MSK, TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_19_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_19_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_19_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_19_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_19_set", A, value );

    /* (0x0000020c bits 29:24) field ADC_D_MAP_19 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_19_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_19_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_19_get", A, 15);
    /* (0x0000020c bits 29:24) field ADC_D_MAP_19 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_19_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000020c bits 29:24) field ADC_D_MAP_19 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
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
        /* (0x0000020c bits 29:24) field ADC_D_MAP_19 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_get", stop_bit, 5 );
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
    /* (0x0000020c bits 29:24) field ADC_D_MAP_19 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_MSK, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_18_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_18_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_18_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_18_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_18_set", A, value );

    /* (0x0000020c bits 23:18) field ADC_D_MAP_18 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_18_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_18_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_18_get", A, 15);
    /* (0x0000020c bits 23:18) field ADC_D_MAP_18 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_18_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000020c bits 23:18) field ADC_D_MAP_18 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
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
        /* (0x0000020c bits 23:18) field ADC_D_MAP_18 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_get", stop_bit, 5 );
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
    /* (0x0000020c bits 23:18) field ADC_D_MAP_18 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_MSK, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_17_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_17_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_17_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_17_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_17_set", A, value );

    /* (0x0000020c bits 17:12) field ADC_D_MAP_17 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_17_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_17_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_17_get", A, 15);
    /* (0x0000020c bits 17:12) field ADC_D_MAP_17 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_17_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000020c bits 17:12) field ADC_D_MAP_17 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
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
        /* (0x0000020c bits 17:12) field ADC_D_MAP_17 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_get", stop_bit, 5 );
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
    /* (0x0000020c bits 17:12) field ADC_D_MAP_17 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_MSK, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_16_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_16_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_16_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_16_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_16_set", A, value );

    /* (0x0000020c bits 11:6) field ADC_D_MAP_16 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_16_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_16_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_16_get", A, 15);
    /* (0x0000020c bits 11:6) field ADC_D_MAP_16 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_16_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000020c bits 11:6) field ADC_D_MAP_16 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
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
        /* (0x0000020c bits 11:6) field ADC_D_MAP_16 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_get", stop_bit, 5 );
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
    /* (0x0000020c bits 11:6) field ADC_D_MAP_16 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_MSK, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_15_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_15_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_15_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_15_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_15_set", A, value );

    /* (0x0000020c bits 5:0) field ADC_D_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_15_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_15_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_15_get", A, 15);
    /* (0x0000020c bits 5:0) field ADC_D_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_15_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000020c bits 5:0) field ADC_D_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
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
        /* (0x0000020c bits 5:0) field ADC_D_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_get", stop_bit, 5 );
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
    /* (0x0000020c bits 5:0) field ADC_D_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_MSK, TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_24_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_24_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_24_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_24_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_24_set", A, value );

    /* (0x00000210 bits 29:24) field ADC_D_MAP_24 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_24_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_24_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_24_get", A, 15);
    /* (0x00000210 bits 29:24) field ADC_D_MAP_24 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_24_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000210 bits 29:24) field ADC_D_MAP_24 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
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
        /* (0x00000210 bits 29:24) field ADC_D_MAP_24 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_get", stop_bit, 5 );
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
    /* (0x00000210 bits 29:24) field ADC_D_MAP_24 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_MSK, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_23_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_23_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_23_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_23_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_23_set", A, value );

    /* (0x00000210 bits 23:18) field ADC_D_MAP_23 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_23_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_23_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_23_get", A, 15);
    /* (0x00000210 bits 23:18) field ADC_D_MAP_23 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_23_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000210 bits 23:18) field ADC_D_MAP_23 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
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
        /* (0x00000210 bits 23:18) field ADC_D_MAP_23 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_get", stop_bit, 5 );
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
    /* (0x00000210 bits 23:18) field ADC_D_MAP_23 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_MSK, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_22_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_22_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_22_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_22_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_22_set", A, value );

    /* (0x00000210 bits 17:12) field ADC_D_MAP_22 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_22_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_22_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_22_get", A, 15);
    /* (0x00000210 bits 17:12) field ADC_D_MAP_22 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_22_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000210 bits 17:12) field ADC_D_MAP_22 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
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
        /* (0x00000210 bits 17:12) field ADC_D_MAP_22 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_get", stop_bit, 5 );
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
    /* (0x00000210 bits 17:12) field ADC_D_MAP_22 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_MSK, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_21_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_21_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_21_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_21_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_21_set", A, value );

    /* (0x00000210 bits 11:6) field ADC_D_MAP_21 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_21_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_21_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_21_get", A, 15);
    /* (0x00000210 bits 11:6) field ADC_D_MAP_21 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_21_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000210 bits 11:6) field ADC_D_MAP_21 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
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
        /* (0x00000210 bits 11:6) field ADC_D_MAP_21 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_get", stop_bit, 5 );
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
    /* (0x00000210 bits 11:6) field ADC_D_MAP_21 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_MSK, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_20_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_20_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_20_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_20_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_20_set", A, value );

    /* (0x00000210 bits 5:0) field ADC_D_MAP_20 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_20_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_20_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_20_get", A, 15);
    /* (0x00000210 bits 5:0) field ADC_D_MAP_20 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_20_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000210 bits 5:0) field ADC_D_MAP_20 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
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
        /* (0x00000210 bits 5:0) field ADC_D_MAP_20 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_get", stop_bit, 5 );
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
    /* (0x00000210 bits 5:0) field ADC_D_MAP_20 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_MSK, TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_29_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_29_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_29_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_29_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_29_set", A, value );

    /* (0x00000214 bits 29:24) field ADC_D_MAP_29 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_29_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_29_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_29_get", A, 15);
    /* (0x00000214 bits 29:24) field ADC_D_MAP_29 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_29_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000214 bits 29:24) field ADC_D_MAP_29 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
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
        /* (0x00000214 bits 29:24) field ADC_D_MAP_29 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_get", stop_bit, 5 );
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
    /* (0x00000214 bits 29:24) field ADC_D_MAP_29 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_MSK, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_28_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_28_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_28_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_28_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_28_set", A, value );

    /* (0x00000214 bits 23:18) field ADC_D_MAP_28 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_28_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_28_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_28_get", A, 15);
    /* (0x00000214 bits 23:18) field ADC_D_MAP_28 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_28_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000214 bits 23:18) field ADC_D_MAP_28 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
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
        /* (0x00000214 bits 23:18) field ADC_D_MAP_28 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_get", stop_bit, 5 );
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
    /* (0x00000214 bits 23:18) field ADC_D_MAP_28 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_MSK, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_27_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_27_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_27_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_27_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_27_set", A, value );

    /* (0x00000214 bits 17:12) field ADC_D_MAP_27 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_27_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_27_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_27_get", A, 15);
    /* (0x00000214 bits 17:12) field ADC_D_MAP_27 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_27_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000214 bits 17:12) field ADC_D_MAP_27 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
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
        /* (0x00000214 bits 17:12) field ADC_D_MAP_27 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_get", stop_bit, 5 );
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
    /* (0x00000214 bits 17:12) field ADC_D_MAP_27 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_MSK, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_26_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_26_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_26_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_26_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_26_set", A, value );

    /* (0x00000214 bits 11:6) field ADC_D_MAP_26 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_26_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_26_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_26_get", A, 15);
    /* (0x00000214 bits 11:6) field ADC_D_MAP_26 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_26_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000214 bits 11:6) field ADC_D_MAP_26 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
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
        /* (0x00000214 bits 11:6) field ADC_D_MAP_26 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_get", stop_bit, 5 );
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
    /* (0x00000214 bits 11:6) field ADC_D_MAP_26 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_MSK, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_25_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_25_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_25_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_25_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_25_set", A, value );

    /* (0x00000214 bits 5:0) field ADC_D_MAP_25 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_25_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_25_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_25_get", A, 15);
    /* (0x00000214 bits 5:0) field ADC_D_MAP_25 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_25_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000214 bits 5:0) field ADC_D_MAP_25 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
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
        /* (0x00000214 bits 5:0) field ADC_D_MAP_25 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_get", stop_bit, 5 );
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
    /* (0x00000214 bits 5:0) field ADC_D_MAP_25 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_MSK, TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_31_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_31_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_31_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_31_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_31_set", A, value );

    /* (0x00000218 bits 11:6) field ADC_D_MAP_31 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_31_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_31_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_31_get", A, 15);
    /* (0x00000218 bits 11:6) field ADC_D_MAP_31 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_31_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000218 bits 11:6) field ADC_D_MAP_31 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
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
        /* (0x00000218 bits 11:6) field ADC_D_MAP_31 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_get", stop_bit, 5 );
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
    /* (0x00000218 bits 11:6) field ADC_D_MAP_31 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_MSK, TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_30_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_D_MAP_30_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_30_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_30_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_30_set", A, value );

    /* (0x00000218 bits 5:0) field ADC_D_MAP_30 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_30_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_30_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_D_MAP_30_get", A, 15);
    /* (0x00000218 bits 5:0) field ADC_D_MAP_30 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_D_MAP_30_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000218 bits 5:0) field ADC_D_MAP_30 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
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
        /* (0x00000218 bits 5:0) field ADC_D_MAP_30 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_get", stop_bit, 5 );
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
    /* (0x00000218 bits 5:0) field ADC_D_MAP_30 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_MSK, TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_4_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_4_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_4_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_4_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_4_set", A, value );

    /* (0x0000021c bits 29:24) field ADC_T_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_4_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_4_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_4_get", A, 15);
    /* (0x0000021c bits 29:24) field ADC_T_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_4_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000021c bits 29:24) field ADC_T_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
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
        /* (0x0000021c bits 29:24) field ADC_T_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_get", stop_bit, 5 );
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
    /* (0x0000021c bits 29:24) field ADC_T_MAP_4 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_MSK, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_3_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_3_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_3_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_3_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_3_set", A, value );

    /* (0x0000021c bits 23:18) field ADC_T_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_3_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_3_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_3_get", A, 15);
    /* (0x0000021c bits 23:18) field ADC_T_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_3_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000021c bits 23:18) field ADC_T_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
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
        /* (0x0000021c bits 23:18) field ADC_T_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_get", stop_bit, 5 );
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
    /* (0x0000021c bits 23:18) field ADC_T_MAP_3 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_MSK, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_2_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_2_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_2_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_2_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_2_set", A, value );

    /* (0x0000021c bits 17:12) field ADC_T_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_2_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_2_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_2_get", A, 15);
    /* (0x0000021c bits 17:12) field ADC_T_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_2_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000021c bits 17:12) field ADC_T_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
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
        /* (0x0000021c bits 17:12) field ADC_T_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_get", stop_bit, 5 );
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
    /* (0x0000021c bits 17:12) field ADC_T_MAP_2 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_MSK, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_1_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_1_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_1_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_1_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_1_set", A, value );

    /* (0x0000021c bits 11:6) field ADC_T_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_1_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_1_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_1_get", A, 15);
    /* (0x0000021c bits 11:6) field ADC_T_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_1_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000021c bits 11:6) field ADC_T_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
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
        /* (0x0000021c bits 11:6) field ADC_T_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_get", stop_bit, 5 );
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
    /* (0x0000021c bits 11:6) field ADC_T_MAP_1 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_MSK, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_0_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_0_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_0_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_0_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_0_set", A, value );

    /* (0x0000021c bits 5:0) field ADC_T_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_0_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_0_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_0_get", A, 15);
    /* (0x0000021c bits 5:0) field ADC_T_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_0_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000021c bits 5:0) field ADC_T_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
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
        /* (0x0000021c bits 5:0) field ADC_T_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_get", stop_bit, 5 );
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
    /* (0x0000021c bits 5:0) field ADC_T_MAP_0 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_MSK, TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_9_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_9_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_9_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_9_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_9_set", A, value );

    /* (0x00000220 bits 29:24) field ADC_T_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_9_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_9_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_9_get", A, 15);
    /* (0x00000220 bits 29:24) field ADC_T_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_9_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000220 bits 29:24) field ADC_T_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
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
        /* (0x00000220 bits 29:24) field ADC_T_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_get", stop_bit, 5 );
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
    /* (0x00000220 bits 29:24) field ADC_T_MAP_9 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_MSK, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_8_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_8_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_8_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_8_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_8_set", A, value );

    /* (0x00000220 bits 23:18) field ADC_T_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_8_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_8_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_8_get", A, 15);
    /* (0x00000220 bits 23:18) field ADC_T_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_8_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000220 bits 23:18) field ADC_T_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
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
        /* (0x00000220 bits 23:18) field ADC_T_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_get", stop_bit, 5 );
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
    /* (0x00000220 bits 23:18) field ADC_T_MAP_8 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_MSK, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_7_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_7_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_7_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_7_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_7_set", A, value );

    /* (0x00000220 bits 17:12) field ADC_T_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_7_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_7_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_7_get", A, 15);
    /* (0x00000220 bits 17:12) field ADC_T_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_7_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000220 bits 17:12) field ADC_T_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
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
        /* (0x00000220 bits 17:12) field ADC_T_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_get", stop_bit, 5 );
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
    /* (0x00000220 bits 17:12) field ADC_T_MAP_7 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_MSK, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_6_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_6_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_6_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_6_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_6_set", A, value );

    /* (0x00000220 bits 11:6) field ADC_T_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_6_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_6_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_6_get", A, 15);
    /* (0x00000220 bits 11:6) field ADC_T_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_6_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000220 bits 11:6) field ADC_T_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
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
        /* (0x00000220 bits 11:6) field ADC_T_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_get", stop_bit, 5 );
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
    /* (0x00000220 bits 11:6) field ADC_T_MAP_6 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_MSK, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_5_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_5_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_5_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_5_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_5_set", A, value );

    /* (0x00000220 bits 5:0) field ADC_T_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_MSK,
                                                             TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_OFF,
                                                             value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_5_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_5_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_5_get", A, 15);
    /* (0x00000220 bits 5:0) field ADC_T_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_5_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000220 bits 5:0) field ADC_T_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
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
        /* (0x00000220 bits 5:0) field ADC_T_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_OFF + subfield_offset),
                                                                 TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_get", stop_bit, 5 );
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
    /* (0x00000220 bits 5:0) field ADC_T_MAP_5 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_MSK, TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_14_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_14_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_14_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_14_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_14_set", A, value );

    /* (0x00000224 bits 29:24) field ADC_T_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_MSK,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_OFF,
                                                              value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_14_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_14_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_14_get", A, 15);
    /* (0x00000224 bits 29:24) field ADC_T_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_14_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000224 bits 29:24) field ADC_T_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
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
        /* (0x00000224 bits 29:24) field ADC_T_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_OFF + subfield_offset),
                                                                  TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_get", stop_bit, 5 );
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
    /* (0x00000224 bits 29:24) field ADC_T_MAP_14 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_MSK, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_13_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_13_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_13_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_13_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_13_set", A, value );

    /* (0x00000224 bits 23:18) field ADC_T_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_MSK,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_OFF,
                                                              value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_13_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_13_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_13_get", A, 15);
    /* (0x00000224 bits 23:18) field ADC_T_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_13_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000224 bits 23:18) field ADC_T_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
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
        /* (0x00000224 bits 23:18) field ADC_T_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_OFF + subfield_offset),
                                                                  TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_get", stop_bit, 5 );
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
    /* (0x00000224 bits 23:18) field ADC_T_MAP_13 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_MSK, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_12_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_12_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_12_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_12_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_12_set", A, value );

    /* (0x00000224 bits 17:12) field ADC_T_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_MSK,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_OFF,
                                                              value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_12_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_12_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_12_get", A, 15);
    /* (0x00000224 bits 17:12) field ADC_T_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_12_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000224 bits 17:12) field ADC_T_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
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
        /* (0x00000224 bits 17:12) field ADC_T_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_OFF + subfield_offset),
                                                                  TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_get", stop_bit, 5 );
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
    /* (0x00000224 bits 17:12) field ADC_T_MAP_12 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_MSK, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_11_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_11_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_11_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_11_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_11_set", A, value );

    /* (0x00000224 bits 11:6) field ADC_T_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_MSK,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_OFF,
                                                              value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_11_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_11_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_11_get", A, 15);
    /* (0x00000224 bits 11:6) field ADC_T_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_11_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000224 bits 11:6) field ADC_T_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
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
        /* (0x00000224 bits 11:6) field ADC_T_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_OFF + subfield_offset),
                                                                  TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_get", stop_bit, 5 );
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
    /* (0x00000224 bits 11:6) field ADC_T_MAP_11 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_MSK, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_10_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_10_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_10_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_10_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_10_set", A, value );

    /* (0x00000224 bits 5:0) field ADC_T_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_MSK,
                                                              TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_OFF,
                                                              value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_10_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_10_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_10_get", A, 15);
    /* (0x00000224 bits 5:0) field ADC_T_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_10_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000224 bits 5:0) field ADC_T_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
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
        /* (0x00000224 bits 5:0) field ADC_T_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_OFF + subfield_offset),
                                                                  TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_get", stop_bit, 5 );
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
    /* (0x00000224 bits 5:0) field ADC_T_MAP_10 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_MSK, TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_15_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ADC_T_MAP_15_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_15_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_15_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_15_set", A, value );

    /* (0x00000228 bits 5:0) field ADC_T_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP */
    s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_MSK,
                                                              TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_OFF,
                                                              value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_15_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_15_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ADC_T_MAP_15_get", A, 15);
    /* (0x00000228 bits 5:0) field ADC_T_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ADC_T_MAP_15_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000228 bits 5:0) field ADC_T_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP */
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
        /* (0x00000228 bits 5:0) field ADC_T_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP */
        s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_OFF + subfield_offset),
                                                                  TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_get", stop_bit, 5 );
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
    /* (0x00000228 bits 5:0) field ADC_T_MAP_15 of register PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_MSK, TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_BLOCK_SIZE_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_BLOCK_SIZE_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_BLOCK_SIZE_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_BLOCK_SIZE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_BLOCK_SIZE_set", A, value );

    /* (0x0000022c bits 9:8) field BLOCK_SIZE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( b_ptr,
                                                                                                   h_ptr,
                                                                                                   A,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_MSK,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_OFF,
                                                                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_BLOCK_SIZE_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_BLOCK_SIZE_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_BLOCK_SIZE_get", A, 15);
    /* (0x0000022c bits 9:8) field BLOCK_SIZE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( b_ptr,
                                                                                                          h_ptr,
                                                                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_BLOCK_SIZE_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_BLOCK_SIZE_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_BLOCK_SIZE_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_BLOCK_SIZE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_BLOCK_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_BLOCK_SIZE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_BLOCK_SIZE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000022c bits 9:8) field BLOCK_SIZE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
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
        /* (0x0000022c bits 9:8) field BLOCK_SIZE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
        s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( b_ptr,
                                                                                                       h_ptr,
                                                                                                       A,
                                                                                                       subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_OFF + subfield_offset),
                                                                                                       TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_OFF + subfield_offset,
                                                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_BLOCK_SIZE_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_BLOCK_SIZE_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_BLOCK_SIZE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_BLOCK_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_BLOCK_SIZE_get", stop_bit, 1 );
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
    /* (0x0000022c bits 9:8) field BLOCK_SIZE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( b_ptr,
                                                                                                          h_ptr,
                                                                                                          A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_MSK, TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_BLOCK_SIZE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_set", A, value );

    /* (0x0000022c bits 6) field CFG_FROM_PCBI_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( b_ptr,
                                                                                                   h_ptr,
                                                                                                   A,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_CFG_FROM_PCBI_EN_MSK,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_CFG_FROM_PCBI_EN_OFF,
                                                                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_get", A, 15);
    /* (0x0000022c bits 6) field CFG_FROM_PCBI_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( b_ptr,
                                                                                                          h_ptr,
                                                                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_CFG_FROM_PCBI_EN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_CFG_FROM_PCBI_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_PCBI_PARAM_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_PCBI_PARAM_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PCBI_PARAM_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_PCBI_PARAM_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_PCBI_PARAM_EN_set", A, value );

    /* (0x0000022c bits 5) field PCBI_PARAM_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( b_ptr,
                                                                                                   h_ptr,
                                                                                                   A,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_PCBI_PARAM_EN_MSK,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_PCBI_PARAM_EN_OFF,
                                                                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_PCBI_PARAM_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_PCBI_PARAM_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PCBI_PARAM_EN_get", A, 15);
    /* (0x0000022c bits 5) field PCBI_PARAM_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( b_ptr,
                                                                                                          h_ptr,
                                                                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_PCBI_PARAM_EN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_PCBI_PARAM_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_PCBI_PARAM_EN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_INV_DFE_OUT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_INV_DFE_OUT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INV_DFE_OUT_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_INV_DFE_OUT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_INV_DFE_OUT_set", A, value );

    /* (0x0000022c bits 2) field INV_DFE_OUT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( b_ptr,
                                                                                                   h_ptr,
                                                                                                   A,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_OUT_MSK,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_OUT_OFF,
                                                                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_INV_DFE_OUT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INV_DFE_OUT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INV_DFE_OUT_get", A, 15);
    /* (0x0000022c bits 2) field INV_DFE_OUT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( b_ptr,
                                                                                                          h_ptr,
                                                                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_OUT_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_OUT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INV_DFE_OUT_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_INV_DFE_ERR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_INV_DFE_ERR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INV_DFE_ERR_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_INV_DFE_ERR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_INV_DFE_ERR_set", A, value );

    /* (0x0000022c bits 1) field INV_DFE_ERR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( b_ptr,
                                                                                                   h_ptr,
                                                                                                   A,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_ERR_MSK,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_ERR_OFF,
                                                                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_INV_DFE_ERR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INV_DFE_ERR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INV_DFE_ERR_get", A, 15);
    /* (0x0000022c bits 1) field INV_DFE_ERR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( b_ptr,
                                                                                                          h_ptr,
                                                                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_ERR_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_ERR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INV_DFE_ERR_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_INV_DP_FFE_IN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_INV_DP_FFE_IN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INV_DP_FFE_IN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_INV_DP_FFE_IN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_INV_DP_FFE_IN_set", A, value );

    /* (0x0000022c bits 0) field INV_DP_FFE_IN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set( b_ptr,
                                                                                                   h_ptr,
                                                                                                   A,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DP_FFE_IN_MSK,
                                                                                                   TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DP_FFE_IN_OFF,
                                                                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_INV_DP_FFE_IN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INV_DP_FFE_IN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INV_DP_FFE_IN_get", A, 15);
    /* (0x0000022c bits 0) field INV_DP_FFE_IN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read( b_ptr,
                                                                                                          h_ptr,
                                                                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DP_FFE_IN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DP_FFE_IN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INV_DP_FFE_IN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_set", A, value );

    /* (0x00000230 bits 26) field ENCODER_FLOP_BYPASS_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_ENCODER_FLOP_BYPASS_EN_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_ENCODER_FLOP_BYPASS_EN_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_get", A, 15);
    /* (0x00000230 bits 26) field ENCODER_FLOP_BYPASS_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_ENCODER_FLOP_BYPASS_EN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_ENCODER_FLOP_BYPASS_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_set", A, value );

    /* (0x00000230 bits 25) field IMUXCODE_BRIDGE_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_IMUXCODE_BRIDGE_EN_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_IMUXCODE_BRIDGE_EN_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_get", A, 15);
    /* (0x00000230 bits 25) field IMUXCODE_BRIDGE_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_IMUXCODE_BRIDGE_EN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_IMUXCODE_BRIDGE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_set", A, value );

    /* (0x00000230 bits 24) field UNLOCK_EYE_EDGE_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_UNLOCK_EYE_EDGE_EN_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_UNLOCK_EYE_EDGE_EN_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_get", A, 15);
    /* (0x00000230 bits 24) field UNLOCK_EYE_EDGE_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_UNLOCK_EYE_EDGE_EN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_UNLOCK_EYE_EDGE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_set", A, value );

    /* (0x00000230 bits 23:21) field LF_INTEG_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_get", A, 15);
    /* (0x00000230 bits 23:21) field LF_INTEG_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000230 bits 23:21) field LF_INTEG_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
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
        /* (0x00000230 bits 23:21) field LF_INTEG_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_get", stop_bit, 2 );
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
    /* (0x00000230 bits 23:21) field LF_INTEG_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_set", A, value );

    /* (0x00000230 bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_get", A, 15);
    /* (0x00000230 bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000230 bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
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
        /* (0x00000230 bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_get", stop_bit, 4 );
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
    /* (0x00000230 bits 20:16) field LF_INTEG_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_set", A, value );

    /* (0x00000230 bits 15:14) field LF_PROP_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_get", A, 15);
    /* (0x00000230 bits 15:14) field LF_PROP_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000230 bits 15:14) field LF_PROP_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
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
        /* (0x00000230 bits 15:14) field LF_PROP_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_get", stop_bit, 1 );
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
    /* (0x00000230 bits 15:14) field LF_PROP_LEFT_SHIFT of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_set", A, value );

    /* (0x00000230 bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_get", A, 15);
    /* (0x00000230 bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000230 bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
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
        /* (0x00000230 bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_get", stop_bit, 4 );
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
    /* (0x00000230 bits 13:9) field LF_PROP_CTRL_GAIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_BIAS_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_BIAS_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_BIAS_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_BIAS_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_BIAS_set", A, value );

    /* (0x00000230 bits 5:0) field LF_BIAS of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_BIAS_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_BIAS_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_BIAS_get", A, 15);
    /* (0x00000230 bits 5:0) field LF_BIAS of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_BIAS_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_BIAS_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_BIAS_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_BIAS_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_BIAS_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_BIAS_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_BIAS_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000230 bits 5:0) field LF_BIAS of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
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
        /* (0x00000230 bits 5:0) field LF_BIAS of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_BIAS_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_BIAS_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_BIAS_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_BIAS_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_BIAS_get", stop_bit, 5 );
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
    /* (0x00000230 bits 5:0) field LF_BIAS of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_BIAS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_INTERP_IDX_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_INTERP_IDX_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_set", A, value );

    /* (0x00000234 bits 26:21) field INTERP_IDX_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_get", A, 15);
    /* (0x00000234 bits 26:21) field INTERP_IDX_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000234 bits 26:21) field INTERP_IDX_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
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
        /* (0x00000234 bits 26:21) field INTERP_IDX_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_get", stop_bit, 5 );
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
    /* (0x00000234 bits 26:21) field INTERP_IDX_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_MSK, TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_set", A, value );

    /* (0x00000234 bits 20) field INTERP_IDX_OVR_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_EN_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_EN_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_get", A, 15);
    /* (0x00000234 bits 20) field INTERP_IDX_OVR_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_EN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_set", A, value );

    /* (0x00000234 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_get", A, 15);
    /* (0x00000234 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000234 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
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
        /* (0x00000234 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_get", stop_bit, 1 );
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
    /* (0x00000234 bits 19:18) field LF_INTEG_OUT_MODE of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_MSK, TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_set", A, 15);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_set", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_set", A, value );

    /* (0x00000234 bits 11:0) field LF_INTEG_OUT_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_get", A, 15);
    /* (0x00000234 bits 11:0) field LF_INTEG_OUT_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_set", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000234 bits 11:0) field LF_INTEG_OUT_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000234 bits 11:0) field LF_INTEG_OUT_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_get", stop_bit, 11 );
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
    /* (0x00000234 bits 11:0) field LF_INTEG_OUT_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_MSK, TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_CLAMP_MAX_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_CLAMP_MAX_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_CLAMP_MAX_set", A, 15);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_CLAMP_MAX_set", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_CLAMP_MAX_set", A, value );

    /* (0x00000238 bits 11:0) field LF_CLAMP_MAX of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_CLAMP_MAX_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_CLAMP_MAX_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_CLAMP_MAX_get", A, 15);
    /* (0x00000238 bits 11:0) field LF_CLAMP_MAX of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_CLAMP_MAX_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_set", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000238 bits 11:0) field LF_CLAMP_MAX of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2 */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000238 bits 11:0) field LF_CLAMP_MAX of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_get", stop_bit, 11 );
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
    /* (0x00000238 bits 11:0) field LF_CLAMP_MAX of register PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_MSK, TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_EQ_TARGET_AMP_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_EQ_TARGET_AMP_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_EQ_TARGET_AMP_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_EQ_TARGET_AMP_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_EQ_TARGET_AMP_set", A, value );

    /* (0x0000023c bits 17:12) field EQ_TARGET_AMP of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_EQ_TARGET_AMP_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_EQ_TARGET_AMP_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_EQ_TARGET_AMP_get", A, 15);
    /* (0x0000023c bits 17:12) field EQ_TARGET_AMP of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_EQ_TARGET_AMP_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000023c bits 17:12) field EQ_TARGET_AMP of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
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
        /* (0x0000023c bits 17:12) field EQ_TARGET_AMP of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_get", stop_bit, 5 );
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
    /* (0x0000023c bits 17:12) field EQ_TARGET_AMP of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_MSK, TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_LF_CLAMP_MIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_LF_CLAMP_MIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_CLAMP_MIN_set", A, 15);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_LF_CLAMP_MIN_set", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_CLAMP_MIN_set", A, value );

    /* (0x0000023c bits 11:0) field LF_CLAMP_MIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
    s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_LF_CLAMP_MIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_LF_CLAMP_MIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_LF_CLAMP_MIN_get", A, 15);
    /* (0x0000023c bits 11:0) field LF_CLAMP_MIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_LF_CLAMP_MIN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_set", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000023c bits 11:0) field LF_CLAMP_MIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000023c bits 11:0) field LF_CLAMP_MIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
        s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_get", stop_bit, 11 );
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
    /* (0x0000023c bits 11:0) field LF_CLAMP_MIN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_MSK, TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_set", A, value );

    /* (0x00000240 bits 20) field PTW_SUM_OVR_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_EN_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_EN_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_get", A, 15);
    /* (0x00000240 bits 20) field PTW_SUM_OVR_EN of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_EN_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_PTW_SUM_SEL_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_PTW_SUM_SEL_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PTW_SUM_SEL_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_SEL_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_SEL_set", A, value );

    /* (0x00000240 bits 17:12) field PTW_SUM_SEL of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_PTW_SUM_SEL_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_PTW_SUM_SEL_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PTW_SUM_SEL_get", A, 15);
    /* (0x00000240 bits 17:12) field PTW_SUM_SEL of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_SEL_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000240 bits 17:12) field PTW_SUM_SEL of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
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
        /* (0x00000240 bits 17:12) field PTW_SUM_SEL of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
        s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_get", stop_bit, 5 );
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
    /* (0x00000240 bits 17:12) field PTW_SUM_SEL of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_PTW_SUM_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_PTW_SUM_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_set", A, 15);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_set", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_set", A, value );

    /* (0x00000240 bits 11:0) field PTW_SUM_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
    s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_MSK,
                                                                 TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_OFF,
                                                                 value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_PTW_SUM_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_PTW_SUM_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_get", A, 15);
    /* (0x00000240 bits 11:0) field PTW_SUM_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_MSK) >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_PTW_SUM_OVR_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_set", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000240 bits 11:0) field PTW_SUM_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000240 bits 11:0) field PTW_SUM_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
        s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_OFF + subfield_offset),
                                                                     TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_get", stop_bit, 11 );
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
    /* (0x00000240 bits 11:0) field PTW_SUM_OVR of register PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16 */
    reg_value = s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_MSK)
                  >> TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_MSK, TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_set", A, value );

    /* (0x00000250 bits 29:25) field CAPT_RDBK_SEL of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_MSK,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_OFF,
                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_get", A, 15);
    /* (0x00000250 bits 29:25) field CAPT_RDBK_SEL of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_MSK) >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000250 bits 29:25) field CAPT_RDBK_SEL of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
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
        /* (0x00000250 bits 29:25) field CAPT_RDBK_SEL of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
        s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_OFF + subfield_offset),
                                                       TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_get", stop_bit, 4 );
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
    /* (0x00000250 bits 29:25) field CAPT_RDBK_SEL of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_MSK)
                  >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_MSK, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_set", A, value );

    /* (0x00000250 bits 24:19) field INTERP_IDX_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_MSK,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_OFF,
                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_get", A, 15);
    /* (0x00000250 bits 24:19) field INTERP_IDX_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_MSK) >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000250 bits 24:19) field INTERP_IDX_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
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
        /* (0x00000250 bits 24:19) field INTERP_IDX_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
        s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_OFF + subfield_offset),
                                                       TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_get", stop_bit, 5 );
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
    /* (0x00000250 bits 24:19) field INTERP_IDX_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_MSK)
                  >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_MSK, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_set", A, value );

    /* (0x00000250 bits 18:13) field INTEGRAL_ACC_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_MSK,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_OFF,
                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_get", A, 15);
    /* (0x00000250 bits 18:13) field INTEGRAL_ACC_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_MSK) >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000250 bits 18:13) field INTEGRAL_ACC_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
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
        /* (0x00000250 bits 18:13) field INTEGRAL_ACC_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
        s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_OFF + subfield_offset),
                                                       TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_get", stop_bit, 5 );
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
    /* (0x00000250 bits 18:13) field INTEGRAL_ACC_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_MSK)
                  >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_MSK, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_set", A, value );

    /* (0x00000250 bits 12:7) field DFE_STATE_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_MSK,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_OFF,
                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_get", A, 15);
    /* (0x00000250 bits 12:7) field DFE_STATE_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_MSK) >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000250 bits 12:7) field DFE_STATE_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
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
        /* (0x00000250 bits 12:7) field DFE_STATE_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
        s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_OFF + subfield_offset),
                                                       TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_get", stop_bit, 5 );
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
    /* (0x00000250 bits 12:7) field DFE_STATE_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_MSK)
                  >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_MSK, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_set", A, value );

    /* (0x00000250 bits 6:1) field SAMP_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_MSK,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_OFF,
                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_get", A, 15);
    /* (0x00000250 bits 6:1) field SAMP_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_MSK) >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_get", A, value );

    return value;
}
static INLINE void s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000250 bits 6:1) field SAMP_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
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
        /* (0x00000250 bits 6:1) field SAMP_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
        s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_OFF + subfield_offset),
                                                       TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_get", stop_bit, 5 );
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
    /* (0x00000250 bits 6:1) field SAMP_CAPT_TIME of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_MSK)
                  >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_MSK, TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void s16_titan_br2_mdsp_field_CAPTURE_REQ_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_titan_br2_mdsp_field_CAPTURE_REQ_set( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_CAPTURE_REQ_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_titan_br2_mdsp_field_CAPTURE_REQ_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_titan_br2_mdsp_field_CAPTURE_REQ_set", A, value );

    /* (0x00000250 bits 0) field CAPTURE_REQ of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPTURE_REQ_MSK,
                                                   TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPTURE_REQ_OFF,
                                                   value);
}

static INLINE UINT32 s16_titan_br2_mdsp_field_CAPTURE_REQ_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_CAPTURE_REQ_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_CAPTURE_REQ_get", A, 15);
    /* (0x00000250 bits 0) field CAPTURE_REQ of register PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL */
    reg_value = s16_titan_br2_mdsp_reg_DIAG_CONTROL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPTURE_REQ_MSK) >> TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPTURE_REQ_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_CAPTURE_REQ_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_get", A, 15);
    /* (0x00000244 bits 17:12) field INTERP_IDX_CAPT of register PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0 */
    reg_value = s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_INTERP_IDX_CAPT_MSK) >> TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_INTERP_IDX_CAPT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_get", A, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_get", stop_bit, 5 );
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
    /* (0x00000244 bits 17:12) field INTERP_IDX_CAPT of register PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0 */
    reg_value = s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_INTERP_IDX_CAPT_MSK)
                  >> TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_INTERP_IDX_CAPT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_INTERP_IDX_CAPT_MSK, TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_INTERP_IDX_CAPT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_PP_T_DATA_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_PP_T_DATA_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PP_T_DATA_CAPT_get", A, 15);
    /* (0x00000244 bits 11:6) field PP_T_DATA_CAPT of register PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0 */
    reg_value = s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_T_DATA_CAPT_MSK) >> TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_T_DATA_CAPT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_PP_T_DATA_CAPT_get", A, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_range_PP_T_DATA_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_PP_T_DATA_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_PP_T_DATA_CAPT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_PP_T_DATA_CAPT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_PP_T_DATA_CAPT_get", stop_bit, 5 );
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
    /* (0x00000244 bits 11:6) field PP_T_DATA_CAPT of register PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0 */
    reg_value = s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_T_DATA_CAPT_MSK)
                  >> TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_T_DATA_CAPT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_T_DATA_CAPT_MSK, TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_T_DATA_CAPT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_PP_T_DATA_CAPT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_PP_D_DATA_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_PP_D_DATA_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_PP_D_DATA_CAPT_get", A, 15);
    /* (0x00000244 bits 5:0) field PP_D_DATA_CAPT of register PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0 */
    reg_value = s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_D_DATA_CAPT_MSK) >> TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_D_DATA_CAPT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_PP_D_DATA_CAPT_get", A, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_range_PP_D_DATA_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_PP_D_DATA_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_PP_D_DATA_CAPT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_PP_D_DATA_CAPT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_PP_D_DATA_CAPT_get", stop_bit, 5 );
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
    /* (0x00000244 bits 5:0) field PP_D_DATA_CAPT of register PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0 */
    reg_value = s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_D_DATA_CAPT_MSK)
                  >> TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_D_DATA_CAPT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_D_DATA_CAPT_MSK, TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_D_DATA_CAPT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_PP_D_DATA_CAPT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_DFE_STATE_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_DFE_STATE_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_DFE_STATE_CAPT_get", A, 15);
    /* (0x00000248 bits 15:0) field DFE_STATE_CAPT of register PMC_TITAN_BR2_DIGI_REG_DFE_STATE */
    reg_value = s16_titan_br2_mdsp_reg_DFE_STATE_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & TITAN_BR2_DIGI_REG_DFE_STATE_BIT_DFE_STATE_CAPT_MSK) >> TITAN_BR2_DIGI_REG_DFE_STATE_BIT_DFE_STATE_CAPT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_DFE_STATE_CAPT_get", A, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_get", stop_bit, 15 );
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
    /* (0x00000248 bits 15:0) field DFE_STATE_CAPT of register PMC_TITAN_BR2_DIGI_REG_DFE_STATE */
    reg_value = s16_titan_br2_mdsp_reg_DFE_STATE_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_DFE_STATE_BIT_DFE_STATE_CAPT_MSK)
                  >> TITAN_BR2_DIGI_REG_DFE_STATE_BIT_DFE_STATE_CAPT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_DFE_STATE_BIT_DFE_STATE_CAPT_MSK, TITAN_BR2_DIGI_REG_DFE_STATE_BIT_DFE_STATE_CAPT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_INTEGRAL_ACC_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_INTEGRAL_ACC_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_INTEGRAL_ACC_get", A, 15);
    /* (0x0000024c bits 17:0) field INTEGRAL_ACC of register PMC_TITAN_BR2_DIGI_REG_TCB_STATE_0 */
    reg_value = s16_titan_br2_mdsp_reg_TCB_STATE_0_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & TITAN_BR2_DIGI_REG_TCB_STATE_0_BIT_INTEGRAL_ACC_MSK) >> TITAN_BR2_DIGI_REG_TCB_STATE_0_BIT_INTEGRAL_ACC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_titan_br2_mdsp_field_INTEGRAL_ACC_get", A, value );

    return value;
}
static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_get( s16_titan_br2_mdsp_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_get", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_get", stop_bit, 17 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 17) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 17;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000024c bits 17:0) field INTEGRAL_ACC of register PMC_TITAN_BR2_DIGI_REG_TCB_STATE_0 */
    reg_value = s16_titan_br2_mdsp_reg_TCB_STATE_0_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & TITAN_BR2_DIGI_REG_TCB_STATE_0_BIT_INTEGRAL_ACC_MSK)
                  >> TITAN_BR2_DIGI_REG_TCB_STATE_0_BIT_INTEGRAL_ACC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TITAN_BR2_DIGI_REG_TCB_STATE_0_BIT_INTEGRAL_ACC_MSK, TITAN_BR2_DIGI_REG_TCB_STATE_0_BIT_INTEGRAL_ACC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _S16_TITAN_BR2_MDSP_IO_INLINE_H */
