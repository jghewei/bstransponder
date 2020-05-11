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
 *     and register accessor functions for the prgm block
 *****************************************************************************/
#ifndef _PRGM_IO_INLINE_H
#define _PRGM_IO_INLINE_H

#include "cbrc_api.h"
#include "prgm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PRGM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for prgm
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
} prgm_buffer_t;
static INLINE void prgm_buffer_init( prgm_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A ) ALWAYS_INLINE;
static INLINE void prgm_buffer_init( prgm_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x40) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "prgm_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void prgm_buffer_flush( prgm_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void prgm_buffer_flush( prgm_buffer_t *b_ptr )
{
    IOLOG( "prgm_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 prgm_reg_read( prgm_buffer_t *b_ptr,
                                    cbrc_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_read( prgm_buffer_t *b_ptr,
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
static INLINE void prgm_reg_write( prgm_buffer_t *b_ptr,
                                   cbrc_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_write( prgm_buffer_t *b_ptr,
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

static INLINE void prgm_field_set( prgm_buffer_t *b_ptr,
                                   cbrc_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_set( prgm_buffer_t *b_ptr,
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

static INLINE void prgm_action_on_write_field_set( prgm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_action_on_write_field_set( prgm_buffer_t *b_ptr,
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

static INLINE void prgm_burst_read( prgm_buffer_t *b_ptr,
                                    cbrc_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void prgm_burst_read( prgm_buffer_t *b_ptr,
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

static INLINE void prgm_burst_write( prgm_buffer_t *b_ptr,
                                     cbrc_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void prgm_burst_write( prgm_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE prgm_poll( prgm_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE prgm_poll( prgm_buffer_t *b_ptr,
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
 *  register access functions for prgm
 * ==================================================================================
 */

static INLINE void prgm_reg_IND_ADR_write( prgm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_IND_ADR_write( prgm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_IND_ADR_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_IND_ADR,
                    value);
}

static INLINE void prgm_reg_IND_ADR_field_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_IND_ADR_field_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_IND_ADR_field_set", A, mask, ofs, value );
    prgm_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_IND_ADR,
                    mask,
                    PMC_PRGM_REG_IND_ADR_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 prgm_reg_IND_ADR_read( prgm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_IND_ADR_read( prgm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_PRGM_REG_IND_ADR);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_IND_ADR_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_reg_IND_ADR_poll( prgm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 value,
                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                          UINT32 max_count,
                                                          UINT32 *num_failed_polls,
                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_reg_IND_ADR_poll( prgm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 value,
                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                          UINT32 max_count,
                                                          UINT32 *num_failed_polls,
                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "prgm_reg_IND_ADR_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return prgm_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_PRGM_REG_IND_ADR,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void prgm_reg_IND_DATA_write( prgm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_IND_DATA_write( prgm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_IND_DATA_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_IND_DATA,
                    value);
}

static INLINE void prgm_reg_IND_DATA_field_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_IND_DATA_field_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_IND_DATA_field_set", A, mask, ofs, value );
    prgm_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_IND_DATA,
                    mask,
                    PMC_PRGM_REG_IND_DATA_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 prgm_reg_IND_DATA_read( prgm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_IND_DATA_read( prgm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_PRGM_REG_IND_DATA);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_IND_DATA_read", reg_value, A);
    return reg_value;
}

static INLINE void prgm_reg_GENERATOR_PAYLOAD_CFG_write( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_GENERATOR_PAYLOAD_CFG_write( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_GENERATOR_PAYLOAD_CFG_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG,
                    value);
}

static INLINE void prgm_reg_GENERATOR_PAYLOAD_CFG_field_set( prgm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_GENERATOR_PAYLOAD_CFG_field_set( prgm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_GENERATOR_PAYLOAD_CFG_field_set", A, mask, ofs, value );
    prgm_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG,
                    mask,
                    PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 prgm_reg_GENERATOR_PAYLOAD_CFG_read( prgm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_GENERATOR_PAYLOAD_CFG_read( prgm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_GENERATOR_PAYLOAD_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void prgm_reg_MON_PAYLOAD_CFG_write( prgm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_PAYLOAD_CFG_write( prgm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_MON_PAYLOAD_CFG_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_MON_PAYLOAD_CFG,
                    value);
}

static INLINE void prgm_reg_MON_PAYLOAD_CFG_field_set( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_PAYLOAD_CFG_field_set( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_MON_PAYLOAD_CFG_field_set", A, mask, ofs, value );
    prgm_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_MON_PAYLOAD_CFG,
                    mask,
                    PMC_PRGM_REG_MON_PAYLOAD_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 prgm_reg_MON_PAYLOAD_CFG_read( prgm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_MON_PAYLOAD_CFG_read( prgm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_PRGM_REG_MON_PAYLOAD_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_MON_PAYLOAD_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void prgm_reg_MON_BYTE_ERR_INT_EN_write( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_BYTE_ERR_INT_EN_write( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_MON_BYTE_ERR_INT_EN_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_MON_BYTE_ERR_INT_EN,
                    value);
}

static INLINE void prgm_reg_MON_BYTE_ERR_INT_EN_field_set( prgm_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_BYTE_ERR_INT_EN_field_set( prgm_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_MON_BYTE_ERR_INT_EN_field_set", A, mask, ofs, value );
    prgm_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_CONFIG,
                    PMC_PRGM_REG_MON_BYTE_ERR_INT_EN,
                    mask,
                    PMC_PRGM_REG_MON_BYTE_ERR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 prgm_reg_MON_BYTE_ERR_INT_EN_read( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_MON_BYTE_ERR_INT_EN_read( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_PRGM_REG_MON_BYTE_ERR_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_MON_BYTE_ERR_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void prgm_reg_MON_BYTE_ERR_INT_STAT_write( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_BYTE_ERR_INT_STAT_write( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_MON_BYTE_ERR_INT_STAT_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT,
                    value);
}

static INLINE void prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( prgm_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( prgm_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set", A, mask, ofs, value );
    prgm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 prgm_reg_MON_BYTE_ERR_INT_STAT_read( prgm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_MON_BYTE_ERR_INT_STAT_read( prgm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_MON_BYTE_ERR_INT_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_reg_MON_BYTE_ERR_INT_STAT_poll( prgm_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_reg_MON_BYTE_ERR_INT_STAT_poll( prgm_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "prgm_reg_MON_BYTE_ERR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return prgm_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void prgm_reg_MON_SYNC_INT_STAT_write( prgm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_SYNC_INT_STAT_write( prgm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_MON_SYNC_INT_STAT_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_PRGM_REG_MON_SYNC_INT_STAT,
                    value);
}

static INLINE void prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( prgm_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( prgm_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set", A, mask, ofs, value );
    prgm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_PRGM_REG_MON_SYNC_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 prgm_reg_MON_SYNC_INT_STAT_read( prgm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_MON_SYNC_INT_STAT_read( prgm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_PRGM_REG_MON_SYNC_INT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_MON_SYNC_INT_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_reg_MON_SYNC_INT_STAT_poll( prgm_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_reg_MON_SYNC_INT_STAT_poll( prgm_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "prgm_reg_MON_SYNC_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return prgm_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_PRGM_REG_MON_SYNC_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void prgm_reg_MON_SYNC_INT_EN_write( prgm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_SYNC_INT_EN_write( prgm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_MON_SYNC_INT_EN_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_PRGM_REG_MON_SYNC_INT_EN,
                    value);
}

static INLINE void prgm_reg_MON_SYNC_INT_EN_field_set( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_MON_SYNC_INT_EN_field_set( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_MON_SYNC_INT_EN_field_set", A, mask, ofs, value );
    prgm_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_PRGM_REG_MON_SYNC_INT_EN,
                    mask,
                    PMC_PRGM_REG_MON_SYNC_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 prgm_reg_MON_SYNC_INT_EN_read( prgm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_MON_SYNC_INT_EN_read( prgm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_PRGM_REG_MON_SYNC_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_MON_SYNC_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 prgm_reg_MON_SYNC_STAT_read( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_MON_SYNC_STAT_read( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_PRGM_REG_MON_SYNC_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_MON_SYNC_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE void prgm_reg_PERF_CNT_TRANSFER_TRIG_write( prgm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_PERF_CNT_TRANSFER_TRIG_write( prgm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prgm_reg_PERF_CNT_TRANSFER_TRIG_write", value );
    prgm_reg_write( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG,
                    value);
}

static INLINE void prgm_reg_PERF_CNT_TRANSFER_TRIG_field_set( prgm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_reg_PERF_CNT_TRANSFER_TRIG_field_set( prgm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "prgm_reg_PERF_CNT_TRANSFER_TRIG_field_set", A, mask, ofs, value );
    prgm_field_set( b_ptr,
                    h_ptr,
                    A,
                    MEM_TYPE_STATUS,
                    PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG,
                    mask,
                    PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 prgm_reg_PERF_CNT_TRANSFER_TRIG_read( prgm_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_reg_PERF_CNT_TRANSFER_TRIG_read( prgm_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = prgm_reg_read( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG);

    IOLOG( "%s -> 0x%08x; A=%d", "prgm_reg_PERF_CNT_TRANSFER_TRIG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_reg_PERF_CNT_TRANSFER_TRIG_poll( prgm_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_reg_PERF_CNT_TRANSFER_TRIG_poll( prgm_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "prgm_reg_PERF_CNT_TRANSFER_TRIG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return prgm_poll( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG,
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
static INLINE void prgm_field_RWB_set( prgm_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_RWB_set( prgm_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_RWB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_RWB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_RWB_set", A, value );

    /* (0x00001440 bits 14) field RWB of register PMC_PRGM_REG_IND_ADR */
    prgm_reg_IND_ADR_field_set( b_ptr,
                                h_ptr,
                                A,
                                PRGM_REG_IND_ADR_BIT_RWB_MSK,
                                PRGM_REG_IND_ADR_BIT_RWB_OFF,
                                value);
}

static INLINE UINT32 prgm_field_RWB_get( prgm_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_RWB_get( prgm_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_RWB_get", A, 15);
    /* (0x00001440 bits 14) field RWB of register PMC_PRGM_REG_IND_ADR */
    reg_value = prgm_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_IND_ADR_BIT_RWB_MSK) >> PRGM_REG_IND_ADR_BIT_RWB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_RWB_get", A, value );

    return value;
}
static INLINE void prgm_field_IADDR_set( prgm_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_IADDR_set( prgm_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_IADDR_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_IADDR_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_IADDR_set", A, value );

    /* (0x00001440 bits 9:6) bits 0:3 use field IADDR of register PMC_PRGM_REG_IND_ADR */
    prgm_reg_IND_ADR_field_set( b_ptr,
                                h_ptr,
                                A,
                                PRGM_REG_IND_ADR_BIT_IADDR_MSK,
                                PRGM_REG_IND_ADR_BIT_IADDR_OFF,
                                value);
}

static INLINE UINT32 prgm_field_IADDR_get( prgm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_IADDR_get( prgm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_IADDR_get", A, 15);
    /* (0x00001440 bits 9:6) bits 0:3 use field IADDR of register PMC_PRGM_REG_IND_ADR */
    reg_value = prgm_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_IND_ADR_BIT_IADDR_MSK) >> PRGM_REG_IND_ADR_BIT_IADDR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_IADDR_get", A, value );

    return value;
}
static INLINE void prgm_field_range_IADDR_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_range_IADDR_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_IADDR_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_IADDR_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_IADDR_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_IADDR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001440 bits 9:6) bits 0:3 use field IADDR of register PMC_PRGM_REG_IND_ADR */
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
        /* (0x00001440 bits 9:6) bits 0:3 use field IADDR of register PMC_PRGM_REG_IND_ADR */
        prgm_reg_IND_ADR_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    subfield_mask << (PRGM_REG_IND_ADR_BIT_IADDR_OFF + subfield_offset),
                                    PRGM_REG_IND_ADR_BIT_IADDR_OFF + subfield_offset,
                                    value >> subfield_shift);
    }
}

static INLINE UINT32 prgm_field_range_IADDR_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_range_IADDR_get( prgm_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_IADDR_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_IADDR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_IADDR_get", stop_bit, 3 );
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
    /* (0x00001440 bits 9:6) bits 0:3 use field IADDR of register PMC_PRGM_REG_IND_ADR */
    reg_value = prgm_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PRGM_REG_IND_ADR_BIT_IADDR_MSK)
                  >> PRGM_REG_IND_ADR_BIT_IADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRGM_REG_IND_ADR_BIT_IADDR_MSK, PRGM_REG_IND_ADR_BIT_IADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_IADDR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void prgm_field_PATH_set( prgm_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_PATH_set( prgm_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_PATH_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_PATH_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_PATH_set", A, value );

    /* (0x00001440 bits 3:0) bits 0:3 use field PATH of register PMC_PRGM_REG_IND_ADR */
    prgm_reg_IND_ADR_field_set( b_ptr,
                                h_ptr,
                                A,
                                PRGM_REG_IND_ADR_BIT_PATH_MSK,
                                PRGM_REG_IND_ADR_BIT_PATH_OFF,
                                value);
}

static INLINE UINT32 prgm_field_PATH_get( prgm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_PATH_get( prgm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_PATH_get", A, 15);
    /* (0x00001440 bits 3:0) bits 0:3 use field PATH of register PMC_PRGM_REG_IND_ADR */
    reg_value = prgm_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_IND_ADR_BIT_PATH_MSK) >> PRGM_REG_IND_ADR_BIT_PATH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_PATH_get", A, value );

    return value;
}
static INLINE void prgm_field_range_PATH_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_range_PATH_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_PATH_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_PATH_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_PATH_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_PATH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001440 bits 3:0) bits 0:3 use field PATH of register PMC_PRGM_REG_IND_ADR */
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
        /* (0x00001440 bits 3:0) bits 0:3 use field PATH of register PMC_PRGM_REG_IND_ADR */
        prgm_reg_IND_ADR_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    subfield_mask << (PRGM_REG_IND_ADR_BIT_PATH_OFF + subfield_offset),
                                    PRGM_REG_IND_ADR_BIT_PATH_OFF + subfield_offset,
                                    value >> subfield_shift);
    }
}

static INLINE UINT32 prgm_field_range_PATH_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_range_PATH_get( prgm_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_PATH_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_PATH_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_PATH_get", stop_bit, 3 );
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
    /* (0x00001440 bits 3:0) bits 0:3 use field PATH of register PMC_PRGM_REG_IND_ADR */
    reg_value = prgm_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PRGM_REG_IND_ADR_BIT_PATH_MSK)
                  >> PRGM_REG_IND_ADR_BIT_PATH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRGM_REG_IND_ADR_BIT_PATH_MSK, PRGM_REG_IND_ADR_BIT_PATH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_PATH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void prgm_field_DATA_set( prgm_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_DATA_set( prgm_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_DATA_set", A, 15);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_DATA_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_DATA_set", A, value );

    /* (0x00001444 bits 15:0) bits 0:15 use field DATA of register PMC_PRGM_REG_IND_DATA */
    prgm_reg_IND_DATA_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 PRGM_REG_IND_DATA_BIT_DATA_MSK,
                                 PRGM_REG_IND_DATA_BIT_DATA_OFF,
                                 value);
}

static INLINE UINT32 prgm_field_DATA_get( prgm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_DATA_get( prgm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_DATA_get", A, 15);
    /* (0x00001444 bits 15:0) bits 0:15 use field DATA of register PMC_PRGM_REG_IND_DATA */
    reg_value = prgm_reg_IND_DATA_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_IND_DATA_BIT_DATA_MSK) >> PRGM_REG_IND_DATA_BIT_DATA_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_DATA_get", A, value );

    return value;
}
static INLINE void prgm_field_range_DATA_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_range_DATA_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_DATA_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_DATA_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_DATA_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_DATA_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001444 bits 15:0) bits 0:15 use field DATA of register PMC_PRGM_REG_IND_DATA */
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
        /* (0x00001444 bits 15:0) bits 0:15 use field DATA of register PMC_PRGM_REG_IND_DATA */
        prgm_reg_IND_DATA_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     subfield_mask << (PRGM_REG_IND_DATA_BIT_DATA_OFF + subfield_offset),
                                     PRGM_REG_IND_DATA_BIT_DATA_OFF + subfield_offset,
                                     value >> subfield_shift);
    }
}

static INLINE UINT32 prgm_field_range_DATA_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_range_DATA_get( prgm_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_DATA_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_DATA_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_DATA_get", stop_bit, 15 );
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
    /* (0x00001444 bits 15:0) bits 0:15 use field DATA of register PMC_PRGM_REG_IND_DATA */
    reg_value = prgm_reg_IND_DATA_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PRGM_REG_IND_DATA_BIT_DATA_MSK)
                  >> PRGM_REG_IND_DATA_BIT_DATA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRGM_REG_IND_DATA_BIT_DATA_MSK, PRGM_REG_IND_DATA_BIT_DATA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_DATA_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void prgm_field_GEN_STS12CSL_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_GEN_STS12CSL_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_GEN_STS12CSL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_GEN_STS12CSL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_GEN_STS12CSL_set", A, value );

    /* (0x00001448 bits 15) field GEN_STS12CSL of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
    prgm_reg_GENERATOR_PAYLOAD_CFG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12CSL_MSK,
                                              PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12CSL_OFF,
                                              value);
}

static INLINE UINT32 prgm_field_GEN_STS12CSL_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_GEN_STS12CSL_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_GEN_STS12CSL_get", A, 15);
    /* (0x00001448 bits 15) field GEN_STS12CSL of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
    reg_value = prgm_reg_GENERATOR_PAYLOAD_CFG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12CSL_MSK) >> PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12CSL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_GEN_STS12CSL_get", A, value );

    return value;
}
static INLINE void prgm_field_GEN_STS12C_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_GEN_STS12C_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_GEN_STS12C_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_GEN_STS12C_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_GEN_STS12C_set", A, value );

    /* (0x00001448 bits 14) field GEN_STS12C of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
    prgm_reg_GENERATOR_PAYLOAD_CFG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12C_MSK,
                                              PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12C_OFF,
                                              value);
}

static INLINE UINT32 prgm_field_GEN_STS12C_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_GEN_STS12C_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_GEN_STS12C_get", A, 15);
    /* (0x00001448 bits 14) field GEN_STS12C of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
    reg_value = prgm_reg_GENERATOR_PAYLOAD_CFG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12C_MSK) >> PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12C_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_GEN_STS12C_get", A, value );

    return value;
}
static INLINE void prgm_field_GEN_MSSLEN_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_GEN_MSSLEN_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_GEN_MSSLEN_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_GEN_MSSLEN_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_GEN_MSSLEN_set", A, value );

    /* (0x00001448 bits 10:8) bits 0:2 use field GEN_MSSLEN of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
    prgm_reg_GENERATOR_PAYLOAD_CFG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_MSK,
                                              PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_OFF,
                                              value);
}

static INLINE UINT32 prgm_field_GEN_MSSLEN_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_GEN_MSSLEN_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_GEN_MSSLEN_get", A, 15);
    /* (0x00001448 bits 10:8) bits 0:2 use field GEN_MSSLEN of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
    reg_value = prgm_reg_GENERATOR_PAYLOAD_CFG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_MSK) >> PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_GEN_MSSLEN_get", A, value );

    return value;
}
static INLINE void prgm_field_range_GEN_MSSLEN_set( prgm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_range_GEN_MSSLEN_set( prgm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_GEN_MSSLEN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_GEN_MSSLEN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_GEN_MSSLEN_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_GEN_MSSLEN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001448 bits 10:8) bits 0:2 use field GEN_MSSLEN of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
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
        /* (0x00001448 bits 10:8) bits 0:2 use field GEN_MSSLEN of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
        prgm_reg_GENERATOR_PAYLOAD_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_OFF + subfield_offset),
                                                  PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 prgm_field_range_GEN_MSSLEN_get( prgm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_range_GEN_MSSLEN_get( prgm_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_GEN_MSSLEN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_GEN_MSSLEN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_GEN_MSSLEN_get", stop_bit, 2 );
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
    /* (0x00001448 bits 10:8) bits 0:2 use field GEN_MSSLEN of register PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG */
    reg_value = prgm_reg_GENERATOR_PAYLOAD_CFG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_MSK)
                  >> PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_MSK, PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_GEN_MSSLEN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void prgm_field_MON_STS12CSL_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON_STS12CSL_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON_STS12CSL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON_STS12CSL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON_STS12CSL_set", A, value );

    /* (0x0000144c bits 15) field MON_STS12CSL of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
    prgm_reg_MON_PAYLOAD_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12CSL_MSK,
                                        PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12CSL_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON_STS12CSL_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON_STS12CSL_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON_STS12CSL_get", A, 15);
    /* (0x0000144c bits 15) field MON_STS12CSL of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
    reg_value = prgm_reg_MON_PAYLOAD_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12CSL_MSK) >> PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12CSL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON_STS12CSL_get", A, value );

    return value;
}
static INLINE void prgm_field_MON_STS12C_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON_STS12C_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON_STS12C_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON_STS12C_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON_STS12C_set", A, value );

    /* (0x0000144c bits 14) field MON_STS12C of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
    prgm_reg_MON_PAYLOAD_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12C_MSK,
                                        PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12C_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON_STS12C_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON_STS12C_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON_STS12C_get", A, 15);
    /* (0x0000144c bits 14) field MON_STS12C of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
    reg_value = prgm_reg_MON_PAYLOAD_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12C_MSK) >> PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12C_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON_STS12C_get", A, value );

    return value;
}
static INLINE void prgm_field_MON_MSSLEN_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON_MSSLEN_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON_MSSLEN_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON_MSSLEN_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON_MSSLEN_set", A, value );

    /* (0x0000144c bits 10:8) bits 0:2 use field MON_MSSLEN of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
    prgm_reg_MON_PAYLOAD_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_MSK,
                                        PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON_MSSLEN_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON_MSSLEN_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON_MSSLEN_get", A, 15);
    /* (0x0000144c bits 10:8) bits 0:2 use field MON_MSSLEN of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
    reg_value = prgm_reg_MON_PAYLOAD_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_MSK) >> PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON_MSSLEN_get", A, value );

    return value;
}
static INLINE void prgm_field_range_MON_MSSLEN_set( prgm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_range_MON_MSSLEN_set( prgm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_MON_MSSLEN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_MON_MSSLEN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_MON_MSSLEN_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_MON_MSSLEN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000144c bits 10:8) bits 0:2 use field MON_MSSLEN of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
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
        /* (0x0000144c bits 10:8) bits 0:2 use field MON_MSSLEN of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
        prgm_reg_MON_PAYLOAD_CFG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_OFF + subfield_offset),
                                            PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 prgm_field_range_MON_MSSLEN_get( prgm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_range_MON_MSSLEN_get( prgm_buffer_t *b_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_range_MON_MSSLEN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prgm_field_range_MON_MSSLEN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prgm_field_range_MON_MSSLEN_get", stop_bit, 2 );
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
    /* (0x0000144c bits 10:8) bits 0:2 use field MON_MSSLEN of register PMC_PRGM_REG_MON_PAYLOAD_CFG */
    reg_value = prgm_reg_MON_PAYLOAD_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_MSK)
                  >> PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_MSK, PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "prgm_field_range_MON_MSSLEN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void prgm_field_MON12_ERR_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON12_ERR_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON12_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON12_ERR_E_set", A, value );

    /* (0x00001454 bits 11) field MON12_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON12_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON12_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON12_ERR_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON12_ERR_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_ERR_E_get", A, 15);
    /* (0x00001454 bits 11) field MON12_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON12_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON12_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON12_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON11_ERR_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON11_ERR_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON11_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON11_ERR_E_set", A, value );

    /* (0x00001454 bits 10) field MON11_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON11_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON11_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON11_ERR_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON11_ERR_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_ERR_E_get", A, 15);
    /* (0x00001454 bits 10) field MON11_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON11_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON11_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON11_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON10_ERR_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON10_ERR_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON10_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON10_ERR_E_set", A, value );

    /* (0x00001454 bits 9) field MON10_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON10_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON10_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON10_ERR_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON10_ERR_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_ERR_E_get", A, 15);
    /* (0x00001454 bits 9) field MON10_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON10_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON10_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON10_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON9_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON9_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON9_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON9_ERR_E_set", A, value );

    /* (0x00001454 bits 8) field MON9_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON9_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON9_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON9_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON9_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_ERR_E_get", A, 15);
    /* (0x00001454 bits 8) field MON9_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON9_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON9_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON9_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON8_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON8_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON8_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON8_ERR_E_set", A, value );

    /* (0x00001454 bits 7) field MON8_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON8_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON8_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON8_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON8_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_ERR_E_get", A, 15);
    /* (0x00001454 bits 7) field MON8_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON8_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON8_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON8_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON7_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON7_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON7_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON7_ERR_E_set", A, value );

    /* (0x00001454 bits 6) field MON7_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON7_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON7_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON7_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON7_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_ERR_E_get", A, 15);
    /* (0x00001454 bits 6) field MON7_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON7_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON7_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON7_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON6_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON6_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON6_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON6_ERR_E_set", A, value );

    /* (0x00001454 bits 5) field MON6_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON6_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON6_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON6_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON6_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_ERR_E_get", A, 15);
    /* (0x00001454 bits 5) field MON6_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON6_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON6_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON6_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON5_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON5_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON5_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON5_ERR_E_set", A, value );

    /* (0x00001454 bits 4) field MON5_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON5_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON5_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON5_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON5_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_ERR_E_get", A, 15);
    /* (0x00001454 bits 4) field MON5_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON5_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON5_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON5_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON4_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON4_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON4_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON4_ERR_E_set", A, value );

    /* (0x00001454 bits 3) field MON4_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON4_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON4_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON4_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON4_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_ERR_E_get", A, 15);
    /* (0x00001454 bits 3) field MON4_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON4_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON4_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON4_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON3_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON3_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON3_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON3_ERR_E_set", A, value );

    /* (0x00001454 bits 2) field MON3_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON3_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON3_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON3_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON3_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_ERR_E_get", A, 15);
    /* (0x00001454 bits 2) field MON3_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON3_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON3_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON3_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON2_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON2_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON2_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON2_ERR_E_set", A, value );

    /* (0x00001454 bits 1) field MON2_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON2_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON2_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON2_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON2_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_ERR_E_get", A, 15);
    /* (0x00001454 bits 1) field MON2_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON2_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON2_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON2_ERR_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON1_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON1_ERR_E_set( prgm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON1_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON1_ERR_E_set", A, value );

    /* (0x00001454 bits 0) field MON1_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    prgm_reg_MON_BYTE_ERR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON1_ERR_E_MSK,
                                            PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON1_ERR_E_OFF,
                                            value);
}

static INLINE UINT32 prgm_field_MON1_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON1_ERR_E_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_ERR_E_get", A, 15);
    /* (0x00001454 bits 0) field MON1_ERR_E of register PMC_PRGM_REG_MON_BYTE_ERR_INT_EN */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_EN_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON1_ERR_E_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON1_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON1_ERR_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void prgm_field_MON12_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON12_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON12_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON12_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 11) field MON12_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON12_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON12_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON12_ERR_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON12_ERR_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_ERR_I_get", A, 15);
    /* (0x00001450 bits 11) field MON12_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON12_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON12_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON12_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON12_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON12_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON12_ERR_I_poll", A, value );

    /* (0x00001450 bits 11) field MON12_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON12_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON12_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON11_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON11_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON11_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON11_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 10) field MON11_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON11_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON11_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON11_ERR_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON11_ERR_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_ERR_I_get", A, 15);
    /* (0x00001450 bits 10) field MON11_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON11_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON11_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON11_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON11_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON11_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON11_ERR_I_poll", A, value );

    /* (0x00001450 bits 10) field MON11_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON11_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON11_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON10_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON10_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON10_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON10_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 9) field MON10_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON10_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON10_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON10_ERR_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON10_ERR_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_ERR_I_get", A, 15);
    /* (0x00001450 bits 9) field MON10_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON10_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON10_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON10_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON10_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON10_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON10_ERR_I_poll", A, value );

    /* (0x00001450 bits 9) field MON10_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON10_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON10_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON9_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON9_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON9_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON9_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 8) field MON9_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON9_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON9_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON9_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON9_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_ERR_I_get", A, 15);
    /* (0x00001450 bits 8) field MON9_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON9_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON9_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON9_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON9_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON9_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON9_ERR_I_poll", A, value );

    /* (0x00001450 bits 8) field MON9_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON9_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON9_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON8_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON8_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON8_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON8_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 7) field MON8_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON8_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON8_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON8_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON8_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_ERR_I_get", A, 15);
    /* (0x00001450 bits 7) field MON8_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON8_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON8_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON8_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON8_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON8_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON8_ERR_I_poll", A, value );

    /* (0x00001450 bits 7) field MON8_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON8_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON8_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON7_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON7_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON7_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON7_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 6) field MON7_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON7_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON7_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON7_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON7_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_ERR_I_get", A, 15);
    /* (0x00001450 bits 6) field MON7_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON7_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON7_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON7_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON7_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON7_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON7_ERR_I_poll", A, value );

    /* (0x00001450 bits 6) field MON7_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON7_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON7_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON6_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON6_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON6_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON6_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 5) field MON6_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON6_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON6_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON6_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON6_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_ERR_I_get", A, 15);
    /* (0x00001450 bits 5) field MON6_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON6_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON6_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON6_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON6_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON6_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON6_ERR_I_poll", A, value );

    /* (0x00001450 bits 5) field MON6_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON6_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON6_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON5_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON5_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON5_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON5_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 4) field MON5_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON5_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON5_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON5_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON5_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_ERR_I_get", A, 15);
    /* (0x00001450 bits 4) field MON5_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON5_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON5_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON5_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON5_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON5_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON5_ERR_I_poll", A, value );

    /* (0x00001450 bits 4) field MON5_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON5_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON5_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON4_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON4_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON4_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON4_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 3) field MON4_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON4_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON4_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON4_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON4_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_ERR_I_get", A, 15);
    /* (0x00001450 bits 3) field MON4_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON4_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON4_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON4_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON4_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON4_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON4_ERR_I_poll", A, value );

    /* (0x00001450 bits 3) field MON4_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON4_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON4_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON3_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON3_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON3_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON3_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 2) field MON3_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON3_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON3_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON3_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON3_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_ERR_I_get", A, 15);
    /* (0x00001450 bits 2) field MON3_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON3_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON3_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON3_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON3_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON3_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON3_ERR_I_poll", A, value );

    /* (0x00001450 bits 2) field MON3_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON3_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON3_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON2_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON2_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON2_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON2_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 1) field MON2_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON2_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON2_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON2_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON2_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_ERR_I_get", A, 15);
    /* (0x00001450 bits 1) field MON2_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON2_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON2_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON2_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON2_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON2_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON2_ERR_I_poll", A, value );

    /* (0x00001450 bits 1) field MON2_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON2_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON2_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON1_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON1_ERR_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON1_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON1_ERR_I_set_to_clear", A, value );

    /* (0x00001450 bits 0) field MON1_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON1_ERR_I_MSK,
                                                              PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON1_ERR_I_OFF,
                                                              value);
}

static INLINE UINT32 prgm_field_MON1_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON1_ERR_I_get( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_ERR_I_get", A, 15);
    /* (0x00001450 bits 0) field MON1_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    reg_value = prgm_reg_MON_BYTE_ERR_INT_STAT_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON1_ERR_I_MSK) >> PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON1_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON1_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON1_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON1_ERR_I_poll( prgm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON1_ERR_I_poll", A, value );

    /* (0x00001450 bits 0) field MON1_ERR_I of register PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT */
    return prgm_reg_MON_BYTE_ERR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON1_ERR_I_MSK,
                                                (value<<PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON1_ERR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON12_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON12_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON12_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON12_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 11) field MON12_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON12_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON12_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON12_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON12_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_SYNC_I_get", A, 15);
    /* (0x00001464 bits 11) field MON12_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON12_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON12_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON12_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON12_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON12_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON12_SYNC_I_poll", A, value );

    /* (0x00001464 bits 11) field MON12_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON12_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON12_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON11_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON11_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON11_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON11_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 10) field MON11_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON11_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON11_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON11_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON11_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_SYNC_I_get", A, 15);
    /* (0x00001464 bits 10) field MON11_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON11_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON11_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON11_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON11_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON11_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON11_SYNC_I_poll", A, value );

    /* (0x00001464 bits 10) field MON11_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON11_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON11_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON10_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON10_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON10_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON10_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 9) field MON10_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON10_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON10_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON10_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON10_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_SYNC_I_get", A, 15);
    /* (0x00001464 bits 9) field MON10_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON10_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON10_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON10_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON10_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON10_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON10_SYNC_I_poll", A, value );

    /* (0x00001464 bits 9) field MON10_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON10_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON10_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON9_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON9_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON9_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON9_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 8) field MON9_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON9_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON9_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON9_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON9_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_SYNC_I_get", A, 15);
    /* (0x00001464 bits 8) field MON9_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON9_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON9_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON9_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON9_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON9_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON9_SYNC_I_poll", A, value );

    /* (0x00001464 bits 8) field MON9_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON9_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON9_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON8_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON8_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON8_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON8_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 7) field MON8_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON8_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON8_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON8_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON8_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_SYNC_I_get", A, 15);
    /* (0x00001464 bits 7) field MON8_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON8_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON8_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON8_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON8_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON8_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON8_SYNC_I_poll", A, value );

    /* (0x00001464 bits 7) field MON8_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON8_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON8_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON7_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON7_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON7_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON7_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 6) field MON7_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON7_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON7_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON7_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON7_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_SYNC_I_get", A, 15);
    /* (0x00001464 bits 6) field MON7_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON7_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON7_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON7_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON7_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON7_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON7_SYNC_I_poll", A, value );

    /* (0x00001464 bits 6) field MON7_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON7_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON7_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON6_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON6_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON6_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON6_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 5) field MON6_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON6_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON6_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON6_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON6_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_SYNC_I_get", A, 15);
    /* (0x00001464 bits 5) field MON6_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON6_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON6_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON6_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON6_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON6_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON6_SYNC_I_poll", A, value );

    /* (0x00001464 bits 5) field MON6_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON6_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON6_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON5_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON5_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON5_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON5_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 4) field MON5_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON5_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON5_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON5_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON5_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_SYNC_I_get", A, 15);
    /* (0x00001464 bits 4) field MON5_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON5_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON5_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON5_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON5_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON5_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON5_SYNC_I_poll", A, value );

    /* (0x00001464 bits 4) field MON5_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON5_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON5_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON4_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON4_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON4_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON4_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 3) field MON4_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON4_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON4_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON4_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON4_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_SYNC_I_get", A, 15);
    /* (0x00001464 bits 3) field MON4_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON4_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON4_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON4_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON4_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON4_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON4_SYNC_I_poll", A, value );

    /* (0x00001464 bits 3) field MON4_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON4_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON4_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON3_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON3_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON3_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON3_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 2) field MON3_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON3_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON3_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON3_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON3_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_SYNC_I_get", A, 15);
    /* (0x00001464 bits 2) field MON3_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON3_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON3_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON3_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON3_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON3_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON3_SYNC_I_poll", A, value );

    /* (0x00001464 bits 2) field MON3_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON3_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON3_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON2_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON2_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON2_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON2_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 1) field MON2_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON2_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON2_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON2_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON2_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_SYNC_I_get", A, 15);
    /* (0x00001464 bits 1) field MON2_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON2_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON2_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON2_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON2_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON2_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON2_SYNC_I_poll", A, value );

    /* (0x00001464 bits 1) field MON2_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON2_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON2_SYNC_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void prgm_field_MON1_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON1_SYNC_I_set_to_clear( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_SYNC_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON1_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON1_SYNC_I_set_to_clear", A, value );

    /* (0x00001464 bits 0) field MON1_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON1_SYNC_I_MSK,
                                                          PRGM_REG_MON_SYNC_INT_STAT_BIT_MON1_SYNC_I_OFF,
                                                          value);
}

static INLINE UINT32 prgm_field_MON1_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON1_SYNC_I_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_SYNC_I_get", A, 15);
    /* (0x00001464 bits 0) field MON1_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    reg_value = prgm_reg_MON_SYNC_INT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_STAT_BIT_MON1_SYNC_I_MSK) >> PRGM_REG_MON_SYNC_INT_STAT_BIT_MON1_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON1_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON1_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_MON1_SYNC_I_poll( prgm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_MON1_SYNC_I_poll", A, value );

    /* (0x00001464 bits 0) field MON1_SYNC_I of register PMC_PRGM_REG_MON_SYNC_INT_STAT */
    return prgm_reg_MON_SYNC_INT_STAT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            PRGM_REG_MON_SYNC_INT_STAT_BIT_MON1_SYNC_I_MSK,
                                            (value<<PRGM_REG_MON_SYNC_INT_STAT_BIT_MON1_SYNC_I_OFF),
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
static INLINE void prgm_field_MON12_SYNC_E_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON12_SYNC_E_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON12_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON12_SYNC_E_set", A, value );

    /* (0x00001468 bits 11) field MON12_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON12_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON12_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON12_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON12_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_SYNC_E_get", A, 15);
    /* (0x00001468 bits 11) field MON12_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON12_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON12_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON12_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON11_SYNC_E_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON11_SYNC_E_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON11_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON11_SYNC_E_set", A, value );

    /* (0x00001468 bits 10) field MON11_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON11_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON11_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON11_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON11_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_SYNC_E_get", A, 15);
    /* (0x00001468 bits 10) field MON11_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON11_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON11_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON11_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON10_SYNC_E_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON10_SYNC_E_set( prgm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON10_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON10_SYNC_E_set", A, value );

    /* (0x00001468 bits 9) field MON10_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON10_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON10_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON10_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON10_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_SYNC_E_get", A, 15);
    /* (0x00001468 bits 9) field MON10_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON10_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON10_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON10_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON9_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON9_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON9_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON9_SYNC_E_set", A, value );

    /* (0x00001468 bits 8) field MON9_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON9_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON9_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON9_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON9_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_SYNC_E_get", A, 15);
    /* (0x00001468 bits 8) field MON9_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON9_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON9_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON9_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON8_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON8_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON8_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON8_SYNC_E_set", A, value );

    /* (0x00001468 bits 7) field MON8_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON8_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON8_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON8_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON8_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_SYNC_E_get", A, 15);
    /* (0x00001468 bits 7) field MON8_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON8_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON8_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON8_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON7_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON7_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON7_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON7_SYNC_E_set", A, value );

    /* (0x00001468 bits 6) field MON7_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON7_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON7_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON7_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON7_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_SYNC_E_get", A, 15);
    /* (0x00001468 bits 6) field MON7_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON7_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON7_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON7_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON6_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON6_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON6_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON6_SYNC_E_set", A, value );

    /* (0x00001468 bits 5) field MON6_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON6_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON6_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON6_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON6_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_SYNC_E_get", A, 15);
    /* (0x00001468 bits 5) field MON6_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON6_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON6_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON6_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON5_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON5_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON5_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON5_SYNC_E_set", A, value );

    /* (0x00001468 bits 4) field MON5_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON5_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON5_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON5_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON5_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_SYNC_E_get", A, 15);
    /* (0x00001468 bits 4) field MON5_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON5_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON5_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON5_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON4_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON4_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON4_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON4_SYNC_E_set", A, value );

    /* (0x00001468 bits 3) field MON4_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON4_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON4_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON4_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON4_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_SYNC_E_get", A, 15);
    /* (0x00001468 bits 3) field MON4_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON4_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON4_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON4_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON3_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON3_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON3_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON3_SYNC_E_set", A, value );

    /* (0x00001468 bits 2) field MON3_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON3_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON3_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON3_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON3_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_SYNC_E_get", A, 15);
    /* (0x00001468 bits 2) field MON3_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON3_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON3_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON3_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON2_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON2_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON2_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON2_SYNC_E_set", A, value );

    /* (0x00001468 bits 1) field MON2_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON2_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON2_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON2_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON2_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_SYNC_E_get", A, 15);
    /* (0x00001468 bits 1) field MON2_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON2_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON2_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON2_SYNC_E_get", A, value );

    return value;
}
static INLINE void prgm_field_MON1_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_MON1_SYNC_E_set( prgm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_SYNC_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_MON1_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_MON1_SYNC_E_set", A, value );

    /* (0x00001468 bits 0) field MON1_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    prgm_reg_MON_SYNC_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON1_SYNC_E_MSK,
                                        PRGM_REG_MON_SYNC_INT_EN_BIT_MON1_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 prgm_field_MON1_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON1_SYNC_E_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_SYNC_E_get", A, 15);
    /* (0x00001468 bits 0) field MON1_SYNC_E of register PMC_PRGM_REG_MON_SYNC_INT_EN */
    reg_value = prgm_reg_MON_SYNC_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_INT_EN_BIT_MON1_SYNC_E_MSK) >> PRGM_REG_MON_SYNC_INT_EN_BIT_MON1_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON1_SYNC_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 prgm_field_BUSY_get( prgm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_BUSY_get( prgm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_BUSY_get", A, 15);
    /* (0x00001440 bits 15) field BUSY of register PMC_PRGM_REG_IND_ADR */
    reg_value = prgm_reg_IND_ADR_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_IND_ADR_BIT_BUSY_MSK) >> PRGM_REG_IND_ADR_BIT_BUSY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_BUSY_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_BUSY_poll( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_BUSY_poll( prgm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_BUSY_poll", A, value );

    /* (0x00001440 bits 15) field BUSY of register PMC_PRGM_REG_IND_ADR */
    return prgm_reg_IND_ADR_poll( b_ptr,
                                  h_ptr,
                                  A,
                                  PRGM_REG_IND_ADR_BIT_BUSY_MSK,
                                  (value<<PRGM_REG_IND_ADR_BIT_BUSY_OFF),
                                  cmp,
                                  max_count,
                                  num_failed_polls,
                                  delay_between_polls_in_microseconds);
}

static INLINE UINT32 prgm_field_MON12_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON12_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON12_SYNC_V_get", A, 15);
    /* (0x0000146c bits 11) field MON12_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON12_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON12_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON12_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON11_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON11_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON11_SYNC_V_get", A, 15);
    /* (0x0000146c bits 10) field MON11_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON11_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON11_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON11_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON10_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON10_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON10_SYNC_V_get", A, 15);
    /* (0x0000146c bits 9) field MON10_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON10_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON10_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON10_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON9_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON9_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON9_SYNC_V_get", A, 15);
    /* (0x0000146c bits 8) field MON9_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON9_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON9_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON9_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON8_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON8_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON8_SYNC_V_get", A, 15);
    /* (0x0000146c bits 7) field MON8_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON8_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON8_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON8_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON7_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON7_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON7_SYNC_V_get", A, 15);
    /* (0x0000146c bits 6) field MON7_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON7_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON7_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON7_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON6_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON6_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON6_SYNC_V_get", A, 15);
    /* (0x0000146c bits 5) field MON6_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON6_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON6_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON6_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON5_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON5_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON5_SYNC_V_get", A, 15);
    /* (0x0000146c bits 4) field MON5_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON5_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON5_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON5_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON4_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON4_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON4_SYNC_V_get", A, 15);
    /* (0x0000146c bits 3) field MON4_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON4_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON4_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON4_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON3_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON3_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON3_SYNC_V_get", A, 15);
    /* (0x0000146c bits 2) field MON3_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON3_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON3_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON3_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON2_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON2_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON2_SYNC_V_get", A, 15);
    /* (0x0000146c bits 1) field MON2_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON2_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON2_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON2_SYNC_V_get", A, value );

    return value;
}
static INLINE UINT32 prgm_field_MON1_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_MON1_SYNC_V_get( prgm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_MON1_SYNC_V_get", A, 15);
    /* (0x0000146c bits 0) field MON1_SYNC_V of register PMC_PRGM_REG_MON_SYNC_STAT */
    reg_value = prgm_reg_MON_SYNC_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PRGM_REG_MON_SYNC_STAT_BIT_MON1_SYNC_V_MSK) >> PRGM_REG_MON_SYNC_STAT_BIT_MON1_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_MON1_SYNC_V_get", A, value );

    return value;
}
static INLINE void prgm_field_TIP_set( prgm_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void prgm_field_TIP_set( prgm_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_TIP_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prgm_field_TIP_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "prgm_field_TIP_set", A, value );

    /* (0x00001470 bits 0) field TIP of register PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG */
    prgm_reg_PERF_CNT_TRANSFER_TRIG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PRGM_REG_PERF_CNT_TRANSFER_TRIG_BIT_TIP_MSK,
                                               PRGM_REG_PERF_CNT_TRANSFER_TRIG_BIT_TIP_OFF,
                                               value);
}

static INLINE UINT32 prgm_field_TIP_get( prgm_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prgm_field_TIP_get( prgm_buffer_t *b_ptr,
                                         cbrc_handle_t *h_ptr,
                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "prgm_field_TIP_get", A, 15);
    /* (0x00001470 bits 0) field TIP of register PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG */
    reg_value = prgm_reg_PERF_CNT_TRANSFER_TRIG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PRGM_REG_PERF_CNT_TRANSFER_TRIG_BIT_TIP_MSK) >> PRGM_REG_PERF_CNT_TRANSFER_TRIG_BIT_TIP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "prgm_field_TIP_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prgm_field_TIP_poll( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value,
                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                        UINT32 max_count,
                                                        UINT32 *num_failed_polls,
                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prgm_field_TIP_poll( prgm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value,
                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                        UINT32 max_count,
                                                        UINT32 *num_failed_polls,
                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "prgm_field_TIP_poll", A, value );

    /* (0x00001470 bits 0) field TIP of register PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG */
    return prgm_reg_PERF_CNT_TRANSFER_TRIG_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PRGM_REG_PERF_CNT_TRANSFER_TRIG_BIT_TIP_MSK,
                                                 (value<<PRGM_REG_PERF_CNT_TRANSFER_TRIG_BIT_TIP_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRGM_IO_INLINE_H */
