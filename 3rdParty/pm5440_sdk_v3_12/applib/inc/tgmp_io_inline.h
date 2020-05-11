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
 *     and register accessor functions for the tgmp block
 *****************************************************************************/
#ifndef _TGMP_IO_INLINE_H
#define _TGMP_IO_INLINE_H

#include "tgmp_loc.h"
#include "tgmp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TGMP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for tgmp
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
    tgmp_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} tgmp_buffer_t;
static INLINE void tgmp_buffer_init( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void tgmp_buffer_init( tgmp_buffer_t *b_ptr, tgmp_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "tgmp_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void tgmp_buffer_flush( tgmp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void tgmp_buffer_flush( tgmp_buffer_t *b_ptr )
{
    IOLOG( "tgmp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 tgmp_reg_read( tgmp_buffer_t *b_ptr,
                                    tgmp_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 tgmp_reg_read( tgmp_buffer_t *b_ptr,
                                    tgmp_handle_t *h_ptr,
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
static INLINE void tgmp_reg_write( tgmp_buffer_t *b_ptr,
                                   tgmp_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_write( tgmp_buffer_t *b_ptr,
                                   tgmp_handle_t *h_ptr,
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

static INLINE void tgmp_field_set( tgmp_buffer_t *b_ptr,
                                   tgmp_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_field_set( tgmp_buffer_t *b_ptr,
                                   tgmp_handle_t *h_ptr,
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

static INLINE void tgmp_action_on_write_field_set( tgmp_buffer_t *b_ptr,
                                                   tgmp_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_action_on_write_field_set( tgmp_buffer_t *b_ptr,
                                                   tgmp_handle_t *h_ptr,
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

static INLINE void tgmp_burst_read( tgmp_buffer_t *b_ptr,
                                    tgmp_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void tgmp_burst_read( tgmp_buffer_t *b_ptr,
                                    tgmp_handle_t *h_ptr,
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

static INLINE void tgmp_burst_write( tgmp_buffer_t *b_ptr,
                                     tgmp_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void tgmp_burst_write( tgmp_buffer_t *b_ptr,
                                     tgmp_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE tgmp_poll( tgmp_buffer_t *b_ptr,
                                              tgmp_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE tgmp_poll( tgmp_buffer_t *b_ptr,
                                              tgmp_handle_t *h_ptr,
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
 *  register access functions for tgmp
 * ==================================================================================
 */

static INLINE void tgmp_reg_CHANNEL_RESET_REG_array_write( tgmp_buffer_t *b_ptr,
                                                           tgmp_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_CHANNEL_RESET_REG_array_write( tgmp_buffer_t *b_ptr,
                                                           tgmp_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tgmp_reg_CHANNEL_RESET_REG_array_write", value );
    tgmp_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG(N),
                    value);
}

static INLINE void tgmp_reg_CHANNEL_RESET_REG_array_field_set( tgmp_buffer_t *b_ptr,
                                                               tgmp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_CHANNEL_RESET_REG_array_field_set( tgmp_buffer_t *b_ptr,
                                                               tgmp_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "tgmp_reg_CHANNEL_RESET_REG_array_field_set", N, mask, ofs, value );
    tgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG(N),
                    mask,
                    PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tgmp_reg_CHANNEL_RESET_REG_array_read( tgmp_buffer_t *b_ptr,
                                                            tgmp_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tgmp_reg_CHANNEL_RESET_REG_array_read( tgmp_buffer_t *b_ptr,
                                                            tgmp_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = tgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "tgmp_reg_CHANNEL_RESET_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_write( tgmp_buffer_t *b_ptr,
                                                                  tgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_write( tgmp_buffer_t *b_ptr,
                                                                  tgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_write", 3, ofs, len);
    tgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN(ofs),
                      len,
                      value);

}

static INLINE void tgmp_reg_TXJC_RES_OVRFLW_EN_array_field_set( tgmp_buffer_t *b_ptr,
                                                                tgmp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_TXJC_RES_OVRFLW_EN_array_field_set( tgmp_buffer_t *b_ptr,
                                                                tgmp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tgmp_reg_TXJC_RES_OVRFLW_EN_array_field_set", mask, ofs, value );
    tgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN(N),
                    mask,
                    PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_read( tgmp_buffer_t *b_ptr,
                                                                 tgmp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_read( tgmp_buffer_t *b_ptr,
                                                                 tgmp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_read", ofs, len, 3 );
    tgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 tgmp_reg_TXJC_RES_OVRFLW_EN_array_read( tgmp_buffer_t *b_ptr,
                                                             tgmp_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tgmp_reg_TXJC_RES_OVRFLW_EN_array_read( tgmp_buffer_t *b_ptr,
                                                             tgmp_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = tgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN(N));

    IOLOG( "%s -> 0x%08x;", "tgmp_reg_TXJC_RES_OVRFLW_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_write( tgmp_buffer_t *b_ptr,
                                                                   tgmp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_write( tgmp_buffer_t *b_ptr,
                                                                   tgmp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_write", 3, ofs, len);
    tgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT(ofs),
                      len,
                      value);

}

static INLINE void tgmp_reg_TXJC_RES_OVRFLW_INT_array_action_on_write_field_set( tgmp_buffer_t *b_ptr,
                                                                                 tgmp_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_TXJC_RES_OVRFLW_INT_array_action_on_write_field_set( tgmp_buffer_t *b_ptr,
                                                                                 tgmp_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tgmp_reg_TXJC_RES_OVRFLW_INT_array_action_on_write_field_set", mask, ofs, value );
    tgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_read( tgmp_buffer_t *b_ptr,
                                                                  tgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_read( tgmp_buffer_t *b_ptr,
                                                                  tgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_read", ofs, len, 3 );
    tgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 tgmp_reg_TXJC_RES_OVRFLW_INT_array_read( tgmp_buffer_t *b_ptr,
                                                              tgmp_handle_t *h_ptr,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tgmp_reg_TXJC_RES_OVRFLW_INT_array_read( tgmp_buffer_t *b_ptr,
                                                              tgmp_handle_t *h_ptr,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = tgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT(N));

    IOLOG( "%s -> 0x%08x;", "tgmp_reg_TXJC_RES_OVRFLW_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE tgmp_reg_TXJC_RES_OVRFLW_INT_array_poll( tgmp_buffer_t *b_ptr,
                                                                            tgmp_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE tgmp_reg_TXJC_RES_OVRFLW_INT_array_poll( tgmp_buffer_t *b_ptr,
                                                                            tgmp_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "tgmp_reg_TXJC_RES_OVRFLW_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return tgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void tgmp_field_CHAN_RSTB_set( tgmp_buffer_t *b_ptr,
                                             tgmp_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_field_CHAN_RSTB_set( tgmp_buffer_t *b_ptr,
                                             tgmp_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "tgmp_field_CHAN_RSTB_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tgmp_field_CHAN_RSTB_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "tgmp_field_CHAN_RSTB_set", N, value );

    /* ((0x00000004 + (N) * 0x4) bits 0) field CHAN_RSTB of register PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG index N=0..95 */
    tgmp_reg_CHANNEL_RESET_REG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                TGMP96_CORE_REG_CHANNEL_RESET_REG_BIT_CHAN_RSTB_MSK,
                                                TGMP96_CORE_REG_CHANNEL_RESET_REG_BIT_CHAN_RSTB_OFF,
                                                value);
}

static INLINE UINT32 tgmp_field_CHAN_RSTB_get( tgmp_buffer_t *b_ptr,
                                               tgmp_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tgmp_field_CHAN_RSTB_get( tgmp_buffer_t *b_ptr,
                                               tgmp_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "tgmp_field_CHAN_RSTB_get", N, 95);
    /* ((0x00000004 + (N) * 0x4) bits 0) field CHAN_RSTB of register PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG index N=0..95 */
    reg_value = tgmp_reg_CHANNEL_RESET_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & TGMP96_CORE_REG_CHANNEL_RESET_REG_BIT_CHAN_RSTB_MSK) >> TGMP96_CORE_REG_CHANNEL_RESET_REG_BIT_CHAN_RSTB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "tgmp_field_CHAN_RSTB_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_N_burstsize3
 * ==================================================================================
 */
static INLINE void tgmp_lfield_TXJC_RES_OVRFLW_E_set( tgmp_buffer_t *b_ptr,
                                                      tgmp_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void tgmp_lfield_TXJC_RES_OVRFLW_E_set( tgmp_buffer_t *b_ptr,
                                                      tgmp_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "tgmp_lfield_TXJC_RES_OVRFLW_E_set");
    tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   value);
}

static INLINE void tgmp_lfield_TXJC_RES_OVRFLW_E_get( tgmp_buffer_t *b_ptr,
                                                      tgmp_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void tgmp_lfield_TXJC_RES_OVRFLW_E_get( tgmp_buffer_t *b_ptr,
                                                      tgmp_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "tgmp_lfield_TXJC_RES_OVRFLW_E_get");
    tgmp_reg_TXJC_RES_OVRFLW_EN_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  3,
                                                  (UINT32 *)value);
}

static INLINE void tgmp_lfield_range_TXJC_RES_OVRFLW_E_set( tgmp_buffer_t *b_ptr,
                                                            tgmp_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_lfield_range_TXJC_RES_OVRFLW_E_set( tgmp_buffer_t *b_ptr,
                                                            tgmp_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "tgmp_lfield_range_TXJC_RES_OVRFLW_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "tgmp_lfield_range_TXJC_RES_OVRFLW_E_set", start_bit, stop_bit, value);
    tgmp_reg_TXJC_RES_OVRFLW_EN_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 tgmp_lfield_range_TXJC_RES_OVRFLW_E_get( tgmp_buffer_t *b_ptr,
                                                              tgmp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 tgmp_lfield_range_TXJC_RES_OVRFLW_E_get( tgmp_buffer_t *b_ptr,
                                                              tgmp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "tgmp_lfield_range_TXJC_RES_OVRFLW_E_get", start_bit, stop_bit );
    }
    reg_value = tgmp_reg_TXJC_RES_OVRFLW_EN_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "tgmp_lfield_range_TXJC_RES_OVRFLW_E_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_burstsize3
 * ==================================================================================
 */
static INLINE void tgmp_lfield_TXJC_RES_OVRFLW_I_set_to_clear( tgmp_buffer_t *b_ptr,
                                                               tgmp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void tgmp_lfield_TXJC_RES_OVRFLW_I_set_to_clear( tgmp_buffer_t *b_ptr,
                                                               tgmp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "tgmp_lfield_TXJC_RES_OVRFLW_I_set_to_clear");
    tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    0,
                                                    3,
                                                    value);
}

static INLINE void tgmp_lfield_TXJC_RES_OVRFLW_I_get( tgmp_buffer_t *b_ptr,
                                                      tgmp_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void tgmp_lfield_TXJC_RES_OVRFLW_I_get( tgmp_buffer_t *b_ptr,
                                                      tgmp_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "tgmp_lfield_TXJC_RES_OVRFLW_I_get");
    tgmp_reg_TXJC_RES_OVRFLW_INT_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   (UINT32 *)value);
}

static INLINE void tgmp_lfield_range_TXJC_RES_OVRFLW_I_set_to_clear( tgmp_buffer_t *b_ptr,
                                                                     tgmp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tgmp_lfield_range_TXJC_RES_OVRFLW_I_set_to_clear( tgmp_buffer_t *b_ptr,
                                                                     tgmp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "tgmp_lfield_range_TXJC_RES_OVRFLW_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "tgmp_lfield_range_TXJC_RES_OVRFLW_I_set_to_clear", start_bit, stop_bit, value);
    tgmp_reg_TXJC_RES_OVRFLW_INT_array_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  word_number,
                                                                  mask,
                                                                  field_ofs,
                                                                  value);
}

static INLINE UINT32 tgmp_lfield_range_TXJC_RES_OVRFLW_I_get( tgmp_buffer_t *b_ptr,
                                                              tgmp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 tgmp_lfield_range_TXJC_RES_OVRFLW_I_get( tgmp_buffer_t *b_ptr,
                                                              tgmp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "tgmp_lfield_range_TXJC_RES_OVRFLW_I_get", start_bit, stop_bit );
    }
    reg_value = tgmp_reg_TXJC_RES_OVRFLW_INT_array_read( b_ptr,
                                                         h_ptr,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "tgmp_lfield_range_TXJC_RES_OVRFLW_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE tgmp_lfield_range_TXJC_RES_OVRFLW_I_poll( tgmp_buffer_t *b_ptr,
                                                                             tgmp_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE tgmp_lfield_range_TXJC_RES_OVRFLW_I_poll( tgmp_buffer_t *b_ptr,
                                                                             tgmp_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "tgmp_lfield_range_TXJC_RES_OVRFLW_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "tgmp_lfield_range_TXJC_RES_OVRFLW_I_set_to_clear", start_bit, stop_bit);
    return tgmp_reg_TXJC_RES_OVRFLW_INT_array_poll( b_ptr,
                                                    h_ptr,
                                                    word_number,
                                                    mask,
                                                    ( value << field_ofs),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TGMP_IO_INLINE_H */
