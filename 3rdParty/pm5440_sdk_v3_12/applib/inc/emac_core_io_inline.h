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
 *     and register accessor functions for the emac_core block
 *****************************************************************************/
#ifndef _EMAC_CORE_IO_INLINE_H
#define _EMAC_CORE_IO_INLINE_H

#include "emac_loc.h"
#include "emac_core_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EMAC_CORE_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for emac_core
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
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} emac_core_buffer_t;
static INLINE void emac_core_buffer_init( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void emac_core_buffer_init( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "emac_core_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void emac_core_buffer_flush( emac_core_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void emac_core_buffer_flush( emac_core_buffer_t *b_ptr )
{
    IOLOG( "emac_core_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 emac_core_reg_read( emac_core_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_read( emac_core_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
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
static INLINE void emac_core_reg_write( emac_core_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_write( emac_core_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
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

static INLINE void emac_core_field_set( emac_core_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_set( emac_core_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
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

static INLINE void emac_core_action_on_write_field_set( emac_core_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_action_on_write_field_set( emac_core_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
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

static INLINE void emac_core_burst_read( emac_core_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_core_burst_read( emac_core_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
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

static INLINE void emac_core_burst_write( emac_core_buffer_t *b_ptr,
                                          emac_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_core_burst_write( emac_core_buffer_t *b_ptr,
                                          emac_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE emac_core_poll( emac_core_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 value,
                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                   UINT32 max_count,
                                                   UINT32 *num_failed_polls,
                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_core_poll( emac_core_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
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
 *  register access functions for emac_core
 * ==================================================================================
 */

static INLINE void emac_core_reg_SEG_ACTIVATION_CTRL_write( emac_core_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_SEG_ACTIVATION_CTRL_write( emac_core_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_core_reg_SEG_ACTIVATION_CTRL_write", value );
    emac_core_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_SEG_ACTIVATION_CTRL,
                         value);
}

static INLINE void emac_core_reg_SEG_ACTIVATION_CTRL_field_set( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_SEG_ACTIVATION_CTRL_field_set( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_core_reg_SEG_ACTIVATION_CTRL_field_set", mask, ofs, value );
    emac_core_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_SEG_ACTIVATION_CTRL,
                         mask,
                         PMC_CORE_REG_SEG_ACTIVATION_CTRL_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_core_reg_SEG_ACTIVATION_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_SEG_ACTIVATION_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_CORE_REG_SEG_ACTIVATION_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_SEG_ACTIVATION_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_core_reg_SEG_MODE_CTRL_write( emac_core_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_SEG_MODE_CTRL_write( emac_core_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_core_reg_SEG_MODE_CTRL_write", value );
    emac_core_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_SEG_MODE_CTRL,
                         value);
}

static INLINE void emac_core_reg_SEG_MODE_CTRL_field_set( emac_core_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_SEG_MODE_CTRL_field_set( emac_core_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_core_reg_SEG_MODE_CTRL_field_set", mask, ofs, value );
    emac_core_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_SEG_MODE_CTRL,
                         mask,
                         PMC_CORE_REG_SEG_MODE_CTRL_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_core_reg_SEG_MODE_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_SEG_MODE_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_CORE_REG_SEG_MODE_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_SEG_MODE_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_core_reg_USER_DEFINED_TX_PREAMBLE_write( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_USER_DEFINED_TX_PREAMBLE_write( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_core_reg_USER_DEFINED_TX_PREAMBLE_write", value );
    emac_core_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE,
                         value);
}

static INLINE void emac_core_reg_USER_DEFINED_TX_PREAMBLE_field_set( emac_core_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_USER_DEFINED_TX_PREAMBLE_field_set( emac_core_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_core_reg_USER_DEFINED_TX_PREAMBLE_field_set", mask, ofs, value );
    emac_core_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE,
                         mask,
                         PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_core_reg_USER_DEFINED_TX_PREAMBLE_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_USER_DEFINED_TX_PREAMBLE_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_USER_DEFINED_TX_PREAMBLE_read", reg_value);
    return reg_value;
}

static INLINE void emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_write( emac_core_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_write( emac_core_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_write", value );
    emac_core_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL,
                         value);
}

static INLINE void emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_field_set( emac_core_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_field_set( emac_core_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_field_set", mask, ofs, value );
    emac_core_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL,
                         mask,
                         PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__write( emac_core_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__write( emac_core_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__write", value );
    emac_core_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_,
                         value);
}

static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( emac_core_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( emac_core_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set", mask, ofs, value );
    emac_core_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_,
                         mask,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read", reg_value);
    return reg_value;
}

static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__write( emac_core_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__write( emac_core_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__write", value );
    emac_core_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_,
                         value);
}

static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( emac_core_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( emac_core_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set", mask, ofs, value );
    emac_core_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_,
                         mask,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read", reg_value);
    return reg_value;
}

static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__write( emac_core_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__write( emac_core_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__write", value );
    emac_core_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_,
                         value);
}

static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( emac_core_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( emac_core_buffer_t *b_ptr,
                                                                           emac_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set", mask, ofs, value );
    emac_core_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_,
                         mask,
                         PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read", reg_value);
    return reg_value;
}

static INLINE void emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_write( emac_core_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_write( emac_core_buffer_t *b_ptr,
                                                                          emac_handle_t *h_ptr,
                                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_write", value );
    emac_core_reg_write( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL,
                         value);
}

static INLINE void emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_field_set( emac_core_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_field_set( emac_core_buffer_t *b_ptr,
                                                                              emac_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_field_set", mask, ofs, value );
    emac_core_field_set( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL,
                         mask,
                         PMC_CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_CONFIG,
                                    PMC_CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_read", reg_value);
    return reg_value;
}

static INLINE UINT32 emac_core_reg_TX_CLK_SELECTION_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_TX_CLK_SELECTION_CTRL_read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_TX_CLK_SELECTION_CTRL);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_TX_CLK_SELECTION_CTRL_read", reg_value);
    return reg_value;
}

static INLINE UINT32 emac_core_reg_TDM_CREDIT_STAT_1_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_TDM_CREDIT_STAT_1_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_TDM_CREDIT_STAT_1_3_);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_TDM_CREDIT_STAT_1_3__read", reg_value);
    return reg_value;
}

static INLINE UINT32 emac_core_reg_TDM_CREDIT_STAT_2_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_TDM_CREDIT_STAT_2_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_TDM_CREDIT_STAT_2_3_);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_TDM_CREDIT_STAT_2_3__read", reg_value);
    return reg_value;
}

static INLINE UINT32 emac_core_reg_TDM_CREDIT_STAT_3_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_reg_TDM_CREDIT_STAT_3_3__read( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = emac_core_reg_read( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_TDM_CREDIT_STAT_3_3_);

    IOLOG( "%s -> 0x%08x;", "emac_core_reg_TDM_CREDIT_STAT_3_3__read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void emac_core_field_SEG_CTL_set( emac_core_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG_CTL_set( emac_core_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG_CTL_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG_CTL_set", value );

    /* (0x0000b000 bits 11:0) bits 0:11 use field SEG_CTL of register PMC_CORE_REG_SEG_ACTIVATION_CTRL */
    emac_core_reg_SEG_ACTIVATION_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_MSK,
                                                 CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_OFF,
                                                 value);
}

static INLINE UINT32 emac_core_field_SEG_CTL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG_CTL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b000 bits 11:0) bits 0:11 use field SEG_CTL of register PMC_CORE_REG_SEG_ACTIVATION_CTRL */
    reg_value = emac_core_reg_SEG_ACTIVATION_CTRL_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_MSK) >> CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG_CTL_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG_CTL_set( emac_core_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG_CTL_set( emac_core_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG_CTL_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG_CTL_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG_CTL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b000 bits 11:0) bits 0:11 use field SEG_CTL of register PMC_CORE_REG_SEG_ACTIVATION_CTRL */
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
        /* (0x0000b000 bits 11:0) bits 0:11 use field SEG_CTL of register PMC_CORE_REG_SEG_ACTIVATION_CTRL */
        emac_core_reg_SEG_ACTIVATION_CTRL_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_OFF + subfield_offset),
                                                     CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG_CTL_get( emac_core_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG_CTL_get( emac_core_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG_CTL_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG_CTL_get", stop_bit, 11 );
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
    /* (0x0000b000 bits 11:0) bits 0:11 use field SEG_CTL of register PMC_CORE_REG_SEG_ACTIVATION_CTRL */
    reg_value = emac_core_reg_SEG_ACTIVATION_CTRL_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_MSK)
                  >> CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_MSK, CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG_CTL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_MODE_CTL_set( emac_core_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_MODE_CTL_set( emac_core_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_MODE_CTL_set", value, 16777215);
    IOLOG( "%s <= 0x%08x", "emac_core_field_MODE_CTL_set", value );

    /* (0x0000b004 bits 23:0) bits 0:23 use field MODE_CTL of register PMC_CORE_REG_SEG_MODE_CTRL */
    emac_core_reg_SEG_MODE_CTRL_field_set( b_ptr,
                                           h_ptr,
                                           CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_MSK,
                                           CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_OFF,
                                           value);
}

static INLINE UINT32 emac_core_field_MODE_CTL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_MODE_CTL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b004 bits 23:0) bits 0:23 use field MODE_CTL of register PMC_CORE_REG_SEG_MODE_CTRL */
    reg_value = emac_core_reg_SEG_MODE_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_MSK) >> CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_MODE_CTL_get", value );

    return value;
}
static INLINE void emac_core_field_range_MODE_CTL_set( emac_core_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_MODE_CTL_set( emac_core_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_MODE_CTL_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_MODE_CTL_set", stop_bit, 23 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_MODE_CTL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b004 bits 23:0) bits 0:23 use field MODE_CTL of register PMC_CORE_REG_SEG_MODE_CTRL */
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
        /* (0x0000b004 bits 23:0) bits 0:23 use field MODE_CTL of register PMC_CORE_REG_SEG_MODE_CTRL */
        emac_core_reg_SEG_MODE_CTRL_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_OFF + subfield_offset),
                                               CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_MODE_CTL_get( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_MODE_CTL_get( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_MODE_CTL_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_MODE_CTL_get", stop_bit, 23 );
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
    /* (0x0000b004 bits 23:0) bits 0:23 use field MODE_CTL of register PMC_CORE_REG_SEG_MODE_CTRL */
    reg_value = emac_core_reg_SEG_MODE_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_MSK)
                  >> CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_MSK, CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_MODE_CTL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_TX_PRMBL_CTL_set( emac_core_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_TX_PRMBL_CTL_set( emac_core_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_TX_PRMBL_CTL_set", value, 16777215);
    IOLOG( "%s <= 0x%08x", "emac_core_field_TX_PRMBL_CTL_set", value );

    /* (0x0000b00c bits 23:0) bits 0:23 use field TX_PRMBL_CTL of register PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE */
    emac_core_reg_USER_DEFINED_TX_PREAMBLE_field_set( b_ptr,
                                                      h_ptr,
                                                      CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_MSK,
                                                      CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_OFF,
                                                      value);
}

static INLINE UINT32 emac_core_field_TX_PRMBL_CTL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_TX_PRMBL_CTL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b00c bits 23:0) bits 0:23 use field TX_PRMBL_CTL of register PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE */
    reg_value = emac_core_reg_USER_DEFINED_TX_PREAMBLE_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_MSK) >> CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_TX_PRMBL_CTL_get", value );

    return value;
}
static INLINE void emac_core_field_range_TX_PRMBL_CTL_set( emac_core_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_TX_PRMBL_CTL_set( emac_core_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_TX_PRMBL_CTL_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_TX_PRMBL_CTL_set", stop_bit, 23 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_TX_PRMBL_CTL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b00c bits 23:0) bits 0:23 use field TX_PRMBL_CTL of register PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE */
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
        /* (0x0000b00c bits 23:0) bits 0:23 use field TX_PRMBL_CTL of register PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE */
        emac_core_reg_USER_DEFINED_TX_PREAMBLE_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_OFF + subfield_offset),
                                                          CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_TX_PRMBL_CTL_get( emac_core_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_TX_PRMBL_CTL_get( emac_core_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_TX_PRMBL_CTL_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_TX_PRMBL_CTL_get", stop_bit, 23 );
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
    /* (0x0000b00c bits 23:0) bits 0:23 use field TX_PRMBL_CTL of register PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE */
    reg_value = emac_core_reg_USER_DEFINED_TX_PREAMBLE_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_MSK)
                  >> CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_MSK, CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_TX_PRMBL_CTL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_WAN_RS_COL_CNT_set( emac_core_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_WAN_RS_COL_CNT_set( emac_core_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_WAN_RS_COL_CNT_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "emac_core_field_WAN_RS_COL_CNT_set", value );

    /* (0x0000b020 bits 15:0) bits 0:15 use field WAN_RS_COL_CNT of register PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL */
    emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_MSK,
                                                            CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_WAN_RS_COL_CNT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_WAN_RS_COL_CNT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b020 bits 15:0) bits 0:15 use field WAN_RS_COL_CNT of register PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL */
    reg_value = emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_MSK) >> CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_WAN_RS_COL_CNT_get", value );

    return value;
}
static INLINE void emac_core_field_range_WAN_RS_COL_CNT_set( emac_core_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_WAN_RS_COL_CNT_set( emac_core_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_WAN_RS_COL_CNT_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_WAN_RS_COL_CNT_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_WAN_RS_COL_CNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b020 bits 15:0) bits 0:15 use field WAN_RS_COL_CNT of register PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL */
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
        /* (0x0000b020 bits 15:0) bits 0:15 use field WAN_RS_COL_CNT of register PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL */
        emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_OFF + subfield_offset),
                                                                CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_WAN_RS_COL_CNT_get( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_WAN_RS_COL_CNT_get( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_WAN_RS_COL_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_WAN_RS_COL_CNT_get", stop_bit, 15 );
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
    /* (0x0000b020 bits 15:0) bits 0:15 use field WAN_RS_COL_CNT of register PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL */
    reg_value = emac_core_reg_BASE_R_FAULT_STATEMACHINE_CTRL_read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_MSK)
                  >> CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_MSK, CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_WAN_RS_COL_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG3_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG3_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG3_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG3_INIT_CREDIT_set", value );

    /* (0x0000b024 bits 31:24) bits 0:7 use field SEG3_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG3_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG3_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b024 bits 31:24) bits 0:7 use field SEG3_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG3_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG3_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG3_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG3_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG3_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG3_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b024 bits 31:24) bits 0:7 use field SEG3_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
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
        /* (0x0000b024 bits 31:24) bits 0:7 use field SEG3_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG3_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG3_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG3_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG3_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b024 bits 31:24) bits 0:7 use field SEG3_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG3_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG2_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG2_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG2_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG2_INIT_CREDIT_set", value );

    /* (0x0000b024 bits 23:16) bits 0:7 use field SEG2_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG2_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG2_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b024 bits 23:16) bits 0:7 use field SEG2_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG2_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG2_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG2_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG2_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG2_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG2_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b024 bits 23:16) bits 0:7 use field SEG2_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
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
        /* (0x0000b024 bits 23:16) bits 0:7 use field SEG2_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG2_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG2_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG2_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG2_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b024 bits 23:16) bits 0:7 use field SEG2_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG2_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG1_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG1_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG1_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG1_INIT_CREDIT_set", value );

    /* (0x0000b024 bits 15:8) bits 0:7 use field SEG1_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG1_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG1_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b024 bits 15:8) bits 0:7 use field SEG1_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG1_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG1_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG1_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG1_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG1_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG1_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b024 bits 15:8) bits 0:7 use field SEG1_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
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
        /* (0x0000b024 bits 15:8) bits 0:7 use field SEG1_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG1_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG1_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG1_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG1_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b024 bits 15:8) bits 0:7 use field SEG1_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG1_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG0_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG0_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG0_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG0_INIT_CREDIT_set", value );

    /* (0x0000b024 bits 7:0) bits 0:7 use field SEG0_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG0_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG0_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b024 bits 7:0) bits 0:7 use field SEG0_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG0_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG0_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG0_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG0_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG0_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG0_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b024 bits 7:0) bits 0:7 use field SEG0_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
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
        /* (0x0000b024 bits 7:0) bits 0:7 use field SEG0_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG0_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG0_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG0_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG0_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b024 bits 7:0) bits 0:7 use field SEG0_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_1_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG0_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG7_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG7_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG7_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG7_INIT_CREDIT_set", value );

    /* (0x0000b028 bits 31:24) bits 0:7 use field SEG7_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG7_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG7_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b028 bits 31:24) bits 0:7 use field SEG7_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG7_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG7_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG7_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG7_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG7_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG7_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b028 bits 31:24) bits 0:7 use field SEG7_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
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
        /* (0x0000b028 bits 31:24) bits 0:7 use field SEG7_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG7_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG7_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG7_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG7_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b028 bits 31:24) bits 0:7 use field SEG7_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG7_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG6_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG6_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG6_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG6_INIT_CREDIT_set", value );

    /* (0x0000b028 bits 23:16) bits 0:7 use field SEG6_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG6_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG6_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b028 bits 23:16) bits 0:7 use field SEG6_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG6_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG6_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG6_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG6_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG6_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG6_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b028 bits 23:16) bits 0:7 use field SEG6_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
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
        /* (0x0000b028 bits 23:16) bits 0:7 use field SEG6_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG6_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG6_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG6_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG6_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b028 bits 23:16) bits 0:7 use field SEG6_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG6_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG5_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG5_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG5_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG5_INIT_CREDIT_set", value );

    /* (0x0000b028 bits 15:8) bits 0:7 use field SEG5_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG5_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG5_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b028 bits 15:8) bits 0:7 use field SEG5_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG5_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG5_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG5_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG5_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG5_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG5_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b028 bits 15:8) bits 0:7 use field SEG5_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
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
        /* (0x0000b028 bits 15:8) bits 0:7 use field SEG5_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG5_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG5_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG5_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG5_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b028 bits 15:8) bits 0:7 use field SEG5_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG5_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG4_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG4_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG4_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG4_INIT_CREDIT_set", value );

    /* (0x0000b028 bits 7:0) bits 0:7 use field SEG4_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG4_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG4_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b028 bits 7:0) bits 0:7 use field SEG4_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG4_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG4_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG4_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG4_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG4_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG4_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b028 bits 7:0) bits 0:7 use field SEG4_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
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
        /* (0x0000b028 bits 7:0) bits 0:7 use field SEG4_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG4_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG4_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG4_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG4_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b028 bits 7:0) bits 0:7 use field SEG4_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_2_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG4_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG11_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG11_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG11_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG11_INIT_CREDIT_set", value );

    /* (0x0000b02c bits 31:24) bits 0:7 use field SEG11_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG11_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG11_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b02c bits 31:24) bits 0:7 use field SEG11_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG11_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG11_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG11_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG11_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG11_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG11_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b02c bits 31:24) bits 0:7 use field SEG11_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
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
        /* (0x0000b02c bits 31:24) bits 0:7 use field SEG11_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG11_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG11_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG11_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG11_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b02c bits 31:24) bits 0:7 use field SEG11_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG11_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG10_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG10_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG10_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG10_INIT_CREDIT_set", value );

    /* (0x0000b02c bits 23:16) bits 0:7 use field SEG10_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG10_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG10_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b02c bits 23:16) bits 0:7 use field SEG10_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG10_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG10_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG10_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG10_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG10_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG10_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b02c bits 23:16) bits 0:7 use field SEG10_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
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
        /* (0x0000b02c bits 23:16) bits 0:7 use field SEG10_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG10_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG10_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                  emac_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG10_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG10_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b02c bits 23:16) bits 0:7 use field SEG10_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG10_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG9_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG9_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG9_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG9_INIT_CREDIT_set", value );

    /* (0x0000b02c bits 15:8) bits 0:7 use field SEG9_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG9_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG9_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b02c bits 15:8) bits 0:7 use field SEG9_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG9_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG9_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG9_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG9_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG9_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG9_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b02c bits 15:8) bits 0:7 use field SEG9_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
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
        /* (0x0000b02c bits 15:8) bits 0:7 use field SEG9_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG9_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG9_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG9_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG9_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b02c bits 15:8) bits 0:7 use field SEG9_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG9_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_SEG8_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_SEG8_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_SEG8_INIT_CREDIT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "emac_core_field_SEG8_INIT_CREDIT_set", value );

    /* (0x0000b02c bits 7:0) bits 0:7 use field SEG8_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_MSK,
                                                            CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_OFF,
                                                            value);
}

static INLINE UINT32 emac_core_field_SEG8_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG8_INIT_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b02c bits 7:0) bits 0:7 use field SEG8_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG8_INIT_CREDIT_get", value );

    return value;
}
static INLINE void emac_core_field_range_SEG8_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_range_SEG8_INIT_CREDIT_set( emac_core_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG8_INIT_CREDIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG8_INIT_CREDIT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG8_INIT_CREDIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000b02c bits 7:0) bits 0:7 use field SEG8_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
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
        /* (0x0000b02c bits 7:0) bits 0:7 use field SEG8_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
        emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_OFF + subfield_offset),
                                                                CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 emac_core_field_range_SEG8_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG8_INIT_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG8_INIT_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG8_INIT_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b02c bits 7:0) bits 0:7 use field SEG8_INIT_CREDIT of register PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_INITIALIZATION_3_3__read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_MSK, CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG8_INIT_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_core_field_VL_INTVL_set( emac_core_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_core_field_VL_INTVL_set( emac_core_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_core_field_VL_INTVL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "emac_core_field_VL_INTVL_set", value );

    /* (0x0000b040 bits 0) field VL_INTVL of register PMC_CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL */
    emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_field_set( b_ptr,
                                                               h_ptr,
                                                               CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_BIT_VL_INTVL_MSK,
                                                               CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_BIT_VL_INTVL_OFF,
                                                               value);
}

static INLINE UINT32 emac_core_field_VL_INTVL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_VL_INTVL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b040 bits 0) field VL_INTVL of register PMC_CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL */
    reg_value = emac_core_reg_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_read( b_ptr,
                                                                      h_ptr);
    value = (reg_value & CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_BIT_VL_INTVL_MSK) >> CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_BIT_VL_INTVL_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_VL_INTVL_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 emac_core_field_TXCLK_CTL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_TXCLK_CTL_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b008 bits 23:0) bits 0:23 use field TXCLK_CTL of register PMC_CORE_REG_TX_CLK_SELECTION_CTRL */
    reg_value = emac_core_reg_TX_CLK_SELECTION_CTRL_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & CORE_REG_TX_CLK_SELECTION_CTRL_BIT_TXCLK_CTL_MSK) >> CORE_REG_TX_CLK_SELECTION_CTRL_BIT_TXCLK_CTL_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_TXCLK_CTL_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_TXCLK_CTL_get( emac_core_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_TXCLK_CTL_get( emac_core_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_TXCLK_CTL_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_TXCLK_CTL_get", stop_bit, 23 );
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
    /* (0x0000b008 bits 23:0) bits 0:23 use field TXCLK_CTL of register PMC_CORE_REG_TX_CLK_SELECTION_CTRL */
    reg_value = emac_core_reg_TX_CLK_SELECTION_CTRL_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & CORE_REG_TX_CLK_SELECTION_CTRL_BIT_TXCLK_CTL_MSK)
                  >> CORE_REG_TX_CLK_SELECTION_CTRL_BIT_TXCLK_CTL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TX_CLK_SELECTION_CTRL_BIT_TXCLK_CTL_MSK, CORE_REG_TX_CLK_SELECTION_CTRL_BIT_TXCLK_CTL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_TXCLK_CTL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG3_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG3_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b030 bits 31:24) bits 0:7 use field SEG3_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_1_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG3_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG3_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG3_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG3_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG3_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG3_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG3_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b030 bits 31:24) bits 0:7 use field SEG3_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_1_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG3_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG3_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG3_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG3_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG3_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG2_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG2_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b030 bits 23:16) bits 0:7 use field SEG2_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_1_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG2_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG2_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG2_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG2_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG2_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG2_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG2_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b030 bits 23:16) bits 0:7 use field SEG2_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_1_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG2_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG2_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG2_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG2_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG2_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG1_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG1_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b030 bits 15:8) bits 0:7 use field SEG1_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_1_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG1_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG1_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG1_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG1_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG1_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG1_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG1_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b030 bits 15:8) bits 0:7 use field SEG1_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_1_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG1_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG1_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG1_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG1_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG1_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG0_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG0_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b030 bits 7:0) bits 0:7 use field SEG0_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_1_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG0_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG0_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG0_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG0_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG0_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG0_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG0_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b030 bits 7:0) bits 0:7 use field SEG0_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_1_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_1_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG0_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG0_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG0_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG0_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG0_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG7_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG7_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b034 bits 31:24) bits 0:7 use field SEG7_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_2_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG7_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG7_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG7_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG7_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG7_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG7_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG7_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b034 bits 31:24) bits 0:7 use field SEG7_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_2_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG7_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG7_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG7_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG7_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG7_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG6_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG6_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b034 bits 23:16) bits 0:7 use field SEG6_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_2_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG6_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG6_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG6_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG6_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG6_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG6_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG6_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b034 bits 23:16) bits 0:7 use field SEG6_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_2_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG6_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG6_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG6_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG6_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG6_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG5_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG5_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b034 bits 15:8) bits 0:7 use field SEG5_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_2_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG5_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG5_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG5_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG5_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG5_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG5_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG5_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b034 bits 15:8) bits 0:7 use field SEG5_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_2_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG5_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG5_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG5_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG5_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG5_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG4_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG4_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b034 bits 7:0) bits 0:7 use field SEG4_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_2_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG4_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG4_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG4_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG4_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG4_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG4_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG4_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b034 bits 7:0) bits 0:7 use field SEG4_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_2_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_2_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG4_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG4_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG4_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG4_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG4_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG11_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG11_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b038 bits 31:24) bits 0:7 use field SEG11_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_3_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG11_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG11_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG11_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG11_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG11_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG11_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG11_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b038 bits 31:24) bits 0:7 use field SEG11_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_3_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG11_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG11_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG11_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG11_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG11_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG10_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG10_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b038 bits 23:16) bits 0:7 use field SEG10_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_3_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG10_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG10_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG10_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG10_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG10_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG10_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG10_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b038 bits 23:16) bits 0:7 use field SEG10_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_3_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG10_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG10_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG10_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG10_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG10_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG9_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG9_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b038 bits 15:8) bits 0:7 use field SEG9_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_3_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG9_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG9_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG9_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG9_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG9_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG9_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG9_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b038 bits 15:8) bits 0:7 use field SEG9_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_3_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG9_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG9_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG9_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG9_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG9_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_core_field_SEG8_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_SEG8_REG_CREDIT_get( emac_core_buffer_t *b_ptr, emac_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000b038 bits 7:0) bits 0:7 use field SEG8_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_3_3__read( b_ptr,
                                                         h_ptr);
    value = (reg_value & CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG8_REG_CREDIT_MSK) >> CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG8_REG_CREDIT_OFF;
    IOLOG( "%s -> 0x%08x", "emac_core_field_SEG8_REG_CREDIT_get", value );

    return value;
}
static INLINE UINT32 emac_core_field_range_SEG8_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_core_field_range_SEG8_REG_CREDIT_get( emac_core_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_core_field_range_SEG8_REG_CREDIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_core_field_range_SEG8_REG_CREDIT_get", stop_bit, 7 );
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
    /* (0x0000b038 bits 7:0) bits 0:7 use field SEG8_REG_CREDIT of register PMC_CORE_REG_TDM_CREDIT_STAT_3_3_ */
    reg_value = emac_core_reg_TDM_CREDIT_STAT_3_3__read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG8_REG_CREDIT_MSK)
                  >> CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG8_REG_CREDIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG8_REG_CREDIT_MSK, CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG8_REG_CREDIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "emac_core_field_range_SEG8_REG_CREDIT_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_CORE_IO_INLINE_H */
