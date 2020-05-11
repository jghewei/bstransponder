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
 *     and register accessor functions for the sfis_tx block
 *****************************************************************************/
#ifndef _SFIS_TX_IO_INLINE_H
#define _SFIS_TX_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "sfis_tx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SFIS_TX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sfis_tx
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
    lineotn_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sfis_tx_buffer_t;
static INLINE void sfis_tx_buffer_init( sfis_tx_buffer_t *b_ptr,
                                        lineotn_handle_t *h_ptr,
                                        UINT32 A ) ALWAYS_INLINE;
static INLINE void sfis_tx_buffer_init( sfis_tx_buffer_t *b_ptr,
                                        lineotn_handle_t *h_ptr,
                                        UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "sfis_tx_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sfis_tx_buffer_flush( sfis_tx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sfis_tx_buffer_flush( sfis_tx_buffer_t *b_ptr )
{
    IOLOG( "sfis_tx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sfis_tx_reg_read( sfis_tx_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_reg_read( sfis_tx_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
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
static INLINE void sfis_tx_reg_write( sfis_tx_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_write( sfis_tx_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
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

static INLINE void sfis_tx_field_set( sfis_tx_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_set( sfis_tx_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
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

static INLINE void sfis_tx_action_on_write_field_set( sfis_tx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_action_on_write_field_set( sfis_tx_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
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

static INLINE void sfis_tx_burst_read( sfis_tx_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void sfis_tx_burst_read( sfis_tx_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
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

static INLINE void sfis_tx_burst_write( sfis_tx_buffer_t *b_ptr,
                                        lineotn_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void sfis_tx_burst_write( sfis_tx_buffer_t *b_ptr,
                                        lineotn_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sfis_tx_poll( sfis_tx_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfis_tx_poll( sfis_tx_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
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
 *  register access functions for sfis_tx
 * ==================================================================================
 */

static INLINE void sfis_tx_reg_TX_CFG_write( sfis_tx_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_TX_CFG_write( sfis_tx_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfis_tx_reg_TX_CFG_write", value );
    sfis_tx_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_SFIS_TX_REG_TX_CFG,
                       value);
}

static INLINE void sfis_tx_reg_TX_CFG_field_set( sfis_tx_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_TX_CFG_field_set( sfis_tx_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfis_tx_reg_TX_CFG_field_set", A, mask, ofs, value );
    sfis_tx_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_SFIS_TX_REG_TX_CFG,
                       mask,
                       PMC_SFIS_TX_REG_TX_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 sfis_tx_reg_TX_CFG_read( sfis_tx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_reg_TX_CFG_read( sfis_tx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfis_tx_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_SFIS_TX_REG_TX_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfis_tx_reg_TX_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void sfis_tx_reg_TX_BUFFER_CFG_write( sfis_tx_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_TX_BUFFER_CFG_write( sfis_tx_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfis_tx_reg_TX_BUFFER_CFG_write", value );
    sfis_tx_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_SFIS_TX_REG_TX_BUFFER_CFG,
                       value);
}

static INLINE void sfis_tx_reg_TX_BUFFER_CFG_field_set( sfis_tx_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_TX_BUFFER_CFG_field_set( sfis_tx_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfis_tx_reg_TX_BUFFER_CFG_field_set", A, mask, ofs, value );
    sfis_tx_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_SFIS_TX_REG_TX_BUFFER_CFG,
                       mask,
                       PMC_SFIS_TX_REG_TX_BUFFER_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 sfis_tx_reg_TX_BUFFER_CFG_read( sfis_tx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_reg_TX_BUFFER_CFG_read( sfis_tx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfis_tx_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_SFIS_TX_REG_TX_BUFFER_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "sfis_tx_reg_TX_BUFFER_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void sfis_tx_reg_TX_INT_EN_write( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_TX_INT_EN_write( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfis_tx_reg_TX_INT_EN_write", value );
    sfis_tx_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFIS_TX_REG_TX_INT_EN,
                       value);
}

static INLINE void sfis_tx_reg_TX_INT_EN_field_set( sfis_tx_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_TX_INT_EN_field_set( sfis_tx_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfis_tx_reg_TX_INT_EN_field_set", A, mask, ofs, value );
    sfis_tx_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFIS_TX_REG_TX_INT_EN,
                       mask,
                       PMC_SFIS_TX_REG_TX_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 sfis_tx_reg_TX_INT_EN_read( sfis_tx_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_reg_TX_INT_EN_read( sfis_tx_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfis_tx_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_SFIS_TX_REG_TX_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "sfis_tx_reg_TX_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void sfis_tx_reg_TX_INT_write( sfis_tx_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_TX_INT_write( sfis_tx_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sfis_tx_reg_TX_INT_write", value );
    sfis_tx_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_SFIS_TX_REG_TX_INT,
                       value);
}

static INLINE void sfis_tx_reg_TX_INT_action_on_write_field_set( sfis_tx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_reg_TX_INT_action_on_write_field_set( sfis_tx_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "sfis_tx_reg_TX_INT_action_on_write_field_set", A, mask, ofs, value );
    sfis_tx_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SFIS_TX_REG_TX_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 sfis_tx_reg_TX_INT_read( sfis_tx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_reg_TX_INT_read( sfis_tx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = sfis_tx_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_SFIS_TX_REG_TX_INT);

    IOLOG( "%s -> 0x%08x; A=%d", "sfis_tx_reg_TX_INT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sfis_tx_reg_TX_INT_poll( sfis_tx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfis_tx_reg_TX_INT_poll( sfis_tx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sfis_tx_reg_TX_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sfis_tx_poll( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SFIS_TX_REG_TX_INT,
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
static INLINE void sfis_tx_field_TX_ODD_INV_set( sfis_tx_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TX_ODD_INV_set( sfis_tx_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_ODD_INV_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TX_ODD_INV_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TX_ODD_INV_set", A, value );

    /* (0x00016080 bits 5) field TX_ODD_INV of register PMC_SFIS_TX_REG_TX_CFG */
    sfis_tx_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  SFIS_TX_REG_TX_CFG_BIT_TX_ODD_INV_MSK,
                                  SFIS_TX_REG_TX_CFG_BIT_TX_ODD_INV_OFF,
                                  value);
}

static INLINE UINT32 sfis_tx_field_TX_ODD_INV_get( sfis_tx_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TX_ODD_INV_get( sfis_tx_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_ODD_INV_get", A, 3);
    /* (0x00016080 bits 5) field TX_ODD_INV of register PMC_SFIS_TX_REG_TX_CFG */
    reg_value = sfis_tx_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_CFG_BIT_TX_ODD_INV_MSK) >> SFIS_TX_REG_TX_CFG_BIT_TX_ODD_INV_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TX_ODD_INV_get", A, value );

    return value;
}
static INLINE void sfis_tx_field_TX_NUM_LANES_set( sfis_tx_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TX_NUM_LANES_set( sfis_tx_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_NUM_LANES_set", A, 3);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TX_NUM_LANES_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TX_NUM_LANES_set", A, value );

    /* (0x00016080 bits 3:0) bits 0:3 use field TX_NUM_LANES of register PMC_SFIS_TX_REG_TX_CFG */
    sfis_tx_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_MSK,
                                  SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_OFF,
                                  value);
}

static INLINE UINT32 sfis_tx_field_TX_NUM_LANES_get( sfis_tx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TX_NUM_LANES_get( sfis_tx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_NUM_LANES_get", A, 3);
    /* (0x00016080 bits 3:0) bits 0:3 use field TX_NUM_LANES of register PMC_SFIS_TX_REG_TX_CFG */
    reg_value = sfis_tx_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_MSK) >> SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TX_NUM_LANES_get", A, value );

    return value;
}
static INLINE void sfis_tx_field_range_TX_NUM_LANES_set( sfis_tx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_range_TX_NUM_LANES_set( sfis_tx_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_range_TX_NUM_LANES_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfis_tx_field_range_TX_NUM_LANES_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfis_tx_field_range_TX_NUM_LANES_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "sfis_tx_field_range_TX_NUM_LANES_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00016080 bits 3:0) bits 0:3 use field TX_NUM_LANES of register PMC_SFIS_TX_REG_TX_CFG */
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
        /* (0x00016080 bits 3:0) bits 0:3 use field TX_NUM_LANES of register PMC_SFIS_TX_REG_TX_CFG */
        sfis_tx_reg_TX_CFG_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      subfield_mask << (SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_OFF + subfield_offset),
                                      SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 sfis_tx_field_range_TX_NUM_LANES_get( sfis_tx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_range_TX_NUM_LANES_get( sfis_tx_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_range_TX_NUM_LANES_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sfis_tx_field_range_TX_NUM_LANES_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sfis_tx_field_range_TX_NUM_LANES_get", stop_bit, 3 );
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
    /* (0x00016080 bits 3:0) bits 0:3 use field TX_NUM_LANES of register PMC_SFIS_TX_REG_TX_CFG */
    reg_value = sfis_tx_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_MSK)
                  >> SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_MSK, SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "sfis_tx_field_range_TX_NUM_LANES_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void sfis_tx_field_TX_FIFO_AUTO_CENTER_set( sfis_tx_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TX_FIFO_AUTO_CENTER_set( sfis_tx_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_FIFO_AUTO_CENTER_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TX_FIFO_AUTO_CENTER_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TX_FIFO_AUTO_CENTER_set", A, value );

    /* (0x0001608c bits 6) field TX_FIFO_AUTO_CENTER of register PMC_SFIS_TX_REG_TX_BUFFER_CFG */
    sfis_tx_reg_TX_BUFFER_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_AUTO_CENTER_MSK,
                                         SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_AUTO_CENTER_OFF,
                                         value);
}

static INLINE UINT32 sfis_tx_field_TX_FIFO_AUTO_CENTER_get( sfis_tx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TX_FIFO_AUTO_CENTER_get( sfis_tx_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_FIFO_AUTO_CENTER_get", A, 3);
    /* (0x0001608c bits 6) field TX_FIFO_AUTO_CENTER of register PMC_SFIS_TX_REG_TX_BUFFER_CFG */
    reg_value = sfis_tx_reg_TX_BUFFER_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_AUTO_CENTER_MSK) >> SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_AUTO_CENTER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TX_FIFO_AUTO_CENTER_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void sfis_tx_field_TX_FIFO_CENTER_set( sfis_tx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TX_FIFO_CENTER_set( sfis_tx_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_FIFO_CENTER_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TX_FIFO_CENTER_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TX_FIFO_CENTER_set", A, value );

    /* (0x0001608c bits 7) field TX_FIFO_CENTER of register PMC_SFIS_TX_REG_TX_BUFFER_CFG */
    sfis_tx_reg_TX_BUFFER_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_CENTER_MSK,
                                         SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_CENTER_OFF,
                                         value);
}

static INLINE UINT32 sfis_tx_field_TX_FIFO_CENTER_get( sfis_tx_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TX_FIFO_CENTER_get( sfis_tx_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_FIFO_CENTER_get", A, 3);
    /* (0x0001608c bits 7) field TX_FIFO_CENTER of register PMC_SFIS_TX_REG_TX_BUFFER_CFG */
    reg_value = sfis_tx_reg_TX_BUFFER_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_CENTER_MSK) >> SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_CENTER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TX_FIFO_CENTER_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void sfis_tx_field_TXOVR_E_set( sfis_tx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TXOVR_E_set( sfis_tx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TXOVR_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TXOVR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TXOVR_E_set", A, value );

    /* (0x00016088 bits 1) field TXOVR_E of register PMC_SFIS_TX_REG_TX_INT_EN */
    sfis_tx_reg_TX_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     SFIS_TX_REG_TX_INT_EN_BIT_TXOVR_E_MSK,
                                     SFIS_TX_REG_TX_INT_EN_BIT_TXOVR_E_OFF,
                                     value);
}

static INLINE UINT32 sfis_tx_field_TXOVR_E_get( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TXOVR_E_get( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TXOVR_E_get", A, 3);
    /* (0x00016088 bits 1) field TXOVR_E of register PMC_SFIS_TX_REG_TX_INT_EN */
    reg_value = sfis_tx_reg_TX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_INT_EN_BIT_TXOVR_E_MSK) >> SFIS_TX_REG_TX_INT_EN_BIT_TXOVR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TXOVR_E_get", A, value );

    return value;
}
static INLINE void sfis_tx_field_TXUDR_E_set( sfis_tx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TXUDR_E_set( sfis_tx_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TXUDR_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TXUDR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TXUDR_E_set", A, value );

    /* (0x00016088 bits 0) field TXUDR_E of register PMC_SFIS_TX_REG_TX_INT_EN */
    sfis_tx_reg_TX_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     SFIS_TX_REG_TX_INT_EN_BIT_TXUDR_E_MSK,
                                     SFIS_TX_REG_TX_INT_EN_BIT_TXUDR_E_OFF,
                                     value);
}

static INLINE UINT32 sfis_tx_field_TXUDR_E_get( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TXUDR_E_get( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TXUDR_E_get", A, 3);
    /* (0x00016088 bits 0) field TXUDR_E of register PMC_SFIS_TX_REG_TX_INT_EN */
    reg_value = sfis_tx_reg_TX_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_INT_EN_BIT_TXUDR_E_MSK) >> SFIS_TX_REG_TX_INT_EN_BIT_TXUDR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TXUDR_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void sfis_tx_field_TXOVR_I_set_to_clear( sfis_tx_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TXOVR_I_set_to_clear( sfis_tx_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TXOVR_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TXOVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TXOVR_I_set_to_clear", A, value );

    /* (0x00016084 bits 1) field TXOVR_I of register PMC_SFIS_TX_REG_TX_INT */
    sfis_tx_reg_TX_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  SFIS_TX_REG_TX_INT_BIT_TXOVR_I_MSK,
                                                  SFIS_TX_REG_TX_INT_BIT_TXOVR_I_OFF,
                                                  value);
}

static INLINE UINT32 sfis_tx_field_TXOVR_I_get( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TXOVR_I_get( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TXOVR_I_get", A, 3);
    /* (0x00016084 bits 1) field TXOVR_I of register PMC_SFIS_TX_REG_TX_INT */
    reg_value = sfis_tx_reg_TX_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_INT_BIT_TXOVR_I_MSK) >> SFIS_TX_REG_TX_INT_BIT_TXOVR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TXOVR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfis_tx_field_TXOVR_I_poll( sfis_tx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfis_tx_field_TXOVR_I_poll( sfis_tx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfis_tx_field_TXOVR_I_poll", A, value );

    /* (0x00016084 bits 1) field TXOVR_I of register PMC_SFIS_TX_REG_TX_INT */
    return sfis_tx_reg_TX_INT_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    SFIS_TX_REG_TX_INT_BIT_TXOVR_I_MSK,
                                    (value<<SFIS_TX_REG_TX_INT_BIT_TXOVR_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void sfis_tx_field_TXUDR_I_set_to_clear( sfis_tx_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TXUDR_I_set_to_clear( sfis_tx_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TXUDR_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TXUDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TXUDR_I_set_to_clear", A, value );

    /* (0x00016084 bits 0) field TXUDR_I of register PMC_SFIS_TX_REG_TX_INT */
    sfis_tx_reg_TX_INT_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  SFIS_TX_REG_TX_INT_BIT_TXUDR_I_MSK,
                                                  SFIS_TX_REG_TX_INT_BIT_TXUDR_I_OFF,
                                                  value);
}

static INLINE UINT32 sfis_tx_field_TXUDR_I_get( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TXUDR_I_get( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TXUDR_I_get", A, 3);
    /* (0x00016084 bits 0) field TXUDR_I of register PMC_SFIS_TX_REG_TX_INT */
    reg_value = sfis_tx_reg_TX_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_INT_BIT_TXUDR_I_MSK) >> SFIS_TX_REG_TX_INT_BIT_TXUDR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TXUDR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sfis_tx_field_TXUDR_I_poll( sfis_tx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sfis_tx_field_TXUDR_I_poll( sfis_tx_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "sfis_tx_field_TXUDR_I_poll", A, value );

    /* (0x00016084 bits 0) field TXUDR_I of register PMC_SFIS_TX_REG_TX_INT */
    return sfis_tx_reg_TX_INT_poll( b_ptr,
                                    h_ptr,
                                    A,
                                    SFIS_TX_REG_TX_INT_BIT_TXUDR_I_MSK,
                                    (value<<SFIS_TX_REG_TX_INT_BIT_TXUDR_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void sfis_tx_field_TX_SW_RST_set( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sfis_tx_field_TX_SW_RST_set( sfis_tx_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_SW_RST_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sfis_tx_field_TX_SW_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "sfis_tx_field_TX_SW_RST_set", A, value );

    /* (0x00016080 bits 4) field TX_SW_RST of register PMC_SFIS_TX_REG_TX_CFG */
    sfis_tx_reg_TX_CFG_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  SFIS_TX_REG_TX_CFG_BIT_TX_SW_RST_MSK,
                                  SFIS_TX_REG_TX_CFG_BIT_TX_SW_RST_OFF,
                                  value);
}

static INLINE UINT32 sfis_tx_field_TX_SW_RST_get( sfis_tx_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sfis_tx_field_TX_SW_RST_get( sfis_tx_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "sfis_tx_field_TX_SW_RST_get", A, 3);
    /* (0x00016080 bits 4) field TX_SW_RST of register PMC_SFIS_TX_REG_TX_CFG */
    reg_value = sfis_tx_reg_TX_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SFIS_TX_REG_TX_CFG_BIT_TX_SW_RST_MSK) >> SFIS_TX_REG_TX_CFG_BIT_TX_SW_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "sfis_tx_field_TX_SW_RST_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SFIS_TX_IO_INLINE_H */