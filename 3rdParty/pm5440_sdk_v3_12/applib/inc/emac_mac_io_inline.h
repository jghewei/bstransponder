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
 *     and register accessor functions for the emac_mac block
 *****************************************************************************/
#ifndef _EMAC_MAC_IO_INLINE_H
#define _EMAC_MAC_IO_INLINE_H

#include "emac_loc.h"
#include "emac_mac_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EMAC_MAC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for emac_mac
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
    emac_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} emac_mac_buffer_t;
static INLINE void emac_mac_buffer_init( emac_mac_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 A ) ALWAYS_INLINE;
static INLINE void emac_mac_buffer_init( emac_mac_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*1024) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "emac_mac_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void emac_mac_buffer_flush( emac_mac_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void emac_mac_buffer_flush( emac_mac_buffer_t *b_ptr )
{
    IOLOG( "emac_mac_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 emac_mac_reg_read( emac_mac_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_read( emac_mac_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*1024),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg);
    return value;
}
static INLINE void emac_mac_reg_write( emac_mac_buffer_t *b_ptr,
                                       emac_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_write( emac_mac_buffer_t *b_ptr,
                                       emac_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*1024),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                         value);
}

static INLINE void emac_mac_field_set( emac_mac_buffer_t *b_ptr,
                                       emac_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_set( emac_mac_buffer_t *b_ptr,
                                       emac_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*1024),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void emac_mac_action_on_write_field_set( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_action_on_write_field_set( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*1024),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                         value<<ofs);
}

static INLINE void emac_mac_burst_read( emac_mac_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_mac_burst_read( emac_mac_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*1024),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                          len,
                          value);
}

static INLINE void emac_mac_burst_write( emac_mac_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_mac_burst_write( emac_mac_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*1024),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE emac_mac_poll( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_mac_poll( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*1024),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for emac_mac
 * ==================================================================================
 */

static INLINE void emac_mac_reg_CTRL_AND_CFG_write( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CTRL_AND_CFG_write( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CTRL_AND_CFG_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CTRL_AND_CFG,
                        value);
}

static INLINE void emac_mac_reg_CTRL_AND_CFG_field_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CTRL_AND_CFG_field_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CTRL_AND_CFG_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CTRL_AND_CFG,
                        mask,
                        PMC_MAC_REG_CTRL_AND_CFG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CTRL_AND_CFG_read( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CTRL_AND_CFG_read( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CTRL_AND_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CTRL_AND_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_MAC_ADR_LSB__write( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_MAC_ADR_LSB__write( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_MAC_ADR_LSB__write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_MAC_ADR_LSB_,
                        value);
}

static INLINE void emac_mac_reg_MAC_ADR_LSB__field_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_MAC_ADR_LSB__field_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_MAC_ADR_LSB__field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_MAC_ADR_LSB_,
                        mask,
                        PMC_MAC_REG_MAC_ADR_LSB_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_MAC_ADR_LSB__read( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_MAC_ADR_LSB__read( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_MAC_ADR_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_MAC_ADR_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_MAC_ADR_MSB__write( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_MAC_ADR_MSB__write( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_MAC_ADR_MSB__write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_MAC_ADR_MSB_,
                        value);
}

static INLINE void emac_mac_reg_MAC_ADR_MSB__field_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_MAC_ADR_MSB__field_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_MAC_ADR_MSB__field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_MAC_ADR_MSB_,
                        mask,
                        PMC_MAC_REG_MAC_ADR_MSB_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_MAC_ADR_MSB__read( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_MAC_ADR_MSB__read( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_MAC_ADR_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_MAC_ADR_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_FRAME_LENGTH_write( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_FRAME_LENGTH_write( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_FRAME_LENGTH_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_FRAME_LENGTH,
                        value);
}

static INLINE void emac_mac_reg_FRAME_LENGTH_field_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_FRAME_LENGTH_field_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_FRAME_LENGTH_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_FRAME_LENGTH,
                        mask,
                        PMC_MAC_REG_FRAME_LENGTH_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_FRAME_LENGTH_read( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_FRAME_LENGTH_read( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_FRAME_LENGTH);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_FRAME_LENGTH_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_TX_INTER_PACKET_GAP_IPG__write( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_TX_INTER_PACKET_GAP_IPG__write( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_TX_INTER_PACKET_GAP_IPG__write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_,
                        value);
}

static INLINE void emac_mac_reg_TX_INTER_PACKET_GAP_IPG__field_set( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_TX_INTER_PACKET_GAP_IPG__field_set( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_TX_INTER_PACKET_GAP_IPG__field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_,
                        mask,
                        PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_TX_INTER_PACKET_GAP_IPG__read( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_TX_INTER_PACKET_GAP_IPG__read( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_TX_INTER_PACKET_GAP_IPG__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_RX_APPLICATION_CREDIT_RESET_write( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_RX_APPLICATION_CREDIT_RESET_write( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_RX_APPLICATION_CREDIT_RESET_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_RX_APPLICATION_CREDIT_RESET,
                        value);
}

static INLINE void emac_mac_reg_RX_APPLICATION_CREDIT_RESET_field_set( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_RX_APPLICATION_CREDIT_RESET_field_set( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_RX_APPLICATION_CREDIT_RESET_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_RX_APPLICATION_CREDIT_RESET,
                        mask,
                        PMC_MAC_REG_RX_APPLICATION_CREDIT_RESET_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_RX_APPLICATION_CREDIT_RESET_read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_RX_APPLICATION_CREDIT_RESET_read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_RX_APPLICATION_CREDIT_RESET);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_RX_APPLICATION_CREDIT_RESET_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_write( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_write( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION,
                        value);
}

static INLINE void emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_field_set( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_field_set( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION,
                        mask,
                        PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_write( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_write( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA,
                        value);
}

static INLINE void emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_field_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_field_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA,
                        mask,
                        PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_read( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_read( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_write( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_write( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA,
                        value);
}

static INLINE void emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_field_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_field_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA,
                        mask,
                        PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_read( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_read( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_write( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_write( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA,
                        value);
}

static INLINE void emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_field_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_field_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA,
                        mask,
                        PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_read( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_read( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_write( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_write( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA,
                        value);
}

static INLINE void emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_field_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_field_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA,
                        mask,
                        PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_read( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_read( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_write( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_write( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH,
                        value);
}

static INLINE void emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_field_set( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_field_set( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH,
                        mask,
                        PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_read( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_read( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_write( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_write( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH,
                        value);
}

static INLINE void emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_field_set( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_field_set( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH,
                        mask,
                        PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_read( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_read( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_write( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_write( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH,
                        value);
}

static INLINE void emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_field_set( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_field_set( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH,
                        mask,
                        PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_read( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_read( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_write( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_write( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH,
                        value);
}

static INLINE void emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_field_set( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_field_set( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH,
                        mask,
                        PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_read( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_read( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_TX_PREAMBLE_LSB__write( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_TX_PREAMBLE_LSB__write( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_TX_PREAMBLE_LSB__write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_TX_PREAMBLE_LSB_,
                        value);
}

static INLINE void emac_mac_reg_TX_PREAMBLE_LSB__field_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_TX_PREAMBLE_LSB__field_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_TX_PREAMBLE_LSB__field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_TX_PREAMBLE_LSB_,
                        mask,
                        PMC_MAC_REG_TX_PREAMBLE_LSB_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_TX_PREAMBLE_LSB__read( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_TX_PREAMBLE_LSB__read( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_TX_PREAMBLE_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_TX_PREAMBLE_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_TX_PREAMBLE_MSB__write( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_TX_PREAMBLE_MSB__write( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_TX_PREAMBLE_MSB__write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_TX_PREAMBLE_MSB_,
                        value);
}

static INLINE void emac_mac_reg_TX_PREAMBLE_MSB__field_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_TX_PREAMBLE_MSB__field_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_TX_PREAMBLE_MSB__field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_TX_PREAMBLE_MSB_,
                        mask,
                        PMC_MAC_REG_TX_PREAMBLE_MSB_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_TX_PREAMBLE_MSB__read( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_TX_PREAMBLE_MSB__read( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_TX_PREAMBLE_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_TX_PREAMBLE_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_mac_reg_SCRATCH_write( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_SCRATCH_write( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_mac_reg_SCRATCH_write", value );
    emac_mac_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_SCRATCH,
                        value);
}

static INLINE void emac_mac_reg_SCRATCH_field_set( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_reg_SCRATCH_field_set( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_mac_reg_SCRATCH_field_set", A, mask, ofs, value );
    emac_mac_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_MAC_REG_SCRATCH,
                        mask,
                        PMC_MAC_REG_SCRATCH_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_mac_reg_SCRATCH_read( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_SCRATCH_read( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_SCRATCH);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_SCRATCH_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_REVISION_read( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_REVISION_read( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_REVISION);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_REVISION_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STAT_read( emac_mac_buffer_t *b_ptr,
                                             emac_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STAT_read( emac_mac_buffer_t *b_ptr,
                                             emac_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_MAC_REG_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_RX_PAUSE_STAT_read( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_RX_PAUSE_STAT_read( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_RX_PAUSE_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_RX_PAUSE_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                 emac_handle_t *h_ptr,
                                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_VLANTXOK_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_VLANTXOK_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_VLANTXOK_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_VLANTXOK_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_VLANTXOK_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_VLANTXOK_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_VLANRXOK_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_VLANRXOK_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_VLANRXOK_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_VLANRXOK_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_VLANRXOK_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_VLANRXOK_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINOCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINOCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINOCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINOCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINOCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINOCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTERRS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTERRS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                   emac_handle_t *h_ptr,
                                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                   emac_handle_t *h_ptr,
                                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                   emac_handle_t *h_ptr,
                                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                   emac_handle_t *h_ptr,
                                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                     emac_handle_t *h_ptr,
                                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                     emac_handle_t *h_ptr,
                                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                     emac_handle_t *h_ptr,
                                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                     emac_handle_t *h_ptr,
                                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                      emac_handle_t *h_ptr,
                                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                       emac_handle_t *h_ptr,
                                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                       emac_handle_t *h_ptr,
                                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                       emac_handle_t *h_ptr,
                                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                       emac_handle_t *h_ptr,
                                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                        emac_handle_t *h_ptr,
                                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                        emac_handle_t *h_ptr,
                                                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                        emac_handle_t *h_ptr,
                                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                        emac_handle_t *h_ptr,
                                                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                       emac_handle_t *h_ptr,
                                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                       emac_handle_t *h_ptr,
                                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                       emac_handle_t *h_ptr,
                                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                       emac_handle_t *h_ptr,
                                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                  emac_handle_t *h_ptr,
                                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                  emac_handle_t *h_ptr,
                                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                  emac_handle_t *h_ptr,
                                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                  emac_handle_t *h_ptr,
                                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINERRS_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINERRS_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINERRS_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_IFINERRS_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_IFINERRS_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_IFINERRS_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB__read( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB__read( emac_mac_buffer_t *b_ptr,
                                                                            emac_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_mac_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB__read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void emac_mac_field_SEND_RF_set( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_SEND_RF_set( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SEND_RF_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_SEND_RF_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_SEND_RF_set", A, value );

    /* (0x00008008 bits 29) field SEND_RF of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_SEND_RF_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_SEND_RF_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_SEND_RF_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_SEND_RF_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SEND_RF_get", A, 11);
    /* (0x00008008 bits 29) field SEND_RF of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_SEND_RF_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_SEND_RF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_SEND_RF_get", A, value );

    return value;
}
static INLINE void emac_mac_field_SEND_LF_set( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_SEND_LF_set( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SEND_LF_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_SEND_LF_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_SEND_LF_set", A, value );

    /* (0x00008008 bits 28) field SEND_LF of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_SEND_LF_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_SEND_LF_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_SEND_LF_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_SEND_LF_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SEND_LF_get", A, 11);
    /* (0x00008008 bits 28) field SEND_LF of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_SEND_LF_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_SEND_LF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_SEND_LF_get", A, value );

    return value;
}
static INLINE void emac_mac_field_FLT_HDL_DIS_set( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_FLT_HDL_DIS_set( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_FLT_HDL_DIS_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_FLT_HDL_DIS_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_FLT_HDL_DIS_set", A, value );

    /* (0x00008008 bits 27) field FLT_HDL_DIS of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_FLT_HDL_DIS_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_FLT_HDL_DIS_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_FLT_HDL_DIS_get( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_FLT_HDL_DIS_get( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_FLT_HDL_DIS_get", A, 11);
    /* (0x00008008 bits 27) field FLT_HDL_DIS of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_FLT_HDL_DIS_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_FLT_HDL_DIS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_FLT_HDL_DIS_get", A, value );

    return value;
}
static INLINE void emac_mac_field_TX_FLUSH_set( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_TX_FLUSH_set( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_FLUSH_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_TX_FLUSH_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_TX_FLUSH_set", A, value );

    /* (0x00008008 bits 22) field TX_FLUSH of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_TX_FLUSH_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_TX_FLUSH_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_TX_FLUSH_get( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_TX_FLUSH_get( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_FLUSH_get", A, 11);
    /* (0x00008008 bits 22) field TX_FLUSH of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_TX_FLUSH_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_TX_FLUSH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_TX_FLUSH_get", A, value );

    return value;
}
static INLINE void emac_mac_field_RX_SFD_ANY_set( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_RX_SFD_ANY_set( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_RX_SFD_ANY_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_RX_SFD_ANY_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_RX_SFD_ANY_set", A, value );

    /* (0x00008008 bits 21) field RX_SFD_ANY of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_RX_SFD_ANY_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_RX_SFD_ANY_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_RX_SFD_ANY_get( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_RX_SFD_ANY_get( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_RX_SFD_ANY_get", A, 11);
    /* (0x00008008 bits 21) field RX_SFD_ANY of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_RX_SFD_ANY_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_RX_SFD_ANY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_RX_SFD_ANY_get", A, value );

    return value;
}
static INLINE void emac_mac_field_PAUSE_PFC_COMP_set( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_PAUSE_PFC_COMP_set( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PAUSE_PFC_COMP_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_PAUSE_PFC_COMP_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_PAUSE_PFC_COMP_set", A, value );

    /* (0x00008008 bits 20) field PAUSE_PFC_COMP of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_PAUSE_PFC_COMP_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_PAUSE_PFC_COMP_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_PAUSE_PFC_COMP_get( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_PAUSE_PFC_COMP_get( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PAUSE_PFC_COMP_get", A, 11);
    /* (0x00008008 bits 20) field PAUSE_PFC_COMP of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_PAUSE_PFC_COMP_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_PAUSE_PFC_COMP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_PAUSE_PFC_COMP_get", A, value );

    return value;
}
static INLINE void emac_mac_field_PFC_MODE_set( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_PFC_MODE_set( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PFC_MODE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_PFC_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_PFC_MODE_set", A, value );

    /* (0x00008008 bits 19) field PFC_MODE of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_PFC_MODE_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_PFC_MODE_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_PFC_MODE_get( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_PFC_MODE_get( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PFC_MODE_get", A, 11);
    /* (0x00008008 bits 19) field PFC_MODE of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_PFC_MODE_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_PFC_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_PFC_MODE_get", A, value );

    return value;
}
static INLINE void emac_mac_field_RS_COL_CNT_EXT_set( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_RS_COL_CNT_EXT_set( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_RS_COL_CNT_EXT_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_RS_COL_CNT_EXT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_RS_COL_CNT_EXT_set", A, value );

    /* (0x00008008 bits 18) field RS_COL_CNT_EXT of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_RS_COL_CNT_EXT_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_RS_COL_CNT_EXT_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_RS_COL_CNT_EXT_get( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_RS_COL_CNT_EXT_get( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_RS_COL_CNT_EXT_get", A, 11);
    /* (0x00008008 bits 18) field RS_COL_CNT_EXT of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_RS_COL_CNT_EXT_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_RS_COL_CNT_EXT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_RS_COL_CNT_EXT_get", A, value );

    return value;
}
static INLINE void emac_mac_field_NO_LGTH_CHECK_set( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_NO_LGTH_CHECK_set( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_NO_LGTH_CHECK_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_NO_LGTH_CHECK_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_NO_LGTH_CHECK_set", A, value );

    /* (0x00008008 bits 17) field NO_LGTH_CHECK of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_NO_LGTH_CHECK_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_NO_LGTH_CHECK_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_NO_LGTH_CHECK_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_NO_LGTH_CHECK_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_NO_LGTH_CHECK_get", A, 11);
    /* (0x00008008 bits 17) field NO_LGTH_CHECK of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_NO_LGTH_CHECK_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_NO_LGTH_CHECK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_NO_LGTH_CHECK_get", A, value );

    return value;
}
static INLINE void emac_mac_field_SEND_IDLE_set( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_SEND_IDLE_set( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SEND_IDLE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_SEND_IDLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_SEND_IDLE_set", A, value );

    /* (0x00008008 bits 16) field SEND_IDLE of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_SEND_IDLE_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_SEND_IDLE_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_SEND_IDLE_get( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_SEND_IDLE_get( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SEND_IDLE_get", A, 11);
    /* (0x00008008 bits 16) field SEND_IDLE of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_SEND_IDLE_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_SEND_IDLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_SEND_IDLE_get", A, value );

    return value;
}
static INLINE void emac_mac_field_CMD_FRAME_EN_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CMD_FRAME_EN_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CMD_FRAME_EN_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CMD_FRAME_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CMD_FRAME_EN_set", A, value );

    /* (0x00008008 bits 13) field CMD_FRAME_EN of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_CMD_FRAME_EN_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CMD_FRAME_EN_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CMD_FRAME_EN_get", A, 11);
    /* (0x00008008 bits 13) field CMD_FRAME_EN of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CMD_FRAME_EN_get", A, value );

    return value;
}
static INLINE void emac_mac_field_SW_RESET_set( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_SW_RESET_set( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SW_RESET_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_SW_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_SW_RESET_set", A, value );

    /* (0x00008008 bits 12) field SW_RESET of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_SW_RESET_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_SW_RESET_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_SW_RESET_get( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_SW_RESET_get( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SW_RESET_get", A, 11);
    /* (0x00008008 bits 12) field SW_RESET of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_SW_RESET_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_SW_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_SW_RESET_get", A, value );

    return value;
}
static INLINE void emac_mac_field_TX_PAD_EN_set( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_TX_PAD_EN_set( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_PAD_EN_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_TX_PAD_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_TX_PAD_EN_set", A, value );

    /* (0x00008008 bits 11) field TX_PAD_EN of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_TX_PAD_EN_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_TX_PAD_EN_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_TX_PAD_EN_get( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_TX_PAD_EN_get( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_PAD_EN_get", A, 11);
    /* (0x00008008 bits 11) field TX_PAD_EN of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_TX_PAD_EN_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_TX_PAD_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_TX_PAD_EN_get", A, value );

    return value;
}
static INLINE void emac_mac_field_TX_ADDR_INS_set( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_TX_ADDR_INS_set( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_ADDR_INS_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_TX_ADDR_INS_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_TX_ADDR_INS_set", A, value );

    /* (0x00008008 bits 9) field TX_ADDR_INS of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_TX_ADDR_INS_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_TX_ADDR_INS_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_TX_ADDR_INS_get( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_TX_ADDR_INS_get( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_ADDR_INS_get", A, 11);
    /* (0x00008008 bits 9) field TX_ADDR_INS of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_TX_ADDR_INS_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_TX_ADDR_INS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_TX_ADDR_INS_get", A, value );

    return value;
}
static INLINE void emac_mac_field_PAUSE_IGNORE_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_PAUSE_IGNORE_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PAUSE_IGNORE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_PAUSE_IGNORE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_PAUSE_IGNORE_set", A, value );

    /* (0x00008008 bits 8) field PAUSE_IGNORE of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_PAUSE_IGNORE_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_PAUSE_IGNORE_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_PAUSE_IGNORE_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_PAUSE_IGNORE_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PAUSE_IGNORE_get", A, 11);
    /* (0x00008008 bits 8) field PAUSE_IGNORE of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_PAUSE_IGNORE_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_PAUSE_IGNORE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_PAUSE_IGNORE_get", A, value );

    return value;
}
static INLINE void emac_mac_field_PAUSE_FWD_set( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_PAUSE_FWD_set( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PAUSE_FWD_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_PAUSE_FWD_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_PAUSE_FWD_set", A, value );

    /* (0x00008008 bits 7) field PAUSE_FWD of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_PAUSE_FWD_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_PAUSE_FWD_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_PAUSE_FWD_get( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_PAUSE_FWD_get( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PAUSE_FWD_get", A, 11);
    /* (0x00008008 bits 7) field PAUSE_FWD of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_PAUSE_FWD_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_PAUSE_FWD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_PAUSE_FWD_get", A, value );

    return value;
}
static INLINE void emac_mac_field_CRC_FWD_set( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CRC_FWD_set( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CRC_FWD_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CRC_FWD_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CRC_FWD_set", A, value );

    /* (0x00008008 bits 6) field CRC_FWD of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_CRC_FWD_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_CRC_FWD_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_CRC_FWD_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CRC_FWD_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CRC_FWD_get", A, 11);
    /* (0x00008008 bits 6) field CRC_FWD of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_CRC_FWD_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_CRC_FWD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CRC_FWD_get", A, value );

    return value;
}
static INLINE void emac_mac_field_PROMIS_EN_set( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_PROMIS_EN_set( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PROMIS_EN_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_PROMIS_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_PROMIS_EN_set", A, value );

    /* (0x00008008 bits 4) field PROMIS_EN of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_PROMIS_EN_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_PROMIS_EN_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_PROMIS_EN_get( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_PROMIS_EN_get( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PROMIS_EN_get", A, 11);
    /* (0x00008008 bits 4) field PROMIS_EN of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_PROMIS_EN_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_PROMIS_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_PROMIS_EN_get", A, value );

    return value;
}
static INLINE void emac_mac_field_WAN_MODE_set( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_WAN_MODE_set( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_WAN_MODE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_WAN_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_WAN_MODE_set", A, value );

    /* (0x00008008 bits 3) field WAN_MODE of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_WAN_MODE_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_WAN_MODE_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_WAN_MODE_get( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_WAN_MODE_get( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_WAN_MODE_get", A, 11);
    /* (0x00008008 bits 3) field WAN_MODE of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_WAN_MODE_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_WAN_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_WAN_MODE_get", A, value );

    return value;
}
static INLINE void emac_mac_field_RX_ENA_set( emac_mac_buffer_t *b_ptr,
                                              emac_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_RX_ENA_set( emac_mac_buffer_t *b_ptr,
                                              emac_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_RX_ENA_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_RX_ENA_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_RX_ENA_set", A, value );

    /* (0x00008008 bits 1) field RX_ENA of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_RX_ENA_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_RX_ENA_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_RX_ENA_get( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_RX_ENA_get( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_RX_ENA_get", A, 11);
    /* (0x00008008 bits 1) field RX_ENA of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_RX_ENA_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_RX_ENA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_RX_ENA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_TX_ENA_set( emac_mac_buffer_t *b_ptr,
                                              emac_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_TX_ENA_set( emac_mac_buffer_t *b_ptr,
                                              emac_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_ENA_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_TX_ENA_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_TX_ENA_set", A, value );

    /* (0x00008008 bits 0) field TX_ENA of register PMC_MAC_REG_CTRL_AND_CFG */
    emac_mac_reg_CTRL_AND_CFG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_CTRL_AND_CFG_BIT_TX_ENA_MSK,
                                         MAC_REG_CTRL_AND_CFG_BIT_TX_ENA_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_TX_ENA_get( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_TX_ENA_get( emac_mac_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_ENA_get", A, 11);
    /* (0x00008008 bits 0) field TX_ENA of register PMC_MAC_REG_CTRL_AND_CFG */
    reg_value = emac_mac_reg_CTRL_AND_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_CTRL_AND_CFG_BIT_TX_ENA_MSK) >> MAC_REG_CTRL_AND_CFG_BIT_TX_ENA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_TX_ENA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_MAC_ADDR_LSB_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_MAC_ADDR_LSB_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_MAC_ADDR_LSB_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_MAC_ADDR_LSB_set", A, value );

    /* (0x0000800c bits 31:0) bits 0:31 use field MAC_ADDR_LSB of register PMC_MAC_REG_MAC_ADR_LSB_ */
    emac_mac_reg_MAC_ADR_LSB__field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_MSK,
                                         MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_MAC_ADDR_LSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_MAC_ADDR_LSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_MAC_ADDR_LSB_get", A, 11);
    /* (0x0000800c bits 31:0) bits 0:31 use field MAC_ADDR_LSB of register PMC_MAC_REG_MAC_ADR_LSB_ */
    reg_value = emac_mac_reg_MAC_ADR_LSB__read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_MSK) >> MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_MAC_ADDR_LSB_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_MAC_ADDR_LSB_set( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_MAC_ADDR_LSB_set( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_MAC_ADDR_LSB_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_MAC_ADDR_LSB_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_MAC_ADDR_LSB_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_MAC_ADDR_LSB_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000800c bits 31:0) bits 0:31 use field MAC_ADDR_LSB of register PMC_MAC_REG_MAC_ADR_LSB_ */
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
        /* (0x0000800c bits 31:0) bits 0:31 use field MAC_ADDR_LSB of register PMC_MAC_REG_MAC_ADR_LSB_ */
        emac_mac_reg_MAC_ADR_LSB__field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_OFF + subfield_offset),
                                             MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_MAC_ADDR_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_MAC_ADDR_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_MAC_ADDR_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_MAC_ADDR_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_MAC_ADDR_LSB_get", stop_bit, 31 );
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
    /* (0x0000800c bits 31:0) bits 0:31 use field MAC_ADDR_LSB of register PMC_MAC_REG_MAC_ADR_LSB_ */
    reg_value = emac_mac_reg_MAC_ADR_LSB__read(  b_ptr, h_ptr, A);
    field_value = (reg_value & MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_MSK)
                  >> MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_MSK, MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_MAC_ADDR_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_MAC_ADDR_MSB_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_MAC_ADDR_MSB_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_MAC_ADDR_MSB_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_MAC_ADDR_MSB_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_MAC_ADDR_MSB_set", A, value );

    /* (0x00008010 bits 15:0) bits 0:15 use field MAC_ADDR_MSB of register PMC_MAC_REG_MAC_ADR_MSB_ */
    emac_mac_reg_MAC_ADR_MSB__field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_MSK,
                                         MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_MAC_ADDR_MSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_MAC_ADDR_MSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_MAC_ADDR_MSB_get", A, 11);
    /* (0x00008010 bits 15:0) bits 0:15 use field MAC_ADDR_MSB of register PMC_MAC_REG_MAC_ADR_MSB_ */
    reg_value = emac_mac_reg_MAC_ADR_MSB__read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_MSK) >> MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_MAC_ADDR_MSB_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_MAC_ADDR_MSB_set( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_MAC_ADDR_MSB_set( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_MAC_ADDR_MSB_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_MAC_ADDR_MSB_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_MAC_ADDR_MSB_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_MAC_ADDR_MSB_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008010 bits 15:0) bits 0:15 use field MAC_ADDR_MSB of register PMC_MAC_REG_MAC_ADR_MSB_ */
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
        /* (0x00008010 bits 15:0) bits 0:15 use field MAC_ADDR_MSB of register PMC_MAC_REG_MAC_ADR_MSB_ */
        emac_mac_reg_MAC_ADR_MSB__field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_OFF + subfield_offset),
                                             MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_MAC_ADDR_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_MAC_ADDR_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_MAC_ADDR_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_MAC_ADDR_MSB_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_MAC_ADDR_MSB_get", stop_bit, 15 );
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
    /* (0x00008010 bits 15:0) bits 0:15 use field MAC_ADDR_MSB of register PMC_MAC_REG_MAC_ADR_MSB_ */
    reg_value = emac_mac_reg_MAC_ADR_MSB__read(  b_ptr, h_ptr, A);
    field_value = (reg_value & MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_MSK)
                  >> MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_MSK, MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_MAC_ADDR_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_FRM_LENGTH_set( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_FRM_LENGTH_set( emac_mac_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_FRM_LENGTH_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_FRM_LENGTH_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_FRM_LENGTH_set", A, value );

    /* (0x00008014 bits 15:0) bits 0:15 use field FRM_LENGTH of register PMC_MAC_REG_FRAME_LENGTH */
    emac_mac_reg_FRAME_LENGTH_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_MSK,
                                         MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_OFF,
                                         value);
}

static INLINE UINT32 emac_mac_field_FRM_LENGTH_get( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_FRM_LENGTH_get( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_FRM_LENGTH_get", A, 11);
    /* (0x00008014 bits 15:0) bits 0:15 use field FRM_LENGTH of register PMC_MAC_REG_FRAME_LENGTH */
    reg_value = emac_mac_reg_FRAME_LENGTH_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_MSK) >> MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_FRM_LENGTH_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_FRM_LENGTH_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_FRM_LENGTH_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_FRM_LENGTH_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_FRM_LENGTH_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_FRM_LENGTH_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_FRM_LENGTH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008014 bits 15:0) bits 0:15 use field FRM_LENGTH of register PMC_MAC_REG_FRAME_LENGTH */
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
        /* (0x00008014 bits 15:0) bits 0:15 use field FRM_LENGTH of register PMC_MAC_REG_FRAME_LENGTH */
        emac_mac_reg_FRAME_LENGTH_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_OFF + subfield_offset),
                                             MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_FRM_LENGTH_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_FRM_LENGTH_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_FRM_LENGTH_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_FRM_LENGTH_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_FRM_LENGTH_get", stop_bit, 15 );
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
    /* (0x00008014 bits 15:0) bits 0:15 use field FRM_LENGTH of register PMC_MAC_REG_FRAME_LENGTH */
    reg_value = emac_mac_reg_FRAME_LENGTH_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_MSK)
                  >> MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_MSK, MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_FRM_LENGTH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_TX_IPG_LENGTH_set( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_TX_IPG_LENGTH_set( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_IPG_LENGTH_set", A, 11);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_TX_IPG_LENGTH_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_TX_IPG_LENGTH_set", A, value );

    /* (0x00008044 bits 6:0) bits 0:6 use field TX_IPG_LENGTH of register PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_ */
    emac_mac_reg_TX_INTER_PACKET_GAP_IPG__field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_MSK,
                                                     MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_OFF,
                                                     value);
}

static INLINE UINT32 emac_mac_field_TX_IPG_LENGTH_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_TX_IPG_LENGTH_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_IPG_LENGTH_get", A, 11);
    /* (0x00008044 bits 6:0) bits 0:6 use field TX_IPG_LENGTH of register PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_ */
    reg_value = emac_mac_reg_TX_INTER_PACKET_GAP_IPG__read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_MSK) >> MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_TX_IPG_LENGTH_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_TX_IPG_LENGTH_set( emac_mac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_TX_IPG_LENGTH_set( emac_mac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_TX_IPG_LENGTH_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_TX_IPG_LENGTH_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_TX_IPG_LENGTH_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_TX_IPG_LENGTH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008044 bits 6:0) bits 0:6 use field TX_IPG_LENGTH of register PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_ */
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
        /* (0x00008044 bits 6:0) bits 0:6 use field TX_IPG_LENGTH of register PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_ */
        emac_mac_reg_TX_INTER_PACKET_GAP_IPG__field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_OFF + subfield_offset),
                                                         MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_TX_IPG_LENGTH_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_TX_IPG_LENGTH_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_TX_IPG_LENGTH_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_TX_IPG_LENGTH_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_TX_IPG_LENGTH_get", stop_bit, 6 );
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
    /* (0x00008044 bits 6:0) bits 0:6 use field TX_IPG_LENGTH of register PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_ */
    reg_value = emac_mac_reg_TX_INTER_PACKET_GAP_IPG__read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_MSK)
                  >> MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_MSK, MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_TX_IPG_LENGTH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CREDIT_RESET_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CREDIT_RESET_set( emac_mac_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CREDIT_RESET_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CREDIT_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CREDIT_RESET_set", A, value );

    /* (0x00008048 bits 0) field CREDIT_RESET of register PMC_MAC_REG_RX_APPLICATION_CREDIT_RESET */
    emac_mac_reg_RX_APPLICATION_CREDIT_RESET_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        MAC_REG_RX_APPLICATION_CREDIT_RESET_BIT_CREDIT_RESET_MSK,
                                                        MAC_REG_RX_APPLICATION_CREDIT_RESET_BIT_CREDIT_RESET_OFF,
                                                        value);
}

static INLINE UINT32 emac_mac_field_CREDIT_RESET_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CREDIT_RESET_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CREDIT_RESET_get", A, 11);
    /* (0x00008048 bits 0) field CREDIT_RESET of register PMC_MAC_REG_RX_APPLICATION_CREDIT_RESET */
    reg_value = emac_mac_reg_RX_APPLICATION_CREDIT_RESET_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_RX_APPLICATION_CREDIT_RESET_BIT_CREDIT_RESET_MSK) >> MAC_REG_RX_APPLICATION_CREDIT_RESET_BIT_CREDIT_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CREDIT_RESET_get", A, value );

    return value;
}
static INLINE void emac_mac_field_CREDIT_INIT_set( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CREDIT_INIT_set( emac_mac_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CREDIT_INIT_set", A, 11);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CREDIT_INIT_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CREDIT_INIT_set", A, value );

    /* (0x0000804c bits 7:0) bits 0:7 use field CREDIT_INIT of register PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION */
    emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_MSK,
                                                                 MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_OFF,
                                                                 value);
}

static INLINE UINT32 emac_mac_field_CREDIT_INIT_get( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CREDIT_INIT_get( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CREDIT_INIT_get", A, 11);
    /* (0x0000804c bits 7:0) bits 0:7 use field CREDIT_INIT of register PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION */
    reg_value = emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_MSK) >> MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CREDIT_INIT_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CREDIT_INIT_set( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CREDIT_INIT_set( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CREDIT_INIT_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CREDIT_INIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CREDIT_INIT_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CREDIT_INIT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000804c bits 7:0) bits 0:7 use field CREDIT_INIT of register PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION */
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
        /* (0x0000804c bits 7:0) bits 0:7 use field CREDIT_INIT of register PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION */
        emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_OFF + subfield_offset),
                                                                     MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CREDIT_INIT_get( emac_mac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CREDIT_INIT_get( emac_mac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CREDIT_INIT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CREDIT_INIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CREDIT_INIT_get", stop_bit, 7 );
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
    /* (0x0000804c bits 7:0) bits 0:7 use field CREDIT_INIT of register PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION */
    reg_value = emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_MSK)
                  >> MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_MSK, MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CREDIT_INIT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL1_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL1_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL1_PAUSE_QUANTA_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL1_PAUSE_QUANTA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL1_PAUSE_QUANTA_set", A, value );

    /* (0x00008054 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
    emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_MSK,
                                                   MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_OFF,
                                                   value);
}

static INLINE UINT32 emac_mac_field_CL1_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL1_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL1_PAUSE_QUANTA_get", A, 11);
    /* (0x00008054 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_MSK) >> MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL1_PAUSE_QUANTA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL1_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL1_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL1_PAUSE_QUANTA_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL1_PAUSE_QUANTA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL1_PAUSE_QUANTA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL1_PAUSE_QUANTA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008054 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
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
        /* (0x00008054 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
        emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_OFF + subfield_offset),
                                                       MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL1_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL1_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL1_PAUSE_QUANTA_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL1_PAUSE_QUANTA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL1_PAUSE_QUANTA_get", stop_bit, 15 );
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
    /* (0x00008054 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_MSK)
                  >> MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_MSK, MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL1_PAUSE_QUANTA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL0_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL0_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL0_PAUSE_QUANTA_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL0_PAUSE_QUANTA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL0_PAUSE_QUANTA_set", A, value );

    /* (0x00008054 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
    emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_MSK,
                                                   MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_OFF,
                                                   value);
}

static INLINE UINT32 emac_mac_field_CL0_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL0_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL0_PAUSE_QUANTA_get", A, 11);
    /* (0x00008054 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_MSK) >> MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL0_PAUSE_QUANTA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL0_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL0_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL0_PAUSE_QUANTA_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL0_PAUSE_QUANTA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL0_PAUSE_QUANTA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL0_PAUSE_QUANTA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008054 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
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
        /* (0x00008054 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
        emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_OFF + subfield_offset),
                                                       MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL0_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL0_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL0_PAUSE_QUANTA_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL0_PAUSE_QUANTA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL0_PAUSE_QUANTA_get", stop_bit, 15 );
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
    /* (0x00008054 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_MSK)
                  >> MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_MSK, MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL0_PAUSE_QUANTA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL3_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL3_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL3_PAUSE_QUANTA_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL3_PAUSE_QUANTA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL3_PAUSE_QUANTA_set", A, value );

    /* (0x00008058 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
    emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_MSK,
                                                   MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_OFF,
                                                   value);
}

static INLINE UINT32 emac_mac_field_CL3_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL3_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL3_PAUSE_QUANTA_get", A, 11);
    /* (0x00008058 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_MSK) >> MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL3_PAUSE_QUANTA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL3_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL3_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL3_PAUSE_QUANTA_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL3_PAUSE_QUANTA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL3_PAUSE_QUANTA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL3_PAUSE_QUANTA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008058 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
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
        /* (0x00008058 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
        emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_OFF + subfield_offset),
                                                       MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL3_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL3_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL3_PAUSE_QUANTA_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL3_PAUSE_QUANTA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL3_PAUSE_QUANTA_get", stop_bit, 15 );
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
    /* (0x00008058 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_MSK)
                  >> MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_MSK, MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL3_PAUSE_QUANTA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL2_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL2_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL2_PAUSE_QUANTA_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL2_PAUSE_QUANTA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL2_PAUSE_QUANTA_set", A, value );

    /* (0x00008058 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
    emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_MSK,
                                                   MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_OFF,
                                                   value);
}

static INLINE UINT32 emac_mac_field_CL2_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL2_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL2_PAUSE_QUANTA_get", A, 11);
    /* (0x00008058 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_MSK) >> MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL2_PAUSE_QUANTA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL2_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL2_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL2_PAUSE_QUANTA_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL2_PAUSE_QUANTA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL2_PAUSE_QUANTA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL2_PAUSE_QUANTA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008058 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
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
        /* (0x00008058 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
        emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_OFF + subfield_offset),
                                                       MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL2_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL2_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL2_PAUSE_QUANTA_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL2_PAUSE_QUANTA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL2_PAUSE_QUANTA_get", stop_bit, 15 );
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
    /* (0x00008058 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_MSK)
                  >> MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_MSK, MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL2_PAUSE_QUANTA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL5_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL5_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL5_PAUSE_QUANTA_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL5_PAUSE_QUANTA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL5_PAUSE_QUANTA_set", A, value );

    /* (0x0000805c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
    emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_MSK,
                                                   MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_OFF,
                                                   value);
}

static INLINE UINT32 emac_mac_field_CL5_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL5_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL5_PAUSE_QUANTA_get", A, 11);
    /* (0x0000805c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_MSK) >> MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL5_PAUSE_QUANTA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL5_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL5_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL5_PAUSE_QUANTA_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL5_PAUSE_QUANTA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL5_PAUSE_QUANTA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL5_PAUSE_QUANTA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000805c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
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
        /* (0x0000805c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
        emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_OFF + subfield_offset),
                                                       MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL5_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL5_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL5_PAUSE_QUANTA_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL5_PAUSE_QUANTA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL5_PAUSE_QUANTA_get", stop_bit, 15 );
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
    /* (0x0000805c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_MSK)
                  >> MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_MSK, MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL5_PAUSE_QUANTA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL4_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL4_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL4_PAUSE_QUANTA_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL4_PAUSE_QUANTA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL4_PAUSE_QUANTA_set", A, value );

    /* (0x0000805c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
    emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_MSK,
                                                   MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_OFF,
                                                   value);
}

static INLINE UINT32 emac_mac_field_CL4_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL4_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL4_PAUSE_QUANTA_get", A, 11);
    /* (0x0000805c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_MSK) >> MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL4_PAUSE_QUANTA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL4_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL4_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL4_PAUSE_QUANTA_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL4_PAUSE_QUANTA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL4_PAUSE_QUANTA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL4_PAUSE_QUANTA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000805c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
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
        /* (0x0000805c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
        emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_OFF + subfield_offset),
                                                       MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL4_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL4_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL4_PAUSE_QUANTA_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL4_PAUSE_QUANTA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL4_PAUSE_QUANTA_get", stop_bit, 15 );
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
    /* (0x0000805c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_MSK)
                  >> MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_MSK, MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL4_PAUSE_QUANTA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL7_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL7_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL7_PAUSE_QUANTA_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL7_PAUSE_QUANTA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL7_PAUSE_QUANTA_set", A, value );

    /* (0x00008060 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
    emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_MSK,
                                                   MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_OFF,
                                                   value);
}

static INLINE UINT32 emac_mac_field_CL7_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL7_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL7_PAUSE_QUANTA_get", A, 11);
    /* (0x00008060 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_MSK) >> MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL7_PAUSE_QUANTA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL7_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL7_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL7_PAUSE_QUANTA_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL7_PAUSE_QUANTA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL7_PAUSE_QUANTA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL7_PAUSE_QUANTA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008060 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
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
        /* (0x00008060 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
        emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_OFF + subfield_offset),
                                                       MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL7_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL7_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL7_PAUSE_QUANTA_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL7_PAUSE_QUANTA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL7_PAUSE_QUANTA_get", stop_bit, 15 );
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
    /* (0x00008060 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_MSK)
                  >> MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_MSK, MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL7_PAUSE_QUANTA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL6_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL6_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL6_PAUSE_QUANTA_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL6_PAUSE_QUANTA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL6_PAUSE_QUANTA_set", A, value );

    /* (0x00008060 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
    emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_MSK,
                                                   MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_OFF,
                                                   value);
}

static INLINE UINT32 emac_mac_field_CL6_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL6_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL6_PAUSE_QUANTA_get", A, 11);
    /* (0x00008060 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_MSK) >> MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL6_PAUSE_QUANTA_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL6_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL6_PAUSE_QUANTA_set( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL6_PAUSE_QUANTA_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL6_PAUSE_QUANTA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL6_PAUSE_QUANTA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL6_PAUSE_QUANTA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008060 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
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
        /* (0x00008060 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
        emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_OFF + subfield_offset),
                                                       MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL6_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL6_PAUSE_QUANTA_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL6_PAUSE_QUANTA_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL6_PAUSE_QUANTA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL6_PAUSE_QUANTA_get", stop_bit, 15 );
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
    /* (0x00008060 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA */
    reg_value = emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_MSK)
                  >> MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_MSK, MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL6_PAUSE_QUANTA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL1_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL1_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL1_PAUSE_QUANTA_THR_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL1_PAUSE_QUANTA_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL1_PAUSE_QUANTA_THR_set", A, value );

    /* (0x00008064 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
    emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_MSK,
                                                      MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_OFF,
                                                      value);
}

static INLINE UINT32 emac_mac_field_CL1_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL1_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL1_PAUSE_QUANTA_THR_get", A, 11);
    /* (0x00008064 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_MSK) >> MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL1_PAUSE_QUANTA_THR_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL1_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL1_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL1_PAUSE_QUANTA_THR_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL1_PAUSE_QUANTA_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL1_PAUSE_QUANTA_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL1_PAUSE_QUANTA_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008064 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
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
        /* (0x00008064 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
        emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_OFF + subfield_offset),
                                                          MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL1_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL1_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL1_PAUSE_QUANTA_THR_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL1_PAUSE_QUANTA_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL1_PAUSE_QUANTA_THR_get", stop_bit, 15 );
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
    /* (0x00008064 bits 31:16) bits 0:15 use field CL1_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_MSK)
                  >> MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_MSK, MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL1_PAUSE_QUANTA_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL0_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL0_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL0_PAUSE_QUANTA_THR_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL0_PAUSE_QUANTA_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL0_PAUSE_QUANTA_THR_set", A, value );

    /* (0x00008064 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
    emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_MSK,
                                                      MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_OFF,
                                                      value);
}

static INLINE UINT32 emac_mac_field_CL0_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL0_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL0_PAUSE_QUANTA_THR_get", A, 11);
    /* (0x00008064 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_MSK) >> MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL0_PAUSE_QUANTA_THR_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL0_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL0_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL0_PAUSE_QUANTA_THR_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL0_PAUSE_QUANTA_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL0_PAUSE_QUANTA_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL0_PAUSE_QUANTA_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008064 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
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
        /* (0x00008064 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
        emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_OFF + subfield_offset),
                                                          MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL0_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL0_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL0_PAUSE_QUANTA_THR_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL0_PAUSE_QUANTA_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL0_PAUSE_QUANTA_THR_get", stop_bit, 15 );
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
    /* (0x00008064 bits 15:0) bits 0:15 use field CL0_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_MSK)
                  >> MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_MSK, MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL0_PAUSE_QUANTA_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL3_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL3_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL3_PAUSE_QUANTA_THR_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL3_PAUSE_QUANTA_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL3_PAUSE_QUANTA_THR_set", A, value );

    /* (0x00008068 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
    emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_MSK,
                                                      MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_OFF,
                                                      value);
}

static INLINE UINT32 emac_mac_field_CL3_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL3_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL3_PAUSE_QUANTA_THR_get", A, 11);
    /* (0x00008068 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_MSK) >> MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL3_PAUSE_QUANTA_THR_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL3_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL3_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL3_PAUSE_QUANTA_THR_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL3_PAUSE_QUANTA_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL3_PAUSE_QUANTA_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL3_PAUSE_QUANTA_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008068 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
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
        /* (0x00008068 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
        emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_OFF + subfield_offset),
                                                          MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL3_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL3_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL3_PAUSE_QUANTA_THR_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL3_PAUSE_QUANTA_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL3_PAUSE_QUANTA_THR_get", stop_bit, 15 );
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
    /* (0x00008068 bits 31:16) bits 0:15 use field CL3_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_MSK)
                  >> MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_MSK, MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL3_PAUSE_QUANTA_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL2_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL2_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL2_PAUSE_QUANTA_THR_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL2_PAUSE_QUANTA_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL2_PAUSE_QUANTA_THR_set", A, value );

    /* (0x00008068 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
    emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_MSK,
                                                      MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_OFF,
                                                      value);
}

static INLINE UINT32 emac_mac_field_CL2_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL2_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL2_PAUSE_QUANTA_THR_get", A, 11);
    /* (0x00008068 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_MSK) >> MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL2_PAUSE_QUANTA_THR_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL2_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL2_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL2_PAUSE_QUANTA_THR_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL2_PAUSE_QUANTA_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL2_PAUSE_QUANTA_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL2_PAUSE_QUANTA_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008068 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
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
        /* (0x00008068 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
        emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_OFF + subfield_offset),
                                                          MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL2_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL2_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL2_PAUSE_QUANTA_THR_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL2_PAUSE_QUANTA_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL2_PAUSE_QUANTA_THR_get", stop_bit, 15 );
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
    /* (0x00008068 bits 15:0) bits 0:15 use field CL2_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_MSK)
                  >> MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_MSK, MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL2_PAUSE_QUANTA_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL5_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL5_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL5_PAUSE_QUANTA_THR_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL5_PAUSE_QUANTA_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL5_PAUSE_QUANTA_THR_set", A, value );

    /* (0x0000806c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
    emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_MSK,
                                                      MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_OFF,
                                                      value);
}

static INLINE UINT32 emac_mac_field_CL5_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL5_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL5_PAUSE_QUANTA_THR_get", A, 11);
    /* (0x0000806c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_MSK) >> MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL5_PAUSE_QUANTA_THR_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL5_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL5_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL5_PAUSE_QUANTA_THR_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL5_PAUSE_QUANTA_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL5_PAUSE_QUANTA_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL5_PAUSE_QUANTA_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000806c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
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
        /* (0x0000806c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
        emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_OFF + subfield_offset),
                                                          MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL5_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL5_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL5_PAUSE_QUANTA_THR_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL5_PAUSE_QUANTA_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL5_PAUSE_QUANTA_THR_get", stop_bit, 15 );
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
    /* (0x0000806c bits 31:16) bits 0:15 use field CL5_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_MSK)
                  >> MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_MSK, MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL5_PAUSE_QUANTA_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL4_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL4_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL4_PAUSE_QUANTA_THR_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL4_PAUSE_QUANTA_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL4_PAUSE_QUANTA_THR_set", A, value );

    /* (0x0000806c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
    emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_MSK,
                                                      MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_OFF,
                                                      value);
}

static INLINE UINT32 emac_mac_field_CL4_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL4_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL4_PAUSE_QUANTA_THR_get", A, 11);
    /* (0x0000806c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_MSK) >> MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL4_PAUSE_QUANTA_THR_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL4_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL4_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL4_PAUSE_QUANTA_THR_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL4_PAUSE_QUANTA_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL4_PAUSE_QUANTA_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL4_PAUSE_QUANTA_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000806c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
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
        /* (0x0000806c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
        emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_OFF + subfield_offset),
                                                          MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL4_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL4_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL4_PAUSE_QUANTA_THR_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL4_PAUSE_QUANTA_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL4_PAUSE_QUANTA_THR_get", stop_bit, 15 );
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
    /* (0x0000806c bits 15:0) bits 0:15 use field CL4_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_MSK)
                  >> MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_MSK, MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL4_PAUSE_QUANTA_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL7_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL7_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL7_PAUSE_QUANTA_THR_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL7_PAUSE_QUANTA_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL7_PAUSE_QUANTA_THR_set", A, value );

    /* (0x00008070 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
    emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_MSK,
                                                      MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_OFF,
                                                      value);
}

static INLINE UINT32 emac_mac_field_CL7_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL7_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL7_PAUSE_QUANTA_THR_get", A, 11);
    /* (0x00008070 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_MSK) >> MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL7_PAUSE_QUANTA_THR_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL7_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL7_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL7_PAUSE_QUANTA_THR_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL7_PAUSE_QUANTA_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL7_PAUSE_QUANTA_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL7_PAUSE_QUANTA_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008070 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
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
        /* (0x00008070 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
        emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_OFF + subfield_offset),
                                                          MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL7_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL7_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL7_PAUSE_QUANTA_THR_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL7_PAUSE_QUANTA_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL7_PAUSE_QUANTA_THR_get", stop_bit, 15 );
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
    /* (0x00008070 bits 31:16) bits 0:15 use field CL7_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_MSK)
                  >> MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_MSK, MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL7_PAUSE_QUANTA_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_CL6_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_CL6_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL6_PAUSE_QUANTA_THR_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_CL6_PAUSE_QUANTA_THR_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_CL6_PAUSE_QUANTA_THR_set", A, value );

    /* (0x00008070 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
    emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_MSK,
                                                      MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_OFF,
                                                      value);
}

static INLINE UINT32 emac_mac_field_CL6_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CL6_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CL6_PAUSE_QUANTA_THR_get", A, 11);
    /* (0x00008070 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_MSK) >> MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CL6_PAUSE_QUANTA_THR_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_CL6_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_CL6_PAUSE_QUANTA_THR_set( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL6_PAUSE_QUANTA_THR_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL6_PAUSE_QUANTA_THR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL6_PAUSE_QUANTA_THR_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL6_PAUSE_QUANTA_THR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008070 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
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
        /* (0x00008070 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
        emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_OFF + subfield_offset),
                                                          MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_CL6_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CL6_PAUSE_QUANTA_THR_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CL6_PAUSE_QUANTA_THR_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CL6_PAUSE_QUANTA_THR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CL6_PAUSE_QUANTA_THR_get", stop_bit, 15 );
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
    /* (0x00008070 bits 15:0) bits 0:15 use field CL6_PAUSE_QUANTA_THR of register PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH */
    reg_value = emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_MSK)
                  >> MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_MSK, MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CL6_PAUSE_QUANTA_THR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_TX_PREAMBLE_LSB_set( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_TX_PREAMBLE_LSB_set( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_PREAMBLE_LSB_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_TX_PREAMBLE_LSB_set", A, value );

    /* (0x00008280 bits 31:0) bits 0:31 use field TX_PREAMBLE_LSB of register PMC_MAC_REG_TX_PREAMBLE_LSB_ */
    emac_mac_reg_TX_PREAMBLE_LSB__field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_MSK,
                                             MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_OFF,
                                             value);
}

static INLINE UINT32 emac_mac_field_TX_PREAMBLE_LSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_TX_PREAMBLE_LSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_PREAMBLE_LSB_get", A, 11);
    /* (0x00008280 bits 31:0) bits 0:31 use field TX_PREAMBLE_LSB of register PMC_MAC_REG_TX_PREAMBLE_LSB_ */
    reg_value = emac_mac_reg_TX_PREAMBLE_LSB__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_MSK) >> MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_TX_PREAMBLE_LSB_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_TX_PREAMBLE_LSB_set( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_TX_PREAMBLE_LSB_set( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_TX_PREAMBLE_LSB_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_TX_PREAMBLE_LSB_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_TX_PREAMBLE_LSB_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_TX_PREAMBLE_LSB_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008280 bits 31:0) bits 0:31 use field TX_PREAMBLE_LSB of register PMC_MAC_REG_TX_PREAMBLE_LSB_ */
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
        /* (0x00008280 bits 31:0) bits 0:31 use field TX_PREAMBLE_LSB of register PMC_MAC_REG_TX_PREAMBLE_LSB_ */
        emac_mac_reg_TX_PREAMBLE_LSB__field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_OFF + subfield_offset),
                                                 MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_TX_PREAMBLE_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_TX_PREAMBLE_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_TX_PREAMBLE_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_TX_PREAMBLE_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_TX_PREAMBLE_LSB_get", stop_bit, 31 );
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
    /* (0x00008280 bits 31:0) bits 0:31 use field TX_PREAMBLE_LSB of register PMC_MAC_REG_TX_PREAMBLE_LSB_ */
    reg_value = emac_mac_reg_TX_PREAMBLE_LSB__read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_MSK)
                  >> MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_MSK, MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_TX_PREAMBLE_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_mac_field_TX_PREAMBLE_MSB_set( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_TX_PREAMBLE_MSB_set( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_PREAMBLE_MSB_set", A, 11);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_mac_field_TX_PREAMBLE_MSB_set", value, 16777215);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_TX_PREAMBLE_MSB_set", A, value );

    /* (0x00008284 bits 23:0) bits 0:23 use field TX_PREAMBLE_MSB of register PMC_MAC_REG_TX_PREAMBLE_MSB_ */
    emac_mac_reg_TX_PREAMBLE_MSB__field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_MSK,
                                             MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_OFF,
                                             value);
}

static INLINE UINT32 emac_mac_field_TX_PREAMBLE_MSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_TX_PREAMBLE_MSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_TX_PREAMBLE_MSB_get", A, 11);
    /* (0x00008284 bits 23:0) bits 0:23 use field TX_PREAMBLE_MSB of register PMC_MAC_REG_TX_PREAMBLE_MSB_ */
    reg_value = emac_mac_reg_TX_PREAMBLE_MSB__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_MSK) >> MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_TX_PREAMBLE_MSB_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_TX_PREAMBLE_MSB_set( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_TX_PREAMBLE_MSB_set( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_TX_PREAMBLE_MSB_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_TX_PREAMBLE_MSB_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_TX_PREAMBLE_MSB_set", stop_bit, 23 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_TX_PREAMBLE_MSB_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008284 bits 23:0) bits 0:23 use field TX_PREAMBLE_MSB of register PMC_MAC_REG_TX_PREAMBLE_MSB_ */
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
        /* (0x00008284 bits 23:0) bits 0:23 use field TX_PREAMBLE_MSB of register PMC_MAC_REG_TX_PREAMBLE_MSB_ */
        emac_mac_reg_TX_PREAMBLE_MSB__field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_OFF + subfield_offset),
                                                 MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_TX_PREAMBLE_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_TX_PREAMBLE_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_TX_PREAMBLE_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_TX_PREAMBLE_MSB_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_TX_PREAMBLE_MSB_get", stop_bit, 23 );
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
    /* (0x00008284 bits 23:0) bits 0:23 use field TX_PREAMBLE_MSB of register PMC_MAC_REG_TX_PREAMBLE_MSB_ */
    reg_value = emac_mac_reg_TX_PREAMBLE_MSB__read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_MSK)
                  >> MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_MSK, MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_TX_PREAMBLE_MSB_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset scratch
 * ==================================================================================
 */
static INLINE void emac_mac_field_SCRATCH_set( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_SCRATCH_set( emac_mac_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SCRATCH_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "emac_mac_field_SCRATCH_set", A, value );

    /* (0x00008004 bits 31:0) bits 0:31 use field SCRATCH of register PMC_MAC_REG_SCRATCH */
    emac_mac_reg_SCRATCH_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MAC_REG_SCRATCH_BIT_SCRATCH_MSK,
                                    MAC_REG_SCRATCH_BIT_SCRATCH_OFF,
                                    value);
}

static INLINE UINT32 emac_mac_field_SCRATCH_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_SCRATCH_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_SCRATCH_get", A, 11);
    /* (0x00008004 bits 31:0) bits 0:31 use field SCRATCH of register PMC_MAC_REG_SCRATCH */
    reg_value = emac_mac_reg_SCRATCH_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_SCRATCH_BIT_SCRATCH_MSK) >> MAC_REG_SCRATCH_BIT_SCRATCH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_SCRATCH_get", A, value );

    return value;
}
static INLINE void emac_mac_field_range_SCRATCH_set( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_mac_field_range_SCRATCH_set( emac_mac_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_SCRATCH_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_SCRATCH_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_SCRATCH_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_SCRATCH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00008004 bits 31:0) bits 0:31 use field SCRATCH of register PMC_MAC_REG_SCRATCH */
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
        /* (0x00008004 bits 31:0) bits 0:31 use field SCRATCH of register PMC_MAC_REG_SCRATCH */
        emac_mac_reg_SCRATCH_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (MAC_REG_SCRATCH_BIT_SCRATCH_OFF + subfield_offset),
                                        MAC_REG_SCRATCH_BIT_SCRATCH_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 emac_mac_field_range_SCRATCH_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_SCRATCH_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_SCRATCH_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_SCRATCH_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_SCRATCH_get", stop_bit, 31 );
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
    /* (0x00008004 bits 31:0) bits 0:31 use field SCRATCH of register PMC_MAC_REG_SCRATCH */
    reg_value = emac_mac_reg_SCRATCH_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & MAC_REG_SCRATCH_BIT_SCRATCH_MSK)
                  >> MAC_REG_SCRATCH_BIT_SCRATCH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_SCRATCH_BIT_SCRATCH_MSK, MAC_REG_SCRATCH_BIT_SCRATCH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_SCRATCH_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 emac_mac_field_PRGM_REVISION_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_PRGM_REVISION_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PRGM_REVISION_get", A, 11);
    /* (0x00008000 bits 31:16) bits 0:15 use field PRGM_REVISION of register PMC_MAC_REG_REVISION */
    reg_value = emac_mac_reg_REVISION_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_REVISION_BIT_PRGM_REVISION_MSK) >> MAC_REG_REVISION_BIT_PRGM_REVISION_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_PRGM_REVISION_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_PRGM_REVISION_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_PRGM_REVISION_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_PRGM_REVISION_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_PRGM_REVISION_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_PRGM_REVISION_get", stop_bit, 15 );
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
    /* (0x00008000 bits 31:16) bits 0:15 use field PRGM_REVISION of register PMC_MAC_REG_REVISION */
    reg_value = emac_mac_reg_REVISION_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & MAC_REG_REVISION_BIT_PRGM_REVISION_MSK)
                  >> MAC_REG_REVISION_BIT_PRGM_REVISION_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_REVISION_BIT_PRGM_REVISION_MSK, MAC_REG_REVISION_BIT_PRGM_REVISION_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_PRGM_REVISION_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_CORE_VERSION_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CORE_VERSION_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CORE_VERSION_get", A, 11);
    /* (0x00008000 bits 15:8) bits 0:7 use field CORE_VERSION of register PMC_MAC_REG_REVISION */
    reg_value = emac_mac_reg_REVISION_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_REVISION_BIT_CORE_VERSION_MSK) >> MAC_REG_REVISION_BIT_CORE_VERSION_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CORE_VERSION_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_CORE_VERSION_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CORE_VERSION_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CORE_VERSION_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CORE_VERSION_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CORE_VERSION_get", stop_bit, 7 );
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
    /* (0x00008000 bits 15:8) bits 0:7 use field CORE_VERSION of register PMC_MAC_REG_REVISION */
    reg_value = emac_mac_reg_REVISION_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & MAC_REG_REVISION_BIT_CORE_VERSION_MSK)
                  >> MAC_REG_REVISION_BIT_CORE_VERSION_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_REVISION_BIT_CORE_VERSION_MSK, MAC_REG_REVISION_BIT_CORE_VERSION_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CORE_VERSION_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_CORE_REVISION_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_CORE_REVISION_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_CORE_REVISION_get", A, 11);
    /* (0x00008000 bits 7:0) bits 0:7 use field CORE_REVISION of register PMC_MAC_REG_REVISION */
    reg_value = emac_mac_reg_REVISION_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_REVISION_BIT_CORE_REVISION_MSK) >> MAC_REG_REVISION_BIT_CORE_REVISION_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_CORE_REVISION_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_CORE_REVISION_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_CORE_REVISION_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_CORE_REVISION_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_CORE_REVISION_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_CORE_REVISION_get", stop_bit, 7 );
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
    /* (0x00008000 bits 7:0) bits 0:7 use field CORE_REVISION of register PMC_MAC_REG_REVISION */
    reg_value = emac_mac_reg_REVISION_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & MAC_REG_REVISION_BIT_CORE_REVISION_MSK)
                  >> MAC_REG_REVISION_BIT_CORE_REVISION_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_REVISION_BIT_CORE_REVISION_MSK, MAC_REG_REVISION_BIT_CORE_REVISION_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_CORE_REVISION_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_PHY_LOS_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_PHY_LOS_get( emac_mac_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PHY_LOS_get", A, 11);
    /* (0x00008040 bits 2) field PHY_LOS of register PMC_MAC_REG_STAT */
    reg_value = emac_mac_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_STAT_BIT_PHY_LOS_MSK) >> MAC_REG_STAT_BIT_PHY_LOS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_PHY_LOS_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_RX_REM_FAULT_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_RX_REM_FAULT_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_RX_REM_FAULT_get", A, 11);
    /* (0x00008040 bits 1) field RX_REM_FAULT of register PMC_MAC_REG_STAT */
    reg_value = emac_mac_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_STAT_BIT_RX_REM_FAULT_MSK) >> MAC_REG_STAT_BIT_RX_REM_FAULT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_RX_REM_FAULT_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_RX_LOC_FAULT_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_RX_LOC_FAULT_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_RX_LOC_FAULT_get", A, 11);
    /* (0x00008040 bits 0) field RX_LOC_FAULT of register PMC_MAC_REG_STAT */
    reg_value = emac_mac_reg_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_STAT_BIT_RX_LOC_FAULT_MSK) >> MAC_REG_STAT_BIT_RX_LOC_FAULT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_RX_LOC_FAULT_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_PAUSE_STATUS_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_PAUSE_STATUS_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_PAUSE_STATUS_get", A, 11);
    /* (0x00008074 bits 7:0) bits 0:7 use field PAUSE_STATUS of register PMC_MAC_REG_RX_PAUSE_STAT */
    reg_value = emac_mac_reg_RX_PAUSE_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & MAC_REG_RX_PAUSE_STAT_BIT_PAUSE_STATUS_MSK) >> MAC_REG_RX_PAUSE_STAT_BIT_PAUSE_STATUS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_PAUSE_STATUS_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_PAUSE_STATUS_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_PAUSE_STATUS_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_PAUSE_STATUS_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_PAUSE_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_PAUSE_STATUS_get", stop_bit, 7 );
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
    /* (0x00008074 bits 7:0) bits 0:7 use field PAUSE_STATUS of register PMC_MAC_REG_RX_PAUSE_STAT */
    reg_value = emac_mac_reg_RX_PAUSE_STAT_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & MAC_REG_RX_PAUSE_STAT_BIT_PAUSE_STATUS_MSK)
                  >> MAC_REG_RX_PAUSE_STAT_BIT_PAUSE_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_RX_PAUSE_STAT_BIT_PAUSE_STATUS_MSK, MAC_REG_RX_PAUSE_STAT_BIT_PAUSE_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_PAUSE_STATUS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AFRAMESTXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AFRAMESTXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AFRAMESTXOK_LSB_get", A, 11);
    /* (0x00008080 bits 31:0) bits 0:31 use field AFRAMESTXOK_LSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_LSB__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_BIT_AFRAMESTXOK_LSB_MSK) >> MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_BIT_AFRAMESTXOK_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AFRAMESTXOK_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AFRAMESTXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AFRAMESTXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AFRAMESTXOK_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AFRAMESTXOK_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AFRAMESTXOK_LSB_get", stop_bit, 31 );
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
    /* (0x00008080 bits 31:0) bits 0:31 use field AFRAMESTXOK_LSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_LSB__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_BIT_AFRAMESTXOK_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_BIT_AFRAMESTXOK_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_BIT_AFRAMESTXOK_LSB_MSK, MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_BIT_AFRAMESTXOK_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AFRAMESTXOK_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AFRAMESTXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AFRAMESTXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AFRAMESTXOK_MSB_get", A, 11);
    /* (0x00008084 bits 7:0) bits 0:7 use field AFRAMESTXOK_MSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_MSB__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_BIT_AFRAMESTXOK_MSB_MSK) >> MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_BIT_AFRAMESTXOK_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AFRAMESTXOK_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AFRAMESTXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AFRAMESTXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AFRAMESTXOK_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AFRAMESTXOK_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AFRAMESTXOK_MSB_get", stop_bit, 7 );
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
    /* (0x00008084 bits 7:0) bits 0:7 use field AFRAMESTXOK_MSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_MSB__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_BIT_AFRAMESTXOK_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_BIT_AFRAMESTXOK_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_BIT_AFRAMESTXOK_MSB_MSK, MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_BIT_AFRAMESTXOK_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AFRAMESTXOK_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AFRAMESRXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AFRAMESRXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AFRAMESRXOK_LSB_get", A, 11);
    /* (0x00008088 bits 31:0) bits 0:31 use field AFRAMESRXOK_LSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_LSB__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_BIT_AFRAMESRXOK_LSB_MSK) >> MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_BIT_AFRAMESRXOK_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AFRAMESRXOK_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AFRAMESRXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AFRAMESRXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AFRAMESRXOK_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AFRAMESRXOK_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AFRAMESRXOK_LSB_get", stop_bit, 31 );
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
    /* (0x00008088 bits 31:0) bits 0:31 use field AFRAMESRXOK_LSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_LSB__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_BIT_AFRAMESRXOK_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_BIT_AFRAMESRXOK_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_BIT_AFRAMESRXOK_LSB_MSK, MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_BIT_AFRAMESRXOK_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AFRAMESRXOK_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AFRAMESRXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AFRAMESRXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AFRAMESRXOK_MSB_get", A, 11);
    /* (0x0000808c bits 7:0) bits 0:7 use field AFRAMESRXOK_MSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_MSB__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_BIT_AFRAMESRXOK_MSB_MSK) >> MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_BIT_AFRAMESRXOK_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AFRAMESRXOK_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AFRAMESRXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AFRAMESRXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AFRAMESRXOK_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AFRAMESRXOK_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AFRAMESRXOK_MSB_get", stop_bit, 7 );
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
    /* (0x0000808c bits 7:0) bits 0:7 use field AFRAMESRXOK_MSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_MSB__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_BIT_AFRAMESRXOK_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_BIT_AFRAMESRXOK_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_BIT_AFRAMESRXOK_MSB_MSK, MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_BIT_AFRAMESRXOK_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AFRAMESRXOK_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AFRAMECHECKSEQUENCEERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AFRAMECHECKSEQUENCEERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AFRAMECHECKSEQUENCEERRORS_LSB_get", A, 11);
    /* (0x00008090 bits 31:0) bits 0:31 use field AFRAMECHECKSEQUENCEERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_BIT_AFRAMECHECKSEQUENCEERRORS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_BIT_AFRAMECHECKSEQUENCEERRORS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AFRAMECHECKSEQUENCEERRORS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_LSB_get", stop_bit, 31 );
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
    /* (0x00008090 bits 31:0) bits 0:31 use field AFRAMECHECKSEQUENCEERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_BIT_AFRAMECHECKSEQUENCEERRORS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_BIT_AFRAMECHECKSEQUENCEERRORS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_BIT_AFRAMECHECKSEQUENCEERRORS_LSB_MSK, MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_BIT_AFRAMECHECKSEQUENCEERRORS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AFRAMECHECKSEQUENCEERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AFRAMECHECKSEQUENCEERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AFRAMECHECKSEQUENCEERRORS_MSB_get", A, 11);
    /* (0x00008094 bits 7:0) bits 0:7 use field AFRAMECHECKSEQUENCEERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_BIT_AFRAMECHECKSEQUENCEERRORS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_BIT_AFRAMECHECKSEQUENCEERRORS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AFRAMECHECKSEQUENCEERRORS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_MSB_get", stop_bit, 7 );
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
    /* (0x00008094 bits 7:0) bits 0:7 use field AFRAMECHECKSEQUENCEERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_BIT_AFRAMECHECKSEQUENCEERRORS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_BIT_AFRAMECHECKSEQUENCEERRORS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_BIT_AFRAMECHECKSEQUENCEERRORS_MSB_MSK, MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_BIT_AFRAMECHECKSEQUENCEERRORS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AALIGNMENTERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AALIGNMENTERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AALIGNMENTERRORS_LSB_get", A, 11);
    /* (0x00008098 bits 31:0) bits 0:31 use field AALIGNMENTERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_LSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_BIT_AALIGNMENTERRORS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_BIT_AALIGNMENTERRORS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AALIGNMENTERRORS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AALIGNMENTERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AALIGNMENTERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AALIGNMENTERRORS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AALIGNMENTERRORS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AALIGNMENTERRORS_LSB_get", stop_bit, 31 );
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
    /* (0x00008098 bits 31:0) bits 0:31 use field AALIGNMENTERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_LSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_BIT_AALIGNMENTERRORS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_BIT_AALIGNMENTERRORS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_BIT_AALIGNMENTERRORS_LSB_MSK, MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_BIT_AALIGNMENTERRORS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AALIGNMENTERRORS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AALIGNMENTERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AALIGNMENTERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AALIGNMENTERRORS_MSB_get", A, 11);
    /* (0x0000809c bits 7:0) bits 0:7 use field AALIGNMENTERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_MSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_BIT_AALIGNMENTERRORS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_BIT_AALIGNMENTERRORS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AALIGNMENTERRORS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AALIGNMENTERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AALIGNMENTERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AALIGNMENTERRORS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AALIGNMENTERRORS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AALIGNMENTERRORS_MSB_get", stop_bit, 7 );
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
    /* (0x0000809c bits 7:0) bits 0:7 use field AALIGNMENTERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_MSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_BIT_AALIGNMENTERRORS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_BIT_AALIGNMENTERRORS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_BIT_AALIGNMENTERRORS_MSB_MSK, MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_BIT_AALIGNMENTERRORS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AALIGNMENTERRORS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_APAUSEMACCTRLFRAMESTX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_APAUSEMACCTRLFRAMESTX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_APAUSEMACCTRLFRAMESTX_LSB_get", A, 11);
    /* (0x000080a0 bits 31:0) bits 0:31 use field APAUSEMACCTRLFRAMESTX_LSB of register PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_BIT_APAUSEMACCTRLFRAMESTX_LSB_MSK) >> MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_BIT_APAUSEMACCTRLFRAMESTX_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_APAUSEMACCTRLFRAMESTX_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESTX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESTX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_APAUSEMACCTRLFRAMESTX_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_APAUSEMACCTRLFRAMESTX_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_APAUSEMACCTRLFRAMESTX_LSB_get", stop_bit, 31 );
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
    /* (0x000080a0 bits 31:0) bits 0:31 use field APAUSEMACCTRLFRAMESTX_LSB of register PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_BIT_APAUSEMACCTRLFRAMESTX_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_BIT_APAUSEMACCTRLFRAMESTX_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_BIT_APAUSEMACCTRLFRAMESTX_LSB_MSK, MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_BIT_APAUSEMACCTRLFRAMESTX_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_APAUSEMACCTRLFRAMESTX_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_APAUSEMACCTRLFRAMESTX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_APAUSEMACCTRLFRAMESTX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_APAUSEMACCTRLFRAMESTX_MSB_get", A, 11);
    /* (0x000080a4 bits 7:0) bits 0:7 use field APAUSEMACCTRLFRAMESTX_MSB of register PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_BIT_APAUSEMACCTRLFRAMESTX_MSB_MSK) >> MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_BIT_APAUSEMACCTRLFRAMESTX_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_APAUSEMACCTRLFRAMESTX_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESTX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESTX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_APAUSEMACCTRLFRAMESTX_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_APAUSEMACCTRLFRAMESTX_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_APAUSEMACCTRLFRAMESTX_MSB_get", stop_bit, 7 );
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
    /* (0x000080a4 bits 7:0) bits 0:7 use field APAUSEMACCTRLFRAMESTX_MSB of register PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_BIT_APAUSEMACCTRLFRAMESTX_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_BIT_APAUSEMACCTRLFRAMESTX_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_BIT_APAUSEMACCTRLFRAMESTX_MSB_MSK, MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_BIT_APAUSEMACCTRLFRAMESTX_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_APAUSEMACCTRLFRAMESTX_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_APAUSEMACCTRLFRAMESRX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_APAUSEMACCTRLFRAMESRX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_APAUSEMACCTRLFRAMESRX_LSB_get", A, 11);
    /* (0x000080a8 bits 31:0) bits 0:31 use field APAUSEMACCTRLFRAMESRX_LSB of register PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_BIT_APAUSEMACCTRLFRAMESRX_LSB_MSK) >> MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_BIT_APAUSEMACCTRLFRAMESRX_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_APAUSEMACCTRLFRAMESRX_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESRX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESRX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_APAUSEMACCTRLFRAMESRX_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_APAUSEMACCTRLFRAMESRX_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_APAUSEMACCTRLFRAMESRX_LSB_get", stop_bit, 31 );
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
    /* (0x000080a8 bits 31:0) bits 0:31 use field APAUSEMACCTRLFRAMESRX_LSB of register PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_BIT_APAUSEMACCTRLFRAMESRX_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_BIT_APAUSEMACCTRLFRAMESRX_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_BIT_APAUSEMACCTRLFRAMESRX_LSB_MSK, MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_BIT_APAUSEMACCTRLFRAMESRX_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_APAUSEMACCTRLFRAMESRX_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_APAUSEMACCTRLFRAMESRX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_APAUSEMACCTRLFRAMESRX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_APAUSEMACCTRLFRAMESRX_MSB_get", A, 11);
    /* (0x000080ac bits 7:0) bits 0:7 use field APAUSEMACCTRLFRAMESRX_MSB of register PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_BIT_APAUSEMACCTRLFRAMESRX_MSB_MSK) >> MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_BIT_APAUSEMACCTRLFRAMESRX_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_APAUSEMACCTRLFRAMESRX_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESRX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESRX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_APAUSEMACCTRLFRAMESRX_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_APAUSEMACCTRLFRAMESRX_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_APAUSEMACCTRLFRAMESRX_MSB_get", stop_bit, 7 );
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
    /* (0x000080ac bits 7:0) bits 0:7 use field APAUSEMACCTRLFRAMESRX_MSB of register PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB__read( b_ptr,
                                                                            h_ptr,
                                                                            A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_BIT_APAUSEMACCTRLFRAMESRX_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_BIT_APAUSEMACCTRLFRAMESRX_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_BIT_APAUSEMACCTRLFRAMESRX_MSB_MSK, MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_BIT_APAUSEMACCTRLFRAMESRX_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_APAUSEMACCTRLFRAMESRX_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AFRAMETOOLONGERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AFRAMETOOLONGERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AFRAMETOOLONGERRORS_LSB_get", A, 11);
    /* (0x000080b0 bits 31:0) bits 0:31 use field AFRAMETOOLONGERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_BIT_AFRAMETOOLONGERRORS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_BIT_AFRAMETOOLONGERRORS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AFRAMETOOLONGERRORS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AFRAMETOOLONGERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AFRAMETOOLONGERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AFRAMETOOLONGERRORS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AFRAMETOOLONGERRORS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AFRAMETOOLONGERRORS_LSB_get", stop_bit, 31 );
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
    /* (0x000080b0 bits 31:0) bits 0:31 use field AFRAMETOOLONGERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_BIT_AFRAMETOOLONGERRORS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_BIT_AFRAMETOOLONGERRORS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_BIT_AFRAMETOOLONGERRORS_LSB_MSK, MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_BIT_AFRAMETOOLONGERRORS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AFRAMETOOLONGERRORS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AFRAMETOOLONGERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AFRAMETOOLONGERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AFRAMETOOLONGERRORS_MSB_get", A, 11);
    /* (0x000080b4 bits 7:0) bits 0:7 use field AFRAMETOOLONGERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_BIT_AFRAMETOOLONGERRORS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_BIT_AFRAMETOOLONGERRORS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AFRAMETOOLONGERRORS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AFRAMETOOLONGERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AFRAMETOOLONGERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AFRAMETOOLONGERRORS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AFRAMETOOLONGERRORS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AFRAMETOOLONGERRORS_MSB_get", stop_bit, 7 );
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
    /* (0x000080b4 bits 7:0) bits 0:7 use field AFRAMETOOLONGERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_BIT_AFRAMETOOLONGERRORS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_BIT_AFRAMETOOLONGERRORS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_BIT_AFRAMETOOLONGERRORS_MSB_MSK, MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_BIT_AFRAMETOOLONGERRORS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AFRAMETOOLONGERRORS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AINRANGELENGTHERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AINRANGELENGTHERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AINRANGELENGTHERRORS_LSB_get", A, 11);
    /* (0x000080b8 bits 31:0) bits 0:31 use field AINRANGELENGTHERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_LSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_BIT_AINRANGELENGTHERRORS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_BIT_AINRANGELENGTHERRORS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AINRANGELENGTHERRORS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AINRANGELENGTHERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AINRANGELENGTHERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AINRANGELENGTHERRORS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AINRANGELENGTHERRORS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AINRANGELENGTHERRORS_LSB_get", stop_bit, 31 );
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
    /* (0x000080b8 bits 31:0) bits 0:31 use field AINRANGELENGTHERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_LSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_BIT_AINRANGELENGTHERRORS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_BIT_AINRANGELENGTHERRORS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_BIT_AINRANGELENGTHERRORS_LSB_MSK, MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_BIT_AINRANGELENGTHERRORS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AINRANGELENGTHERRORS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AINRANGELENGTHERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AINRANGELENGTHERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AINRANGELENGTHERRORS_MSB_get", A, 11);
    /* (0x000080bc bits 7:0) bits 0:7 use field AINRANGELENGTHERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_MSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_BIT_AINRANGELENGTHERRORS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_BIT_AINRANGELENGTHERRORS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AINRANGELENGTHERRORS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AINRANGELENGTHERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AINRANGELENGTHERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AINRANGELENGTHERRORS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AINRANGELENGTHERRORS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AINRANGELENGTHERRORS_MSB_get", stop_bit, 7 );
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
    /* (0x000080bc bits 7:0) bits 0:7 use field AINRANGELENGTHERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_MSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_BIT_AINRANGELENGTHERRORS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_BIT_AINRANGELENGTHERRORS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_BIT_AINRANGELENGTHERRORS_MSB_MSK, MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_BIT_AINRANGELENGTHERRORS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AINRANGELENGTHERRORS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_VLANTXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_VLANTXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_VLANTXOK_LSB_get", A, 11);
    /* (0x000080c0 bits 31:0) bits 0:31 use field VLANTXOK_LSB of register PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_VLANTXOK_LSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_BIT_VLANTXOK_LSB_MSK) >> MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_BIT_VLANTXOK_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_VLANTXOK_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_VLANTXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_VLANTXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_VLANTXOK_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_VLANTXOK_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_VLANTXOK_LSB_get", stop_bit, 31 );
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
    /* (0x000080c0 bits 31:0) bits 0:31 use field VLANTXOK_LSB of register PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_VLANTXOK_LSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_BIT_VLANTXOK_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_BIT_VLANTXOK_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_BIT_VLANTXOK_LSB_MSK, MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_BIT_VLANTXOK_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_VLANTXOK_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_VLANTXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_VLANTXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_VLANTXOK_MSB_get", A, 11);
    /* (0x000080c4 bits 7:0) bits 0:7 use field VLANTXOK_MSB of register PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_VLANTXOK_MSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_BIT_VLANTXOK_MSB_MSK) >> MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_BIT_VLANTXOK_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_VLANTXOK_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_VLANTXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_VLANTXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_VLANTXOK_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_VLANTXOK_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_VLANTXOK_MSB_get", stop_bit, 7 );
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
    /* (0x000080c4 bits 7:0) bits 0:7 use field VLANTXOK_MSB of register PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_VLANTXOK_MSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_BIT_VLANTXOK_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_BIT_VLANTXOK_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_BIT_VLANTXOK_MSB_MSK, MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_BIT_VLANTXOK_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_VLANTXOK_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_VLANRXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_VLANRXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_VLANRXOK_LSB_get", A, 11);
    /* (0x000080c8 bits 31:0) bits 0:31 use field VLANRXOK_LSB of register PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_VLANRXOK_LSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_BIT_VLANRXOK_LSB_MSK) >> MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_BIT_VLANRXOK_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_VLANRXOK_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_VLANRXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_VLANRXOK_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_VLANRXOK_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_VLANRXOK_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_VLANRXOK_LSB_get", stop_bit, 31 );
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
    /* (0x000080c8 bits 31:0) bits 0:31 use field VLANRXOK_LSB of register PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_VLANRXOK_LSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_BIT_VLANRXOK_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_BIT_VLANRXOK_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_BIT_VLANRXOK_LSB_MSK, MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_BIT_VLANRXOK_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_VLANRXOK_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_VLANRXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_VLANRXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_VLANRXOK_MSB_get", A, 11);
    /* (0x000080cc bits 7:0) bits 0:7 use field VLANRXOK_MSB of register PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_VLANRXOK_MSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_BIT_VLANRXOK_MSB_MSK) >> MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_BIT_VLANRXOK_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_VLANRXOK_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_VLANRXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_VLANRXOK_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_VLANRXOK_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_VLANRXOK_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_VLANRXOK_MSB_get", stop_bit, 7 );
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
    /* (0x000080cc bits 7:0) bits 0:7 use field VLANRXOK_MSB of register PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_VLANRXOK_MSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_BIT_VLANRXOK_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_BIT_VLANRXOK_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_BIT_VLANRXOK_MSB_MSK, MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_BIT_VLANRXOK_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_VLANRXOK_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTOCTS_LSB_get", A, 11);
    /* (0x000080d0 bits 31:0) bits 0:31 use field IFOUTOCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_LSB__read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_BIT_IFOUTOCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_BIT_IFOUTOCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTOCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTOCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTOCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTOCTS_LSB_get", stop_bit, 31 );
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
    /* (0x000080d0 bits 31:0) bits 0:31 use field IFOUTOCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_LSB__read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_BIT_IFOUTOCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_BIT_IFOUTOCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_BIT_IFOUTOCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_BIT_IFOUTOCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTOCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTOCTS_MSB_get", A, 11);
    /* (0x000080d4 bits 7:0) bits 0:7 use field IFOUTOCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_MSB__read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_BIT_IFOUTOCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_BIT_IFOUTOCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTOCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTOCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTOCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTOCTS_MSB_get", stop_bit, 7 );
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
    /* (0x000080d4 bits 7:0) bits 0:7 use field IFOUTOCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_MSB__read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_BIT_IFOUTOCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_BIT_IFOUTOCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_BIT_IFOUTOCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_BIT_IFOUTOCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTOCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINOCTS_LSB_get", A, 11);
    /* (0x000080d8 bits 31:0) bits 0:31 use field IFINOCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINOCTS_LSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_BIT_IFINOCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_BIT_IFINOCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINOCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINOCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINOCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINOCTS_LSB_get", stop_bit, 31 );
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
    /* (0x000080d8 bits 31:0) bits 0:31 use field IFINOCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINOCTS_LSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_BIT_IFINOCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_BIT_IFINOCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_BIT_IFINOCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_BIT_IFINOCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINOCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINOCTS_MSB_get", A, 11);
    /* (0x000080dc bits 7:0) bits 0:7 use field IFINOCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINOCTS_MSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_BIT_IFINOCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_BIT_IFINOCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINOCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINOCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINOCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINOCTS_MSB_get", stop_bit, 7 );
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
    /* (0x000080dc bits 7:0) bits 0:7 use field IFINOCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINOCTS_MSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_BIT_IFINOCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_BIT_IFINOCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_BIT_IFINOCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_BIT_IFINOCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINOCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINUCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINUCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINUCASTPKTS_LSB_get", A, 11);
    /* (0x000080e0 bits 31:0) bits 0:31 use field IFINUCASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_LSB__read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_BIT_IFINUCASTPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_BIT_IFINUCASTPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINUCASTPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINUCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINUCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINUCASTPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINUCASTPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINUCASTPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x000080e0 bits 31:0) bits 0:31 use field IFINUCASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_LSB__read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_BIT_IFINUCASTPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_BIT_IFINUCASTPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_BIT_IFINUCASTPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_BIT_IFINUCASTPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINUCASTPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINUCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINUCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINUCASTPKTS_MSB_get", A, 11);
    /* (0x000080e4 bits 7:0) bits 0:7 use field IFINUCASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_MSB__read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_BIT_IFINUCASTPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_BIT_IFINUCASTPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINUCASTPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINUCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINUCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINUCASTPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINUCASTPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINUCASTPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x000080e4 bits 7:0) bits 0:7 use field IFINUCASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_MSB__read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_BIT_IFINUCASTPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_BIT_IFINUCASTPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_BIT_IFINUCASTPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_BIT_IFINUCASTPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINUCASTPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINMULTICASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINMULTICASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINMULTICASTPKTS_LSB_get", A, 11);
    /* (0x000080e8 bits 31:0) bits 0:31 use field IFINMULTICASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_LSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_BIT_IFINMULTICASTPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_BIT_IFINMULTICASTPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINMULTICASTPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINMULTICASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINMULTICASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINMULTICASTPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINMULTICASTPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINMULTICASTPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x000080e8 bits 31:0) bits 0:31 use field IFINMULTICASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_LSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_BIT_IFINMULTICASTPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_BIT_IFINMULTICASTPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_BIT_IFINMULTICASTPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_BIT_IFINMULTICASTPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINMULTICASTPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINMULTICASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINMULTICASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINMULTICASTPKTS_MSB_get", A, 11);
    /* (0x000080ec bits 7:0) bits 0:7 use field IFINMULTICASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_MSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_BIT_IFINMULTICASTPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_BIT_IFINMULTICASTPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINMULTICASTPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINMULTICASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINMULTICASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINMULTICASTPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINMULTICASTPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINMULTICASTPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x000080ec bits 7:0) bits 0:7 use field IFINMULTICASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_MSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_BIT_IFINMULTICASTPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_BIT_IFINMULTICASTPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_BIT_IFINMULTICASTPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_BIT_IFINMULTICASTPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINMULTICASTPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINBROADCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINBROADCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINBROADCASTPKTS_LSB_get", A, 11);
    /* (0x000080f0 bits 31:0) bits 0:31 use field IFINBROADCASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_LSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_BIT_IFINBROADCASTPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_BIT_IFINBROADCASTPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINBROADCASTPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINBROADCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINBROADCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINBROADCASTPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINBROADCASTPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINBROADCASTPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x000080f0 bits 31:0) bits 0:31 use field IFINBROADCASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_LSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_BIT_IFINBROADCASTPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_BIT_IFINBROADCASTPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_BIT_IFINBROADCASTPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_BIT_IFINBROADCASTPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINBROADCASTPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINBROADCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINBROADCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINBROADCASTPKTS_MSB_get", A, 11);
    /* (0x000080f4 bits 7:0) bits 0:7 use field IFINBROADCASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_MSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_BIT_IFINBROADCASTPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_BIT_IFINBROADCASTPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINBROADCASTPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINBROADCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINBROADCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINBROADCASTPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINBROADCASTPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINBROADCASTPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x000080f4 bits 7:0) bits 0:7 use field IFINBROADCASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_MSB__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_BIT_IFINBROADCASTPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_BIT_IFINBROADCASTPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_BIT_IFINBROADCASTPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_BIT_IFINBROADCASTPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINBROADCASTPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTERRORS_LSB_get", A, 11);
    /* (0x000080f8 bits 31:0) bits 0:31 use field IFOUTERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTERRS_LSB__read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_BIT_IFOUTERRORS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_BIT_IFOUTERRORS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTERRORS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTERRORS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTERRORS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTERRORS_LSB_get", stop_bit, 31 );
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
    /* (0x000080f8 bits 31:0) bits 0:31 use field IFOUTERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTERRS_LSB__read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_BIT_IFOUTERRORS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_BIT_IFOUTERRORS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_BIT_IFOUTERRORS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_BIT_IFOUTERRORS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTERRORS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTERRORS_MSB_get", A, 11);
    /* (0x000080fc bits 7:0) bits 0:7 use field IFOUTERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTERRS_MSB__read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_BIT_IFOUTERRORS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_BIT_IFOUTERRORS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTERRORS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTERRORS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTERRORS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTERRORS_MSB_get", stop_bit, 7 );
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
    /* (0x000080fc bits 7:0) bits 0:7 use field IFOUTERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTERRS_MSB__read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_BIT_IFOUTERRORS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_BIT_IFOUTERRORS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_BIT_IFOUTERRORS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_BIT_IFOUTERRORS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTERRORS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTUCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTUCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTUCASTPKTS_LSB_get", A, 11);
    /* (0x00008108 bits 31:0) bits 0:31 use field IFOUTUCASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_LSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_BIT_IFOUTUCASTPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_BIT_IFOUTUCASTPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTUCASTPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTUCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTUCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTUCASTPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTUCASTPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTUCASTPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008108 bits 31:0) bits 0:31 use field IFOUTUCASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_LSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_BIT_IFOUTUCASTPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_BIT_IFOUTUCASTPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_BIT_IFOUTUCASTPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_BIT_IFOUTUCASTPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTUCASTPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTUCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTUCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTUCASTPKTS_MSB_get", A, 11);
    /* (0x0000810c bits 7:0) bits 0:7 use field IFOUTUCASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_MSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_BIT_IFOUTUCASTPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_BIT_IFOUTUCASTPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTUCASTPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTUCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTUCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTUCASTPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTUCASTPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTUCASTPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000810c bits 7:0) bits 0:7 use field IFOUTUCASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_MSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_BIT_IFOUTUCASTPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_BIT_IFOUTUCASTPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_BIT_IFOUTUCASTPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_BIT_IFOUTUCASTPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTUCASTPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTMULTICASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTMULTICASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTMULTICASTPKTS_LSB_get", A, 11);
    /* (0x00008110 bits 31:0) bits 0:31 use field IFOUTMULTICASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_BIT_IFOUTMULTICASTPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_BIT_IFOUTMULTICASTPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTMULTICASTPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTMULTICASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTMULTICASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTMULTICASTPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTMULTICASTPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTMULTICASTPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008110 bits 31:0) bits 0:31 use field IFOUTMULTICASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_BIT_IFOUTMULTICASTPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_BIT_IFOUTMULTICASTPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_BIT_IFOUTMULTICASTPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_BIT_IFOUTMULTICASTPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTMULTICASTPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTMULTICASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTMULTICASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTMULTICASTPKTS_MSB_get", A, 11);
    /* (0x00008114 bits 7:0) bits 0:7 use field IFOUTMULTICASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_BIT_IFOUTMULTICASTPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_BIT_IFOUTMULTICASTPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTMULTICASTPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTMULTICASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTMULTICASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTMULTICASTPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTMULTICASTPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTMULTICASTPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x00008114 bits 7:0) bits 0:7 use field IFOUTMULTICASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_BIT_IFOUTMULTICASTPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_BIT_IFOUTMULTICASTPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_BIT_IFOUTMULTICASTPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_BIT_IFOUTMULTICASTPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTMULTICASTPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTBROADCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTBROADCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTBROADCASTPKTS_LSB_get", A, 11);
    /* (0x00008118 bits 31:0) bits 0:31 use field IFOUTBROADCASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_BIT_IFOUTBROADCASTPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_BIT_IFOUTBROADCASTPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTBROADCASTPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTBROADCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTBROADCASTPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTBROADCASTPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTBROADCASTPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTBROADCASTPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008118 bits 31:0) bits 0:31 use field IFOUTBROADCASTPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_BIT_IFOUTBROADCASTPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_BIT_IFOUTBROADCASTPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_BIT_IFOUTBROADCASTPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_BIT_IFOUTBROADCASTPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTBROADCASTPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFOUTBROADCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFOUTBROADCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFOUTBROADCASTPKTS_MSB_get", A, 11);
    /* (0x0000811c bits 7:0) bits 0:7 use field IFOUTBROADCASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_BIT_IFOUTBROADCASTPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_BIT_IFOUTBROADCASTPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFOUTBROADCASTPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFOUTBROADCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFOUTBROADCASTPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFOUTBROADCASTPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFOUTBROADCASTPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFOUTBROADCASTPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000811c bits 7:0) bits 0:7 use field IFOUTBROADCASTPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB__read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_BIT_IFOUTBROADCASTPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_BIT_IFOUTBROADCASTPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_BIT_IFOUTBROADCASTPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_BIT_IFOUTBROADCASTPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFOUTBROADCASTPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSDROPEVENTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSDROPEVENTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSDROPEVENTS_LSB_get", A, 11);
    /* (0x00008120 bits 31:0) bits 0:31 use field ETHERSTATSDROPEVENTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_BIT_ETHERSTATSDROPEVENTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_BIT_ETHERSTATSDROPEVENTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSDROPEVENTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSDROPEVENTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSDROPEVENTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSDROPEVENTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSDROPEVENTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSDROPEVENTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008120 bits 31:0) bits 0:31 use field ETHERSTATSDROPEVENTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_BIT_ETHERSTATSDROPEVENTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_BIT_ETHERSTATSDROPEVENTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_BIT_ETHERSTATSDROPEVENTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_BIT_ETHERSTATSDROPEVENTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSDROPEVENTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSDROPEVENTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSDROPEVENTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSDROPEVENTS_MSB_get", A, 11);
    /* (0x00008124 bits 7:0) bits 0:7 use field ETHERSTATSDROPEVENTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_BIT_ETHERSTATSDROPEVENTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_BIT_ETHERSTATSDROPEVENTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSDROPEVENTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSDROPEVENTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSDROPEVENTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSDROPEVENTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSDROPEVENTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSDROPEVENTS_MSB_get", stop_bit, 7 );
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
    /* (0x00008124 bits 7:0) bits 0:7 use field ETHERSTATSDROPEVENTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_BIT_ETHERSTATSDROPEVENTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_BIT_ETHERSTATSDROPEVENTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_BIT_ETHERSTATSDROPEVENTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_BIT_ETHERSTATSDROPEVENTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSDROPEVENTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSOCTS_LSB_get", A, 11);
    /* (0x00008128 bits 31:0) bits 0:31 use field ETHERSTATSOCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_LSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_BIT_ETHERSTATSOCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_BIT_ETHERSTATSOCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSOCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSOCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSOCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSOCTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008128 bits 31:0) bits 0:31 use field ETHERSTATSOCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_LSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_BIT_ETHERSTATSOCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_BIT_ETHERSTATSOCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_BIT_ETHERSTATSOCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_BIT_ETHERSTATSOCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSOCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSOCTS_MSB_get", A, 11);
    /* (0x0000812c bits 7:0) bits 0:7 use field ETHERSTATSOCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_MSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_BIT_ETHERSTATSOCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_BIT_ETHERSTATSOCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSOCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSOCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSOCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSOCTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000812c bits 7:0) bits 0:7 use field ETHERSTATSOCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_MSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_BIT_ETHERSTATSOCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_BIT_ETHERSTATSOCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_BIT_ETHERSTATSOCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_BIT_ETHERSTATSOCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSOCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS_LSB_get", A, 11);
    /* (0x00008130 bits 31:0) bits 0:31 use field ETHERSTATSPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_LSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_BIT_ETHERSTATSPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_BIT_ETHERSTATSPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008130 bits 31:0) bits 0:31 use field ETHERSTATSPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_LSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_BIT_ETHERSTATSPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_BIT_ETHERSTATSPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_BIT_ETHERSTATSPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_BIT_ETHERSTATSPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS_MSB_get", A, 11);
    /* (0x00008134 bits 7:0) bits 0:7 use field ETHERSTATSPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_MSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_BIT_ETHERSTATSPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_BIT_ETHERSTATSPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x00008134 bits 7:0) bits 0:7 use field ETHERSTATSPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_MSB__read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_BIT_ETHERSTATSPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_BIT_ETHERSTATSPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_BIT_ETHERSTATSPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_BIT_ETHERSTATSPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSUNDERSIZEPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSUNDERSIZEPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSUNDERSIZEPKTS_LSB_get", A, 11);
    /* (0x00008138 bits 31:0) bits 0:31 use field ETHERSTATSUNDERSIZEPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB__read( b_ptr,
                                                                              h_ptr,
                                                                              A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_BIT_ETHERSTATSUNDERSIZEPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_BIT_ETHERSTATSUNDERSIZEPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSUNDERSIZEPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008138 bits 31:0) bits 0:31 use field ETHERSTATSUNDERSIZEPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB__read( b_ptr,
                                                                              h_ptr,
                                                                              A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_BIT_ETHERSTATSUNDERSIZEPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_BIT_ETHERSTATSUNDERSIZEPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_BIT_ETHERSTATSUNDERSIZEPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_BIT_ETHERSTATSUNDERSIZEPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSUNDERSIZEPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSUNDERSIZEPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSUNDERSIZEPKTS_MSB_get", A, 11);
    /* (0x0000813c bits 7:0) bits 0:7 use field ETHERSTATSUNDERSIZEPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB__read( b_ptr,
                                                                              h_ptr,
                                                                              A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_BIT_ETHERSTATSUNDERSIZEPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_BIT_ETHERSTATSUNDERSIZEPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSUNDERSIZEPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000813c bits 7:0) bits 0:7 use field ETHERSTATSUNDERSIZEPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB__read( b_ptr,
                                                                              h_ptr,
                                                                              A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_BIT_ETHERSTATSUNDERSIZEPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_BIT_ETHERSTATSUNDERSIZEPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_BIT_ETHERSTATSUNDERSIZEPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_BIT_ETHERSTATSUNDERSIZEPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS64OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS64OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS64OCTS_LSB_get", A, 11);
    /* (0x00008140 bits 31:0) bits 0:31 use field ETHERSTATSPKTS64OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_BIT_ETHERSTATSPKTS64OCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_BIT_ETHERSTATSPKTS64OCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS64OCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS64OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS64OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS64OCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS64OCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS64OCTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008140 bits 31:0) bits 0:31 use field ETHERSTATSPKTS64OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_BIT_ETHERSTATSPKTS64OCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_BIT_ETHERSTATSPKTS64OCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_BIT_ETHERSTATSPKTS64OCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_BIT_ETHERSTATSPKTS64OCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS64OCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS64OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS64OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS64OCTS_MSB_get", A, 11);
    /* (0x00008144 bits 7:0) bits 0:7 use field ETHERSTATSPKTS64OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_BIT_ETHERSTATSPKTS64OCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_BIT_ETHERSTATSPKTS64OCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS64OCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS64OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS64OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS64OCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS64OCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS64OCTS_MSB_get", stop_bit, 7 );
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
    /* (0x00008144 bits 7:0) bits 0:7 use field ETHERSTATSPKTS64OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_BIT_ETHERSTATSPKTS64OCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_BIT_ETHERSTATSPKTS64OCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_BIT_ETHERSTATSPKTS64OCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_BIT_ETHERSTATSPKTS64OCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS64OCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS65TO127OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS65TO127OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS65TO127OCTS_LSB_get", A, 11);
    /* (0x00008148 bits 31:0) bits 0:31 use field ETHERSTATSPKTS65TO127OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB__read( b_ptr,
                                                                                h_ptr,
                                                                                A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_BIT_ETHERSTATSPKTS65TO127OCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_BIT_ETHERSTATSPKTS65TO127OCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS65TO127OCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008148 bits 31:0) bits 0:31 use field ETHERSTATSPKTS65TO127OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB__read( b_ptr,
                                                                                h_ptr,
                                                                                A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_BIT_ETHERSTATSPKTS65TO127OCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_BIT_ETHERSTATSPKTS65TO127OCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_BIT_ETHERSTATSPKTS65TO127OCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_BIT_ETHERSTATSPKTS65TO127OCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS65TO127OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS65TO127OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS65TO127OCTS_MSB_get", A, 11);
    /* (0x0000814c bits 7:0) bits 0:7 use field ETHERSTATSPKTS65TO127OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB__read( b_ptr,
                                                                                h_ptr,
                                                                                A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_BIT_ETHERSTATSPKTS65TO127OCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_BIT_ETHERSTATSPKTS65TO127OCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS65TO127OCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                             emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000814c bits 7:0) bits 0:7 use field ETHERSTATSPKTS65TO127OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB__read( b_ptr,
                                                                                h_ptr,
                                                                                A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_BIT_ETHERSTATSPKTS65TO127OCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_BIT_ETHERSTATSPKTS65TO127OCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_BIT_ETHERSTATSPKTS65TO127OCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_BIT_ETHERSTATSPKTS65TO127OCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS128TO255OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS128TO255OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS128TO255OCTS_LSB_get", A, 11);
    /* (0x00008150 bits 31:0) bits 0:31 use field ETHERSTATSPKTS128TO255OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB__read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_BIT_ETHERSTATSPKTS128TO255OCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_BIT_ETHERSTATSPKTS128TO255OCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS128TO255OCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008150 bits 31:0) bits 0:31 use field ETHERSTATSPKTS128TO255OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB__read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_BIT_ETHERSTATSPKTS128TO255OCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_BIT_ETHERSTATSPKTS128TO255OCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_BIT_ETHERSTATSPKTS128TO255OCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_BIT_ETHERSTATSPKTS128TO255OCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS128TO255OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS128TO255OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS128TO255OCTS_MSB_get", A, 11);
    /* (0x00008154 bits 7:0) bits 0:7 use field ETHERSTATSPKTS128TO255OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB__read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_BIT_ETHERSTATSPKTS128TO255OCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_BIT_ETHERSTATSPKTS128TO255OCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS128TO255OCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_MSB_get", stop_bit, 7 );
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
    /* (0x00008154 bits 7:0) bits 0:7 use field ETHERSTATSPKTS128TO255OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB__read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_BIT_ETHERSTATSPKTS128TO255OCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_BIT_ETHERSTATSPKTS128TO255OCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_BIT_ETHERSTATSPKTS128TO255OCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_BIT_ETHERSTATSPKTS128TO255OCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS256TO511OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS256TO511OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS256TO511OCTS_LSB_get", A, 11);
    /* (0x00008158 bits 31:0) bits 0:31 use field ETHERSTATSPKTS256TO511OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB__read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_BIT_ETHERSTATSPKTS256TO511OCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_BIT_ETHERSTATSPKTS256TO511OCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS256TO511OCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008158 bits 31:0) bits 0:31 use field ETHERSTATSPKTS256TO511OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB__read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_BIT_ETHERSTATSPKTS256TO511OCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_BIT_ETHERSTATSPKTS256TO511OCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_BIT_ETHERSTATSPKTS256TO511OCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_BIT_ETHERSTATSPKTS256TO511OCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS256TO511OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS256TO511OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS256TO511OCTS_MSB_get", A, 11);
    /* (0x0000815c bits 7:0) bits 0:7 use field ETHERSTATSPKTS256TO511OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB__read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_BIT_ETHERSTATSPKTS256TO511OCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_BIT_ETHERSTATSPKTS256TO511OCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS256TO511OCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000815c bits 7:0) bits 0:7 use field ETHERSTATSPKTS256TO511OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB__read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_BIT_ETHERSTATSPKTS256TO511OCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_BIT_ETHERSTATSPKTS256TO511OCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_BIT_ETHERSTATSPKTS256TO511OCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_BIT_ETHERSTATSPKTS256TO511OCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_LSB_get", A, 11);
    /* (0x00008160 bits 31:0) bits 0:31 use field ETHERSTATSPKTS512TO1023OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB__read( b_ptr,
                                                                                  h_ptr,
                                                                                  A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_BIT_ETHERSTATSPKTS512TO1023OCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_BIT_ETHERSTATSPKTS512TO1023OCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008160 bits 31:0) bits 0:31 use field ETHERSTATSPKTS512TO1023OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB__read( b_ptr,
                                                                                  h_ptr,
                                                                                  A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_BIT_ETHERSTATSPKTS512TO1023OCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_BIT_ETHERSTATSPKTS512TO1023OCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_BIT_ETHERSTATSPKTS512TO1023OCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_BIT_ETHERSTATSPKTS512TO1023OCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_MSB_get", A, 11);
    /* (0x00008164 bits 7:0) bits 0:7 use field ETHERSTATSPKTS512TO1023OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB__read( b_ptr,
                                                                                  h_ptr,
                                                                                  A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_BIT_ETHERSTATSPKTS512TO1023OCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_BIT_ETHERSTATSPKTS512TO1023OCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_MSB_get", stop_bit, 7 );
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
    /* (0x00008164 bits 7:0) bits 0:7 use field ETHERSTATSPKTS512TO1023OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB__read( b_ptr,
                                                                                  h_ptr,
                                                                                  A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_BIT_ETHERSTATSPKTS512TO1023OCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_BIT_ETHERSTATSPKTS512TO1023OCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_BIT_ETHERSTATSPKTS512TO1023OCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_BIT_ETHERSTATSPKTS512TO1023OCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_LSB_get", A, 11);
    /* (0x00008168 bits 31:0) bits 0:31 use field ETHERSTATSPKTS1024TO1518OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB__read( b_ptr,
                                                                                   h_ptr,
                                                                                   A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008168 bits 31:0) bits 0:31 use field ETHERSTATSPKTS1024TO1518OCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB__read( b_ptr,
                                                                                   h_ptr,
                                                                                   A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_MSB_get", A, 11);
    /* (0x0000816c bits 7:0) bits 0:7 use field ETHERSTATSPKTS1024TO1518OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB__read( b_ptr,
                                                                                   h_ptr,
                                                                                   A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000816c bits 7:0) bits 0:7 use field ETHERSTATSPKTS1024TO1518OCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB__read( b_ptr,
                                                                                   h_ptr,
                                                                                   A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get", A, 11);
    /* (0x00008170 bits 31:0) bits 0:31 use field ETHERSTATSPKTS1519TOMAXOCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB__read( b_ptr,
                                                                                  h_ptr,
                                                                                  A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008170 bits 31:0) bits 0:31 use field ETHERSTATSPKTS1519TOMAXOCTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB__read( b_ptr,
                                                                                  h_ptr,
                                                                                  A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                         emac_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get", A, 11);
    /* (0x00008174 bits 7:0) bits 0:7 use field ETHERSTATSPKTS1519TOMAXOCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB__read( b_ptr,
                                                                                  h_ptr,
                                                                                  A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                               emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get", stop_bit, 7 );
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
    /* (0x00008174 bits 7:0) bits 0:7 use field ETHERSTATSPKTS1519TOMAXOCTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB__read( b_ptr,
                                                                                  h_ptr,
                                                                                  A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSOVERSIZEPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSOVERSIZEPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSOVERSIZEPKTS_LSB_get", A, 11);
    /* (0x00008178 bits 31:0) bits 0:31 use field ETHERSTATSOVERSIZEPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB__read( b_ptr,
                                                                             h_ptr,
                                                                             A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_BIT_ETHERSTATSOVERSIZEPKTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_BIT_ETHERSTATSOVERSIZEPKTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSOVERSIZEPKTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008178 bits 31:0) bits 0:31 use field ETHERSTATSOVERSIZEPKTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB__read( b_ptr,
                                                                             h_ptr,
                                                                             A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_BIT_ETHERSTATSOVERSIZEPKTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_BIT_ETHERSTATSOVERSIZEPKTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_BIT_ETHERSTATSOVERSIZEPKTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_BIT_ETHERSTATSOVERSIZEPKTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSOVERSIZEPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSOVERSIZEPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSOVERSIZEPKTS_MSB_get", A, 11);
    /* (0x0000817c bits 7:0) bits 0:7 use field ETHERSTATSOVERSIZEPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB__read( b_ptr,
                                                                             h_ptr,
                                                                             A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_BIT_ETHERSTATSOVERSIZEPKTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_BIT_ETHERSTATSOVERSIZEPKTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSOVERSIZEPKTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000817c bits 7:0) bits 0:7 use field ETHERSTATSOVERSIZEPKTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB__read( b_ptr,
                                                                             h_ptr,
                                                                             A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_BIT_ETHERSTATSOVERSIZEPKTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_BIT_ETHERSTATSOVERSIZEPKTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_BIT_ETHERSTATSOVERSIZEPKTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_BIT_ETHERSTATSOVERSIZEPKTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSJABBER_LSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSJABBER_LSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSJABBER_LSB_get", A, 11);
    /* (0x00008180 bits 31:0) bits 0:31 use field ETHERSTATSJABBER_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_LSB__read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_BIT_ETHERSTATSJABBER_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_BIT_ETHERSTATSJABBER_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSJABBER_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSJABBER_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSJABBER_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSJABBER_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSJABBER_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSJABBER_LSB_get", stop_bit, 31 );
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
    /* (0x00008180 bits 31:0) bits 0:31 use field ETHERSTATSJABBER_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_LSB__read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_BIT_ETHERSTATSJABBER_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_BIT_ETHERSTATSJABBER_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_BIT_ETHERSTATSJABBER_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_BIT_ETHERSTATSJABBER_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSJABBER_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSJABBER_MSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSJABBER_MSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSJABBER_MSB_get", A, 11);
    /* (0x00008184 bits 7:0) bits 0:7 use field ETHERSTATSJABBER_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_MSB__read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_BIT_ETHERSTATSJABBER_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_BIT_ETHERSTATSJABBER_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSJABBER_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSJABBER_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSJABBER_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                    emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSJABBER_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSJABBER_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSJABBER_MSB_get", stop_bit, 7 );
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
    /* (0x00008184 bits 7:0) bits 0:7 use field ETHERSTATSJABBER_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_MSB__read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_BIT_ETHERSTATSJABBER_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_BIT_ETHERSTATSJABBER_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_BIT_ETHERSTATSJABBER_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_BIT_ETHERSTATSJABBER_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSJABBER_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSFRAGMENTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSFRAGMENTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSFRAGMENTS_LSB_get", A, 11);
    /* (0x00008188 bits 31:0) bits 0:31 use field ETHERSTATSFRAGMENTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB__read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_BIT_ETHERSTATSFRAGMENTS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_BIT_ETHERSTATSFRAGMENTS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSFRAGMENTS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSFRAGMENTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSFRAGMENTS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSFRAGMENTS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSFRAGMENTS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSFRAGMENTS_LSB_get", stop_bit, 31 );
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
    /* (0x00008188 bits 31:0) bits 0:31 use field ETHERSTATSFRAGMENTS_LSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB__read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_BIT_ETHERSTATSFRAGMENTS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_BIT_ETHERSTATSFRAGMENTS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_BIT_ETHERSTATSFRAGMENTS_LSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_BIT_ETHERSTATSFRAGMENTS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSFRAGMENTS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ETHERSTATSFRAGMENTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ETHERSTATSFRAGMENTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ETHERSTATSFRAGMENTS_MSB_get", A, 11);
    /* (0x0000818c bits 7:0) bits 0:7 use field ETHERSTATSFRAGMENTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB__read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_BIT_ETHERSTATSFRAGMENTS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_BIT_ETHERSTATSFRAGMENTS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ETHERSTATSFRAGMENTS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ETHERSTATSFRAGMENTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ETHERSTATSFRAGMENTS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ETHERSTATSFRAGMENTS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ETHERSTATSFRAGMENTS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ETHERSTATSFRAGMENTS_MSB_get", stop_bit, 7 );
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
    /* (0x0000818c bits 7:0) bits 0:7 use field ETHERSTATSFRAGMENTS_MSB of register PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB__read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_BIT_ETHERSTATSFRAGMENTS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_BIT_ETHERSTATSFRAGMENTS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_BIT_ETHERSTATSFRAGMENTS_MSB_MSK, MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_BIT_ETHERSTATSFRAGMENTS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ETHERSTATSFRAGMENTS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINERRORS_LSB_get", A, 11);
    /* (0x00008190 bits 31:0) bits 0:31 use field IFINERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINERRS_LSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINERRS_LSB_BIT_IFINERRORS_LSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINERRS_LSB_BIT_IFINERRORS_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINERRORS_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINERRORS_LSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINERRORS_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINERRORS_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINERRORS_LSB_get", stop_bit, 31 );
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
    /* (0x00008190 bits 31:0) bits 0:31 use field IFINERRORS_LSB of register PMC_MAC_REG_STATISTIC_CNT_IFINERRS_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINERRS_LSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINERRS_LSB_BIT_IFINERRORS_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINERRS_LSB_BIT_IFINERRORS_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINERRS_LSB_BIT_IFINERRORS_LSB_MSK, MAC_REG_STATISTIC_CNT_IFINERRS_LSB_BIT_IFINERRORS_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINERRORS_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_IFINERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_IFINERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_IFINERRORS_MSB_get", A, 11);
    /* (0x00008194 bits 7:0) bits 0:7 use field IFINERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINERRS_MSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_IFINERRS_MSB_BIT_IFINERRORS_MSB_MSK) >> MAC_REG_STATISTIC_CNT_IFINERRS_MSB_BIT_IFINERRORS_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_IFINERRORS_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_IFINERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_IFINERRORS_MSB_get( emac_mac_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_IFINERRORS_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_IFINERRORS_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_IFINERRORS_MSB_get", stop_bit, 7 );
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
    /* (0x00008194 bits 7:0) bits 0:7 use field IFINERRORS_MSB of register PMC_MAC_REG_STATISTIC_CNT_IFINERRS_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_IFINERRS_MSB__read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_IFINERRS_MSB_BIT_IFINERRORS_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_IFINERRS_MSB_BIT_IFINERRORS_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_IFINERRS_MSB_BIT_IFINERRORS_MSB_MSK, MAC_REG_STATISTIC_CNT_IFINERRS_MSB_BIT_IFINERRORS_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_IFINERRORS_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_0_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_0_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_0_LSB_get", A, 11);
    /* (0x00008198 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_0_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_BIT_ACBFCPAUSEFRAMESTX_0_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_BIT_ACBFCPAUSEFRAMESTX_0_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_0_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_LSB_get", stop_bit, 31 );
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
    /* (0x00008198 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_0_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_BIT_ACBFCPAUSEFRAMESTX_0_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_BIT_ACBFCPAUSEFRAMESTX_0_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_BIT_ACBFCPAUSEFRAMESTX_0_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_BIT_ACBFCPAUSEFRAMESTX_0_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_0_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_0_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_0_MSB_get", A, 11);
    /* (0x0000819c bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_0_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_BIT_ACBFCPAUSEFRAMESTX_0_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_BIT_ACBFCPAUSEFRAMESTX_0_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_0_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_MSB_get", stop_bit, 7 );
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
    /* (0x0000819c bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_0_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_BIT_ACBFCPAUSEFRAMESTX_0_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_BIT_ACBFCPAUSEFRAMESTX_0_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_BIT_ACBFCPAUSEFRAMESTX_0_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_BIT_ACBFCPAUSEFRAMESTX_0_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBCFPAUSEFRAMESTX_1_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBCFPAUSEFRAMESTX_1_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBCFPAUSEFRAMESTX_1_LSB_get", A, 11);
    /* (0x000081a0 bits 31:0) bits 0:31 use field ACBCFPAUSEFRAMESTX_1_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_BIT_ACBCFPAUSEFRAMESTX_1_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_BIT_ACBCFPAUSEFRAMESTX_1_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBCFPAUSEFRAMESTX_1_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_LSB_get", stop_bit, 31 );
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
    /* (0x000081a0 bits 31:0) bits 0:31 use field ACBCFPAUSEFRAMESTX_1_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_BIT_ACBCFPAUSEFRAMESTX_1_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_BIT_ACBCFPAUSEFRAMESTX_1_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_BIT_ACBCFPAUSEFRAMESTX_1_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_BIT_ACBCFPAUSEFRAMESTX_1_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBCFPAUSEFRAMESTX_1_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBCFPAUSEFRAMESTX_1_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBCFPAUSEFRAMESTX_1_MSB_get", A, 11);
    /* (0x000081a4 bits 7:0) bits 0:7 use field ACBCFPAUSEFRAMESTX_1_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_BIT_ACBCFPAUSEFRAMESTX_1_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_BIT_ACBCFPAUSEFRAMESTX_1_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBCFPAUSEFRAMESTX_1_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_MSB_get", stop_bit, 7 );
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
    /* (0x000081a4 bits 7:0) bits 0:7 use field ACBCFPAUSEFRAMESTX_1_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_BIT_ACBCFPAUSEFRAMESTX_1_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_BIT_ACBCFPAUSEFRAMESTX_1_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_BIT_ACBCFPAUSEFRAMESTX_1_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_BIT_ACBCFPAUSEFRAMESTX_1_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_2_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_2_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_2_LSB_get", A, 11);
    /* (0x000081a8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_2_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_BIT_ACBFCPAUSEFRAMESTX_2_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_BIT_ACBFCPAUSEFRAMESTX_2_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_2_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_LSB_get", stop_bit, 31 );
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
    /* (0x000081a8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_2_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_BIT_ACBFCPAUSEFRAMESTX_2_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_BIT_ACBFCPAUSEFRAMESTX_2_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_BIT_ACBFCPAUSEFRAMESTX_2_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_BIT_ACBFCPAUSEFRAMESTX_2_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_2_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_2_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_2_MSB_get", A, 11);
    /* (0x000081ac bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_2_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_BIT_ACBFCPAUSEFRAMESTX_2_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_BIT_ACBFCPAUSEFRAMESTX_2_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_2_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_MSB_get", stop_bit, 7 );
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
    /* (0x000081ac bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_2_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_BIT_ACBFCPAUSEFRAMESTX_2_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_BIT_ACBFCPAUSEFRAMESTX_2_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_BIT_ACBFCPAUSEFRAMESTX_2_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_BIT_ACBFCPAUSEFRAMESTX_2_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_3_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_3_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_3_LSB_get", A, 11);
    /* (0x000081b0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_3_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_BIT_ACBFCPAUSEFRAMESTX_3_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_BIT_ACBFCPAUSEFRAMESTX_3_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_3_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_LSB_get", stop_bit, 31 );
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
    /* (0x000081b0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_3_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_BIT_ACBFCPAUSEFRAMESTX_3_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_BIT_ACBFCPAUSEFRAMESTX_3_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_BIT_ACBFCPAUSEFRAMESTX_3_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_BIT_ACBFCPAUSEFRAMESTX_3_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_3_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_3_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_3_MSB_get", A, 11);
    /* (0x000081b4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_3_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_BIT_ACBFCPAUSEFRAMESTX_3_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_BIT_ACBFCPAUSEFRAMESTX_3_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_3_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_MSB_get", stop_bit, 7 );
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
    /* (0x000081b4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_3_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_BIT_ACBFCPAUSEFRAMESTX_3_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_BIT_ACBFCPAUSEFRAMESTX_3_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_BIT_ACBFCPAUSEFRAMESTX_3_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_BIT_ACBFCPAUSEFRAMESTX_3_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_4_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_4_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_4_LSB_get", A, 11);
    /* (0x000081b8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_4_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_BIT_ACBFCPAUSEFRAMESTX_4_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_BIT_ACBFCPAUSEFRAMESTX_4_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_4_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_LSB_get", stop_bit, 31 );
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
    /* (0x000081b8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_4_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_BIT_ACBFCPAUSEFRAMESTX_4_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_BIT_ACBFCPAUSEFRAMESTX_4_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_BIT_ACBFCPAUSEFRAMESTX_4_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_BIT_ACBFCPAUSEFRAMESTX_4_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_4_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_4_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_4_MSB_get", A, 11);
    /* (0x000081bc bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_4_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_BIT_ACBFCPAUSEFRAMESTX_4_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_BIT_ACBFCPAUSEFRAMESTX_4_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_4_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_MSB_get", stop_bit, 7 );
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
    /* (0x000081bc bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_4_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_BIT_ACBFCPAUSEFRAMESTX_4_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_BIT_ACBFCPAUSEFRAMESTX_4_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_BIT_ACBFCPAUSEFRAMESTX_4_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_BIT_ACBFCPAUSEFRAMESTX_4_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_5_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_5_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_5_LSB_get", A, 11);
    /* (0x000081c0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_5_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_BIT_ACBFCPAUSEFRAMESTX_5_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_BIT_ACBFCPAUSEFRAMESTX_5_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_5_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_LSB_get", stop_bit, 31 );
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
    /* (0x000081c0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_5_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_BIT_ACBFCPAUSEFRAMESTX_5_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_BIT_ACBFCPAUSEFRAMESTX_5_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_BIT_ACBFCPAUSEFRAMESTX_5_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_BIT_ACBFCPAUSEFRAMESTX_5_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_5_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_5_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_5_MSB_get", A, 11);
    /* (0x000081c4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_5_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_BIT_ACBFCPAUSEFRAMESTX_5_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_BIT_ACBFCPAUSEFRAMESTX_5_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_5_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_MSB_get", stop_bit, 7 );
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
    /* (0x000081c4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_5_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_BIT_ACBFCPAUSEFRAMESTX_5_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_BIT_ACBFCPAUSEFRAMESTX_5_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_BIT_ACBFCPAUSEFRAMESTX_5_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_BIT_ACBFCPAUSEFRAMESTX_5_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_6_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_6_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_6_LSB_get", A, 11);
    /* (0x000081c8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_6_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_BIT_ACBFCPAUSEFRAMESTX_6_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_BIT_ACBFCPAUSEFRAMESTX_6_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_6_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_LSB_get", stop_bit, 31 );
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
    /* (0x000081c8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_6_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_BIT_ACBFCPAUSEFRAMESTX_6_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_BIT_ACBFCPAUSEFRAMESTX_6_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_BIT_ACBFCPAUSEFRAMESTX_6_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_BIT_ACBFCPAUSEFRAMESTX_6_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_6_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_6_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_6_MSB_get", A, 11);
    /* (0x000081cc bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_6_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_BIT_ACBFCPAUSEFRAMESTX_6_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_BIT_ACBFCPAUSEFRAMESTX_6_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_6_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_MSB_get", stop_bit, 7 );
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
    /* (0x000081cc bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_6_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_BIT_ACBFCPAUSEFRAMESTX_6_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_BIT_ACBFCPAUSEFRAMESTX_6_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_BIT_ACBFCPAUSEFRAMESTX_6_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_BIT_ACBFCPAUSEFRAMESTX_6_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_7_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_7_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_7_LSB_get", A, 11);
    /* (0x000081d0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_7_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_BIT_ACBFCPAUSEFRAMESTX_7_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_BIT_ACBFCPAUSEFRAMESTX_7_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_7_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_LSB_get", stop_bit, 31 );
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
    /* (0x000081d0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESTX_7_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_BIT_ACBFCPAUSEFRAMESTX_7_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_BIT_ACBFCPAUSEFRAMESTX_7_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_BIT_ACBFCPAUSEFRAMESTX_7_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_BIT_ACBFCPAUSEFRAMESTX_7_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_7_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_7_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESTX_7_MSB_get", A, 11);
    /* (0x000081d4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_7_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_BIT_ACBFCPAUSEFRAMESTX_7_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_BIT_ACBFCPAUSEFRAMESTX_7_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESTX_7_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_MSB_get", stop_bit, 7 );
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
    /* (0x000081d4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESTX_7_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_BIT_ACBFCPAUSEFRAMESTX_7_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_BIT_ACBFCPAUSEFRAMESTX_7_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_BIT_ACBFCPAUSEFRAMESTX_7_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_BIT_ACBFCPAUSEFRAMESTX_7_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_0_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_0_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_0_LSB_get", A, 11);
    /* (0x000081d8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_0_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_BIT_ACBFCPAUSEFRAMESRX_0_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_BIT_ACBFCPAUSEFRAMESRX_0_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_0_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_LSB_get", stop_bit, 31 );
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
    /* (0x000081d8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_0_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_BIT_ACBFCPAUSEFRAMESRX_0_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_BIT_ACBFCPAUSEFRAMESRX_0_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_BIT_ACBFCPAUSEFRAMESRX_0_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_BIT_ACBFCPAUSEFRAMESRX_0_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_0_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_0_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_0_MSB_get", A, 11);
    /* (0x000081dc bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_0_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_BIT_ACBFCPAUSEFRAMESRX_0_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_BIT_ACBFCPAUSEFRAMESRX_0_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_0_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_MSB_get", stop_bit, 7 );
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
    /* (0x000081dc bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_0_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_BIT_ACBFCPAUSEFRAMESRX_0_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_BIT_ACBFCPAUSEFRAMESRX_0_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_BIT_ACBFCPAUSEFRAMESRX_0_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_BIT_ACBFCPAUSEFRAMESRX_0_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_1_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_1_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_1_LSB_get", A, 11);
    /* (0x000081e0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_1_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_BIT_ACBFCPAUSEFRAMESRX_1_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_BIT_ACBFCPAUSEFRAMESRX_1_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_1_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_LSB_get", stop_bit, 31 );
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
    /* (0x000081e0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_1_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_BIT_ACBFCPAUSEFRAMESRX_1_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_BIT_ACBFCPAUSEFRAMESRX_1_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_BIT_ACBFCPAUSEFRAMESRX_1_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_BIT_ACBFCPAUSEFRAMESRX_1_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_1_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_1_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_1_MSB_get", A, 11);
    /* (0x000081e4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_1_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_BIT_ACBFCPAUSEFRAMESRX_1_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_BIT_ACBFCPAUSEFRAMESRX_1_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_1_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_MSB_get", stop_bit, 7 );
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
    /* (0x000081e4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_1_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_BIT_ACBFCPAUSEFRAMESRX_1_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_BIT_ACBFCPAUSEFRAMESRX_1_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_BIT_ACBFCPAUSEFRAMESRX_1_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_BIT_ACBFCPAUSEFRAMESRX_1_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_2_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_2_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_2_LSB_get", A, 11);
    /* (0x000081e8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_2_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_BIT_ACBFCPAUSEFRAMESRX_2_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_BIT_ACBFCPAUSEFRAMESRX_2_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_2_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_LSB_get", stop_bit, 31 );
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
    /* (0x000081e8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_2_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_BIT_ACBFCPAUSEFRAMESRX_2_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_BIT_ACBFCPAUSEFRAMESRX_2_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_BIT_ACBFCPAUSEFRAMESRX_2_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_BIT_ACBFCPAUSEFRAMESRX_2_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_2_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_2_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_2_MSB_get", A, 11);
    /* (0x000081ec bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_2_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_BIT_ACBFCPAUSEFRAMESRX_2_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_BIT_ACBFCPAUSEFRAMESRX_2_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_2_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_MSB_get", stop_bit, 7 );
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
    /* (0x000081ec bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_2_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_BIT_ACBFCPAUSEFRAMESRX_2_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_BIT_ACBFCPAUSEFRAMESRX_2_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_BIT_ACBFCPAUSEFRAMESRX_2_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_BIT_ACBFCPAUSEFRAMESRX_2_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_3_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_3_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_3_LSB_get", A, 11);
    /* (0x000081f0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_3_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_BIT_ACBFCPAUSEFRAMESRX_3_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_BIT_ACBFCPAUSEFRAMESRX_3_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_3_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_LSB_get", stop_bit, 31 );
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
    /* (0x000081f0 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_3_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_BIT_ACBFCPAUSEFRAMESRX_3_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_BIT_ACBFCPAUSEFRAMESRX_3_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_BIT_ACBFCPAUSEFRAMESRX_3_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_BIT_ACBFCPAUSEFRAMESRX_3_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_3_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_3_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_3_MSB_get", A, 11);
    /* (0x000081f4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_3_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_BIT_ACBFCPAUSEFRAMESRX_3_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_BIT_ACBFCPAUSEFRAMESRX_3_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_3_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_MSB_get", stop_bit, 7 );
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
    /* (0x000081f4 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_3_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_BIT_ACBFCPAUSEFRAMESRX_3_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_BIT_ACBFCPAUSEFRAMESRX_3_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_BIT_ACBFCPAUSEFRAMESRX_3_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_BIT_ACBFCPAUSEFRAMESRX_3_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_4_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_4_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_4_LSB_get", A, 11);
    /* (0x000081f8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_4_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_BIT_ACBFCPAUSEFRAMESRX_4_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_BIT_ACBFCPAUSEFRAMESRX_4_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_4_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_LSB_get", stop_bit, 31 );
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
    /* (0x000081f8 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_4_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_BIT_ACBFCPAUSEFRAMESRX_4_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_BIT_ACBFCPAUSEFRAMESRX_4_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_BIT_ACBFCPAUSEFRAMESRX_4_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_BIT_ACBFCPAUSEFRAMESRX_4_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_4_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_4_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_4_MSB_get", A, 11);
    /* (0x000081fc bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_4_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_BIT_ACBFCPAUSEFRAMESRX_4_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_BIT_ACBFCPAUSEFRAMESRX_4_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_4_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_MSB_get", stop_bit, 7 );
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
    /* (0x000081fc bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_4_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_BIT_ACBFCPAUSEFRAMESRX_4_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_BIT_ACBFCPAUSEFRAMESRX_4_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_BIT_ACBFCPAUSEFRAMESRX_4_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_BIT_ACBFCPAUSEFRAMESRX_4_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_5_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_5_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_5_LSB_get", A, 11);
    /* (0x00008200 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_5_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_BIT_ACBFCPAUSEFRAMESRX_5_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_BIT_ACBFCPAUSEFRAMESRX_5_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_5_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_LSB_get", stop_bit, 31 );
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
    /* (0x00008200 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_5_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_BIT_ACBFCPAUSEFRAMESRX_5_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_BIT_ACBFCPAUSEFRAMESRX_5_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_BIT_ACBFCPAUSEFRAMESRX_5_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_BIT_ACBFCPAUSEFRAMESRX_5_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_5_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_5_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_5_MSB_get", A, 11);
    /* (0x00008204 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_5_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_BIT_ACBFCPAUSEFRAMESRX_5_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_BIT_ACBFCPAUSEFRAMESRX_5_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_5_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_MSB_get", stop_bit, 7 );
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
    /* (0x00008204 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_5_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_BIT_ACBFCPAUSEFRAMESRX_5_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_BIT_ACBFCPAUSEFRAMESRX_5_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_BIT_ACBFCPAUSEFRAMESRX_5_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_BIT_ACBFCPAUSEFRAMESRX_5_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_6_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_6_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_6_LSB_get", A, 11);
    /* (0x00008208 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_6_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_BIT_ACBFCPAUSEFRAMESRX_6_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_BIT_ACBFCPAUSEFRAMESRX_6_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_6_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_LSB_get", stop_bit, 31 );
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
    /* (0x00008208 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_6_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_BIT_ACBFCPAUSEFRAMESRX_6_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_BIT_ACBFCPAUSEFRAMESRX_6_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_BIT_ACBFCPAUSEFRAMESRX_6_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_BIT_ACBFCPAUSEFRAMESRX_6_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_6_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_6_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_6_MSB_get", A, 11);
    /* (0x0000820c bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_6_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_BIT_ACBFCPAUSEFRAMESRX_6_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_BIT_ACBFCPAUSEFRAMESRX_6_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_6_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_MSB_get", stop_bit, 7 );
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
    /* (0x0000820c bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_6_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_BIT_ACBFCPAUSEFRAMESRX_6_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_BIT_ACBFCPAUSEFRAMESRX_6_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_BIT_ACBFCPAUSEFRAMESRX_6_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_BIT_ACBFCPAUSEFRAMESRX_6_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_7_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_7_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_7_LSB_get", A, 11);
    /* (0x00008210 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_7_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_BIT_ACBFCPAUSEFRAMESRX_7_LSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_BIT_ACBFCPAUSEFRAMESRX_7_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_7_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_LSB_get", stop_bit, 31 );
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
    /* (0x00008210 bits 31:0) bits 0:31 use field ACBFCPAUSEFRAMESRX_7_LSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_BIT_ACBFCPAUSEFRAMESRX_7_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_BIT_ACBFCPAUSEFRAMESRX_7_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_BIT_ACBFCPAUSEFRAMESRX_7_LSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_BIT_ACBFCPAUSEFRAMESRX_7_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_7_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_7_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_ACBFCPAUSEFRAMESRX_7_MSB_get", A, 11);
    /* (0x00008214 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_7_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_BIT_ACBFCPAUSEFRAMESRX_7_MSB_MSK) >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_BIT_ACBFCPAUSEFRAMESRX_7_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_ACBFCPAUSEFRAMESRX_7_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_MSB_get", stop_bit, 7 );
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
    /* (0x00008214 bits 7:0) bits 0:7 use field ACBFCPAUSEFRAMESRX_7_MSB of register PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB__read( b_ptr,
                                                                           h_ptr,
                                                                           A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_BIT_ACBFCPAUSEFRAMESRX_7_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_BIT_ACBFCPAUSEFRAMESRX_7_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_BIT_ACBFCPAUSEFRAMESRX_7_MSB_MSK, MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_BIT_ACBFCPAUSEFRAMESRX_7_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AMACCONTROLFRAMESTX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AMACCONTROLFRAMESTX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AMACCONTROLFRAMESTX_LSB_get", A, 11);
    /* (0x00008218 bits 31:0) bits 0:31 use field AMACCONTROLFRAMESTX_LSB of register PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB__read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_BIT_AMACCONTROLFRAMESTX_LSB_MSK) >> MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_BIT_AMACCONTROLFRAMESTX_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AMACCONTROLFRAMESTX_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AMACCONTROLFRAMESTX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AMACCONTROLFRAMESTX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AMACCONTROLFRAMESTX_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AMACCONTROLFRAMESTX_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AMACCONTROLFRAMESTX_LSB_get", stop_bit, 31 );
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
    /* (0x00008218 bits 31:0) bits 0:31 use field AMACCONTROLFRAMESTX_LSB of register PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB__read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_BIT_AMACCONTROLFRAMESTX_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_BIT_AMACCONTROLFRAMESTX_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_BIT_AMACCONTROLFRAMESTX_LSB_MSK, MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_BIT_AMACCONTROLFRAMESTX_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AMACCONTROLFRAMESTX_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AMACCONTROLFRAMESTX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AMACCONTROLFRAMESTX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AMACCONTROLFRAMESTX_MSB_get", A, 11);
    /* (0x0000821c bits 7:0) bits 0:7 use field AMACCONTROLFRAMESTX_MSB of register PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB__read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_BIT_AMACCONTROLFRAMESTX_MSB_MSK) >> MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_BIT_AMACCONTROLFRAMESTX_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AMACCONTROLFRAMESTX_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AMACCONTROLFRAMESTX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AMACCONTROLFRAMESTX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AMACCONTROLFRAMESTX_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AMACCONTROLFRAMESTX_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AMACCONTROLFRAMESTX_MSB_get", stop_bit, 7 );
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
    /* (0x0000821c bits 7:0) bits 0:7 use field AMACCONTROLFRAMESTX_MSB of register PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB__read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_BIT_AMACCONTROLFRAMESTX_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_BIT_AMACCONTROLFRAMESTX_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_BIT_AMACCONTROLFRAMESTX_MSB_MSK, MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_BIT_AMACCONTROLFRAMESTX_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AMACCONTROLFRAMESTX_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AMACCONTROLFRAMESRX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AMACCONTROLFRAMESRX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AMACCONTROLFRAMESRX_LSB_get", A, 11);
    /* (0x00008220 bits 31:0) bits 0:31 use field AMACCONTROLFRAMESRX_LSB of register PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB__read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_BIT_AMACCONTROLFRAMESRX_LSB_MSK) >> MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_BIT_AMACCONTROLFRAMESRX_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AMACCONTROLFRAMESRX_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AMACCONTROLFRAMESRX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AMACCONTROLFRAMESRX_LSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AMACCONTROLFRAMESRX_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AMACCONTROLFRAMESRX_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AMACCONTROLFRAMESRX_LSB_get", stop_bit, 31 );
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
    /* (0x00008220 bits 31:0) bits 0:31 use field AMACCONTROLFRAMESRX_LSB of register PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB__read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_BIT_AMACCONTROLFRAMESRX_LSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_BIT_AMACCONTROLFRAMESRX_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_BIT_AMACCONTROLFRAMESRX_LSB_MSK, MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_BIT_AMACCONTROLFRAMESRX_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AMACCONTROLFRAMESRX_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_mac_field_AMACCONTROLFRAMESRX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_AMACCONTROLFRAMESRX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_AMACCONTROLFRAMESRX_MSB_get", A, 11);
    /* (0x00008224 bits 7:0) bits 0:7 use field AMACCONTROLFRAMESRX_MSB of register PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB__read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    value = (reg_value & MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_BIT_AMACCONTROLFRAMESRX_MSB_MSK) >> MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_BIT_AMACCONTROLFRAMESRX_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_mac_field_AMACCONTROLFRAMESRX_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_mac_field_range_AMACCONTROLFRAMESRX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_mac_field_range_AMACCONTROLFRAMESRX_MSB_get( emac_mac_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_mac_field_range_AMACCONTROLFRAMESRX_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_mac_field_range_AMACCONTROLFRAMESRX_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_mac_field_range_AMACCONTROLFRAMESRX_MSB_get", stop_bit, 7 );
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
    /* (0x00008224 bits 7:0) bits 0:7 use field AMACCONTROLFRAMESRX_MSB of register PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_ */
    reg_value = emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB__read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    field_value = (reg_value & MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_BIT_AMACCONTROLFRAMESRX_MSB_MSK)
                  >> MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_BIT_AMACCONTROLFRAMESRX_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_BIT_AMACCONTROLFRAMESRX_MSB_MSK, MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_BIT_AMACCONTROLFRAMESRX_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_mac_field_range_AMACCONTROLFRAMESRX_MSB_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_MAC_IO_INLINE_H */
