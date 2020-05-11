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
 *     and register accessor functions for the stl256 block
 *****************************************************************************/
#ifndef _STL256_IO_INLINE_H
#define _STL256_IO_INLINE_H

#include "cbrc_api.h"
#include "stl256_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define STL256_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for stl256
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
    cbrc_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} stl256_buffer_t;
static INLINE void stl256_buffer_init( stl256_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A ) ALWAYS_INLINE;
static INLINE void stl256_buffer_init( stl256_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x40) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "stl256_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void stl256_buffer_flush( stl256_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void stl256_buffer_flush( stl256_buffer_t *b_ptr )
{
    IOLOG( "stl256_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 stl256_reg_read( stl256_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 stl256_reg_read( stl256_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x40),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg);
    return value;
}
static INLINE void stl256_reg_write( stl256_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_reg_write( stl256_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x40),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                         value);
}

static INLINE void stl256_field_set( stl256_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_set( stl256_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x40),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void stl256_action_on_write_field_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_action_on_write_field_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x40),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                         value<<ofs);
}

static INLINE void stl256_burst_read( stl256_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void stl256_burst_read( stl256_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x40),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                          len,
                          value);
}

static INLINE void stl256_burst_write( stl256_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void stl256_burst_write( stl256_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x40),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE stl256_poll( stl256_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_poll( stl256_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x40),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for stl256
 * ==================================================================================
 */

static INLINE void stl256_reg_CTRL_write( stl256_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_reg_CTRL_write( stl256_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "stl256_reg_CTRL_write", value );
    stl256_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_STL256_REG_CTRL,
                      value);
}

static INLINE void stl256_reg_CTRL_field_set( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_reg_CTRL_field_set( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "stl256_reg_CTRL_field_set", A, mask, ofs, value );
    stl256_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_STL256_REG_CTRL,
                      mask,
                      PMC_STL256_REG_CTRL_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 stl256_reg_CTRL_read( stl256_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_reg_CTRL_read( stl256_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = stl256_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_STL256_REG_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "stl256_reg_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void stl256_reg_INT_EN_write( stl256_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_reg_INT_EN_write( stl256_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "stl256_reg_INT_EN_write", value );
    stl256_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_STL256_REG_INT_EN,
                      value);
}

static INLINE void stl256_reg_INT_EN_field_set( stl256_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_reg_INT_EN_field_set( stl256_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "stl256_reg_INT_EN_field_set", A, mask, ofs, value );
    stl256_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_STL256_REG_INT_EN,
                      mask,
                      PMC_STL256_REG_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 stl256_reg_INT_EN_read( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_reg_INT_EN_read( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = stl256_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_STL256_REG_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "stl256_reg_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void stl256_reg_INT_STA_write( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_reg_INT_STA_write( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "stl256_reg_INT_STA_write", value );
    stl256_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_STL256_REG_INT_STA,
                      value);
}

static INLINE void stl256_reg_INT_STA_action_on_write_field_set( stl256_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_reg_INT_STA_action_on_write_field_set( stl256_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "stl256_reg_INT_STA_action_on_write_field_set", A, mask, ofs, value );
    stl256_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_STL256_REG_INT_STA,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 stl256_reg_INT_STA_read( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_reg_INT_STA_read( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = stl256_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_STL256_REG_INT_STA);

    IOLOG( "%s -> 0x%08x; A=%d", "stl256_reg_INT_STA_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_reg_INT_STA_poll( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_reg_INT_STA_poll( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "stl256_reg_INT_STA_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return stl256_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_STL256_REG_INT_STA,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 stl256_reg_INT_VAL_read( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_reg_INT_VAL_read( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = stl256_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_STL256_REG_INT_VAL);

    IOLOG( "%s -> 0x%08x; A=%d", "stl256_reg_INT_VAL_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_reg_INT_VAL_poll( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_reg_INT_VAL_poll( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "stl256_reg_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return stl256_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_STL256_REG_INT_VAL,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void stl256_field_FORCE_DESKEW_REALIGNMENT_set( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_FORCE_DESKEW_REALIGNMENT_set( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_FORCE_DESKEW_REALIGNMENT_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_FORCE_DESKEW_REALIGNMENT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_FORCE_DESKEW_REALIGNMENT_set", A, value );

    /* (0x00000400 bits 8) field FORCE_DESKEW_REALIGNMENT of register PMC_STL256_REG_CTRL */
    stl256_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               A,
                               STL256_REG_CTRL_BIT_FORCE_DESKEW_REALIGNMENT_MSK,
                               STL256_REG_CTRL_BIT_FORCE_DESKEW_REALIGNMENT_OFF,
                               value);
}

static INLINE UINT32 stl256_field_FORCE_DESKEW_REALIGNMENT_get( stl256_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_FORCE_DESKEW_REALIGNMENT_get( stl256_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_FORCE_DESKEW_REALIGNMENT_get", A, 2);
    /* (0x00000400 bits 8) field FORCE_DESKEW_REALIGNMENT of register PMC_STL256_REG_CTRL */
    reg_value = stl256_reg_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_CTRL_BIT_FORCE_DESKEW_REALIGNMENT_MSK) >> STL256_REG_CTRL_BIT_FORCE_DESKEW_REALIGNMENT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_FORCE_DESKEW_REALIGNMENT_get", A, value );

    return value;
}
static INLINE void stl256_field_AIS_ACT_DIS_set( stl256_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_AIS_ACT_DIS_set( stl256_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_AIS_ACT_DIS_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_AIS_ACT_DIS_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_AIS_ACT_DIS_set", A, value );

    /* (0x00000400 bits 6) field AIS_ACT_DIS of register PMC_STL256_REG_CTRL */
    stl256_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               A,
                               STL256_REG_CTRL_BIT_AIS_ACT_DIS_MSK,
                               STL256_REG_CTRL_BIT_AIS_ACT_DIS_OFF,
                               value);
}

static INLINE UINT32 stl256_field_AIS_ACT_DIS_get( stl256_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_AIS_ACT_DIS_get( stl256_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_AIS_ACT_DIS_get", A, 2);
    /* (0x00000400 bits 6) field AIS_ACT_DIS of register PMC_STL256_REG_CTRL */
    reg_value = stl256_reg_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_CTRL_BIT_AIS_ACT_DIS_MSK) >> STL256_REG_CTRL_BIT_AIS_ACT_DIS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_AIS_ACT_DIS_get", A, value );

    return value;
}
static INLINE void stl256_field_FORCE_AIS_set( stl256_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_FORCE_AIS_set( stl256_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_FORCE_AIS_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_FORCE_AIS_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_FORCE_AIS_set", A, value );

    /* (0x00000400 bits 5) field FORCE_AIS of register PMC_STL256_REG_CTRL */
    stl256_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               A,
                               STL256_REG_CTRL_BIT_FORCE_AIS_MSK,
                               STL256_REG_CTRL_BIT_FORCE_AIS_OFF,
                               value);
}

static INLINE UINT32 stl256_field_FORCE_AIS_get( stl256_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_FORCE_AIS_get( stl256_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_FORCE_AIS_get", A, 2);
    /* (0x00000400 bits 5) field FORCE_AIS of register PMC_STL256_REG_CTRL */
    reg_value = stl256_reg_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_CTRL_BIT_FORCE_AIS_MSK) >> STL256_REG_CTRL_BIT_FORCE_AIS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_FORCE_AIS_get", A, value );

    return value;
}
static INLINE void stl256_field_AF_THRESH_set( stl256_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_AF_THRESH_set( stl256_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_AF_THRESH_set", A, 2);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_AF_THRESH_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_AF_THRESH_set", A, value );

    /* (0x00000400 bits 4:0) field AF_THRESH of register PMC_STL256_REG_CTRL */
    stl256_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               A,
                               STL256_REG_CTRL_BIT_AF_THRESH_MSK,
                               STL256_REG_CTRL_BIT_AF_THRESH_OFF,
                               value);
}

static INLINE UINT32 stl256_field_AF_THRESH_get( stl256_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_AF_THRESH_get( stl256_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_AF_THRESH_get", A, 2);
    /* (0x00000400 bits 4:0) field AF_THRESH of register PMC_STL256_REG_CTRL */
    reg_value = stl256_reg_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_CTRL_BIT_AF_THRESH_MSK) >> STL256_REG_CTRL_BIT_AF_THRESH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_AF_THRESH_get", A, value );

    return value;
}
static INLINE void stl256_field_range_AF_THRESH_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_range_AF_THRESH_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_range_AF_THRESH_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "stl256_field_range_AF_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "stl256_field_range_AF_THRESH_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "stl256_field_range_AF_THRESH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000400 bits 4:0) field AF_THRESH of register PMC_STL256_REG_CTRL */
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
        /* (0x00000400 bits 4:0) field AF_THRESH of register PMC_STL256_REG_CTRL */
        stl256_reg_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   subfield_mask << (STL256_REG_CTRL_BIT_AF_THRESH_OFF + subfield_offset),
                                   STL256_REG_CTRL_BIT_AF_THRESH_OFF + subfield_offset,
                                   value >> subfield_shift);
    }
}

static INLINE UINT32 stl256_field_range_AF_THRESH_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_range_AF_THRESH_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_range_AF_THRESH_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "stl256_field_range_AF_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "stl256_field_range_AF_THRESH_get", stop_bit, 4 );
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
    /* (0x00000400 bits 4:0) field AF_THRESH of register PMC_STL256_REG_CTRL */
    reg_value = stl256_reg_CTRL_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & STL256_REG_CTRL_BIT_AF_THRESH_MSK)
                  >> STL256_REG_CTRL_BIT_AF_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, STL256_REG_CTRL_BIT_AF_THRESH_MSK, STL256_REG_CTRL_BIT_AF_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "stl256_field_range_AF_THRESH_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void stl256_field_EXCESSIVE_SKEW_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_EXCESSIVE_SKEW_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_EXCESSIVE_SKEW_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_EXCESSIVE_SKEW_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_EXCESSIVE_SKEW_E_set", A, value );

    /* (0x0000040c bits 30) field EXCESSIVE_SKEW_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_EXCESSIVE_SKEW_E_MSK,
                                 STL256_REG_INT_EN_BIT_EXCESSIVE_SKEW_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_EXCESSIVE_SKEW_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_EXCESSIVE_SKEW_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_EXCESSIVE_SKEW_E_get", A, 2);
    /* (0x0000040c bits 30) field EXCESSIVE_SKEW_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_EXCESSIVE_SKEW_E_MSK) >> STL256_REG_INT_EN_BIT_EXCESSIVE_SKEW_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_EXCESSIVE_SKEW_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE_ID_NOT_CONSISTENT_E_set( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE_ID_NOT_CONSISTENT_E_set( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_ID_NOT_CONSISTENT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_E_set", A, value );

    /* (0x0000040c bits 29) field LANE_ID_NOT_CONSISTENT_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE_ID_NOT_CONSISTENT_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE_ID_NOT_CONSISTENT_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE_ID_NOT_CONSISTENT_E_get( stl256_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE_ID_NOT_CONSISTENT_E_get( stl256_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_ID_NOT_CONSISTENT_E_get", A, 2);
    /* (0x0000040c bits 29) field LANE_ID_NOT_CONSISTENT_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE_ID_NOT_CONSISTENT_E_MSK) >> STL256_REG_INT_EN_BIT_LANE_ID_NOT_CONSISTENT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_E_get", A, value );

    return value;
}
static INLINE void stl256_field_MARKERS_NOT_UNIQUE_E_set( stl256_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_MARKERS_NOT_UNIQUE_E_set( stl256_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_MARKERS_NOT_UNIQUE_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_E_set", A, value );

    /* (0x0000040c bits 28) field MARKERS_NOT_UNIQUE_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_MARKERS_NOT_UNIQUE_E_MSK,
                                 STL256_REG_INT_EN_BIT_MARKERS_NOT_UNIQUE_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_MARKERS_NOT_UNIQUE_E_get( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_MARKERS_NOT_UNIQUE_E_get( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_MARKERS_NOT_UNIQUE_E_get", A, 2);
    /* (0x0000040c bits 28) field MARKERS_NOT_UNIQUE_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_MARKERS_NOT_UNIQUE_E_MSK) >> STL256_REG_INT_EN_BIT_MARKERS_NOT_UNIQUE_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE3_OOR_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE3_OOR_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOR_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE3_OOR_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE3_OOR_STL_E_set", A, value );

    /* (0x0000040c bits 27) field LANE3_OOR_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE3_OOR_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE3_OOR_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE3_OOR_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_OOR_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOR_STL_E_get", A, 2);
    /* (0x0000040c bits 27) field LANE3_OOR_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE3_OOR_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE3_OOR_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_OOR_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE2_OOR_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE2_OOR_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOR_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE2_OOR_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE2_OOR_STL_E_set", A, value );

    /* (0x0000040c bits 26) field LANE2_OOR_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE2_OOR_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE2_OOR_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE2_OOR_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_OOR_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOR_STL_E_get", A, 2);
    /* (0x0000040c bits 26) field LANE2_OOR_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE2_OOR_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE2_OOR_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_OOR_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE1_OOR_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE1_OOR_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOR_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE1_OOR_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE1_OOR_STL_E_set", A, value );

    /* (0x0000040c bits 25) field LANE1_OOR_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE1_OOR_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE1_OOR_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE1_OOR_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_OOR_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOR_STL_E_get", A, 2);
    /* (0x0000040c bits 25) field LANE1_OOR_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE1_OOR_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE1_OOR_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_OOR_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE0_OOR_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE0_OOR_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOR_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE0_OOR_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE0_OOR_STL_E_set", A, value );

    /* (0x0000040c bits 24) field LANE0_OOR_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE0_OOR_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE0_OOR_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE0_OOR_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_OOR_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOR_STL_E_get", A, 2);
    /* (0x0000040c bits 24) field LANE0_OOR_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE0_OOR_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE0_OOR_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_OOR_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE3_DLOS_E_set( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE3_DLOS_E_set( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOS_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE3_DLOS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE3_DLOS_E_set", A, value );

    /* (0x0000040c bits 23) field LANE3_DLOS_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE3_DLOS_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE3_DLOS_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE3_DLOS_E_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_DLOS_E_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOS_E_get", A, 2);
    /* (0x0000040c bits 23) field LANE3_DLOS_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE3_DLOS_E_MSK) >> STL256_REG_INT_EN_BIT_LANE3_DLOS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_DLOS_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE2_DLOS_E_set( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE2_DLOS_E_set( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOS_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE2_DLOS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE2_DLOS_E_set", A, value );

    /* (0x0000040c bits 22) field LANE2_DLOS_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE2_DLOS_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE2_DLOS_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE2_DLOS_E_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_DLOS_E_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOS_E_get", A, 2);
    /* (0x0000040c bits 22) field LANE2_DLOS_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE2_DLOS_E_MSK) >> STL256_REG_INT_EN_BIT_LANE2_DLOS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_DLOS_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE1_DLOS_E_set( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE1_DLOS_E_set( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOS_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE1_DLOS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE1_DLOS_E_set", A, value );

    /* (0x0000040c bits 21) field LANE1_DLOS_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE1_DLOS_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE1_DLOS_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE1_DLOS_E_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_DLOS_E_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOS_E_get", A, 2);
    /* (0x0000040c bits 21) field LANE1_DLOS_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE1_DLOS_E_MSK) >> STL256_REG_INT_EN_BIT_LANE1_DLOS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_DLOS_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE0_DLOS_E_set( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE0_DLOS_E_set( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOS_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE0_DLOS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE0_DLOS_E_set", A, value );

    /* (0x0000040c bits 20) field LANE0_DLOS_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE0_DLOS_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE0_DLOS_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE0_DLOS_E_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_DLOS_E_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOS_E_get", A, 2);
    /* (0x0000040c bits 20) field LANE0_DLOS_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE0_DLOS_E_MSK) >> STL256_REG_INT_EN_BIT_LANE0_DLOS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_DLOS_E_get", A, value );

    return value;
}
static INLINE void stl256_field_ATSF_E_set( stl256_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_ATSF_E_set( stl256_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_ATSF_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_ATSF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_ATSF_E_set", A, value );

    /* (0x0000040c bits 18) field ATSF_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_ATSF_E_MSK,
                                 STL256_REG_INT_EN_BIT_ATSF_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_ATSF_E_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_ATSF_E_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_ATSF_E_get", A, 2);
    /* (0x0000040c bits 18) field ATSF_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_ATSF_E_MSK) >> STL256_REG_INT_EN_BIT_ATSF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_ATSF_E_get", A, value );

    return value;
}
static INLINE void stl256_field_DLOL_E_set( stl256_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_DLOL_E_set( stl256_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DLOL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_DLOL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_DLOL_E_set", A, value );

    /* (0x0000040c bits 17) field DLOL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_DLOL_E_MSK,
                                 STL256_REG_INT_EN_BIT_DLOL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_DLOL_E_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DLOL_E_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DLOL_E_get", A, 2);
    /* (0x0000040c bits 17) field DLOL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_DLOL_E_MSK) >> STL256_REG_INT_EN_BIT_DLOL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DLOL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_DSTL_AIS_E_set( stl256_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_DSTL_AIS_E_set( stl256_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DSTL_AIS_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_DSTL_AIS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_DSTL_AIS_E_set", A, value );

    /* (0x0000040c bits 16) field DSTL_AIS_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_DSTL_AIS_E_MSK,
                                 STL256_REG_INT_EN_BIT_DSTL_AIS_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_DSTL_AIS_E_get( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DSTL_AIS_E_get( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DSTL_AIS_E_get", A, 2);
    /* (0x0000040c bits 16) field DSTL_AIS_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_DSTL_AIS_E_MSK) >> STL256_REG_INT_EN_BIT_DSTL_AIS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DSTL_AIS_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE3_DLOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE3_DLOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOF_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE3_DLOF_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE3_DLOF_STL_E_set", A, value );

    /* (0x0000040c bits 15) field LANE3_DLOF_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE3_DLOF_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE3_DLOF_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE3_DLOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_DLOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOF_STL_E_get", A, 2);
    /* (0x0000040c bits 15) field LANE3_DLOF_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE3_DLOF_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE3_DLOF_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_DLOF_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE2_DLOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE2_DLOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOF_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE2_DLOF_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE2_DLOF_STL_E_set", A, value );

    /* (0x0000040c bits 14) field LANE2_DLOF_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE2_DLOF_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE2_DLOF_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE2_DLOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_DLOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOF_STL_E_get", A, 2);
    /* (0x0000040c bits 14) field LANE2_DLOF_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE2_DLOF_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE2_DLOF_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_DLOF_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE1_DLOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE1_DLOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOF_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE1_DLOF_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE1_DLOF_STL_E_set", A, value );

    /* (0x0000040c bits 13) field LANE1_DLOF_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE1_DLOF_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE1_DLOF_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE1_DLOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_DLOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOF_STL_E_get", A, 2);
    /* (0x0000040c bits 13) field LANE1_DLOF_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE1_DLOF_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE1_DLOF_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_DLOF_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE0_DLOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE0_DLOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOF_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE0_DLOF_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE0_DLOF_STL_E_set", A, value );

    /* (0x0000040c bits 12) field LANE0_DLOF_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE0_DLOF_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE0_DLOF_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE0_DLOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_DLOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOF_STL_E_get", A, 2);
    /* (0x0000040c bits 12) field LANE0_DLOF_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE0_DLOF_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE0_DLOF_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_DLOF_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE3_OOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE3_OOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOF_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE3_OOF_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE3_OOF_STL_E_set", A, value );

    /* (0x0000040c bits 11) field LANE3_OOF_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE3_OOF_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE3_OOF_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE3_OOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_OOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOF_STL_E_get", A, 2);
    /* (0x0000040c bits 11) field LANE3_OOF_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE3_OOF_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE3_OOF_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_OOF_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE2_OOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE2_OOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOF_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE2_OOF_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE2_OOF_STL_E_set", A, value );

    /* (0x0000040c bits 10) field LANE2_OOF_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE2_OOF_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE2_OOF_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE2_OOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_OOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOF_STL_E_get", A, 2);
    /* (0x0000040c bits 10) field LANE2_OOF_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE2_OOF_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE2_OOF_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_OOF_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE1_OOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE1_OOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOF_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE1_OOF_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE1_OOF_STL_E_set", A, value );

    /* (0x0000040c bits 9) field LANE1_OOF_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE1_OOF_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE1_OOF_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE1_OOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_OOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOF_STL_E_get", A, 2);
    /* (0x0000040c bits 9) field LANE1_OOF_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE1_OOF_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE1_OOF_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_OOF_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE0_OOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE0_OOF_STL_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOF_STL_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE0_OOF_STL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE0_OOF_STL_E_set", A, value );

    /* (0x0000040c bits 8) field LANE0_OOF_STL_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE0_OOF_STL_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE0_OOF_STL_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE0_OOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_OOF_STL_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOF_STL_E_get", A, 2);
    /* (0x0000040c bits 8) field LANE0_OOF_STL_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE0_OOF_STL_E_MSK) >> STL256_REG_INT_EN_BIT_LANE0_OOF_STL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_OOF_STL_E_get", A, value );

    return value;
}
static INLINE void stl256_field_LANE_FIFO_OVR_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE_FIFO_OVR_E_set( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_FIFO_OVR_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE_FIFO_OVR_E_set", A, value );

    /* (0x0000040c bits 4) field LANE_FIFO_OVR_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_LANE_FIFO_OVR_E_MSK,
                                 STL256_REG_INT_EN_BIT_LANE_FIFO_OVR_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_LANE_FIFO_OVR_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE_FIFO_OVR_E_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_FIFO_OVR_E_get", A, 2);
    /* (0x0000040c bits 4) field LANE_FIFO_OVR_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_LANE_FIFO_OVR_E_MSK) >> STL256_REG_INT_EN_BIT_LANE_FIFO_OVR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE_FIFO_OVR_E_get", A, value );

    return value;
}
static INLINE void stl256_field_OOF_E_set( stl256_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_OOF_E_set( stl256_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_OOF_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_OOF_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_OOF_E_set", A, value );

    /* (0x0000040c bits 1) field OOF_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_OOF_E_MSK,
                                 STL256_REG_INT_EN_BIT_OOF_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_OOF_E_get( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_OOF_E_get( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_OOF_E_get", A, 2);
    /* (0x0000040c bits 1) field OOF_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_OOF_E_MSK) >> STL256_REG_INT_EN_BIT_OOF_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_OOF_E_get", A, value );

    return value;
}
static INLINE void stl256_field_DAIS_E_set( stl256_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_DAIS_E_set( stl256_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DAIS_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_DAIS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_DAIS_E_set", A, value );

    /* (0x0000040c bits 0) field DAIS_E of register PMC_STL256_REG_INT_EN */
    stl256_reg_INT_EN_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 STL256_REG_INT_EN_BIT_DAIS_E_MSK,
                                 STL256_REG_INT_EN_BIT_DAIS_E_OFF,
                                 value);
}

static INLINE UINT32 stl256_field_DAIS_E_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DAIS_E_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DAIS_E_get", A, 2);
    /* (0x0000040c bits 0) field DAIS_E of register PMC_STL256_REG_INT_EN */
    reg_value = stl256_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_EN_BIT_DAIS_E_MSK) >> STL256_REG_INT_EN_BIT_DAIS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DAIS_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void stl256_field_EXCESSIVE_SKEW_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_EXCESSIVE_SKEW_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_EXCESSIVE_SKEW_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_EXCESSIVE_SKEW_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_EXCESSIVE_SKEW_I_set_to_clear", A, value );

    /* (0x00000404 bits 30) field EXCESSIVE_SKEW_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_EXCESSIVE_SKEW_I_MSK,
                                                  STL256_REG_INT_STA_BIT_EXCESSIVE_SKEW_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_EXCESSIVE_SKEW_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_EXCESSIVE_SKEW_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_EXCESSIVE_SKEW_I_get", A, 2);
    /* (0x00000404 bits 30) field EXCESSIVE_SKEW_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_EXCESSIVE_SKEW_I_MSK) >> STL256_REG_INT_STA_BIT_EXCESSIVE_SKEW_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_EXCESSIVE_SKEW_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_EXCESSIVE_SKEW_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_EXCESSIVE_SKEW_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_EXCESSIVE_SKEW_I_poll", A, value );

    /* (0x00000404 bits 30) field EXCESSIVE_SKEW_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_EXCESSIVE_SKEW_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_EXCESSIVE_SKEW_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE_ID_NOT_CONSISTENT_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE_ID_NOT_CONSISTENT_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_ID_NOT_CONSISTENT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_I_set_to_clear", A, value );

    /* (0x00000404 bits 29) field LANE_ID_NOT_CONSISTENT_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE_ID_NOT_CONSISTENT_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE_ID_NOT_CONSISTENT_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE_ID_NOT_CONSISTENT_I_get( stl256_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE_ID_NOT_CONSISTENT_I_get( stl256_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_ID_NOT_CONSISTENT_I_get", A, 2);
    /* (0x00000404 bits 29) field LANE_ID_NOT_CONSISTENT_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE_ID_NOT_CONSISTENT_I_MSK) >> STL256_REG_INT_STA_BIT_LANE_ID_NOT_CONSISTENT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE_ID_NOT_CONSISTENT_I_poll( stl256_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE_ID_NOT_CONSISTENT_I_poll( stl256_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_I_poll", A, value );

    /* (0x00000404 bits 29) field LANE_ID_NOT_CONSISTENT_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE_ID_NOT_CONSISTENT_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE_ID_NOT_CONSISTENT_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_MARKERS_NOT_UNIQUE_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_MARKERS_NOT_UNIQUE_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_MARKERS_NOT_UNIQUE_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_I_set_to_clear", A, value );

    /* (0x00000404 bits 28) field MARKERS_NOT_UNIQUE_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_MARKERS_NOT_UNIQUE_I_MSK,
                                                  STL256_REG_INT_STA_BIT_MARKERS_NOT_UNIQUE_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_MARKERS_NOT_UNIQUE_I_get( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_MARKERS_NOT_UNIQUE_I_get( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_MARKERS_NOT_UNIQUE_I_get", A, 2);
    /* (0x00000404 bits 28) field MARKERS_NOT_UNIQUE_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_MARKERS_NOT_UNIQUE_I_MSK) >> STL256_REG_INT_STA_BIT_MARKERS_NOT_UNIQUE_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_MARKERS_NOT_UNIQUE_I_poll( stl256_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_MARKERS_NOT_UNIQUE_I_poll( stl256_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_I_poll", A, value );

    /* (0x00000404 bits 28) field MARKERS_NOT_UNIQUE_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_MARKERS_NOT_UNIQUE_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_MARKERS_NOT_UNIQUE_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE3_OOR_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE3_OOR_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOR_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE3_OOR_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE3_OOR_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 27) field LANE3_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE3_OOR_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE3_OOR_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE3_OOR_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_OOR_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOR_STL_I_get", A, 2);
    /* (0x00000404 bits 27) field LANE3_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE3_OOR_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE3_OOR_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_OOR_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOR_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOR_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE3_OOR_STL_I_poll", A, value );

    /* (0x00000404 bits 27) field LANE3_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE3_OOR_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE3_OOR_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE2_OOR_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE2_OOR_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOR_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE2_OOR_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE2_OOR_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 26) field LANE2_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE2_OOR_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE2_OOR_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE2_OOR_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_OOR_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOR_STL_I_get", A, 2);
    /* (0x00000404 bits 26) field LANE2_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE2_OOR_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE2_OOR_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_OOR_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOR_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOR_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE2_OOR_STL_I_poll", A, value );

    /* (0x00000404 bits 26) field LANE2_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE2_OOR_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE2_OOR_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE1_OOR_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE1_OOR_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOR_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE1_OOR_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE1_OOR_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 25) field LANE1_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE1_OOR_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE1_OOR_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE1_OOR_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_OOR_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOR_STL_I_get", A, 2);
    /* (0x00000404 bits 25) field LANE1_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE1_OOR_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE1_OOR_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_OOR_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOR_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOR_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE1_OOR_STL_I_poll", A, value );

    /* (0x00000404 bits 25) field LANE1_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE1_OOR_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE1_OOR_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE0_OOR_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE0_OOR_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOR_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE0_OOR_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE0_OOR_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 24) field LANE0_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE0_OOR_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE0_OOR_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE0_OOR_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_OOR_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOR_STL_I_get", A, 2);
    /* (0x00000404 bits 24) field LANE0_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE0_OOR_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE0_OOR_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_OOR_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOR_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOR_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE0_OOR_STL_I_poll", A, value );

    /* (0x00000404 bits 24) field LANE0_OOR_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE0_OOR_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE0_OOR_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE3_DLOS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE3_DLOS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOS_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE3_DLOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE3_DLOS_I_set_to_clear", A, value );

    /* (0x00000404 bits 23) field LANE3_DLOS_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE3_DLOS_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE3_DLOS_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE3_DLOS_I_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_DLOS_I_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOS_I_get", A, 2);
    /* (0x00000404 bits 23) field LANE3_DLOS_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE3_DLOS_I_MSK) >> STL256_REG_INT_STA_BIT_LANE3_DLOS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_DLOS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOS_I_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOS_I_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE3_DLOS_I_poll", A, value );

    /* (0x00000404 bits 23) field LANE3_DLOS_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE3_DLOS_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE3_DLOS_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE2_DLOS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE2_DLOS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOS_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE2_DLOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE2_DLOS_I_set_to_clear", A, value );

    /* (0x00000404 bits 22) field LANE2_DLOS_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE2_DLOS_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE2_DLOS_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE2_DLOS_I_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_DLOS_I_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOS_I_get", A, 2);
    /* (0x00000404 bits 22) field LANE2_DLOS_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE2_DLOS_I_MSK) >> STL256_REG_INT_STA_BIT_LANE2_DLOS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_DLOS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOS_I_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOS_I_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE2_DLOS_I_poll", A, value );

    /* (0x00000404 bits 22) field LANE2_DLOS_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE2_DLOS_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE2_DLOS_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE1_DLOS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE1_DLOS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOS_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE1_DLOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE1_DLOS_I_set_to_clear", A, value );

    /* (0x00000404 bits 21) field LANE1_DLOS_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE1_DLOS_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE1_DLOS_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE1_DLOS_I_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_DLOS_I_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOS_I_get", A, 2);
    /* (0x00000404 bits 21) field LANE1_DLOS_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE1_DLOS_I_MSK) >> STL256_REG_INT_STA_BIT_LANE1_DLOS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_DLOS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOS_I_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOS_I_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE1_DLOS_I_poll", A, value );

    /* (0x00000404 bits 21) field LANE1_DLOS_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE1_DLOS_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE1_DLOS_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE0_DLOS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE0_DLOS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOS_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE0_DLOS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE0_DLOS_I_set_to_clear", A, value );

    /* (0x00000404 bits 20) field LANE0_DLOS_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE0_DLOS_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE0_DLOS_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE0_DLOS_I_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_DLOS_I_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOS_I_get", A, 2);
    /* (0x00000404 bits 20) field LANE0_DLOS_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE0_DLOS_I_MSK) >> STL256_REG_INT_STA_BIT_LANE0_DLOS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_DLOS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOS_I_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOS_I_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE0_DLOS_I_poll", A, value );

    /* (0x00000404 bits 20) field LANE0_DLOS_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE0_DLOS_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE0_DLOS_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_ATSF_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_ATSF_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_ATSF_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_ATSF_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_ATSF_I_set_to_clear", A, value );

    /* (0x00000404 bits 18) field ATSF_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_ATSF_I_MSK,
                                                  STL256_REG_INT_STA_BIT_ATSF_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_ATSF_I_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_ATSF_I_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_ATSF_I_get", A, 2);
    /* (0x00000404 bits 18) field ATSF_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_ATSF_I_MSK) >> STL256_REG_INT_STA_BIT_ATSF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_ATSF_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_ATSF_I_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_ATSF_I_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_ATSF_I_poll", A, value );

    /* (0x00000404 bits 18) field ATSF_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_ATSF_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_ATSF_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_DLOL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_DLOL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DLOL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_DLOL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_DLOL_I_set_to_clear", A, value );

    /* (0x00000404 bits 17) field DLOL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_DLOL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_DLOL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_DLOL_I_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DLOL_I_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DLOL_I_get", A, 2);
    /* (0x00000404 bits 17) field DLOL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_DLOL_I_MSK) >> STL256_REG_INT_STA_BIT_DLOL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DLOL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DLOL_I_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DLOL_I_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_DLOL_I_poll", A, value );

    /* (0x00000404 bits 17) field DLOL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_DLOL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_DLOL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_DSTL_AIS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_DSTL_AIS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DSTL_AIS_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_DSTL_AIS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_DSTL_AIS_I_set_to_clear", A, value );

    /* (0x00000404 bits 16) field DSTL_AIS_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_DSTL_AIS_I_MSK,
                                                  STL256_REG_INT_STA_BIT_DSTL_AIS_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_DSTL_AIS_I_get( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DSTL_AIS_I_get( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DSTL_AIS_I_get", A, 2);
    /* (0x00000404 bits 16) field DSTL_AIS_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_DSTL_AIS_I_MSK) >> STL256_REG_INT_STA_BIT_DSTL_AIS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DSTL_AIS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DSTL_AIS_I_poll( stl256_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DSTL_AIS_I_poll( stl256_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_DSTL_AIS_I_poll", A, value );

    /* (0x00000404 bits 16) field DSTL_AIS_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_DSTL_AIS_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_DSTL_AIS_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE3_DLOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE3_DLOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOF_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE3_DLOF_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE3_DLOF_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 15) field LANE3_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE3_DLOF_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE3_DLOF_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE3_DLOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_DLOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOF_STL_I_get", A, 2);
    /* (0x00000404 bits 15) field LANE3_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE3_DLOF_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE3_DLOF_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_DLOF_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE3_DLOF_STL_I_poll", A, value );

    /* (0x00000404 bits 15) field LANE3_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE3_DLOF_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE3_DLOF_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE2_DLOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE2_DLOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOF_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE2_DLOF_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE2_DLOF_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 14) field LANE2_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE2_DLOF_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE2_DLOF_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE2_DLOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_DLOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOF_STL_I_get", A, 2);
    /* (0x00000404 bits 14) field LANE2_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE2_DLOF_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE2_DLOF_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_DLOF_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE2_DLOF_STL_I_poll", A, value );

    /* (0x00000404 bits 14) field LANE2_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE2_DLOF_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE2_DLOF_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE1_DLOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE1_DLOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOF_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE1_DLOF_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE1_DLOF_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 13) field LANE1_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE1_DLOF_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE1_DLOF_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE1_DLOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_DLOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOF_STL_I_get", A, 2);
    /* (0x00000404 bits 13) field LANE1_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE1_DLOF_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE1_DLOF_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_DLOF_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE1_DLOF_STL_I_poll", A, value );

    /* (0x00000404 bits 13) field LANE1_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE1_DLOF_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE1_DLOF_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE0_DLOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE0_DLOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOF_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE0_DLOF_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE0_DLOF_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 12) field LANE0_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE0_DLOF_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE0_DLOF_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE0_DLOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_DLOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOF_STL_I_get", A, 2);
    /* (0x00000404 bits 12) field LANE0_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE0_DLOF_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE0_DLOF_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_DLOF_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE0_DLOF_STL_I_poll", A, value );

    /* (0x00000404 bits 12) field LANE0_DLOF_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE0_DLOF_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE0_DLOF_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE3_OOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE3_OOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOF_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE3_OOF_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE3_OOF_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 11) field LANE3_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE3_OOF_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE3_OOF_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE3_OOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_OOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOF_STL_I_get", A, 2);
    /* (0x00000404 bits 11) field LANE3_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE3_OOF_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE3_OOF_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_OOF_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE3_OOF_STL_I_poll", A, value );

    /* (0x00000404 bits 11) field LANE3_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE3_OOF_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE3_OOF_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE2_OOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE2_OOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOF_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE2_OOF_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE2_OOF_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 10) field LANE2_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE2_OOF_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE2_OOF_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE2_OOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_OOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOF_STL_I_get", A, 2);
    /* (0x00000404 bits 10) field LANE2_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE2_OOF_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE2_OOF_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_OOF_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE2_OOF_STL_I_poll", A, value );

    /* (0x00000404 bits 10) field LANE2_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE2_OOF_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE2_OOF_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE1_OOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE1_OOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOF_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE1_OOF_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE1_OOF_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 9) field LANE1_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE1_OOF_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE1_OOF_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE1_OOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_OOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOF_STL_I_get", A, 2);
    /* (0x00000404 bits 9) field LANE1_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE1_OOF_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE1_OOF_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_OOF_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE1_OOF_STL_I_poll", A, value );

    /* (0x00000404 bits 9) field LANE1_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE1_OOF_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE1_OOF_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE0_OOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE0_OOF_STL_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOF_STL_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE0_OOF_STL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE0_OOF_STL_I_set_to_clear", A, value );

    /* (0x00000404 bits 8) field LANE0_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE0_OOF_STL_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE0_OOF_STL_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE0_OOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_OOF_STL_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOF_STL_I_get", A, 2);
    /* (0x00000404 bits 8) field LANE0_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE0_OOF_STL_I_MSK) >> STL256_REG_INT_STA_BIT_LANE0_OOF_STL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_OOF_STL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOF_STL_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE0_OOF_STL_I_poll", A, value );

    /* (0x00000404 bits 8) field LANE0_OOF_STL_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE0_OOF_STL_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE0_OOF_STL_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_LANE_FIFO_OVR_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_LANE_FIFO_OVR_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_FIFO_OVR_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_LANE_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_LANE_FIFO_OVR_I_set_to_clear", A, value );

    /* (0x00000404 bits 4) field LANE_FIFO_OVR_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_LANE_FIFO_OVR_I_MSK,
                                                  STL256_REG_INT_STA_BIT_LANE_FIFO_OVR_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_LANE_FIFO_OVR_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE_FIFO_OVR_I_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_FIFO_OVR_I_get", A, 2);
    /* (0x00000404 bits 4) field LANE_FIFO_OVR_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_LANE_FIFO_OVR_I_MSK) >> STL256_REG_INT_STA_BIT_LANE_FIFO_OVR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE_FIFO_OVR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE_FIFO_OVR_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE_FIFO_OVR_I_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE_FIFO_OVR_I_poll", A, value );

    /* (0x00000404 bits 4) field LANE_FIFO_OVR_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_LANE_FIFO_OVR_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_LANE_FIFO_OVR_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_OOF_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_OOF_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_OOF_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_OOF_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_OOF_I_set_to_clear", A, value );

    /* (0x00000404 bits 1) field OOF_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_OOF_I_MSK,
                                                  STL256_REG_INT_STA_BIT_OOF_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_OOF_I_get( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_OOF_I_get( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_OOF_I_get", A, 2);
    /* (0x00000404 bits 1) field OOF_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_OOF_I_MSK) >> STL256_REG_INT_STA_BIT_OOF_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_OOF_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_OOF_I_poll( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_OOF_I_poll( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_OOF_I_poll", A, value );

    /* (0x00000404 bits 1) field OOF_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_OOF_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_OOF_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void stl256_field_DAIS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void stl256_field_DAIS_I_set_to_clear( stl256_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DAIS_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "stl256_field_DAIS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "stl256_field_DAIS_I_set_to_clear", A, value );

    /* (0x00000404 bits 0) field DAIS_I of register PMC_STL256_REG_INT_STA */
    stl256_reg_INT_STA_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  STL256_REG_INT_STA_BIT_DAIS_I_MSK,
                                                  STL256_REG_INT_STA_BIT_DAIS_I_OFF,
                                                  value);
}

static INLINE UINT32 stl256_field_DAIS_I_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DAIS_I_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DAIS_I_get", A, 2);
    /* (0x00000404 bits 0) field DAIS_I of register PMC_STL256_REG_INT_STA */
    reg_value = stl256_reg_INT_STA_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_STA_BIT_DAIS_I_MSK) >> STL256_REG_INT_STA_BIT_DAIS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DAIS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DAIS_I_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DAIS_I_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_DAIS_I_poll", A, value );

    /* (0x00000404 bits 0) field DAIS_I of register PMC_STL256_REG_INT_STA */
    return stl256_reg_INT_STA_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_STA_BIT_DAIS_I_MSK,
                                    (value<<STL256_REG_INT_STA_BIT_DAIS_I_OFF),
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
static INLINE UINT32 stl256_field_EXCESSIVE_SKEW_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_EXCESSIVE_SKEW_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_EXCESSIVE_SKEW_V_get", A, 2);
    /* (0x00000414 bits 30) field EXCESSIVE_SKEW_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_EXCESSIVE_SKEW_V_MSK) >> STL256_REG_INT_VAL_BIT_EXCESSIVE_SKEW_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_EXCESSIVE_SKEW_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_EXCESSIVE_SKEW_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_EXCESSIVE_SKEW_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_EXCESSIVE_SKEW_V_poll", A, value );

    /* (0x00000414 bits 30) field EXCESSIVE_SKEW_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_EXCESSIVE_SKEW_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_EXCESSIVE_SKEW_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE_ID_NOT_CONSISTENT_V_get( stl256_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE_ID_NOT_CONSISTENT_V_get( stl256_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE_ID_NOT_CONSISTENT_V_get", A, 2);
    /* (0x00000414 bits 29) field LANE_ID_NOT_CONSISTENT_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE_ID_NOT_CONSISTENT_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE_ID_NOT_CONSISTENT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE_ID_NOT_CONSISTENT_V_poll( stl256_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE_ID_NOT_CONSISTENT_V_poll( stl256_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE_ID_NOT_CONSISTENT_V_poll", A, value );

    /* (0x00000414 bits 29) field LANE_ID_NOT_CONSISTENT_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE_ID_NOT_CONSISTENT_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE_ID_NOT_CONSISTENT_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_MARKERS_NOT_UNIQUE_V_get( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_MARKERS_NOT_UNIQUE_V_get( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_MARKERS_NOT_UNIQUE_V_get", A, 2);
    /* (0x00000414 bits 28) field MARKERS_NOT_UNIQUE_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_MARKERS_NOT_UNIQUE_V_MSK) >> STL256_REG_INT_VAL_BIT_MARKERS_NOT_UNIQUE_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_MARKERS_NOT_UNIQUE_V_poll( stl256_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_MARKERS_NOT_UNIQUE_V_poll( stl256_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_MARKERS_NOT_UNIQUE_V_poll", A, value );

    /* (0x00000414 bits 28) field MARKERS_NOT_UNIQUE_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_MARKERS_NOT_UNIQUE_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_MARKERS_NOT_UNIQUE_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE3_OOR_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_OOR_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOR_STL_V_get", A, 2);
    /* (0x00000414 bits 27) field LANE3_OOR_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE3_OOR_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE3_OOR_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_OOR_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOR_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOR_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE3_OOR_STL_V_poll", A, value );

    /* (0x00000414 bits 27) field LANE3_OOR_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE3_OOR_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE3_OOR_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE2_OOR_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_OOR_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOR_STL_V_get", A, 2);
    /* (0x00000414 bits 26) field LANE2_OOR_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE2_OOR_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE2_OOR_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_OOR_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOR_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOR_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE2_OOR_STL_V_poll", A, value );

    /* (0x00000414 bits 26) field LANE2_OOR_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE2_OOR_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE2_OOR_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE1_OOR_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_OOR_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOR_STL_V_get", A, 2);
    /* (0x00000414 bits 25) field LANE1_OOR_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE1_OOR_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE1_OOR_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_OOR_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOR_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOR_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE1_OOR_STL_V_poll", A, value );

    /* (0x00000414 bits 25) field LANE1_OOR_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE1_OOR_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE1_OOR_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE0_OOR_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_OOR_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOR_STL_V_get", A, 2);
    /* (0x00000414 bits 24) field LANE0_OOR_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE0_OOR_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE0_OOR_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_OOR_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOR_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOR_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE0_OOR_STL_V_poll", A, value );

    /* (0x00000414 bits 24) field LANE0_OOR_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE0_OOR_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE0_OOR_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE3_DLOS_V_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_DLOS_V_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOS_V_get", A, 2);
    /* (0x00000414 bits 23) field LANE3_DLOS_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE3_DLOS_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE3_DLOS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_DLOS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOS_V_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOS_V_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE3_DLOS_V_poll", A, value );

    /* (0x00000414 bits 23) field LANE3_DLOS_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE3_DLOS_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE3_DLOS_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE2_DLOS_V_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_DLOS_V_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOS_V_get", A, 2);
    /* (0x00000414 bits 22) field LANE2_DLOS_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE2_DLOS_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE2_DLOS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_DLOS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOS_V_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOS_V_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE2_DLOS_V_poll", A, value );

    /* (0x00000414 bits 22) field LANE2_DLOS_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE2_DLOS_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE2_DLOS_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE1_DLOS_V_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_DLOS_V_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOS_V_get", A, 2);
    /* (0x00000414 bits 21) field LANE1_DLOS_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE1_DLOS_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE1_DLOS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_DLOS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOS_V_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOS_V_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE1_DLOS_V_poll", A, value );

    /* (0x00000414 bits 21) field LANE1_DLOS_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE1_DLOS_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE1_DLOS_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE0_DLOS_V_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_DLOS_V_get( stl256_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOS_V_get", A, 2);
    /* (0x00000414 bits 20) field LANE0_DLOS_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE0_DLOS_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE0_DLOS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_DLOS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOS_V_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOS_V_poll( stl256_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE0_DLOS_V_poll", A, value );

    /* (0x00000414 bits 20) field LANE0_DLOS_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE0_DLOS_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE0_DLOS_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_ATSF_V_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_ATSF_V_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_ATSF_V_get", A, 2);
    /* (0x00000414 bits 18) field ATSF_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_ATSF_V_MSK) >> STL256_REG_INT_VAL_BIT_ATSF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_ATSF_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_ATSF_V_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_ATSF_V_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_ATSF_V_poll", A, value );

    /* (0x00000414 bits 18) field ATSF_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_ATSF_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_ATSF_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_DLOL_V_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DLOL_V_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DLOL_V_get", A, 2);
    /* (0x00000414 bits 17) field DLOL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_DLOL_V_MSK) >> STL256_REG_INT_VAL_BIT_DLOL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DLOL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DLOL_V_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DLOL_V_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_DLOL_V_poll", A, value );

    /* (0x00000414 bits 17) field DLOL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_DLOL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_DLOL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_DSTL_AIS_V_get( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DSTL_AIS_V_get( stl256_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DSTL_AIS_V_get", A, 2);
    /* (0x00000414 bits 16) field DSTL_AIS_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_DSTL_AIS_V_MSK) >> STL256_REG_INT_VAL_BIT_DSTL_AIS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DSTL_AIS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DSTL_AIS_V_poll( stl256_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DSTL_AIS_V_poll( stl256_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_DSTL_AIS_V_poll", A, value );

    /* (0x00000414 bits 16) field DSTL_AIS_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_DSTL_AIS_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_DSTL_AIS_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE3_DLOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_DLOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_DLOF_STL_V_get", A, 2);
    /* (0x00000414 bits 15) field LANE3_DLOF_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE3_DLOF_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE3_DLOF_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_DLOF_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE3_DLOF_STL_V_poll", A, value );

    /* (0x00000414 bits 15) field LANE3_DLOF_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE3_DLOF_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE3_DLOF_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE2_DLOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_DLOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_DLOF_STL_V_get", A, 2);
    /* (0x00000414 bits 14) field LANE2_DLOF_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE2_DLOF_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE2_DLOF_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_DLOF_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE2_DLOF_STL_V_poll", A, value );

    /* (0x00000414 bits 14) field LANE2_DLOF_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE2_DLOF_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE2_DLOF_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE1_DLOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_DLOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_DLOF_STL_V_get", A, 2);
    /* (0x00000414 bits 13) field LANE1_DLOF_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE1_DLOF_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE1_DLOF_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_DLOF_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE1_DLOF_STL_V_poll", A, value );

    /* (0x00000414 bits 13) field LANE1_DLOF_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE1_DLOF_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE1_DLOF_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE0_DLOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_DLOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_DLOF_STL_V_get", A, 2);
    /* (0x00000414 bits 12) field LANE0_DLOF_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE0_DLOF_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE0_DLOF_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_DLOF_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                       cbrc_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE0_DLOF_STL_V_poll", A, value );

    /* (0x00000414 bits 12) field LANE0_DLOF_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE0_DLOF_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE0_DLOF_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE3_OOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE3_OOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE3_OOF_STL_V_get", A, 2);
    /* (0x00000414 bits 11) field LANE3_OOF_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE3_OOF_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE3_OOF_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE3_OOF_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE3_OOF_STL_V_poll", A, value );

    /* (0x00000414 bits 11) field LANE3_OOF_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE3_OOF_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE3_OOF_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE2_OOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE2_OOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE2_OOF_STL_V_get", A, 2);
    /* (0x00000414 bits 10) field LANE2_OOF_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE2_OOF_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE2_OOF_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE2_OOF_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE2_OOF_STL_V_poll", A, value );

    /* (0x00000414 bits 10) field LANE2_OOF_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE2_OOF_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE2_OOF_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE1_OOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE1_OOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE1_OOF_STL_V_get", A, 2);
    /* (0x00000414 bits 9) field LANE1_OOF_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE1_OOF_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE1_OOF_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE1_OOF_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE1_OOF_STL_V_poll", A, value );

    /* (0x00000414 bits 9) field LANE1_OOF_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE1_OOF_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE1_OOF_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_LANE0_OOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_LANE0_OOF_STL_V_get( stl256_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_LANE0_OOF_STL_V_get", A, 2);
    /* (0x00000414 bits 8) field LANE0_OOF_STL_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_LANE0_OOF_STL_V_MSK) >> STL256_REG_INT_VAL_BIT_LANE0_OOF_STL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_LANE0_OOF_STL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOF_STL_V_poll( stl256_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_LANE0_OOF_STL_V_poll", A, value );

    /* (0x00000414 bits 8) field LANE0_OOF_STL_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_LANE0_OOF_STL_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_LANE0_OOF_STL_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_OOF_V_get( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_OOF_V_get( stl256_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_OOF_V_get", A, 2);
    /* (0x00000414 bits 1) field OOF_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_OOF_V_MSK) >> STL256_REG_INT_VAL_BIT_OOF_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_OOF_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_OOF_V_poll( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_OOF_V_poll( stl256_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_OOF_V_poll", A, value );

    /* (0x00000414 bits 1) field OOF_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_OOF_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_OOF_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 stl256_field_DAIS_V_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 stl256_field_DAIS_V_get( stl256_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "stl256_field_DAIS_V_get", A, 2);
    /* (0x00000414 bits 0) field DAIS_V of register PMC_STL256_REG_INT_VAL */
    reg_value = stl256_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & STL256_REG_INT_VAL_BIT_DAIS_V_MSK) >> STL256_REG_INT_VAL_BIT_DAIS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "stl256_field_DAIS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DAIS_V_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE stl256_field_DAIS_V_poll( stl256_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "stl256_field_DAIS_V_poll", A, value );

    /* (0x00000414 bits 0) field DAIS_V of register PMC_STL256_REG_INT_VAL */
    return stl256_reg_INT_VAL_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    STL256_REG_INT_VAL_BIT_DAIS_V_MSK,
                                    (value<<STL256_REG_INT_VAL_BIT_DAIS_V_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _STL256_IO_INLINE_H */
