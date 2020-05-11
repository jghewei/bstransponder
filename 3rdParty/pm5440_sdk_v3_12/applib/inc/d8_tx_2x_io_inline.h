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
 *     and register accessor functions for the d8_tx_2x block
 *****************************************************************************/
#ifndef _D8_TX_2X_IO_INLINE_H
#define _D8_TX_2X_IO_INLINE_H

#include "d8_tx_2x.h"
#include "d8_tx_2x_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define D8_TX_2X_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for d8_tx_2x
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
    d8_tx_2x_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} d8_tx_2x_buffer_t;
static INLINE void d8_tx_2x_buffer_init( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_buffer_init( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "d8_tx_2x_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void d8_tx_2x_buffer_flush( d8_tx_2x_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_buffer_flush( d8_tx_2x_buffer_t *b_ptr )
{
    IOLOG( "d8_tx_2x_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 d8_tx_2x_reg_read( d8_tx_2x_buffer_t *b_ptr,
                                        d8_tx_2x_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_read( d8_tx_2x_buffer_t *b_ptr,
                                        d8_tx_2x_handle_t *h_ptr,
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
static INLINE void d8_tx_2x_reg_write( d8_tx_2x_buffer_t *b_ptr,
                                       d8_tx_2x_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_write( d8_tx_2x_buffer_t *b_ptr,
                                       d8_tx_2x_handle_t *h_ptr,
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

static INLINE void d8_tx_2x_field_set( d8_tx_2x_buffer_t *b_ptr,
                                       d8_tx_2x_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_set( d8_tx_2x_buffer_t *b_ptr,
                                       d8_tx_2x_handle_t *h_ptr,
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

static INLINE void d8_tx_2x_action_on_write_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_action_on_write_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
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

static INLINE void d8_tx_2x_burst_read( d8_tx_2x_buffer_t *b_ptr,
                                        d8_tx_2x_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_burst_read( d8_tx_2x_buffer_t *b_ptr,
                                        d8_tx_2x_handle_t *h_ptr,
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

static INLINE void d8_tx_2x_burst_write( d8_tx_2x_buffer_t *b_ptr,
                                         d8_tx_2x_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_burst_write( d8_tx_2x_buffer_t *b_ptr,
                                         d8_tx_2x_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_poll( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_poll( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
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
 *  register access functions for d8_tx_2x
 * ==================================================================================
 */

static INLINE void d8_tx_2x_reg_SW_RESET_CTRL_write( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_SW_RESET_CTRL_write( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_SW_RESET_CTRL_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL,
                        value);
}

static INLINE void d8_tx_2x_reg_SW_RESET_CTRL_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_SW_RESET_CTRL_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_SW_RESET_CTRL_field_set", mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL,
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_SW_RESET_CTRL_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_SW_RESET_CTRL_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL);

    IOLOG( "%s -> 0x%08x;", "d8_tx_2x_reg_SW_RESET_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_ANALOG_TEST_CTRL_write( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_ANALOG_TEST_CTRL_write( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_ANALOG_TEST_CTRL_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL,
                        value);
}

static INLINE void d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set", mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL,
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_ANALOG_TEST_CTRL_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_ANALOG_TEST_CTRL_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL);

    IOLOG( "%s -> 0x%08x;", "d8_tx_2x_reg_ANALOG_TEST_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_write( d8_tx_2x_buffer_t *b_ptr,
                                                                              d8_tx_2x_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_write( d8_tx_2x_buffer_t *b_ptr,
                                                                              d8_tx_2x_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value )
{
    if (len + ofs > 2)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_write", ofs, len, 2 );
    IOLOG( "%s of %d words ofs=%d len=%d", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_write", 2, ofs, len);
    d8_tx_2x_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1(ofs),
                          len,
                          value);

}

static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                            d8_tx_2x_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                            d8_tx_2x_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_field_set", mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_read( d8_tx_2x_buffer_t *b_ptr,
                                                                             d8_tx_2x_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_read( d8_tx_2x_buffer_t *b_ptr,
                                                                             d8_tx_2x_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value )
{
    if (len + ofs > 2)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_read", ofs, len, 2 );
    d8_tx_2x_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_read", 2, ofs, len);
}

static INLINE UINT32 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1(N));

    IOLOG( "%s -> 0x%08x;", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_read", reg_value);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_write( d8_tx_2x_buffer_t *b_ptr,
                                                                              d8_tx_2x_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_write( d8_tx_2x_buffer_t *b_ptr,
                                                                              d8_tx_2x_handle_t *h_ptr,
                                                                              UINT32 ofs,
                                                                              UINT32 len,
                                                                              UINT32 *value )
{
    if (len + ofs > 2)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_write", ofs, len, 2 );
    IOLOG( "%s of %d words ofs=%d len=%d", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_write", 2, ofs, len);
    d8_tx_2x_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1(ofs),
                          len,
                          value);

}

static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                            d8_tx_2x_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                            d8_tx_2x_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_field_set", mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_read( d8_tx_2x_buffer_t *b_ptr,
                                                                             d8_tx_2x_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_read( d8_tx_2x_buffer_t *b_ptr,
                                                                             d8_tx_2x_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value )
{
    if (len + ofs > 2)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_read", ofs, len, 2 );
    d8_tx_2x_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_read", 2, ofs, len);
}

static INLINE UINT32 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1(N));

    IOLOG( "%s -> 0x%08x;", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_read", reg_value);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                     d8_tx_2x_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                     d8_tx_2x_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1(N),
                        value);
}

static INLINE void d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set", N, mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                      d8_tx_2x_handle_t *h_ptr,
                                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                      d8_tx_2x_handle_t *h_ptr,
                                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                     d8_tx_2x_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                     d8_tx_2x_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2(N),
                        value);
}

static INLINE void d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_field_set", N, mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                      d8_tx_2x_handle_t *h_ptr,
                                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                      d8_tx_2x_handle_t *h_ptr,
                                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_DESKEW_FIFO_CFG_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                             d8_tx_2x_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_DESKEW_FIFO_CFG_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                             d8_tx_2x_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_DESKEW_FIFO_CFG_array_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG(N),
                        value);
}

static INLINE void d8_tx_2x_reg_DESKEW_FIFO_CFG_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                 d8_tx_2x_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_DESKEW_FIFO_CFG_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                 d8_tx_2x_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_DESKEW_FIFO_CFG_array_field_set", N, mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_DESKEW_FIFO_CFG_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_DESKEW_FIFO_CFG_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "d8_tx_2x_reg_DESKEW_FIFO_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_DATAPATH_CFG_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_DATAPATH_CFG_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_DATAPATH_CFG_array_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG(N),
                        value);
}

static INLINE void d8_tx_2x_reg_DATAPATH_CFG_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_DATAPATH_CFG_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_DATAPATH_CFG_array_field_set", N, mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_DATAPATH_CFG_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                           d8_tx_2x_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_DATAPATH_CFG_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                           d8_tx_2x_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "d8_tx_2x_reg_DATAPATH_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                     d8_tx_2x_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                     d8_tx_2x_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1(N),
                        value);
}

static INLINE void d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set", N, mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                      d8_tx_2x_handle_t *h_ptr,
                                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                      d8_tx_2x_handle_t *h_ptr,
                                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                        d8_tx_2x_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                        d8_tx_2x_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2(N),
                        value);
}

static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                            d8_tx_2x_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                            d8_tx_2x_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_field_set", N, mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_PATTERN_MON_CFG_1_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_MON_CFG_1_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_PATTERN_MON_CFG_1_array_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1(N),
                        value);
}

static INLINE void d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                   d8_tx_2x_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                   d8_tx_2x_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set", N, mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                        d8_tx_2x_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_write( d8_tx_2x_buffer_t *b_ptr,
                                                                        d8_tx_2x_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2(N),
                        value);
}

static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                            d8_tx_2x_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                            d8_tx_2x_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_field_set", N, mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2(N),
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_INT_EN_REG_write( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_INT_EN_REG_write( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_INT_EN_REG_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG,
                        value);
}

static INLINE void d8_tx_2x_reg_INT_EN_REG_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_INT_EN_REG_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_INT_EN_REG_field_set", mask, ofs, value );
    d8_tx_2x_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG,
                        mask,
                        PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 d8_tx_2x_reg_INT_EN_REG_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_INT_EN_REG_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "d8_tx_2x_reg_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void d8_tx_2x_reg_INT_REG_write( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_INT_REG_write( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "d8_tx_2x_reg_INT_REG_write", value );
    d8_tx_2x_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SFI51_TX_2X_SLICE_REG_INT_REG,
                        value);
}

static INLINE void d8_tx_2x_reg_INT_REG_action_on_write_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                   d8_tx_2x_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_INT_REG_action_on_write_field_set( d8_tx_2x_buffer_t *b_ptr,
                                                                   d8_tx_2x_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "d8_tx_2x_reg_INT_REG_action_on_write_field_set", mask, ofs, value );
    d8_tx_2x_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_SFI51_TX_2X_SLICE_REG_INT_REG,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 d8_tx_2x_reg_INT_REG_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_INT_REG_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SFI51_TX_2X_SLICE_REG_INT_REG);

    IOLOG( "%s -> 0x%08x;", "d8_tx_2x_reg_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_reg_INT_REG_poll( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_reg_INT_REG_poll( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "d8_tx_2x_reg_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return d8_tx_2x_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SFI51_TX_2X_SLICE_REG_INT_REG,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 d8_tx_2x_reg_INT_STAT_REG_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_INT_STAT_REG_read( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SFI51_TX_2X_SLICE_REG_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "d8_tx_2x_reg_INT_STAT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_reg_INT_STAT_REG_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                   d8_tx_2x_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_reg_INT_STAT_REG_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                   d8_tx_2x_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "d8_tx_2x_reg_INT_STAT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return d8_tx_2x_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SFI51_TX_2X_SLICE_REG_INT_STAT_REG,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_burst_read( d8_tx_2x_buffer_t *b_ptr,
                                                                        d8_tx_2x_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_burst_read( d8_tx_2x_buffer_t *b_ptr,
                                                                        d8_tx_2x_handle_t *h_ptr,
                                                                        UINT32 ofs,
                                                                        UINT32 len,
                                                                        UINT32 *value )
{
    if (len + ofs > 2)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_burst_read", ofs, len, 2 );
    d8_tx_2x_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_burst_read", 2, ofs, len);
}

static INLINE UINT32 d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                    d8_tx_2x_handle_t *h_ptr,
                                                                    UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_read( d8_tx_2x_buffer_t *b_ptr,
                                                                    d8_tx_2x_handle_t *h_ptr,
                                                                    UINT32 N )
{
    UINT32 reg_value;

    reg_value = d8_tx_2x_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1(N));

    IOLOG( "%s -> 0x%08x;", "d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_set", value );

    /* (0x00000000 bits 12) field PSLB_CLK_DEGLITCH_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_B_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_B_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 12) field PSLB_CLK_DEGLITCH_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_B_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_B_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_set", value );

    /* (0x00000000 bits 11) field PSLB_CLK_DEGLITCH_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_A_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_A_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 11) field PSLB_CLK_DEGLITCH_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_A_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_A_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                                  d8_tx_2x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                                  d8_tx_2x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_set", value );

    /* (0x00000000 bits 10) field TPCOUT_CLK_DEGLITCH_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_B_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_B_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 10) field TPCOUT_CLK_DEGLITCH_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_B_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_B_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                                  d8_tx_2x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                                  d8_tx_2x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_set", value );

    /* (0x00000000 bits 9) field TPCOUT_CLK_DEGLITCH_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_A_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_A_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 9) field TPCOUT_CLK_DEGLITCH_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_A_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_A_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_PATT_MON_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_MON_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_MON_B_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PATT_MON_B_RSTB_set", value );

    /* (0x00000000 bits 8) field PATT_MON_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_B_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_B_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_MON_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_MON_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 8) field PATT_MON_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_B_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_B_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_MON_B_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_PATT_MON_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_MON_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_MON_A_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PATT_MON_A_RSTB_set", value );

    /* (0x00000000 bits 7) field PATT_MON_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_A_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_A_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_MON_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_MON_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 7) field PATT_MON_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_A_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_A_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_MON_A_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_PATT_GEN_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_GEN_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_GEN_B_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PATT_GEN_B_RSTB_set", value );

    /* (0x00000000 bits 6) field PATT_GEN_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_B_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_B_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_GEN_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_GEN_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 6) field PATT_GEN_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_B_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_B_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_GEN_B_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_PATT_GEN_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_GEN_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_GEN_A_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PATT_GEN_A_RSTB_set", value );

    /* (0x00000000 bits 5) field PATT_GEN_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_A_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_A_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_GEN_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_GEN_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 5) field PATT_GEN_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_A_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_A_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_GEN_A_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_PISO_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_PISO_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_PISO_B_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_GEN_PISO_B_RSTB_set", value );

    /* (0x00000000 bits 4) field GEN_PISO_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_B_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_B_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_PISO_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_PISO_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field GEN_PISO_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_B_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_B_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_GEN_PISO_B_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_PISO_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_PISO_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_PISO_A_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_GEN_PISO_A_RSTB_set", value );

    /* (0x00000000 bits 3) field GEN_PISO_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_A_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_A_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_PISO_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_PISO_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field GEN_PISO_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_A_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_A_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_GEN_PISO_A_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_DESKEW_FIFO_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_DESKEW_FIFO_B_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_DESKEW_FIFO_B_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_DESKEW_FIFO_B_RSTB_set", value );

    /* (0x00000000 bits 2) field DESKEW_FIFO_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_B_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_B_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_DESKEW_FIFO_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_DESKEW_FIFO_B_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field DESKEW_FIFO_B_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_B_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_B_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_DESKEW_FIFO_B_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_DESKEW_FIFO_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_DESKEW_FIFO_A_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_DESKEW_FIFO_A_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_DESKEW_FIFO_A_RSTB_set", value );

    /* (0x00000000 bits 1) field DESKEW_FIFO_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_A_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_A_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_DESKEW_FIFO_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_DESKEW_FIFO_A_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field DESKEW_FIFO_A_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_A_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_A_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_DESKEW_FIFO_A_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_DIGITAL_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_DIGITAL_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_DIGITAL_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_DIGITAL_RSTB_set", value );

    /* (0x00000000 bits 0) field DIGITAL_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    d8_tx_2x_reg_SW_RESET_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_MSK,
                                          SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_OFF,
                                          value);
}

static INLINE UINT32 d8_tx_2x_field_DIGITAL_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_DIGITAL_RSTB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field DIGITAL_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL */
    reg_value = d8_tx_2x_reg_SW_RESET_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_DIGITAL_RSTB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_JTAG_IN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_JTAG_IN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_JTAG_IN_set", value, 3);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_TX_JTAG_IN_set", value );

    /* (0x00000004 bits 13:12) field TX_JTAG_IN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_MSK,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_OFF,
                                             value);
}

static INLINE UINT32 d8_tx_2x_field_TX_JTAG_IN_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_JTAG_IN_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 13:12) field TX_JTAG_IN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_MSK) >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_TX_JTAG_IN_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_range_TX_JTAG_IN_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_TX_JTAG_IN_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_JTAG_IN_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_JTAG_IN_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_JTAG_IN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 13:12) field TX_JTAG_IN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
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
        /* (0x00000004 bits 13:12) field TX_JTAG_IN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
        d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_OFF + subfield_offset),
                                                 SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_TX_JTAG_IN_get( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_TX_JTAG_IN_get( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_JTAG_IN_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_JTAG_IN_get", stop_bit, 1 );
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
    /* (0x00000004 bits 13:12) field TX_JTAG_IN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_MSK)
                  >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_MSK, SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_JTAG_IN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_JTAG_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_JTAG_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_JTAG_EN_set", value, 3);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_TX_JTAG_EN_set", value );

    /* (0x00000004 bits 11:10) field TX_JTAG_EN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_MSK,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_OFF,
                                             value);
}

static INLINE UINT32 d8_tx_2x_field_TX_JTAG_EN_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_JTAG_EN_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 11:10) field TX_JTAG_EN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_MSK) >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_TX_JTAG_EN_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_range_TX_JTAG_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_TX_JTAG_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_JTAG_EN_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_JTAG_EN_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_JTAG_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 11:10) field TX_JTAG_EN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
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
        /* (0x00000004 bits 11:10) field TX_JTAG_EN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
        d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_OFF + subfield_offset),
                                                 SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_TX_JTAG_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_TX_JTAG_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_JTAG_EN_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_JTAG_EN_get", stop_bit, 1 );
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
    /* (0x00000004 bits 11:10) field TX_JTAG_EN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_MSK)
                  >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_MSK, SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_JTAG_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_DLB_SLICE_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_DLB_SLICE_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_DLB_SLICE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_DLB_SLICE_EN_set", value );

    /* (0x00000004 bits 8) field DLB_SLICE_EN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_SLICE_EN_MSK,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_SLICE_EN_OFF,
                                             value);
}

static INLINE UINT32 d8_tx_2x_field_DLB_SLICE_EN_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_DLB_SLICE_EN_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 8) field DLB_SLICE_EN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_SLICE_EN_MSK) >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_SLICE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_DLB_SLICE_EN_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_DLB_CH_SEL_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_DLB_CH_SEL_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_DLB_CH_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_DLB_CH_SEL_set", value );

    /* (0x00000004 bits 6) field DLB_CH_SEL of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_CH_SEL_MSK,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_CH_SEL_OFF,
                                             value);
}

static INLINE UINT32 d8_tx_2x_field_DLB_CH_SEL_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_DLB_CH_SEL_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 6) field DLB_CH_SEL of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_CH_SEL_MSK) >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_CH_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_DLB_CH_SEL_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_TS_ATMSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TS_ATMSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TS_ATMSB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_TS_ATMSB_set", value );

    /* (0x00000004 bits 5) field TS_ATMSB of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TS_ATMSB_MSK,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TS_ATMSB_OFF,
                                             value);
}

static INLINE UINT32 d8_tx_2x_field_TS_ATMSB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TS_ATMSB_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 5) field TS_ATMSB of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TS_ATMSB_MSK) >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TS_ATMSB_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_TS_ATMSB_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_ATIN_set( d8_tx_2x_buffer_t *b_ptr,
                                            d8_tx_2x_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_ATIN_set( d8_tx_2x_buffer_t *b_ptr,
                                            d8_tx_2x_handle_t *h_ptr,
                                            UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_ATIN_set", value, 31);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_ATIN_set", value );

    /* (0x00000004 bits 4:0) field ATIN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_MSK,
                                             SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_OFF,
                                             value);
}

static INLINE UINT32 d8_tx_2x_field_ATIN_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_ATIN_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 4:0) field ATIN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_MSK) >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_ATIN_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_range_ATIN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_ATIN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_ATIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_ATIN_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_ATIN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 4:0) field ATIN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
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
        /* (0x00000004 bits 4:0) field ATIN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
        d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_OFF + subfield_offset),
                                                 SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_ATIN_get( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_ATIN_get( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_ATIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_ATIN_get", stop_bit, 4 );
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
    /* (0x00000004 bits 4:0) field ATIN of register PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL */
    reg_value = d8_tx_2x_reg_ANALOG_TEST_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_MSK)
                  >> SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_MSK, SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_ATIN_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_burstsize2
 * ==================================================================================
 */
static INLINE void d8_tx_2x_lfield_GEN_USR_PAT_LSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_lfield_GEN_USR_PAT_LSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value[2] )
{
    IOLOG( "%s ", "d8_tx_2x_lfield_GEN_USR_PAT_LSB_set");
    d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_write( b_ptr,
                                                               h_ptr,
                                                               0,
                                                               2,
                                                               value);
}

static INLINE void d8_tx_2x_lfield_GEN_USR_PAT_LSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_lfield_GEN_USR_PAT_LSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value[2] )
{
    IOLOG( "%s ", "d8_tx_2x_lfield_GEN_USR_PAT_LSB_get");
    d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_read( b_ptr,
                                                              h_ptr,
                                                              0,
                                                              2,
                                                              (UINT32 *)value);
}

static INLINE void d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_set", start_bit, stop_bit, value);
    d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_field_set( b_ptr,
                                                             h_ptr,
                                                             word_number,
                                                             mask,
                                                             field_ofs,
                                                             value);
}

static INLINE UINT32 d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_get", start_bit, stop_bit );
    }
    reg_value = d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_read( b_ptr,
                                                                    h_ptr,
                                                                    word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void d8_tx_2x_lfield_MON_USR_PAT_LSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_lfield_MON_USR_PAT_LSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value[2] )
{
    IOLOG( "%s ", "d8_tx_2x_lfield_MON_USR_PAT_LSB_set");
    d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_write( b_ptr,
                                                               h_ptr,
                                                               0,
                                                               2,
                                                               value);
}

static INLINE void d8_tx_2x_lfield_MON_USR_PAT_LSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_lfield_MON_USR_PAT_LSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value[2] )
{
    IOLOG( "%s ", "d8_tx_2x_lfield_MON_USR_PAT_LSB_get");
    d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_read( b_ptr,
                                                              h_ptr,
                                                              0,
                                                              2,
                                                              (UINT32 *)value);
}

static INLINE void d8_tx_2x_lfield_range_MON_USR_PAT_LSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_lfield_range_MON_USR_PAT_LSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "d8_tx_2x_lfield_range_MON_USR_PAT_LSB_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "d8_tx_2x_lfield_range_MON_USR_PAT_LSB_set", start_bit, stop_bit, value);
    d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_field_set( b_ptr,
                                                             h_ptr,
                                                             word_number,
                                                             mask,
                                                             field_ofs,
                                                             value);
}

static INLINE UINT32 d8_tx_2x_lfield_range_MON_USR_PAT_LSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_lfield_range_MON_USR_PAT_LSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "d8_tx_2x_lfield_range_MON_USR_PAT_LSB_get", start_bit, stop_bit );
    }
    reg_value = d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_read( b_ptr,
                                                                    h_ptr,
                                                                    word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "d8_tx_2x_lfield_range_MON_USR_PAT_LSB_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size2
 * ==================================================================================
 */
static INLINE void d8_tx_2x_field_TX_SQUELCH_OVR_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_SQUELCH_OVR_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_SQUELCH_OVR_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_SQUELCH_OVR_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_SQUELCH_OVR_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 17) field TX_SQUELCH_OVR of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_SQUELCH_OVR_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_SQUELCH_OVR_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_SQUELCH_OVR_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 17) field TX_SQUELCH_OVR of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_SQUELCH_OVR_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_SQUELCH_OVR_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_SQUELCH_OVR_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_SQUELCH_OVR_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_SQUELCH_OVR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_SQUELCH_OVR_EN_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 16) field TX_SQUELCH_OVR_EN of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_EN_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_EN_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_SQUELCH_OVR_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                           d8_tx_2x_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_SQUELCH_OVR_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                           d8_tx_2x_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_SQUELCH_OVR_EN_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 16) field TX_SQUELCH_OVR_EN of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_EN_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_SQUELCH_OVR_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_SWING_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_SWING_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_SWING_set", N, 1);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_SWING_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_SWING_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 15:11) field TX_SWING of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_SWING_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_SWING_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_SWING_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 15:11) field TX_SWING of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_SWING_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_TX_SWING_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_TX_SWING_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_TX_SWING_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_SWING_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_SWING_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_SWING_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000014 + (N) * 0x4) bits 15:11) field TX_SWING of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
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
        /* ((0x00000014 + (N) * 0x4) bits 15:11) field TX_SWING of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
        d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              subfield_mask << (SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_OFF + subfield_offset),
                                                              SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_TX_SWING_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_TX_SWING_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_TX_SWING_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_SWING_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_SWING_get", stop_bit, 4 );
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
    /* ((0x00000014 + (N) * 0x4) bits 15:11) field TX_SWING of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_MSK)
                  >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_MSK, SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_SWING_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_ATMSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_ATMSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_ATMSB_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_ATMSB_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_ATMSB_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 8) field TX_ATMSB of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ATMSB_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ATMSB_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_ATMSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_ATMSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_ATMSB_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 8) field TX_ATMSB of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ATMSB_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ATMSB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_ATMSB_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_TEST_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_TEST_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_TEST_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_TEST_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_TEST_EN_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 4) field TX_TEST_EN of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_TEST_EN_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_TEST_EN_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_TEST_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_TEST_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_TEST_EN_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 4) field TX_TEST_EN of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_TEST_EN_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_TEST_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_TEST_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_PISO_PBUS_WIDTH_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_PISO_PBUS_WIDTH_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_PISO_PBUS_WIDTH_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_PISO_PBUS_WIDTH_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_PISO_PBUS_WIDTH_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 3) field TX_PISO_PBUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_PISO_PBUS_WIDTH_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_PISO_PBUS_WIDTH_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_PISO_PBUS_WIDTH_get( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_PISO_PBUS_WIDTH_get( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_PISO_PBUS_WIDTH_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 3) field TX_PISO_PBUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_PISO_PBUS_WIDTH_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_PISO_PBUS_WIDTH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_PISO_PBUS_WIDTH_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_PISO_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PISO_RSTB_set( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_PISO_RSTB_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PISO_RSTB_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_PISO_RSTB_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 2) field PISO_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_RSTB_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_RSTB_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_PISO_RSTB_get( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PISO_RSTB_get( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_PISO_RSTB_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 2) field PISO_RSTB of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_RSTB_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_RSTB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_PISO_RSTB_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_PISO_ENB_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PISO_ENB_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_PISO_ENB_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PISO_ENB_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_PISO_ENB_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 1) field PISO_ENB of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_ENB_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_ENB_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_PISO_ENB_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PISO_ENB_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_PISO_ENB_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 1) field PISO_ENB of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_ENB_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_ENB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_PISO_ENB_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_ENB_set( d8_tx_2x_buffer_t *b_ptr,
                                              d8_tx_2x_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_ENB_set( d8_tx_2x_buffer_t *b_ptr,
                                              d8_tx_2x_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_ENB_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_ENB_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_ENB_set", N, value );

    /* ((0x00000014 + (N) * 0x4) bits 0) field TX_ENB of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ENB_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ENB_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_ENB_get( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_ENB_get( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_ENB_get", N, 1);
    /* ((0x00000014 + (N) * 0x4) bits 0) field TX_ENB of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ENB_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ENB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_ENB_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_MODE_set", N, 1);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_MODE_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_MODE_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 31:8) field TX_MODE of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_MODE_get", N, 1);
    /* ((0x0000001c + (N) * 0x4) bits 31:8) field TX_MODE of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_MODE_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_TX_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_TX_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_TX_MODE_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_MODE_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_MODE_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_MODE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000001c + (N) * 0x4) bits 31:8) field TX_MODE of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x0000001c + (N) * 0x4) bits 31:8) field TX_MODE of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
        d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              subfield_mask << (SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_OFF + subfield_offset),
                                                              SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_TX_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_TX_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_TX_MODE_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_MODE_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_MODE_get", stop_bit, 23 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 23) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 23;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x0000001c + (N) * 0x4) bits 31:8) field TX_MODE of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_MSK)
                  >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_MSK, SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_MODE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_TX_CTRL_set( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TX_CTRL_set( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_CTRL_set", N, 1);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TX_CTRL_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TX_CTRL_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 7:0) field TX_CTRL of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
    d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_MSK,
                                                          SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_TX_CTRL_get( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TX_CTRL_get( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TX_CTRL_get", N, 1);
    /* ((0x0000001c + (N) * 0x4) bits 7:0) field TX_CTRL of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_MSK) >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TX_CTRL_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_TX_CTRL_set( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_TX_CTRL_set( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_TX_CTRL_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_CTRL_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_CTRL_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_CTRL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000001c + (N) * 0x4) bits 7:0) field TX_CTRL of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
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
        /* ((0x0000001c + (N) * 0x4) bits 7:0) field TX_CTRL of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
        d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              subfield_mask << (SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_OFF + subfield_offset),
                                                              SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_TX_CTRL_get( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_TX_CTRL_get( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_TX_CTRL_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_TX_CTRL_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_TX_CTRL_get", stop_bit, 7 );
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
    /* ((0x0000001c + (N) * 0x4) bits 7:0) field TX_CTRL of register PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 index N=0..1 */
    reg_value = d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_MSK)
                  >> SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_MSK, SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_TX_CTRL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_RD_PTR_SYNC_VAL_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_RD_PTR_SYNC_VAL_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_RD_PTR_SYNC_VAL_set", N, 1);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_RD_PTR_SYNC_VAL_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_RD_PTR_SYNC_VAL_set", N, value );

    /* ((0x00000024 + (N) * 0x4) bits 7:4) field RD_PTR_SYNC_VAL of register PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG index N=0..1 */
    d8_tx_2x_reg_DESKEW_FIFO_CFG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_MSK,
                                                  SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_OFF,
                                                  value);
}

static INLINE UINT32 d8_tx_2x_field_RD_PTR_SYNC_VAL_get( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_RD_PTR_SYNC_VAL_get( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_RD_PTR_SYNC_VAL_get", N, 1);
    /* ((0x00000024 + (N) * 0x4) bits 7:4) field RD_PTR_SYNC_VAL of register PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DESKEW_FIFO_CFG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_MSK) >> SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_RD_PTR_SYNC_VAL_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_RD_PTR_SYNC_VAL_set( d8_tx_2x_buffer_t *b_ptr,
                                                             d8_tx_2x_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_RD_PTR_SYNC_VAL_set( d8_tx_2x_buffer_t *b_ptr,
                                                             d8_tx_2x_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_RD_PTR_SYNC_VAL_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_RD_PTR_SYNC_VAL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_RD_PTR_SYNC_VAL_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_RD_PTR_SYNC_VAL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000024 + (N) * 0x4) bits 7:4) field RD_PTR_SYNC_VAL of register PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG index N=0..1 */
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
        /* ((0x00000024 + (N) * 0x4) bits 7:4) field RD_PTR_SYNC_VAL of register PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG index N=0..1 */
        d8_tx_2x_reg_DESKEW_FIFO_CFG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_OFF + subfield_offset),
                                                      SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_RD_PTR_SYNC_VAL_get( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_RD_PTR_SYNC_VAL_get( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_RD_PTR_SYNC_VAL_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_RD_PTR_SYNC_VAL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_RD_PTR_SYNC_VAL_get", stop_bit, 3 );
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
    /* ((0x00000024 + (N) * 0x4) bits 7:4) field RD_PTR_SYNC_VAL of register PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DESKEW_FIFO_CFG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_MSK)
                  >> SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_MSK, SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_RD_PTR_SYNC_VAL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_RD_PTR_SYNC_set( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_RD_PTR_SYNC_set( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_RD_PTR_SYNC_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_RD_PTR_SYNC_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_RD_PTR_SYNC_set", N, value );

    /* ((0x00000024 + (N) * 0x4) bits 0) field RD_PTR_SYNC of register PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG index N=0..1 */
    d8_tx_2x_reg_DESKEW_FIFO_CFG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_MSK,
                                                  SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_OFF,
                                                  value);
}

static INLINE UINT32 d8_tx_2x_field_RD_PTR_SYNC_get( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_RD_PTR_SYNC_get( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_RD_PTR_SYNC_get", N, 1);
    /* ((0x00000024 + (N) * 0x4) bits 0) field RD_PTR_SYNC of register PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DESKEW_FIFO_CFG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_MSK) >> SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_RD_PTR_SYNC_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_INV_DAT_POL_set( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_INV_DAT_POL_set( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_INV_DAT_POL_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_INV_DAT_POL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_INV_DAT_POL_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 10) field INV_DAT_POL of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_INV_DAT_POL_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_INV_DAT_POL_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_INV_DAT_POL_get( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_INV_DAT_POL_get( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_INV_DAT_POL_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 10) field INV_DAT_POL of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_INV_DAT_POL_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_INV_DAT_POL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_INV_DAT_POL_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_TCLKOUT_SEL_set( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TCLKOUT_SEL_set( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TCLKOUT_SEL_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TCLKOUT_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TCLKOUT_SEL_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 9) field TCLKOUT_SEL of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TCLKOUT_SEL_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TCLKOUT_SEL_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_TCLKOUT_SEL_get( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TCLKOUT_SEL_get( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TCLKOUT_SEL_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 9) field TCLKOUT_SEL of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TCLKOUT_SEL_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TCLKOUT_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TCLKOUT_SEL_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 8) field PSLB_CLK_DEGLITCH_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PSLB_CLK_DEGLITCH_EN_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PSLB_CLK_DEGLITCH_EN_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 8) field PSLB_CLK_DEGLITCH_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PSLB_CLK_DEGLITCH_EN_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PSLB_CLK_DEGLITCH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                              d8_tx_2x_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 7) field TPCOUT_CLK_DEGLITCH_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TPCOUT_CLK_DEGLITCH_EN_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TPCOUT_CLK_DEGLITCH_EN_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 7) field TPCOUT_CLK_DEGLITCH_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TPCOUT_CLK_DEGLITCH_EN_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TPCOUT_CLK_DEGLITCH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_PATT_INSERT_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_INSERT_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_PATT_INSERT_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_INSERT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_PATT_INSERT_EN_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 6) field PATT_INSERT_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PATT_INSERT_EN_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PATT_INSERT_EN_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_INSERT_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_INSERT_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_PATT_INSERT_EN_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 6) field PATT_INSERT_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PATT_INSERT_EN_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PATT_INSERT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_PATT_INSERT_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_P2S_DLB_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_P2S_DLB_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_P2S_DLB_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_P2S_DLB_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_P2S_DLB_EN_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 5) field P2S_DLB_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_P2S_DLB_EN_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_P2S_DLB_EN_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_P2S_DLB_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_P2S_DLB_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_P2S_DLB_EN_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 5) field P2S_DLB_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_P2S_DLB_EN_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_P2S_DLB_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_P2S_DLB_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_RX2TX_DLB_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_RX2TX_DLB_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_RX2TX_DLB_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_RX2TX_DLB_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_RX2TX_DLB_EN_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 4) field RX2TX_DLB_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_RX2TX_DLB_EN_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_RX2TX_DLB_EN_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_RX2TX_DLB_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_RX2TX_DLB_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_RX2TX_DLB_EN_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 4) field RX2TX_DLB_EN of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_RX2TX_DLB_EN_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_RX2TX_DLB_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_RX2TX_DLB_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_PISO_BUS_WIDTH_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_PISO_BUS_WIDTH_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_PISO_BUS_WIDTH_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_PISO_BUS_WIDTH_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_GEN_PISO_BUS_WIDTH_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 3) field GEN_PISO_BUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_BUS_WIDTH_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_BUS_WIDTH_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_PISO_BUS_WIDTH_get( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_PISO_BUS_WIDTH_get( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_PISO_BUS_WIDTH_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 3) field GEN_PISO_BUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_BUS_WIDTH_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_BUS_WIDTH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_GEN_PISO_BUS_WIDTH_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_PISO_DIV_RATIO_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_PISO_DIV_RATIO_set( d8_tx_2x_buffer_t *b_ptr,
                                                          d8_tx_2x_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_PISO_DIV_RATIO_set", N, 1);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_PISO_DIV_RATIO_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_GEN_PISO_DIV_RATIO_set", N, value );

    /* ((0x0000002c + (N) * 0x4) bits 2:0) field GEN_PISO_DIV_RATIO of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_MSK,
                                               SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_OFF,
                                               value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_PISO_DIV_RATIO_get( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_PISO_DIV_RATIO_get( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_PISO_DIV_RATIO_get", N, 1);
    /* ((0x0000002c + (N) * 0x4) bits 2:0) field GEN_PISO_DIV_RATIO of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_MSK) >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_GEN_PISO_DIV_RATIO_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_set( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_set( d8_tx_2x_buffer_t *b_ptr,
                                                                d8_tx_2x_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000002c + (N) * 0x4) bits 2:0) field GEN_PISO_DIV_RATIO of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
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
        /* ((0x0000002c + (N) * 0x4) bits 2:0) field GEN_PISO_DIV_RATIO of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
        d8_tx_2x_reg_DATAPATH_CFG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_OFF + subfield_offset),
                                                   SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_get( d8_tx_2x_buffer_t *b_ptr,
                                                                  d8_tx_2x_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_get( d8_tx_2x_buffer_t *b_ptr,
                                                                  d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_get", stop_bit, 2 );
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
    /* ((0x0000002c + (N) * 0x4) bits 2:0) field GEN_PISO_DIV_RATIO of register PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG index N=0..1 */
    reg_value = d8_tx_2x_reg_DATAPATH_CFG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_MSK)
                  >> SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_MSK, SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_MON_BUS_WIDTH_set( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_MON_BUS_WIDTH_set( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_MON_BUS_WIDTH_set", N, 1);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_MON_BUS_WIDTH_set", value, 63);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_GEN_MON_BUS_WIDTH_set", N, value );

    /* ((0x00000034 + (N) * 0x4) bits 13:8) field GEN_MON_BUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_MSK,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_MON_BUS_WIDTH_get( d8_tx_2x_buffer_t *b_ptr,
                                                           d8_tx_2x_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_MON_BUS_WIDTH_get( d8_tx_2x_buffer_t *b_ptr,
                                                           d8_tx_2x_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_MON_BUS_WIDTH_get", N, 1);
    /* ((0x00000034 + (N) * 0x4) bits 13:8) field GEN_MON_BUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_GEN_MON_BUS_WIDTH_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_set( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_set( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_set", stop_bit, 5 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000034 + (N) * 0x4) bits 13:8) field GEN_MON_BUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
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
        /* ((0x00000034 + (N) * 0x4) bits 13:8) field GEN_MON_BUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
        d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              subfield_mask << (SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_OFF + subfield_offset),
                                                              SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_get( d8_tx_2x_buffer_t *b_ptr,
                                                                 d8_tx_2x_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_get( d8_tx_2x_buffer_t *b_ptr,
                                                                 d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_get", stop_bit, 5 );
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
    /* ((0x00000034 + (N) * 0x4) bits 13:8) field GEN_MON_BUS_WIDTH of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_MSK)
                  >> SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_MSK, SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_MODE_set", N, 1);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_MODE_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_GEN_MODE_set", N, value );

    /* ((0x00000034 + (N) * 0x4) bits 7:4) field GEN_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_MSK,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_MODE_get", N, 1);
    /* ((0x00000034 + (N) * 0x4) bits 7:4) field GEN_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_GEN_MODE_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_GEN_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_GEN_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_GEN_MODE_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_GEN_MODE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_GEN_MODE_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_GEN_MODE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000034 + (N) * 0x4) bits 7:4) field GEN_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
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
        /* ((0x00000034 + (N) * 0x4) bits 7:4) field GEN_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
        d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              subfield_mask << (SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_OFF + subfield_offset),
                                                              SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_GEN_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_GEN_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_GEN_MODE_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_GEN_MODE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_GEN_MODE_get", stop_bit, 3 );
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
    /* ((0x00000034 + (N) * 0x4) bits 7:4) field GEN_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_MSK)
                  >> SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_MSK, SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_GEN_MODE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_ERR_INJ_set( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_ERR_INJ_set( d8_tx_2x_buffer_t *b_ptr,
                                                   d8_tx_2x_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_ERR_INJ_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_ERR_INJ_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_GEN_ERR_INJ_set", N, value );

    /* ((0x00000034 + (N) * 0x4) bits 2) field GEN_ERR_INJ of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_ERR_INJ_MSK,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_ERR_INJ_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_ERR_INJ_get( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_ERR_INJ_get( d8_tx_2x_buffer_t *b_ptr,
                                                     d8_tx_2x_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_ERR_INJ_get", N, 1);
    /* ((0x00000034 + (N) * 0x4) bits 2) field GEN_ERR_INJ of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_ERR_INJ_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_ERR_INJ_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_GEN_ERR_INJ_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_INV_set( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_INV_set( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_INV_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_GEN_INV_set", N, value );

    /* ((0x00000034 + (N) * 0x4) bits 1) field GEN_INV of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_INV_MSK,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_INV_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_INV_get( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_INV_get( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_INV_get", N, 1);
    /* ((0x00000034 + (N) * 0x4) bits 1) field GEN_INV of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_INV_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_GEN_INV_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                              d8_tx_2x_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                              d8_tx_2x_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_GEN_EN_set", N, value );

    /* ((0x00000034 + (N) * 0x4) bits 0) field GEN_EN of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_EN_MSK,
                                                          SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_EN_OFF,
                                                          value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_EN_get", N, 1);
    /* ((0x00000034 + (N) * 0x4) bits 0) field GEN_EN of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_EN_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_GEN_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_GEN_USR_PAT_MSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_GEN_USR_PAT_MSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_USR_PAT_MSB_set", N, 1);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_GEN_USR_PAT_MSB_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_GEN_USR_PAT_MSB_set", N, value );

    /* ((0x00000044 + (N) * 0x4) bits 7:0) field GEN_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2 index N=0..1 */
    d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_field_set( b_ptr,
                                                             h_ptr,
                                                             N,
                                                             SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_MSK,
                                                             SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_OFF,
                                                             value);
}

static INLINE UINT32 d8_tx_2x_field_GEN_USR_PAT_MSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_GEN_USR_PAT_MSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_GEN_USR_PAT_MSB_get", N, 1);
    /* ((0x00000044 + (N) * 0x4) bits 7:0) field GEN_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_read( b_ptr,
                                                                    h_ptr,
                                                                    N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_GEN_USR_PAT_MSB_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_GEN_USR_PAT_MSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                             d8_tx_2x_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_GEN_USR_PAT_MSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                             d8_tx_2x_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_GEN_USR_PAT_MSB_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_GEN_USR_PAT_MSB_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_GEN_USR_PAT_MSB_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_GEN_USR_PAT_MSB_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000044 + (N) * 0x4) bits 7:0) field GEN_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2 index N=0..1 */
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
        /* ((0x00000044 + (N) * 0x4) bits 7:0) field GEN_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2 index N=0..1 */
        d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 subfield_mask << (SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_OFF + subfield_offset),
                                                                 SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_GEN_USR_PAT_MSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_GEN_USR_PAT_MSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_GEN_USR_PAT_MSB_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_GEN_USR_PAT_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_GEN_USR_PAT_MSB_get", stop_bit, 7 );
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
    /* ((0x00000044 + (N) * 0x4) bits 7:0) field GEN_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_read( b_ptr,
                                                                    h_ptr,
                                                                    N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_MSK)
                  >> SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_MSK, SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_GEN_USR_PAT_MSB_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_MON_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_MON_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_MODE_set", N, 1);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_MON_MODE_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_MON_MODE_set", N, value );

    /* ((0x0000004c + (N) * 0x4) bits 7:4) field MON_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_MSK,
                                                    SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_OFF,
                                                    value);
}

static INLINE UINT32 d8_tx_2x_field_MON_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_MON_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_MODE_get", N, 1);
    /* ((0x0000004c + (N) * 0x4) bits 7:4) field MON_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_MON_MODE_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_MON_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_MON_MODE_set( d8_tx_2x_buffer_t *b_ptr,
                                                      d8_tx_2x_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_MON_MODE_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_MON_MODE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_MON_MODE_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_MON_MODE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000004c + (N) * 0x4) bits 7:4) field MON_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
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
        /* ((0x0000004c + (N) * 0x4) bits 7:4) field MON_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
        d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_OFF + subfield_offset),
                                                        SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_MON_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_MON_MODE_get( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_MON_MODE_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_MON_MODE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_MON_MODE_get", stop_bit, 3 );
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
    /* ((0x0000004c + (N) * 0x4) bits 7:4) field MON_MODE of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_MSK)
                  >> SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_MSK, SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_MON_MODE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void d8_tx_2x_field_MON_READ_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_MON_READ_set( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_READ_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_MON_READ_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_MON_READ_set", N, value );

    /* ((0x0000004c + (N) * 0x4) bits 2) field MON_READ of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_READ_MSK,
                                                    SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_READ_OFF,
                                                    value);
}

static INLINE UINT32 d8_tx_2x_field_MON_READ_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_MON_READ_get( d8_tx_2x_buffer_t *b_ptr,
                                                  d8_tx_2x_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_READ_get", N, 1);
    /* ((0x0000004c + (N) * 0x4) bits 2) field MON_READ of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_READ_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_READ_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_MON_READ_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_MON_INV_set( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_MON_INV_set( d8_tx_2x_buffer_t *b_ptr,
                                               d8_tx_2x_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_INV_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_MON_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_MON_INV_set", N, value );

    /* ((0x0000004c + (N) * 0x4) bits 1) field MON_INV of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_INV_MSK,
                                                    SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_INV_OFF,
                                                    value);
}

static INLINE UINT32 d8_tx_2x_field_MON_INV_get( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_MON_INV_get( d8_tx_2x_buffer_t *b_ptr,
                                                 d8_tx_2x_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_INV_get", N, 1);
    /* ((0x0000004c + (N) * 0x4) bits 1) field MON_INV of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_INV_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_MON_INV_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_MON_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                              d8_tx_2x_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_MON_EN_set( d8_tx_2x_buffer_t *b_ptr,
                                              d8_tx_2x_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_EN_set", N, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_MON_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_MON_EN_set", N, value );

    /* ((0x0000004c + (N) * 0x4) bits 0) field MON_EN of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_EN_MSK,
                                                    SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_EN_OFF,
                                                    value);
}

static INLINE UINT32 d8_tx_2x_field_MON_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_MON_EN_get( d8_tx_2x_buffer_t *b_ptr,
                                                d8_tx_2x_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_EN_get", N, 1);
    /* ((0x0000004c + (N) * 0x4) bits 0) field MON_EN of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_EN_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_MON_EN_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_MON_USR_PAT_MSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_MON_USR_PAT_MSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                       d8_tx_2x_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_USR_PAT_MSB_set", N, 1);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_MON_USR_PAT_MSB_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "d8_tx_2x_field_MON_USR_PAT_MSB_set", N, value );

    /* ((0x0000005c + (N) * 0x4) bits 7:0) field MON_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2 index N=0..1 */
    d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_field_set( b_ptr,
                                                             h_ptr,
                                                             N,
                                                             SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_MSK,
                                                             SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_OFF,
                                                             value);
}

static INLINE UINT32 d8_tx_2x_field_MON_USR_PAT_MSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_MON_USR_PAT_MSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                         d8_tx_2x_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_MON_USR_PAT_MSB_get", N, 1);
    /* ((0x0000005c + (N) * 0x4) bits 7:0) field MON_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_read( b_ptr,
                                                                    h_ptr,
                                                                    N);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_MSK) >> SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "d8_tx_2x_field_MON_USR_PAT_MSB_get", N, value );

    return value;
}
static INLINE void d8_tx_2x_field_range_MON_USR_PAT_MSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                             d8_tx_2x_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_range_MON_USR_PAT_MSB_set( d8_tx_2x_buffer_t *b_ptr,
                                                             d8_tx_2x_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_MON_USR_PAT_MSB_set", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_MON_USR_PAT_MSB_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_MON_USR_PAT_MSB_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_MON_USR_PAT_MSB_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000005c + (N) * 0x4) bits 7:0) field MON_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2 index N=0..1 */
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
        /* ((0x0000005c + (N) * 0x4) bits 7:0) field MON_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2 index N=0..1 */
        d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 subfield_mask << (SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_OFF + subfield_offset),
                                                                 SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 d8_tx_2x_field_range_MON_USR_PAT_MSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_range_MON_USR_PAT_MSB_get( d8_tx_2x_buffer_t *b_ptr,
                                                               d8_tx_2x_handle_t *h_ptr,
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

    if (N > 1)
        IO_RANGE_CHECK("%s N is %d but max is %d", "d8_tx_2x_field_range_MON_USR_PAT_MSB_get", N, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "d8_tx_2x_field_range_MON_USR_PAT_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "d8_tx_2x_field_range_MON_USR_PAT_MSB_get", stop_bit, 7 );
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
    /* ((0x0000005c + (N) * 0x4) bits 7:0) field MON_USR_PAT_MSB of register PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2 index N=0..1 */
    reg_value = d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_read( b_ptr,
                                                                    h_ptr,
                                                                    N);
    field_value = (reg_value & SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_MSK)
                  >> SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_MSK, SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "d8_tx_2x_field_range_MON_USR_PAT_MSB_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void d8_tx_2x_field_PATT_MON_ERR_B_E_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_MON_ERR_B_E_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_E_set", value );

    /* (0x0000000c bits 1) field PATT_MON_ERR_B_E of register PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG */
    d8_tx_2x_reg_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_B_E_MSK,
                                       SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_B_E_OFF,
                                       value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_B_E_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_B_E_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 1) field PATT_MON_ERR_B_E of register PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG */
    reg_value = d8_tx_2x_reg_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_B_E_MSK) >> SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_B_E_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_E_get", value );

    return value;
}
static INLINE void d8_tx_2x_field_PATT_MON_ERR_A_E_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_MON_ERR_A_E_set( d8_tx_2x_buffer_t *b_ptr,
                                                        d8_tx_2x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_E_set", value );

    /* (0x0000000c bits 0) field PATT_MON_ERR_A_E of register PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG */
    d8_tx_2x_reg_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_A_E_MSK,
                                       SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_A_E_OFF,
                                       value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_A_E_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_A_E_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 0) field PATT_MON_ERR_A_E of register PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG */
    reg_value = d8_tx_2x_reg_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_A_E_MSK) >> SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_A_E_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void d8_tx_2x_field_PATT_MON_ERR_B_I_set_to_clear( d8_tx_2x_buffer_t *b_ptr,
                                                                 d8_tx_2x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_MON_ERR_B_I_set_to_clear( d8_tx_2x_buffer_t *b_ptr,
                                                                 d8_tx_2x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_I_set_to_clear", value );

    /* (0x00000008 bits 1) field PATT_MON_ERR_B_I of register PMC_SFI51_TX_2X_SLICE_REG_INT_REG */
    d8_tx_2x_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_B_I_MSK,
                                                    SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_B_I_OFF,
                                                    value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_B_I_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_B_I_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 1) field PATT_MON_ERR_B_I of register PMC_SFI51_TX_2X_SLICE_REG_INT_REG */
    reg_value = d8_tx_2x_reg_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_B_I_MSK) >> SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_B_I_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_B_I_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_B_I_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_I_poll", value );

    /* (0x00000008 bits 1) field PATT_MON_ERR_B_I of register PMC_SFI51_TX_2X_SLICE_REG_INT_REG */
    return d8_tx_2x_reg_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_B_I_MSK,
                                      (value<<SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_B_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void d8_tx_2x_field_PATT_MON_ERR_A_I_set_to_clear( d8_tx_2x_buffer_t *b_ptr,
                                                                 d8_tx_2x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_field_PATT_MON_ERR_A_I_set_to_clear( d8_tx_2x_buffer_t *b_ptr,
                                                                 d8_tx_2x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_I_set_to_clear", value );

    /* (0x00000008 bits 0) field PATT_MON_ERR_A_I of register PMC_SFI51_TX_2X_SLICE_REG_INT_REG */
    d8_tx_2x_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_A_I_MSK,
                                                    SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_A_I_OFF,
                                                    value);
}

static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_A_I_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_A_I_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field PATT_MON_ERR_A_I of register PMC_SFI51_TX_2X_SLICE_REG_INT_REG */
    reg_value = d8_tx_2x_reg_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_A_I_MSK) >> SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_A_I_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_A_I_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_A_I_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_I_poll", value );

    /* (0x00000008 bits 0) field PATT_MON_ERR_A_I of register PMC_SFI51_TX_2X_SLICE_REG_INT_REG */
    return d8_tx_2x_reg_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_A_I_MSK,
                                      (value<<SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_A_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_B_V_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_B_V_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 1) field PATT_MON_ERR_B_V of register PMC_SFI51_TX_2X_SLICE_REG_INT_STAT_REG */
    reg_value = d8_tx_2x_reg_INT_STAT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_B_V_MSK) >> SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_B_V_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_B_V_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_B_V_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "d8_tx_2x_field_PATT_MON_ERR_B_V_poll", value );

    /* (0x00000010 bits 1) field PATT_MON_ERR_B_V of register PMC_SFI51_TX_2X_SLICE_REG_INT_STAT_REG */
    return d8_tx_2x_reg_INT_STAT_REG_poll( b_ptr,
                                           h_ptr,
                                           SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_B_V_MSK,
                                           (value<<SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_B_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_A_V_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_field_PATT_MON_ERR_A_V_get( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 0) field PATT_MON_ERR_A_V of register PMC_SFI51_TX_2X_SLICE_REG_INT_STAT_REG */
    reg_value = d8_tx_2x_reg_INT_STAT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_A_V_MSK) >> SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_A_V_OFF;
    IOLOG( "%s -> 0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_A_V_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_A_V_poll( d8_tx_2x_buffer_t *b_ptr,
                                                                         d8_tx_2x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "d8_tx_2x_field_PATT_MON_ERR_A_V_poll", value );

    /* (0x00000010 bits 0) field PATT_MON_ERR_A_V of register PMC_SFI51_TX_2X_SLICE_REG_INT_STAT_REG */
    return d8_tx_2x_reg_INT_STAT_REG_poll( b_ptr,
                                           h_ptr,
                                           SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_A_V_MSK,
                                           (value<<SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_A_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_burstsize2
 * ==================================================================================
 */
static INLINE void d8_tx_2x_lfield_MON_ERR_CNT_get( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void d8_tx_2x_lfield_MON_ERR_CNT_get( d8_tx_2x_buffer_t *b_ptr,
                                                    d8_tx_2x_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    IOLOG( "%s ", "d8_tx_2x_lfield_MON_ERR_CNT_get");
    d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_burst_read( b_ptr,
                                                         h_ptr,
                                                         0,
                                                         2,
                                                         (UINT32 *)value);
}

static INLINE UINT32 d8_tx_2x_lfield_range_MON_ERR_CNT_get( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 d8_tx_2x_lfield_range_MON_ERR_CNT_get( d8_tx_2x_buffer_t *b_ptr,
                                                            d8_tx_2x_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "d8_tx_2x_lfield_range_MON_ERR_CNT_get", start_bit, stop_bit );
    }
    reg_value = d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_read( b_ptr,
                                                               h_ptr,
                                                               word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "d8_tx_2x_lfield_range_MON_ERR_CNT_get", start_bit, stop_bit, value );
    return value;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _D8_TX_2X_IO_INLINE_H */
