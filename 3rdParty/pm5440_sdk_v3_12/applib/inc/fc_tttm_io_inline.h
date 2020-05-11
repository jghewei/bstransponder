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
 *     and register accessor functions for the fc_tttm block
 *****************************************************************************/
#ifndef _FC_TTTM_IO_INLINE_H
#define _FC_TTTM_IO_INLINE_H

#include "cbrc_api.h"
#include "fc_tttm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FC_TTTM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for fc_tttm
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
} fc_tttm_buffer_t;
static INLINE void fc_tttm_buffer_init( fc_tttm_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A ) ALWAYS_INLINE;
static INLINE void fc_tttm_buffer_init( fc_tttm_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x200) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "fc_tttm_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void fc_tttm_buffer_flush( fc_tttm_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void fc_tttm_buffer_flush( fc_tttm_buffer_t *b_ptr )
{
    IOLOG( "fc_tttm_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 fc_tttm_reg_read( fc_tttm_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_read( fc_tttm_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x200),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg);
    return value;
}
static INLINE void fc_tttm_reg_write( fc_tttm_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_write( fc_tttm_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x200),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                         value);
}

static INLINE void fc_tttm_field_set( fc_tttm_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_set( fc_tttm_buffer_t *b_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x200),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void fc_tttm_action_on_write_field_set( fc_tttm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_action_on_write_field_set( fc_tttm_buffer_t *b_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x200),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                         value<<ofs);
}

static INLINE void fc_tttm_burst_read( fc_tttm_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc_tttm_burst_read( fc_tttm_buffer_t *b_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x200),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                          len,
                          value);
}

static INLINE void fc_tttm_burst_write( fc_tttm_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc_tttm_burst_write( fc_tttm_buffer_t *b_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x200),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE fc_tttm_poll( fc_tttm_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_poll( fc_tttm_buffer_t *b_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x200),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x200) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for fc_tttm
 * ==================================================================================
 */

static INLINE void fc_tttm_reg_CFG_write( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_CFG_write( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_CFG_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_CFG,
                       value);
}

static INLINE void fc_tttm_reg_CFG_field_set( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_CFG_field_set( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_CFG_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_CFG,
                       mask,
                       PMC_FC_TTTM_REG_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_CFG_read( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_CFG_read( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_GFP_CORE_HEADER_write( fc_tttm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_CORE_HEADER_write( fc_tttm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_GFP_CORE_HEADER_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_CORE_HEADER,
                       value);
}

static INLINE void fc_tttm_reg_GFP_CORE_HEADER_field_set( fc_tttm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_CORE_HEADER_field_set( fc_tttm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_GFP_CORE_HEADER_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_CORE_HEADER,
                       mask,
                       PMC_FC_TTTM_REG_GFP_CORE_HEADER_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_GFP_CORE_HEADER_read( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_GFP_CORE_HEADER_read( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_GFP_CORE_HEADER);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_GFP_CORE_HEADER_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_GFP_PAYLOAD_HEADER_write( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_PAYLOAD_HEADER_write( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_GFP_PAYLOAD_HEADER_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER,
                       value);
}

static INLINE void fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER,
                       mask,
                       PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_GFP_PAYLOAD_HEADER_read( fc_tttm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_GFP_PAYLOAD_HEADER_read( fc_tttm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_GFP_PAYLOAD_HEADER_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_1_4_write( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_1_4_write( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_GFP_RESERVED_BYTES_1_4_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4,
                       value);
}

static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_1_4_field_set( fc_tttm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_1_4_field_set( fc_tttm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_GFP_RESERVED_BYTES_1_4_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4,
                       mask,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_GFP_RESERVED_BYTES_1_4_read( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_GFP_RESERVED_BYTES_1_4_read( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_GFP_RESERVED_BYTES_1_4_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_5_8_write( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_5_8_write( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_GFP_RESERVED_BYTES_5_8_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8,
                       value);
}

static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_5_8_field_set( fc_tttm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_5_8_field_set( fc_tttm_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_GFP_RESERVED_BYTES_5_8_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8,
                       mask,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_GFP_RESERVED_BYTES_5_8_read( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_GFP_RESERVED_BYTES_5_8_read( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_GFP_RESERVED_BYTES_5_8_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_9_12_write( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_9_12_write( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_GFP_RESERVED_BYTES_9_12_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12,
                       value);
}

static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_9_12_field_set( fc_tttm_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_9_12_field_set( fc_tttm_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_GFP_RESERVED_BYTES_9_12_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12,
                       mask,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_GFP_RESERVED_BYTES_9_12_read( fc_tttm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_GFP_RESERVED_BYTES_9_12_read( fc_tttm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_GFP_RESERVED_BYTES_9_12_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_13_16_write( fc_tttm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_13_16_write( fc_tttm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_GFP_RESERVED_BYTES_13_16_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16,
                       value);
}

static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_13_16_field_set( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_GFP_RESERVED_BYTES_13_16_field_set( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_GFP_RESERVED_BYTES_13_16_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16,
                       mask,
                       PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_GFP_RESERVED_BYTES_13_16_read( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_GFP_RESERVED_BYTES_13_16_read( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_GFP_RESERVED_BYTES_13_16_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_write( fc_tttm_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_write( fc_tttm_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB,
                       value);
}

static INLINE void fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set( fc_tttm_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set( fc_tttm_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB,
                       mask,
                       PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_read( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_read( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_write( fc_tttm_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_write( fc_tttm_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB,
                       value);
}

static INLINE void fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set( fc_tttm_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set( fc_tttm_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB,
                       mask,
                       PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_read( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_read( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_ERR_CTRL_BLOCK_LSB_write( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_ERR_CTRL_BLOCK_LSB_write( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_ERR_CTRL_BLOCK_LSB_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB,
                       value);
}

static INLINE void fc_tttm_reg_ERR_CTRL_BLOCK_LSB_field_set( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_ERR_CTRL_BLOCK_LSB_field_set( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_ERR_CTRL_BLOCK_LSB_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB,
                       mask,
                       PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_ERR_CTRL_BLOCK_LSB_read( fc_tttm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_ERR_CTRL_BLOCK_LSB_read( fc_tttm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_ERR_CTRL_BLOCK_LSB_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_ERR_CTRL_BLOCK_MSB_write( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_ERR_CTRL_BLOCK_MSB_write( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_ERR_CTRL_BLOCK_MSB_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB,
                       value);
}

static INLINE void fc_tttm_reg_ERR_CTRL_BLOCK_MSB_field_set( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_ERR_CTRL_BLOCK_MSB_field_set( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_ERR_CTRL_BLOCK_MSB_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB,
                       mask,
                       PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_ERR_CTRL_BLOCK_MSB_read( fc_tttm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_ERR_CTRL_BLOCK_MSB_read( fc_tttm_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_ERR_CTRL_BLOCK_MSB_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_write( fc_tttm_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_write( fc_tttm_buffer_t *b_ptr,
                                                                           cbrc_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR,
                       value);
}

static INLINE void fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_field_set( fc_tttm_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_field_set( fc_tttm_buffer_t *b_ptr,
                                                                               cbrc_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR,
                       mask,
                       PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_read( fc_tttm_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_read( fc_tttm_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_write( fc_tttm_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_write( fc_tttm_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR,
                       value);
}

static INLINE void fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_field_set( fc_tttm_buffer_t *b_ptr,
                                                                                 cbrc_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_field_set( fc_tttm_buffer_t *b_ptr,
                                                                                 cbrc_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR,
                       mask,
                       PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_read( fc_tttm_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_read( fc_tttm_buffer_t *b_ptr,
                                                                              cbrc_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_CNT_UPDATE_write( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_CNT_UPDATE_write( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_CNT_UPDATE_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTM_REG_CNT_UPDATE,
                       value);
}

static INLINE void fc_tttm_reg_CNT_UPDATE_field_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_CNT_UPDATE_field_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_CNT_UPDATE_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTM_REG_CNT_UPDATE,
                       mask,
                       PMC_FC_TTTM_REG_CNT_UPDATE_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_CNT_UPDATE_read( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_CNT_UPDATE_read( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTM_REG_CNT_UPDATE);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_CNT_UPDATE_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_reg_CNT_UPDATE_poll( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_reg_CNT_UPDATE_poll( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_tttm_reg_CNT_UPDATE_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_tttm_poll( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_FC_TTTM_REG_CNT_UPDATE,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void fc_tttm_reg_INT_EN_write( fc_tttm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_INT_EN_write( fc_tttm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_INT_EN_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTM_REG_INT_EN,
                       value);
}

static INLINE void fc_tttm_reg_INT_EN_field_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_INT_EN_field_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_INT_EN_field_set", A, mask, ofs, value );
    fc_tttm_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTM_REG_INT_EN,
                       mask,
                       PMC_FC_TTTM_REG_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fc_tttm_reg_INT_EN_read( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_INT_EN_read( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTM_REG_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_tttm_reg_INT_write( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_INT_write( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_tttm_reg_INT_write", value );
    fc_tttm_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_FC_TTTM_REG_INT,
                       value);
}

static INLINE void fc_tttm_reg_INT_action_on_write_field_set( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_reg_INT_action_on_write_field_set( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_tttm_reg_INT_action_on_write_field_set", A, mask, ofs, value );
    fc_tttm_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_FC_TTTM_REG_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 fc_tttm_reg_INT_read( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_INT_read( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTM_REG_INT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_INT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_reg_INT_poll( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_reg_INT_poll( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_tttm_reg_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_tttm_poll( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_FC_TTTM_REG_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc_tttm_reg_INT_VAL_read( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_INT_VAL_read( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTM_REG_INT_VAL);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_INT_VAL_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_reg_INT_VAL_poll( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_reg_INT_VAL_poll( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_tttm_reg_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_tttm_poll( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_FC_TTTM_REG_INT_VAL,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc_tttm_reg_GFP_FRAME_CNT_read( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_GFP_FRAME_CNT_read( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTM_REG_GFP_FRAME_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_GFP_FRAME_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_tttm_reg_GFP_ERRED_FRAMES_CNT_read( fc_tttm_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_reg_GFP_ERRED_FRAMES_CNT_read( fc_tttm_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_tttm_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_FC_TTTM_REG_GFP_ERRED_FRAMES_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_tttm_reg_GFP_ERRED_FRAMES_CNT_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void fc_tttm_field_DIS_SCRAM_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_DIS_SCRAM_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_DIS_SCRAM_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_DIS_SCRAM_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_DIS_SCRAM_set", A, value );

    /* (0x0002f100 bits 26) field DIS_SCRAM of register PMC_FC_TTTM_REG_CFG */
    fc_tttm_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTM_REG_CFG_BIT_DIS_SCRAM_MSK,
                               FC_TTTM_REG_CFG_BIT_DIS_SCRAM_OFF,
                               value);
}

static INLINE UINT32 fc_tttm_field_DIS_SCRAM_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_DIS_SCRAM_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_DIS_SCRAM_get", A, 11);
    /* (0x0002f100 bits 26) field DIS_SCRAM of register PMC_FC_TTTM_REG_CFG */
    reg_value = fc_tttm_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CFG_BIT_DIS_SCRAM_MSK) >> FC_TTTM_REG_CFG_BIT_DIS_SCRAM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_DIS_SCRAM_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_FORCE_LF_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_FORCE_LF_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_FORCE_LF_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_FORCE_LF_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_FORCE_LF_set", A, value );

    /* (0x0002f100 bits 25) field FORCE_LF of register PMC_FC_TTTM_REG_CFG */
    fc_tttm_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTM_REG_CFG_BIT_FORCE_LF_MSK,
                               FC_TTTM_REG_CFG_BIT_FORCE_LF_OFF,
                               value);
}

static INLINE UINT32 fc_tttm_field_FORCE_LF_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_FORCE_LF_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_FORCE_LF_get", A, 11);
    /* (0x0002f100 bits 25) field FORCE_LF of register PMC_FC_TTTM_REG_CFG */
    reg_value = fc_tttm_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CFG_BIT_FORCE_LF_MSK) >> FC_TTTM_REG_CFG_BIT_FORCE_LF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_FORCE_LF_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_FORCE_ERR_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_FORCE_ERR_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_FORCE_ERR_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_FORCE_ERR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_FORCE_ERR_set", A, value );

    /* (0x0002f100 bits 24) field FORCE_ERR of register PMC_FC_TTTM_REG_CFG */
    fc_tttm_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTM_REG_CFG_BIT_FORCE_ERR_MSK,
                               FC_TTTM_REG_CFG_BIT_FORCE_ERR_OFF,
                               value);
}

static INLINE UINT32 fc_tttm_field_FORCE_ERR_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_FORCE_ERR_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_FORCE_ERR_get", A, 11);
    /* (0x0002f100 bits 24) field FORCE_ERR of register PMC_FC_TTTM_REG_CFG */
    reg_value = fc_tttm_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CFG_BIT_FORCE_ERR_MSK) >> FC_TTTM_REG_CFG_BIT_FORCE_ERR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_FORCE_ERR_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_LFOS_HIBER_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_LFOS_HIBER_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LFOS_HIBER_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_LFOS_HIBER_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_LFOS_HIBER_set", A, value );

    /* (0x0002f100 bits 14) field LFOS_HIBER of register PMC_FC_TTTM_REG_CFG */
    fc_tttm_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTM_REG_CFG_BIT_LFOS_HIBER_MSK,
                               FC_TTTM_REG_CFG_BIT_LFOS_HIBER_OFF,
                               value);
}

static INLINE UINT32 fc_tttm_field_LFOS_HIBER_get( fc_tttm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LFOS_HIBER_get( fc_tttm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LFOS_HIBER_get", A, 11);
    /* (0x0002f100 bits 14) field LFOS_HIBER of register PMC_FC_TTTM_REG_CFG */
    reg_value = fc_tttm_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CFG_BIT_LFOS_HIBER_MSK) >> FC_TTTM_REG_CFG_BIT_LFOS_HIBER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LFOS_HIBER_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_LFOS_SYNC_LOST_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_LFOS_SYNC_LOST_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LFOS_SYNC_LOST_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_LFOS_SYNC_LOST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_LFOS_SYNC_LOST_set", A, value );

    /* (0x0002f100 bits 13) field LFOS_SYNC_LOST of register PMC_FC_TTTM_REG_CFG */
    fc_tttm_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTM_REG_CFG_BIT_LFOS_SYNC_LOST_MSK,
                               FC_TTTM_REG_CFG_BIT_LFOS_SYNC_LOST_OFF,
                               value);
}

static INLINE UINT32 fc_tttm_field_LFOS_SYNC_LOST_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LFOS_SYNC_LOST_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LFOS_SYNC_LOST_get", A, 11);
    /* (0x0002f100 bits 13) field LFOS_SYNC_LOST of register PMC_FC_TTTM_REG_CFG */
    reg_value = fc_tttm_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CFG_BIT_LFOS_SYNC_LOST_MSK) >> FC_TTTM_REG_CFG_BIT_LFOS_SYNC_LOST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LFOS_SYNC_LOST_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_LFOS_LINK_FAIL_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_LFOS_LINK_FAIL_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LFOS_LINK_FAIL_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_LFOS_LINK_FAIL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_LFOS_LINK_FAIL_set", A, value );

    /* (0x0002f100 bits 12) field LFOS_LINK_FAIL of register PMC_FC_TTTM_REG_CFG */
    fc_tttm_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTM_REG_CFG_BIT_LFOS_LINK_FAIL_MSK,
                               FC_TTTM_REG_CFG_BIT_LFOS_LINK_FAIL_OFF,
                               value);
}

static INLINE UINT32 fc_tttm_field_LFOS_LINK_FAIL_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LFOS_LINK_FAIL_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LFOS_LINK_FAIL_get", A, 11);
    /* (0x0002f100 bits 12) field LFOS_LINK_FAIL of register PMC_FC_TTTM_REG_CFG */
    reg_value = fc_tttm_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CFG_BIT_LFOS_LINK_FAIL_MSK) >> FC_TTTM_REG_CFG_BIT_LFOS_LINK_FAIL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LFOS_LINK_FAIL_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_PLI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_PLI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_PLI_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_PLI_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_PLI_set", A, value );

    /* (0x0002f110 bits 31:16) bits 0:15 use field PLI of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
    fc_tttm_reg_GFP_CORE_HEADER_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_MSK,
                                           FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_OFF,
                                           value);
}

static INLINE UINT32 fc_tttm_field_PLI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_PLI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_PLI_get", A, 11);
    /* (0x0002f110 bits 31:16) bits 0:15 use field PLI of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
    reg_value = fc_tttm_reg_GFP_CORE_HEADER_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_MSK) >> FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_PLI_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_PLI_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_PLI_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_PLI_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_PLI_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_PLI_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_PLI_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f110 bits 31:16) bits 0:15 use field PLI of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
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
        /* (0x0002f110 bits 31:16) bits 0:15 use field PLI of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
        fc_tttm_reg_GFP_CORE_HEADER_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_OFF + subfield_offset),
                                               FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_PLI_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_PLI_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_PLI_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_PLI_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_PLI_get", stop_bit, 15 );
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
    /* (0x0002f110 bits 31:16) bits 0:15 use field PLI of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
    reg_value = fc_tttm_reg_GFP_CORE_HEADER_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_MSK)
                  >> FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_MSK, FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_PLI_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_CHEC_set( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_CHEC_set( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_CHEC_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_CHEC_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_CHEC_set", A, value );

    /* (0x0002f110 bits 15:0) bits 0:15 use field CHEC of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
    fc_tttm_reg_GFP_CORE_HEADER_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_MSK,
                                           FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_OFF,
                                           value);
}

static INLINE UINT32 fc_tttm_field_CHEC_get( fc_tttm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_CHEC_get( fc_tttm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_CHEC_get", A, 11);
    /* (0x0002f110 bits 15:0) bits 0:15 use field CHEC of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
    reg_value = fc_tttm_reg_GFP_CORE_HEADER_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_MSK) >> FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_CHEC_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_CHEC_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_CHEC_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_CHEC_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_CHEC_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_CHEC_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_CHEC_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f110 bits 15:0) bits 0:15 use field CHEC of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
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
        /* (0x0002f110 bits 15:0) bits 0:15 use field CHEC of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
        fc_tttm_reg_GFP_CORE_HEADER_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_OFF + subfield_offset),
                                               FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_CHEC_get( fc_tttm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_CHEC_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_CHEC_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_CHEC_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_CHEC_get", stop_bit, 15 );
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
    /* (0x0002f110 bits 15:0) bits 0:15 use field CHEC of register PMC_FC_TTTM_REG_GFP_CORE_HEADER */
    reg_value = fc_tttm_reg_GFP_CORE_HEADER_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_MSK)
                  >> FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_MSK, FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_CHEC_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_PTI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_PTI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_PTI_set", A, 11);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_PTI_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_PTI_set", A, value );

    /* (0x0002f114 bits 31:29) bits 0:2 use field PTI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_MSK,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_OFF,
                                              value);
}

static INLINE UINT32 fc_tttm_field_PTI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_PTI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_PTI_get", A, 11);
    /* (0x0002f114 bits 31:29) bits 0:2 use field PTI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_MSK) >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_PTI_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_PTI_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_PTI_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_PTI_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_PTI_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_PTI_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_PTI_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f114 bits 31:29) bits 0:2 use field PTI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
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
        /* (0x0002f114 bits 31:29) bits 0:2 use field PTI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
        fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_OFF + subfield_offset),
                                                  FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_PTI_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_PTI_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_PTI_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_PTI_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_PTI_get", stop_bit, 2 );
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
    /* (0x0002f114 bits 31:29) bits 0:2 use field PTI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_MSK)
                  >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_MSK, FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_PTI_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_PFI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_PFI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_PFI_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_PFI_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_PFI_set", A, value );

    /* (0x0002f114 bits 28) field PFI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PFI_MSK,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PFI_OFF,
                                              value);
}

static INLINE UINT32 fc_tttm_field_PFI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_PFI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_PFI_get", A, 11);
    /* (0x0002f114 bits 28) field PFI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PFI_MSK) >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PFI_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_PFI_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_EXI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_EXI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_EXI_set", A, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_EXI_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_EXI_set", A, value );

    /* (0x0002f114 bits 27:24) bits 0:3 use field EXI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_MSK,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_OFF,
                                              value);
}

static INLINE UINT32 fc_tttm_field_EXI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_EXI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_EXI_get", A, 11);
    /* (0x0002f114 bits 27:24) bits 0:3 use field EXI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_MSK) >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_EXI_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_EXI_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_EXI_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_EXI_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_EXI_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_EXI_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_EXI_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f114 bits 27:24) bits 0:3 use field EXI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
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
        /* (0x0002f114 bits 27:24) bits 0:3 use field EXI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
        fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_OFF + subfield_offset),
                                                  FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_EXI_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_EXI_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_EXI_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_EXI_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_EXI_get", stop_bit, 3 );
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
    /* (0x0002f114 bits 27:24) bits 0:3 use field EXI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_MSK)
                  >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_MSK, FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_EXI_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_UPI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_UPI_set( fc_tttm_buffer_t *b_ptr,
                                          cbrc_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_UPI_set", A, 11);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_UPI_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_UPI_set", A, value );

    /* (0x0002f114 bits 23:16) bits 0:7 use field UPI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_MSK,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_OFF,
                                              value);
}

static INLINE UINT32 fc_tttm_field_UPI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_UPI_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_UPI_get", A, 11);
    /* (0x0002f114 bits 23:16) bits 0:7 use field UPI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_MSK) >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_UPI_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_UPI_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_UPI_set( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_UPI_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_UPI_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_UPI_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_UPI_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f114 bits 23:16) bits 0:7 use field UPI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
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
        /* (0x0002f114 bits 23:16) bits 0:7 use field UPI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
        fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_OFF + subfield_offset),
                                                  FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_UPI_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_UPI_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_UPI_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_UPI_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_UPI_get", stop_bit, 7 );
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
    /* (0x0002f114 bits 23:16) bits 0:7 use field UPI of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_MSK)
                  >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_MSK, FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_UPI_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_THEC_set( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_THEC_set( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_THEC_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_THEC_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_THEC_set", A, value );

    /* (0x0002f114 bits 15:0) bits 0:15 use field THEC of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_MSK,
                                              FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_OFF,
                                              value);
}

static INLINE UINT32 fc_tttm_field_THEC_get( fc_tttm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_THEC_get( fc_tttm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_THEC_get", A, 11);
    /* (0x0002f114 bits 15:0) bits 0:15 use field THEC of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_MSK) >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_THEC_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_THEC_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_THEC_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_THEC_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_THEC_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_THEC_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_THEC_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f114 bits 15:0) bits 0:15 use field THEC of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
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
        /* (0x0002f114 bits 15:0) bits 0:15 use field THEC of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
        fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_OFF + subfield_offset),
                                                  FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_THEC_get( fc_tttm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_THEC_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_THEC_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_THEC_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_THEC_get", stop_bit, 15 );
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
    /* (0x0002f114 bits 15:0) bits 0:15 use field THEC of register PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER */
    reg_value = fc_tttm_reg_GFP_PAYLOAD_HEADER_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_MSK)
                  >> FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_MSK, FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_THEC_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_GFP_RES1_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_GFP_RES1_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFP_RES1_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_GFP_RES1_set", A, value );

    /* (0x0002f118 bits 31:0) bits 0:31 use field GFP_RES1 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4 */
    fc_tttm_reg_GFP_RESERVED_BYTES_1_4_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_MSK,
                                                  FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_OFF,
                                                  value);
}

static INLINE UINT32 fc_tttm_field_GFP_RES1_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_GFP_RES1_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFP_RES1_get", A, 11);
    /* (0x0002f118 bits 31:0) bits 0:31 use field GFP_RES1 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4 */
    reg_value = fc_tttm_reg_GFP_RESERVED_BYTES_1_4_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_MSK) >> FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_GFP_RES1_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_GFP_RES1_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_GFP_RES1_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFP_RES1_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFP_RES1_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFP_RES1_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFP_RES1_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f118 bits 31:0) bits 0:31 use field GFP_RES1 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4 */
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
        /* (0x0002f118 bits 31:0) bits 0:31 use field GFP_RES1 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4 */
        fc_tttm_reg_GFP_RESERVED_BYTES_1_4_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      subfield_mask << (FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_OFF + subfield_offset),
                                                      FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_GFP_RES1_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_GFP_RES1_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFP_RES1_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFP_RES1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFP_RES1_get", stop_bit, 31 );
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
    /* (0x0002f118 bits 31:0) bits 0:31 use field GFP_RES1 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4 */
    reg_value = fc_tttm_reg_GFP_RESERVED_BYTES_1_4_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_MSK)
                  >> FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_MSK, FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFP_RES1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_GFP_RES2_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_GFP_RES2_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFP_RES2_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_GFP_RES2_set", A, value );

    /* (0x0002f11c bits 31:0) bits 0:31 use field GFP_RES2 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8 */
    fc_tttm_reg_GFP_RESERVED_BYTES_5_8_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_MSK,
                                                  FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_OFF,
                                                  value);
}

static INLINE UINT32 fc_tttm_field_GFP_RES2_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_GFP_RES2_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFP_RES2_get", A, 11);
    /* (0x0002f11c bits 31:0) bits 0:31 use field GFP_RES2 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8 */
    reg_value = fc_tttm_reg_GFP_RESERVED_BYTES_5_8_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_MSK) >> FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_GFP_RES2_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_GFP_RES2_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_GFP_RES2_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFP_RES2_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFP_RES2_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFP_RES2_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFP_RES2_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f11c bits 31:0) bits 0:31 use field GFP_RES2 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8 */
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
        /* (0x0002f11c bits 31:0) bits 0:31 use field GFP_RES2 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8 */
        fc_tttm_reg_GFP_RESERVED_BYTES_5_8_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      subfield_mask << (FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_OFF + subfield_offset),
                                                      FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_GFP_RES2_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_GFP_RES2_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFP_RES2_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFP_RES2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFP_RES2_get", stop_bit, 31 );
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
    /* (0x0002f11c bits 31:0) bits 0:31 use field GFP_RES2 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8 */
    reg_value = fc_tttm_reg_GFP_RESERVED_BYTES_5_8_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_MSK)
                  >> FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_MSK, FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFP_RES2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_GFP_RES3_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_GFP_RES3_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFP_RES3_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_GFP_RES3_set", A, value );

    /* (0x0002f120 bits 31:0) bits 0:31 use field GFP_RES3 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12 */
    fc_tttm_reg_GFP_RESERVED_BYTES_9_12_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_MSK,
                                                   FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_OFF,
                                                   value);
}

static INLINE UINT32 fc_tttm_field_GFP_RES3_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_GFP_RES3_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFP_RES3_get", A, 11);
    /* (0x0002f120 bits 31:0) bits 0:31 use field GFP_RES3 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12 */
    reg_value = fc_tttm_reg_GFP_RESERVED_BYTES_9_12_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_MSK) >> FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_GFP_RES3_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_GFP_RES3_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_GFP_RES3_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFP_RES3_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFP_RES3_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFP_RES3_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFP_RES3_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f120 bits 31:0) bits 0:31 use field GFP_RES3 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12 */
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
        /* (0x0002f120 bits 31:0) bits 0:31 use field GFP_RES3 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12 */
        fc_tttm_reg_GFP_RESERVED_BYTES_9_12_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_OFF + subfield_offset),
                                                       FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_GFP_RES3_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_GFP_RES3_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFP_RES3_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFP_RES3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFP_RES3_get", stop_bit, 31 );
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
    /* (0x0002f120 bits 31:0) bits 0:31 use field GFP_RES3 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12 */
    reg_value = fc_tttm_reg_GFP_RESERVED_BYTES_9_12_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_MSK)
                  >> FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_MSK, FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFP_RES3_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_GFP_RES4_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_GFP_RES4_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFP_RES4_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_GFP_RES4_set", A, value );

    /* (0x0002f124 bits 31:0) bits 0:31 use field GFP_RES4 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16 */
    fc_tttm_reg_GFP_RESERVED_BYTES_13_16_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_MSK,
                                                    FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_OFF,
                                                    value);
}

static INLINE UINT32 fc_tttm_field_GFP_RES4_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_GFP_RES4_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFP_RES4_get", A, 11);
    /* (0x0002f124 bits 31:0) bits 0:31 use field GFP_RES4 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16 */
    reg_value = fc_tttm_reg_GFP_RESERVED_BYTES_13_16_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_MSK) >> FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_GFP_RES4_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_GFP_RES4_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_GFP_RES4_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFP_RES4_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFP_RES4_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFP_RES4_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFP_RES4_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f124 bits 31:0) bits 0:31 use field GFP_RES4 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16 */
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
        /* (0x0002f124 bits 31:0) bits 0:31 use field GFP_RES4 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16 */
        fc_tttm_reg_GFP_RESERVED_BYTES_13_16_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        subfield_mask << (FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_OFF + subfield_offset),
                                                        FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_GFP_RES4_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_GFP_RES4_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFP_RES4_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFP_RES4_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFP_RES4_get", stop_bit, 31 );
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
    /* (0x0002f124 bits 31:0) bits 0:31 use field GFP_RES4 of register PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16 */
    reg_value = fc_tttm_reg_GFP_RESERVED_BYTES_13_16_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_MSK)
                  >> FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_MSK, FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFP_RES4_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_lfield_LFOS_set( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_tttm_lfield_LFOS_set( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value[2] )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_lfield_LFOS_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x", "fc_tttm_lfield_LFOS_set", A, value[1] , value[0] );

    /* (0x0002f128 bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB */
    fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK,
                                                       FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF,
                                                       value[0]);

    /* (0x0002f12c bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB */
    fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK,
                                                       FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF,
                                                       value[1]);
}

static INLINE void fc_tttm_lfield_LFOS_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_tttm_lfield_LFOS_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_lfield_LFOS_get", A, 11);
    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0002f128 bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB */
    reg_value = fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK) >> FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF;
    value[0] |= field_value;

    /* (0x0002f12c bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB */
    reg_value = fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK) >> FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x", "fc_tttm_lfield_LFOS_get", A, value[1] , value[0] );

}
static INLINE void fc_tttm_lfield_range_LFOS_set( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_lfield_range_LFOS_set( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_lfield_range_LFOS_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_lfield_range_LFOS_set", stop_bit, start_bit );
    if (stop_bit > 63) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_lfield_range_LFOS_set", stop_bit, 63 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_lfield_range_LFOS_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f128 bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB */
    if (start_bit <= 31) {
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
        /* (0x0002f128 bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB */
        fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF + subfield_offset),
                                                           FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f12c bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0002f12c bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB */
        fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF + subfield_offset),
                                                           FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_lfield_range_LFOS_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_lfield_range_LFOS_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_lfield_range_LFOS_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_lfield_range_LFOS_get", stop_bit, start_bit );
    if (stop_bit > 63) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_lfield_range_LFOS_get", stop_bit, 63 );
    if (start_bit <= 31) {
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
    /* (0x0002f128 bits 31:0) bits 0:31 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB */
        reg_value = fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK)
                      >> FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK, FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0002f12c bits 31:0) bits 32:63 use field LFOS of register PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB */
        reg_value = fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK)
                      >> FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK, FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_lfield_range_LFOS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_lfield_ECB_set( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_tttm_lfield_ECB_set( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value[2] )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_lfield_ECB_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x", "fc_tttm_lfield_ECB_set", A, value[1] , value[0] );

    /* (0x0002f130 bits 31:0) bits 0:31 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB */
    fc_tttm_reg_ERR_CTRL_BLOCK_LSB_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_MSK,
                                              FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_OFF,
                                              value[0]);

    /* (0x0002f134 bits 31:0) bits 32:63 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB */
    fc_tttm_reg_ERR_CTRL_BLOCK_MSB_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_MSK,
                                              FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_OFF,
                                              value[1]);
}

static INLINE void fc_tttm_lfield_ECB_get( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_tttm_lfield_ECB_get( fc_tttm_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_lfield_ECB_get", A, 11);
    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0002f130 bits 31:0) bits 0:31 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB */
    reg_value = fc_tttm_reg_ERR_CTRL_BLOCK_LSB_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_MSK) >> FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_OFF;
    value[0] |= field_value;

    /* (0x0002f134 bits 31:0) bits 32:63 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB */
    reg_value = fc_tttm_reg_ERR_CTRL_BLOCK_MSB_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_MSK) >> FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x", "fc_tttm_lfield_ECB_get", A, value[1] , value[0] );

}
static INLINE void fc_tttm_lfield_range_ECB_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_lfield_range_ECB_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_lfield_range_ECB_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_lfield_range_ECB_set", stop_bit, start_bit );
    if (stop_bit > 63) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_lfield_range_ECB_set", stop_bit, 63 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_lfield_range_ECB_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f130 bits 31:0) bits 0:31 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB */
    if (start_bit <= 31) {
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
        /* (0x0002f130 bits 31:0) bits 0:31 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB */
        fc_tttm_reg_ERR_CTRL_BLOCK_LSB_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_OFF + subfield_offset),
                                                  FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f134 bits 31:0) bits 32:63 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0002f134 bits 31:0) bits 32:63 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB */
        fc_tttm_reg_ERR_CTRL_BLOCK_MSB_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_OFF + subfield_offset),
                                                  FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_lfield_range_ECB_get( fc_tttm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_lfield_range_ECB_get( fc_tttm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_lfield_range_ECB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_lfield_range_ECB_get", stop_bit, start_bit );
    if (stop_bit > 63) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_lfield_range_ECB_get", stop_bit, 63 );
    if (start_bit <= 31) {
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
    /* (0x0002f130 bits 31:0) bits 0:31 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB */
        reg_value = fc_tttm_reg_ERR_CTRL_BLOCK_LSB_read( b_ptr,
                                                         h_ptr,
                                                         A);
        field_value = (reg_value & FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_MSK)
                      >> FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_MSK, FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0002f134 bits 31:0) bits 32:63 use field ECB of register PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB */
        reg_value = fc_tttm_reg_ERR_CTRL_BLOCK_MSB_read( b_ptr,
                                                         h_ptr,
                                                         A);
        field_value = (reg_value & FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_MSK)
                      >> FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_MSK, FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_lfield_range_ECB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_OSIR_NUM_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_OSIR_NUM_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_OSIR_NUM_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_OSIR_NUM_set", A, value );

    /* (0x0002f13c bits 31:0) bits 0:31 use field OSIR_NUM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR */
    fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_MSK,
                                                                FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_OFF,
                                                                value);
}

static INLINE UINT32 fc_tttm_field_OSIR_NUM_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_OSIR_NUM_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_OSIR_NUM_get", A, 11);
    /* (0x0002f13c bits 31:0) bits 0:31 use field OSIR_NUM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR */
    reg_value = fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    value = (reg_value & FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_MSK) >> FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_OSIR_NUM_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_OSIR_NUM_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_OSIR_NUM_set( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_OSIR_NUM_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_OSIR_NUM_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_OSIR_NUM_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_OSIR_NUM_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f13c bits 31:0) bits 0:31 use field OSIR_NUM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR */
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
        /* (0x0002f13c bits 31:0) bits 0:31 use field OSIR_NUM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR */
        fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_field_set( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    subfield_mask << (FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_OFF + subfield_offset),
                                                                    FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_OFF + subfield_offset,
                                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_OSIR_NUM_get( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_OSIR_NUM_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_OSIR_NUM_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_OSIR_NUM_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_OSIR_NUM_get", stop_bit, 31 );
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
    /* (0x0002f13c bits 31:0) bits 0:31 use field OSIR_NUM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR */
    reg_value = fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    field_value = (reg_value & FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_MSK)
                  >> FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_MSK, FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_OSIR_NUM_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_tttm_field_OSIR_DENOM_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_OSIR_DENOM_set( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_OSIR_DENOM_set", A, 11);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_OSIR_DENOM_set", A, value );

    /* (0x0002f140 bits 31:0) bits 0:31 use field OSIR_DENOM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR */
    fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_MSK,
                                                                  FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_OFF,
                                                                  value);
}

static INLINE UINT32 fc_tttm_field_OSIR_DENOM_get( fc_tttm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_OSIR_DENOM_get( fc_tttm_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_OSIR_DENOM_get", A, 11);
    /* (0x0002f140 bits 31:0) bits 0:31 use field OSIR_DENOM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR */
    reg_value = fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_MSK) >> FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_OSIR_DENOM_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_range_OSIR_DENOM_set( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_range_OSIR_DENOM_set( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_OSIR_DENOM_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_OSIR_DENOM_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_OSIR_DENOM_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_OSIR_DENOM_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002f140 bits 31:0) bits 0:31 use field OSIR_DENOM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR */
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
        /* (0x0002f140 bits 31:0) bits 0:31 use field OSIR_DENOM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR */
        fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_field_set( b_ptr,
                                                                      h_ptr,
                                                                      A,
                                                                      subfield_mask << (FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_OFF + subfield_offset),
                                                                      FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_OFF + subfield_offset,
                                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 fc_tttm_field_range_OSIR_DENOM_get( fc_tttm_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_OSIR_DENOM_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_OSIR_DENOM_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_OSIR_DENOM_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_OSIR_DENOM_get", stop_bit, 31 );
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
    /* (0x0002f140 bits 31:0) bits 0:31 use field OSIR_DENOM of register PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR */
    reg_value = fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    field_value = (reg_value & FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_MSK)
                  >> FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_MSK, FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_OSIR_DENOM_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void fc_tttm_field_CNTR_UPDATE_set( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_CNTR_UPDATE_set( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_CNTR_UPDATE_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_CNTR_UPDATE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_CNTR_UPDATE_set", A, value );

    /* (0x0002f14c bits 0) field CNTR_UPDATE of register PMC_FC_TTTM_REG_CNT_UPDATE */
    fc_tttm_reg_CNT_UPDATE_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      FC_TTTM_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK,
                                      FC_TTTM_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF,
                                      value);
}

static INLINE UINT32 fc_tttm_field_CNTR_UPDATE_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_CNTR_UPDATE_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_CNTR_UPDATE_get", A, 11);
    /* (0x0002f14c bits 0) field CNTR_UPDATE of register PMC_FC_TTTM_REG_CNT_UPDATE */
    reg_value = fc_tttm_reg_CNT_UPDATE_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK) >> FC_TTTM_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_CNTR_UPDATE_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void fc_tttm_field_DPI_E_set( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_DPI_E_set( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_DPI_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_DPI_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_DPI_E_set", A, value );

    /* (0x0002f10c bits 5) field DPI_E of register PMC_FC_TTTM_REG_INT_EN */
    fc_tttm_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTM_REG_INT_EN_BIT_DPI_E_MSK,
                                  FC_TTTM_REG_INT_EN_BIT_DPI_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttm_field_DPI_E_get( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_DPI_E_get( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_DPI_E_get", A, 11);
    /* (0x0002f10c bits 5) field DPI_E of register PMC_FC_TTTM_REG_INT_EN */
    reg_value = fc_tttm_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_EN_BIT_DPI_E_MSK) >> FC_TTTM_REG_INT_EN_BIT_DPI_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_DPI_E_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_FOVR_E_set( fc_tttm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_FOVR_E_set( fc_tttm_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_FOVR_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_FOVR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_FOVR_E_set", A, value );

    /* (0x0002f10c bits 4) field FOVR_E of register PMC_FC_TTTM_REG_INT_EN */
    fc_tttm_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTM_REG_INT_EN_BIT_FOVR_E_MSK,
                                  FC_TTTM_REG_INT_EN_BIT_FOVR_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttm_field_FOVR_E_get( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_FOVR_E_get( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_FOVR_E_get", A, 11);
    /* (0x0002f10c bits 4) field FOVR_E of register PMC_FC_TTTM_REG_INT_EN */
    reg_value = fc_tttm_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_EN_BIT_FOVR_E_MSK) >> FC_TTTM_REG_INT_EN_BIT_FOVR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_FOVR_E_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_BADBLK_E_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_BADBLK_E_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_BADBLK_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_BADBLK_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_BADBLK_E_set", A, value );

    /* (0x0002f10c bits 3) field BADBLK_E of register PMC_FC_TTTM_REG_INT_EN */
    fc_tttm_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTM_REG_INT_EN_BIT_BADBLK_E_MSK,
                                  FC_TTTM_REG_INT_EN_BIT_BADBLK_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttm_field_BADBLK_E_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_BADBLK_E_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_BADBLK_E_get", A, 11);
    /* (0x0002f10c bits 3) field BADBLK_E of register PMC_FC_TTTM_REG_INT_EN */
    reg_value = fc_tttm_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_EN_BIT_BADBLK_E_MSK) >> FC_TTTM_REG_INT_EN_BIT_BADBLK_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_BADBLK_E_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_LOSYNC_E_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_LOSYNC_E_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LOSYNC_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_LOSYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_LOSYNC_E_set", A, value );

    /* (0x0002f10c bits 2) field LOSYNC_E of register PMC_FC_TTTM_REG_INT_EN */
    fc_tttm_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTM_REG_INT_EN_BIT_LOSYNC_E_MSK,
                                  FC_TTTM_REG_INT_EN_BIT_LOSYNC_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttm_field_LOSYNC_E_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LOSYNC_E_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LOSYNC_E_get", A, 11);
    /* (0x0002f10c bits 2) field LOSYNC_E of register PMC_FC_TTTM_REG_INT_EN */
    reg_value = fc_tttm_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_EN_BIT_LOSYNC_E_MSK) >> FC_TTTM_REG_INT_EN_BIT_LOSYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LOSYNC_E_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_HIBER_E_set( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_HIBER_E_set( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_HIBER_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_HIBER_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_HIBER_E_set", A, value );

    /* (0x0002f10c bits 1) field HIBER_E of register PMC_FC_TTTM_REG_INT_EN */
    fc_tttm_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTM_REG_INT_EN_BIT_HIBER_E_MSK,
                                  FC_TTTM_REG_INT_EN_BIT_HIBER_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttm_field_HIBER_E_get( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_HIBER_E_get( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_HIBER_E_get", A, 11);
    /* (0x0002f10c bits 1) field HIBER_E of register PMC_FC_TTTM_REG_INT_EN */
    reg_value = fc_tttm_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_EN_BIT_HIBER_E_MSK) >> FC_TTTM_REG_INT_EN_BIT_HIBER_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_HIBER_E_get", A, value );

    return value;
}
static INLINE void fc_tttm_field_LINK_FAIL_E_set( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_LINK_FAIL_E_set( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LINK_FAIL_E_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_LINK_FAIL_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_LINK_FAIL_E_set", A, value );

    /* (0x0002f10c bits 0) field LINK_FAIL_E of register PMC_FC_TTTM_REG_INT_EN */
    fc_tttm_reg_INT_EN_field_set( b_ptr,
                                  h_ptr,
                                  A,
                                  FC_TTTM_REG_INT_EN_BIT_LINK_FAIL_E_MSK,
                                  FC_TTTM_REG_INT_EN_BIT_LINK_FAIL_E_OFF,
                                  value);
}

static INLINE UINT32 fc_tttm_field_LINK_FAIL_E_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LINK_FAIL_E_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LINK_FAIL_E_get", A, 11);
    /* (0x0002f10c bits 0) field LINK_FAIL_E of register PMC_FC_TTTM_REG_INT_EN */
    reg_value = fc_tttm_reg_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_EN_BIT_LINK_FAIL_E_MSK) >> FC_TTTM_REG_INT_EN_BIT_LINK_FAIL_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LINK_FAIL_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void fc_tttm_field_DPI_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_DPI_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_DPI_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_DPI_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_DPI_I_set_to_clear", A, value );

    /* (0x0002f104 bits 5) field DPI_I of register PMC_FC_TTTM_REG_INT */
    fc_tttm_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTM_REG_INT_BIT_DPI_I_MSK,
                                               FC_TTTM_REG_INT_BIT_DPI_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttm_field_DPI_I_get( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_DPI_I_get( fc_tttm_buffer_t *b_ptr,
                                              cbrc_handle_t *h_ptr,
                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_DPI_I_get", A, 11);
    /* (0x0002f104 bits 5) field DPI_I of register PMC_FC_TTTM_REG_INT */
    reg_value = fc_tttm_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_BIT_DPI_I_MSK) >> FC_TTTM_REG_INT_BIT_DPI_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_DPI_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_DPI_I_poll( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_DPI_I_poll( fc_tttm_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_DPI_I_poll", A, value );

    /* (0x0002f104 bits 5) field DPI_I of register PMC_FC_TTTM_REG_INT */
    return fc_tttm_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTM_REG_INT_BIT_DPI_I_MSK,
                                 (value<<FC_TTTM_REG_INT_BIT_DPI_I_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}

static INLINE void fc_tttm_field_FOVR_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_FOVR_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_FOVR_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_FOVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_FOVR_I_set_to_clear", A, value );

    /* (0x0002f104 bits 4) field FOVR_I of register PMC_FC_TTTM_REG_INT */
    fc_tttm_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTM_REG_INT_BIT_FOVR_I_MSK,
                                               FC_TTTM_REG_INT_BIT_FOVR_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttm_field_FOVR_I_get( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_FOVR_I_get( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_FOVR_I_get", A, 11);
    /* (0x0002f104 bits 4) field FOVR_I of register PMC_FC_TTTM_REG_INT */
    reg_value = fc_tttm_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_BIT_FOVR_I_MSK) >> FC_TTTM_REG_INT_BIT_FOVR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_FOVR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_FOVR_I_poll( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_FOVR_I_poll( fc_tttm_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_FOVR_I_poll", A, value );

    /* (0x0002f104 bits 4) field FOVR_I of register PMC_FC_TTTM_REG_INT */
    return fc_tttm_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTM_REG_INT_BIT_FOVR_I_MSK,
                                 (value<<FC_TTTM_REG_INT_BIT_FOVR_I_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}

static INLINE void fc_tttm_field_BADBLK_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_BADBLK_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_BADBLK_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_BADBLK_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_BADBLK_I_set_to_clear", A, value );

    /* (0x0002f104 bits 3) field BADBLK_I of register PMC_FC_TTTM_REG_INT */
    fc_tttm_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTM_REG_INT_BIT_BADBLK_I_MSK,
                                               FC_TTTM_REG_INT_BIT_BADBLK_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttm_field_BADBLK_I_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_BADBLK_I_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_BADBLK_I_get", A, 11);
    /* (0x0002f104 bits 3) field BADBLK_I of register PMC_FC_TTTM_REG_INT */
    reg_value = fc_tttm_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_BIT_BADBLK_I_MSK) >> FC_TTTM_REG_INT_BIT_BADBLK_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_BADBLK_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_BADBLK_I_poll( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_BADBLK_I_poll( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_BADBLK_I_poll", A, value );

    /* (0x0002f104 bits 3) field BADBLK_I of register PMC_FC_TTTM_REG_INT */
    return fc_tttm_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTM_REG_INT_BIT_BADBLK_I_MSK,
                                 (value<<FC_TTTM_REG_INT_BIT_BADBLK_I_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}

static INLINE void fc_tttm_field_LOSYNC_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_LOSYNC_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LOSYNC_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_LOSYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_LOSYNC_I_set_to_clear", A, value );

    /* (0x0002f104 bits 2) field LOSYNC_I of register PMC_FC_TTTM_REG_INT */
    fc_tttm_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTM_REG_INT_BIT_LOSYNC_I_MSK,
                                               FC_TTTM_REG_INT_BIT_LOSYNC_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttm_field_LOSYNC_I_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LOSYNC_I_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LOSYNC_I_get", A, 11);
    /* (0x0002f104 bits 2) field LOSYNC_I of register PMC_FC_TTTM_REG_INT */
    reg_value = fc_tttm_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_BIT_LOSYNC_I_MSK) >> FC_TTTM_REG_INT_BIT_LOSYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LOSYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_LOSYNC_I_poll( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_LOSYNC_I_poll( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_LOSYNC_I_poll", A, value );

    /* (0x0002f104 bits 2) field LOSYNC_I of register PMC_FC_TTTM_REG_INT */
    return fc_tttm_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTM_REG_INT_BIT_LOSYNC_I_MSK,
                                 (value<<FC_TTTM_REG_INT_BIT_LOSYNC_I_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}

static INLINE void fc_tttm_field_HIBER_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_HIBER_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_HIBER_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_HIBER_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_HIBER_I_set_to_clear", A, value );

    /* (0x0002f104 bits 1) field HIBER_I of register PMC_FC_TTTM_REG_INT */
    fc_tttm_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTM_REG_INT_BIT_HIBER_I_MSK,
                                               FC_TTTM_REG_INT_BIT_HIBER_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttm_field_HIBER_I_get( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_HIBER_I_get( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_HIBER_I_get", A, 11);
    /* (0x0002f104 bits 1) field HIBER_I of register PMC_FC_TTTM_REG_INT */
    reg_value = fc_tttm_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_BIT_HIBER_I_MSK) >> FC_TTTM_REG_INT_BIT_HIBER_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_HIBER_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_HIBER_I_poll( fc_tttm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_HIBER_I_poll( fc_tttm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_HIBER_I_poll", A, value );

    /* (0x0002f104 bits 1) field HIBER_I of register PMC_FC_TTTM_REG_INT */
    return fc_tttm_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTM_REG_INT_BIT_HIBER_I_MSK,
                                 (value<<FC_TTTM_REG_INT_BIT_HIBER_I_OFF),
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);
}

static INLINE void fc_tttm_field_LINK_FAIL_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_LINK_FAIL_I_set_to_clear( fc_tttm_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LINK_FAIL_I_set_to_clear", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_LINK_FAIL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_LINK_FAIL_I_set_to_clear", A, value );

    /* (0x0002f104 bits 0) field LINK_FAIL_I of register PMC_FC_TTTM_REG_INT */
    fc_tttm_reg_INT_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               FC_TTTM_REG_INT_BIT_LINK_FAIL_I_MSK,
                                               FC_TTTM_REG_INT_BIT_LINK_FAIL_I_OFF,
                                               value);
}

static INLINE UINT32 fc_tttm_field_LINK_FAIL_I_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LINK_FAIL_I_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LINK_FAIL_I_get", A, 11);
    /* (0x0002f104 bits 0) field LINK_FAIL_I of register PMC_FC_TTTM_REG_INT */
    reg_value = fc_tttm_reg_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_BIT_LINK_FAIL_I_MSK) >> FC_TTTM_REG_INT_BIT_LINK_FAIL_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LINK_FAIL_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_LINK_FAIL_I_poll( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_LINK_FAIL_I_poll( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_LINK_FAIL_I_poll", A, value );

    /* (0x0002f104 bits 0) field LINK_FAIL_I of register PMC_FC_TTTM_REG_INT */
    return fc_tttm_reg_INT_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 FC_TTTM_REG_INT_BIT_LINK_FAIL_I_MSK,
                                 (value<<FC_TTTM_REG_INT_BIT_LINK_FAIL_I_OFF),
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
static INLINE UINT32 fc_tttm_field_LOSYNC_V_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LOSYNC_V_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LOSYNC_V_get", A, 11);
    /* (0x0002f108 bits 2) field LOSYNC_V of register PMC_FC_TTTM_REG_INT_VAL */
    reg_value = fc_tttm_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_VAL_BIT_LOSYNC_V_MSK) >> FC_TTTM_REG_INT_VAL_BIT_LOSYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LOSYNC_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_LOSYNC_V_poll( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_LOSYNC_V_poll( fc_tttm_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_LOSYNC_V_poll", A, value );

    /* (0x0002f108 bits 2) field LOSYNC_V of register PMC_FC_TTTM_REG_INT_VAL */
    return fc_tttm_reg_INT_VAL_poll( b_ptr,
                                     h_ptr,
                                     A,
                                     FC_TTTM_REG_INT_VAL_BIT_LOSYNC_V_MSK,
                                     (value<<FC_TTTM_REG_INT_VAL_BIT_LOSYNC_V_OFF),
                                     cmp,
                                     max_count,
                                     num_failed_polls,
                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_tttm_field_HIBER_V_get( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_HIBER_V_get( fc_tttm_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_HIBER_V_get", A, 11);
    /* (0x0002f108 bits 1) field HIBER_V of register PMC_FC_TTTM_REG_INT_VAL */
    reg_value = fc_tttm_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_VAL_BIT_HIBER_V_MSK) >> FC_TTTM_REG_INT_VAL_BIT_HIBER_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_HIBER_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_HIBER_V_poll( fc_tttm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_HIBER_V_poll( fc_tttm_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_HIBER_V_poll", A, value );

    /* (0x0002f108 bits 1) field HIBER_V of register PMC_FC_TTTM_REG_INT_VAL */
    return fc_tttm_reg_INT_VAL_poll( b_ptr,
                                     h_ptr,
                                     A,
                                     FC_TTTM_REG_INT_VAL_BIT_HIBER_V_MSK,
                                     (value<<FC_TTTM_REG_INT_VAL_BIT_HIBER_V_OFF),
                                     cmp,
                                     max_count,
                                     num_failed_polls,
                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_tttm_field_LINK_FAIL_V_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_LINK_FAIL_V_get( fc_tttm_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_LINK_FAIL_V_get", A, 11);
    /* (0x0002f108 bits 0) field LINK_FAIL_V of register PMC_FC_TTTM_REG_INT_VAL */
    reg_value = fc_tttm_reg_INT_VAL_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_INT_VAL_BIT_LINK_FAIL_V_MSK) >> FC_TTTM_REG_INT_VAL_BIT_LINK_FAIL_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_LINK_FAIL_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_LINK_FAIL_V_poll( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_LINK_FAIL_V_poll( fc_tttm_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_LINK_FAIL_V_poll", A, value );

    /* (0x0002f108 bits 0) field LINK_FAIL_V of register PMC_FC_TTTM_REG_INT_VAL */
    return fc_tttm_reg_INT_VAL_poll( b_ptr,
                                     h_ptr,
                                     A,
                                     FC_TTTM_REG_INT_VAL_BIT_LINK_FAIL_V_MSK,
                                     (value<<FC_TTTM_REG_INT_VAL_BIT_LINK_FAIL_V_OFF),
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
static INLINE UINT32 fc_tttm_field_GFPCNT_get( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_GFPCNT_get( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFPCNT_get", A, 11);
    /* (0x0002f144 bits 23:0) bits 0:23 use field GFPCNT of register PMC_FC_TTTM_REG_GFP_FRAME_CNT */
    reg_value = fc_tttm_reg_GFP_FRAME_CNT_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_GFP_FRAME_CNT_BIT_GFPCNT_MSK) >> FC_TTTM_REG_GFP_FRAME_CNT_BIT_GFPCNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_GFPCNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_tttm_field_range_GFPCNT_get( fc_tttm_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_GFPCNT_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFPCNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFPCNT_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFPCNT_get", stop_bit, 23 );
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
    /* (0x0002f144 bits 23:0) bits 0:23 use field GFPCNT of register PMC_FC_TTTM_REG_GFP_FRAME_CNT */
    reg_value = fc_tttm_reg_GFP_FRAME_CNT_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_TTTM_REG_GFP_FRAME_CNT_BIT_GFPCNT_MSK)
                  >> FC_TTTM_REG_GFP_FRAME_CNT_BIT_GFPCNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_FRAME_CNT_BIT_GFPCNT_MSK, FC_TTTM_REG_GFP_FRAME_CNT_BIT_GFPCNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFPCNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_tttm_field_GFPERRCNT_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_GFPERRCNT_get( fc_tttm_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_GFPERRCNT_get", A, 11);
    /* (0x0002f148 bits 23:0) bits 0:23 use field GFPERRCNT of register PMC_FC_TTTM_REG_GFP_ERRED_FRAMES_CNT */
    reg_value = fc_tttm_reg_GFP_ERRED_FRAMES_CNT_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_BIT_GFPERRCNT_MSK) >> FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_BIT_GFPERRCNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_GFPERRCNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_tttm_field_range_GFPERRCNT_get( fc_tttm_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_range_GFPERRCNT_get( fc_tttm_buffer_t *b_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_range_GFPERRCNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_tttm_field_range_GFPERRCNT_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_tttm_field_range_GFPERRCNT_get", stop_bit, 23 );
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
    /* (0x0002f148 bits 23:0) bits 0:23 use field GFPERRCNT of register PMC_FC_TTTM_REG_GFP_ERRED_FRAMES_CNT */
    reg_value = fc_tttm_reg_GFP_ERRED_FRAMES_CNT_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_BIT_GFPERRCNT_MSK)
                  >> FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_BIT_GFPERRCNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_BIT_GFPERRCNT_MSK, FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_BIT_GFPERRCNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_tttm_field_range_GFPERRCNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_tttm_field_TIP_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_TIP_get( fc_tttm_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_TIP_get", A, 11);
    /* (0x0002f14c bits 31) field TIP of register PMC_FC_TTTM_REG_CNT_UPDATE */
    reg_value = fc_tttm_reg_CNT_UPDATE_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CNT_UPDATE_BIT_TIP_MSK) >> FC_TTTM_REG_CNT_UPDATE_BIT_TIP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_TIP_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_TIP_poll( fc_tttm_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_tttm_field_TIP_poll( fc_tttm_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_tttm_field_TIP_poll", A, value );

    /* (0x0002f14c bits 31) field TIP of register PMC_FC_TTTM_REG_CNT_UPDATE */
    return fc_tttm_reg_CNT_UPDATE_poll( b_ptr,
                                        h_ptr,
                                        A,
                                        FC_TTTM_REG_CNT_UPDATE_BIT_TIP_MSK,
                                        (value<<FC_TTTM_REG_CNT_UPDATE_BIT_TIP_OFF),
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
static INLINE void fc_tttm_field_SW_RESET_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_tttm_field_SW_RESET_set( fc_tttm_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_SW_RESET_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_tttm_field_SW_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_tttm_field_SW_RESET_set", A, value );

    /* (0x0002f100 bits 0) field SW_RESET of register PMC_FC_TTTM_REG_CFG */
    fc_tttm_reg_CFG_field_set( b_ptr,
                               h_ptr,
                               A,
                               FC_TTTM_REG_CFG_BIT_SW_RESET_MSK,
                               FC_TTTM_REG_CFG_BIT_SW_RESET_OFF,
                               value);
}

static INLINE UINT32 fc_tttm_field_SW_RESET_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_tttm_field_SW_RESET_get( fc_tttm_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_tttm_field_SW_RESET_get", A, 11);
    /* (0x0002f100 bits 0) field SW_RESET of register PMC_FC_TTTM_REG_CFG */
    reg_value = fc_tttm_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_TTTM_REG_CFG_BIT_SW_RESET_MSK) >> FC_TTTM_REG_CFG_BIT_SW_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_tttm_field_SW_RESET_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC_TTTM_IO_INLINE_H */
