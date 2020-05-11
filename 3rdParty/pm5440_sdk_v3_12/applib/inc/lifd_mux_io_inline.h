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
 *     and register accessor functions for the lifd_mux block
 *****************************************************************************/
#ifndef _LIFD_MUX_IO_INLINE_H
#define _LIFD_MUX_IO_INLINE_H

#include "lifd_api.h"
#include "lifd_mux_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define LIFD_MUX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for lifd_mux
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
    lifd_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} lifd_mux_buffer_t;
static INLINE void lifd_mux_buffer_init( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void lifd_mux_buffer_init( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "lifd_mux_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void lifd_mux_buffer_flush( lifd_mux_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void lifd_mux_buffer_flush( lifd_mux_buffer_t *b_ptr )
{
    IOLOG( "lifd_mux_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 lifd_mux_reg_read( lifd_mux_buffer_t *b_ptr,
                                        lifd_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_reg_read( lifd_mux_buffer_t *b_ptr,
                                        lifd_handle_t *h_ptr,
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
static INLINE void lifd_mux_reg_write( lifd_mux_buffer_t *b_ptr,
                                       lifd_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_write( lifd_mux_buffer_t *b_ptr,
                                       lifd_handle_t *h_ptr,
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

static INLINE void lifd_mux_field_set( lifd_mux_buffer_t *b_ptr,
                                       lifd_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_set( lifd_mux_buffer_t *b_ptr,
                                       lifd_handle_t *h_ptr,
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

static INLINE void lifd_mux_action_on_write_field_set( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_action_on_write_field_set( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
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

static INLINE void lifd_mux_burst_read( lifd_mux_buffer_t *b_ptr,
                                        lifd_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void lifd_mux_burst_read( lifd_mux_buffer_t *b_ptr,
                                        lifd_handle_t *h_ptr,
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

static INLINE void lifd_mux_burst_write( lifd_mux_buffer_t *b_ptr,
                                         lifd_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void lifd_mux_burst_write( lifd_mux_buffer_t *b_ptr,
                                         lifd_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE lifd_mux_poll( lifd_mux_buffer_t *b_ptr,
                                                  lifd_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE lifd_mux_poll( lifd_mux_buffer_t *b_ptr,
                                                  lifd_handle_t *h_ptr,
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
 *  register access functions for lifd_mux
 * ==================================================================================
 */

static INLINE void lifd_mux_reg_SFI_IF_write( lifd_mux_buffer_t *b_ptr,
                                              lifd_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_SFI_IF_write( lifd_mux_buffer_t *b_ptr,
                                              lifd_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lifd_mux_reg_SFI_IF_write", value );
    lifd_mux_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_SFI_IF,
                        value);
}

static INLINE void lifd_mux_reg_SFI_IF_field_set( lifd_mux_buffer_t *b_ptr,
                                                  lifd_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_SFI_IF_field_set( lifd_mux_buffer_t *b_ptr,
                                                  lifd_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lifd_mux_reg_SFI_IF_field_set", mask, ofs, value );
    lifd_mux_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_SFI_IF,
                        mask,
                        PMC_LIFD_MUX_REG_SFI_IF_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 lifd_mux_reg_SFI_IF_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_reg_SFI_IF_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lifd_mux_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_LIFD_MUX_REG_SFI_IF);

    IOLOG( "%s -> 0x%08x;", "lifd_mux_reg_SFI_IF_read", reg_value);
    return reg_value;
}

static INLINE void lifd_mux_reg_SFI_C_CKCTL_SS_SEL_write( lifd_mux_buffer_t *b_ptr,
                                                          lifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_SFI_C_CKCTL_SS_SEL_write( lifd_mux_buffer_t *b_ptr,
                                                          lifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lifd_mux_reg_SFI_C_CKCTL_SS_SEL_write", value );
    lifd_mux_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL,
                        value);
}

static INLINE void lifd_mux_reg_SFI_C_CKCTL_SS_SEL_field_set( lifd_mux_buffer_t *b_ptr,
                                                              lifd_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_SFI_C_CKCTL_SS_SEL_field_set( lifd_mux_buffer_t *b_ptr,
                                                              lifd_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lifd_mux_reg_SFI_C_CKCTL_SS_SEL_field_set", mask, ofs, value );
    lifd_mux_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL,
                        mask,
                        PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 lifd_mux_reg_SFI_C_CKCTL_SS_SEL_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_reg_SFI_C_CKCTL_SS_SEL_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lifd_mux_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL);

    IOLOG( "%s -> 0x%08x;", "lifd_mux_reg_SFI_C_CKCTL_SS_SEL_read", reg_value);
    return reg_value;
}

static INLINE void lifd_mux_reg_PIN_LOS_0_19_INVERT_write( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_PIN_LOS_0_19_INVERT_write( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lifd_mux_reg_PIN_LOS_0_19_INVERT_write", value );
    lifd_mux_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT,
                        value);
}

static INLINE void lifd_mux_reg_PIN_LOS_0_19_INVERT_field_set( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_PIN_LOS_0_19_INVERT_field_set( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lifd_mux_reg_PIN_LOS_0_19_INVERT_field_set", mask, ofs, value );
    lifd_mux_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT,
                        mask,
                        PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 lifd_mux_reg_PIN_LOS_0_19_INVERT_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_reg_PIN_LOS_0_19_INVERT_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lifd_mux_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT);

    IOLOG( "%s -> 0x%08x;", "lifd_mux_reg_PIN_LOS_0_19_INVERT_read", reg_value);
    return reg_value;
}

static INLINE void lifd_mux_reg_PIN_LOS_0_19_EN_write( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_PIN_LOS_0_19_EN_write( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lifd_mux_reg_PIN_LOS_0_19_EN_write", value );
    lifd_mux_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN,
                        value);
}

static INLINE void lifd_mux_reg_PIN_LOS_0_19_EN_field_set( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_PIN_LOS_0_19_EN_field_set( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lifd_mux_reg_PIN_LOS_0_19_EN_field_set", mask, ofs, value );
    lifd_mux_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN,
                        mask,
                        PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 lifd_mux_reg_PIN_LOS_0_19_EN_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_reg_PIN_LOS_0_19_EN_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lifd_mux_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN);

    IOLOG( "%s -> 0x%08x;", "lifd_mux_reg_PIN_LOS_0_19_EN_read", reg_value);
    return reg_value;
}

static INLINE void lifd_mux_reg_SERDES_0_19_DLOS_INVERT_write( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_SERDES_0_19_DLOS_INVERT_write( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lifd_mux_reg_SERDES_0_19_DLOS_INVERT_write", value );
    lifd_mux_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT,
                        value);
}

static INLINE void lifd_mux_reg_SERDES_0_19_DLOS_INVERT_field_set( lifd_mux_buffer_t *b_ptr,
                                                                   lifd_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_SERDES_0_19_DLOS_INVERT_field_set( lifd_mux_buffer_t *b_ptr,
                                                                   lifd_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lifd_mux_reg_SERDES_0_19_DLOS_INVERT_field_set", mask, ofs, value );
    lifd_mux_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT,
                        mask,
                        PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 lifd_mux_reg_SERDES_0_19_DLOS_INVERT_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_reg_SERDES_0_19_DLOS_INVERT_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lifd_mux_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT);

    IOLOG( "%s -> 0x%08x;", "lifd_mux_reg_SERDES_0_19_DLOS_INVERT_read", reg_value);
    return reg_value;
}

static INLINE void lifd_mux_reg_SERDES_0_19_DLOS_EN_write( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_SERDES_0_19_DLOS_EN_write( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lifd_mux_reg_SERDES_0_19_DLOS_EN_write", value );
    lifd_mux_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN,
                        value);
}

static INLINE void lifd_mux_reg_SERDES_0_19_DLOS_EN_field_set( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_SERDES_0_19_DLOS_EN_field_set( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lifd_mux_reg_SERDES_0_19_DLOS_EN_field_set", mask, ofs, value );
    lifd_mux_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN,
                        mask,
                        PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 lifd_mux_reg_SERDES_0_19_DLOS_EN_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_reg_SERDES_0_19_DLOS_EN_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lifd_mux_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN);

    IOLOG( "%s -> 0x%08x;", "lifd_mux_reg_SERDES_0_19_DLOS_EN_read", reg_value);
    return reg_value;
}

static INLINE void lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_write( lifd_mux_buffer_t *b_ptr,
                                                                             lifd_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_write( lifd_mux_buffer_t *b_ptr,
                                                                             lifd_handle_t *h_ptr,
                                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_write", value );
    lifd_mux_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19,
                        value);
}

static INLINE void lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_field_set( lifd_mux_buffer_t *b_ptr,
                                                                                 lifd_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_field_set( lifd_mux_buffer_t *b_ptr,
                                                                                 lifd_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_field_set", mask, ofs, value );
    lifd_mux_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19,
                        mask,
                        PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_read( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lifd_mux_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19);

    IOLOG( "%s -> 0x%08x;", "lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void lifd_mux_field_SFI2_RXS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                    lifd_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI2_RXS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                    lifd_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI2_RXS_INV_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI2_RXS_INV_set", value );

    /* (0x00150148 bits 12) field SFI2_RXS_INV of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_INV_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_INV_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI2_RXS_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI2_RXS_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 12) field SFI2_RXS_INV of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_INV_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_INV_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI2_RXS_INV_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI2_RXS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI2_RXS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI2_RXS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI2_RXS_EN_set", value );

    /* (0x00150148 bits 11) field SFI2_RXS_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI2_RXS_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI2_RXS_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 11) field SFI2_RXS_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI2_RXS_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI1_RXS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                    lifd_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI1_RXS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                    lifd_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI1_RXS_INV_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI1_RXS_INV_set", value );

    /* (0x00150148 bits 10) field SFI1_RXS_INV of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_INV_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_INV_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI1_RXS_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI1_RXS_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 10) field SFI1_RXS_INV of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_INV_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_INV_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI1_RXS_INV_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI1_RXS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI1_RXS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI1_RXS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI1_RXS_EN_set", value );

    /* (0x00150148 bits 9) field SFI1_RXS_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI1_RXS_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI1_RXS_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 9) field SFI1_RXS_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI1_RXS_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI_IF_SWAP_EN_set( lifd_mux_buffer_t *b_ptr,
                                                      lifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI_IF_SWAP_EN_set( lifd_mux_buffer_t *b_ptr,
                                                      lifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI_IF_SWAP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI_IF_SWAP_EN_set", value );

    /* (0x00150148 bits 8) field SFI_IF_SWAP_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI_IF_SWAP_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI_IF_SWAP_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI_IF_SWAP_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI_IF_SWAP_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 8) field SFI_IF_SWAP_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI_IF_SWAP_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI_IF_SWAP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI_IF_SWAP_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI2_TX_IFSEL_set( lifd_mux_buffer_t *b_ptr,
                                                     lifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI2_TX_IFSEL_set( lifd_mux_buffer_t *b_ptr,
                                                     lifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI2_TX_IFSEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI2_TX_IFSEL_set", value );

    /* (0x00150148 bits 7) field SFI2_TX_IFSEL of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_TX_IFSEL_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_TX_IFSEL_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI2_TX_IFSEL_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI2_TX_IFSEL_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 7) field SFI2_TX_IFSEL of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI2_TX_IFSEL_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI2_TX_IFSEL_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI2_TX_IFSEL_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI1_TX_IFSEL_set( lifd_mux_buffer_t *b_ptr,
                                                     lifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI1_TX_IFSEL_set( lifd_mux_buffer_t *b_ptr,
                                                     lifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI1_TX_IFSEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI1_TX_IFSEL_set", value );

    /* (0x00150148 bits 6) field SFI1_TX_IFSEL of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_TX_IFSEL_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_TX_IFSEL_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI1_TX_IFSEL_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI1_TX_IFSEL_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 6) field SFI1_TX_IFSEL of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI1_TX_IFSEL_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI1_TX_IFSEL_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI1_TX_IFSEL_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI2_EN_set( lifd_mux_buffer_t *b_ptr,
                                               lifd_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI2_EN_set( lifd_mux_buffer_t *b_ptr,
                                               lifd_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI2_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI2_EN_set", value );

    /* (0x00150148 bits 5) field SFI2_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI2_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI2_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 5) field SFI2_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI2_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI2_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI2_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI1_EN_set( lifd_mux_buffer_t *b_ptr,
                                               lifd_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI1_EN_set( lifd_mux_buffer_t *b_ptr,
                                               lifd_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI1_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI1_EN_set", value );

    /* (0x00150148 bits 4) field SFI1_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI1_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI1_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 4) field SFI1_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI1_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI1_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI1_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI2_OTN_EN_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI2_OTN_EN_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI2_OTN_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI2_OTN_EN_set", value );

    /* (0x00150148 bits 3) field SFI2_OTN_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_OTN_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_OTN_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI2_OTN_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI2_OTN_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 3) field SFI2_OTN_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI2_OTN_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI2_OTN_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI2_OTN_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI1_OTN_EN_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI1_OTN_EN_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI1_OTN_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI1_OTN_EN_set", value );

    /* (0x00150148 bits 2) field SFI1_OTN_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_OTN_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_OTN_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI1_OTN_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI1_OTN_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 2) field SFI1_OTN_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI1_OTN_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI1_OTN_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI1_OTN_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI2_CBRC_EN_set( lifd_mux_buffer_t *b_ptr,
                                                    lifd_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI2_CBRC_EN_set( lifd_mux_buffer_t *b_ptr,
                                                    lifd_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI2_CBRC_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI2_CBRC_EN_set", value );

    /* (0x00150148 bits 1) field SFI2_CBRC_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_CBRC_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI2_CBRC_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI2_CBRC_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI2_CBRC_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 1) field SFI2_CBRC_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI2_CBRC_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI2_CBRC_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI2_CBRC_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI1_CBRC_EN_set( lifd_mux_buffer_t *b_ptr,
                                                    lifd_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI1_CBRC_EN_set( lifd_mux_buffer_t *b_ptr,
                                                    lifd_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI1_CBRC_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI1_CBRC_EN_set", value );

    /* (0x00150148 bits 0) field SFI1_CBRC_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    lifd_mux_reg_SFI_IF_field_set( b_ptr,
                                   h_ptr,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_CBRC_EN_MSK,
                                   LIFD_MUX_REG_SFI_IF_BIT_SFI1_CBRC_EN_OFF,
                                   value);
}

static INLINE UINT32 lifd_mux_field_SFI1_CBRC_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI1_CBRC_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150148 bits 0) field SFI1_CBRC_EN of register PMC_LIFD_MUX_REG_SFI_IF */
    reg_value = lifd_mux_reg_SFI_IF_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_IF_BIT_SFI1_CBRC_EN_MSK) >> LIFD_MUX_REG_SFI_IF_BIT_SFI1_CBRC_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI1_CBRC_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_SFI2_ACB_IF_SEL_set( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI2_ACB_IF_SEL_set( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI2_ACB_IF_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI2_ACB_IF_SEL_set", value );

    /* (0x0015014c bits 3:2) field SFI2_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
    lifd_mux_reg_SFI_C_CKCTL_SS_SEL_field_set( b_ptr,
                                               h_ptr,
                                               LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_MSK,
                                               LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_OFF,
                                               value);
}

static INLINE UINT32 lifd_mux_field_SFI2_ACB_IF_SEL_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI2_ACB_IF_SEL_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0015014c bits 3:2) field SFI2_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
    reg_value = lifd_mux_reg_SFI_C_CKCTL_SS_SEL_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_MSK) >> LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI2_ACB_IF_SEL_get", value );

    return value;
}
static INLINE void lifd_mux_field_range_SFI2_ACB_IF_SEL_set( lifd_mux_buffer_t *b_ptr,
                                                             lifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_range_SFI2_ACB_IF_SEL_set( lifd_mux_buffer_t *b_ptr,
                                                             lifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_SFI2_ACB_IF_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_SFI2_ACB_IF_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_SFI2_ACB_IF_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0015014c bits 3:2) field SFI2_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
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
        /* (0x0015014c bits 3:2) field SFI2_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
        lifd_mux_reg_SFI_C_CKCTL_SS_SEL_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_OFF + subfield_offset),
                                                   LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 lifd_mux_field_range_SFI2_ACB_IF_SEL_get( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_range_SFI2_ACB_IF_SEL_get( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_SFI2_ACB_IF_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_SFI2_ACB_IF_SEL_get", stop_bit, 1 );
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
    /* (0x0015014c bits 3:2) field SFI2_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
    reg_value = lifd_mux_reg_SFI_C_CKCTL_SS_SEL_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_MSK)
                  >> LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_MSK, LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_SFI2_ACB_IF_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lifd_mux_field_SFI1_ACB_IF_SEL_set( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_SFI1_ACB_IF_SEL_set( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_SFI1_ACB_IF_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_SFI1_ACB_IF_SEL_set", value );

    /* (0x0015014c bits 1:0) field SFI1_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
    lifd_mux_reg_SFI_C_CKCTL_SS_SEL_field_set( b_ptr,
                                               h_ptr,
                                               LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_MSK,
                                               LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_OFF,
                                               value);
}

static INLINE UINT32 lifd_mux_field_SFI1_ACB_IF_SEL_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_SFI1_ACB_IF_SEL_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0015014c bits 1:0) field SFI1_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
    reg_value = lifd_mux_reg_SFI_C_CKCTL_SS_SEL_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_MSK) >> LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_SFI1_ACB_IF_SEL_get", value );

    return value;
}
static INLINE void lifd_mux_field_range_SFI1_ACB_IF_SEL_set( lifd_mux_buffer_t *b_ptr,
                                                             lifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_range_SFI1_ACB_IF_SEL_set( lifd_mux_buffer_t *b_ptr,
                                                             lifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_SFI1_ACB_IF_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_SFI1_ACB_IF_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_SFI1_ACB_IF_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0015014c bits 1:0) field SFI1_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
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
        /* (0x0015014c bits 1:0) field SFI1_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
        lifd_mux_reg_SFI_C_CKCTL_SS_SEL_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_OFF + subfield_offset),
                                                   LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 lifd_mux_field_range_SFI1_ACB_IF_SEL_get( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_range_SFI1_ACB_IF_SEL_get( lifd_mux_buffer_t *b_ptr,
                                                               lifd_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_SFI1_ACB_IF_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_SFI1_ACB_IF_SEL_get", stop_bit, 1 );
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
    /* (0x0015014c bits 1:0) field SFI1_ACB_IF_SEL of register PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL */
    reg_value = lifd_mux_reg_SFI_C_CKCTL_SS_SEL_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_MSK)
                  >> LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_MSK, LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_SFI1_ACB_IF_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lifd_mux_field_PIN_LOS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_PIN_LOS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_PIN_LOS_INV_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_PIN_LOS_INV_set", value );

    /* (0x00150150 bits 19:0) field PIN_LOS_INV of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT */
    lifd_mux_reg_PIN_LOS_0_19_INVERT_field_set( b_ptr,
                                                h_ptr,
                                                LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_MSK,
                                                LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_OFF,
                                                value);
}

static INLINE UINT32 lifd_mux_field_PIN_LOS_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_PIN_LOS_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150150 bits 19:0) field PIN_LOS_INV of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT */
    reg_value = lifd_mux_reg_PIN_LOS_0_19_INVERT_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_MSK) >> LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_PIN_LOS_INV_get", value );

    return value;
}
static INLINE void lifd_mux_field_range_PIN_LOS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                         lifd_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_range_PIN_LOS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                         lifd_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_PIN_LOS_INV_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_PIN_LOS_INV_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_PIN_LOS_INV_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00150150 bits 19:0) field PIN_LOS_INV of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00150150 bits 19:0) field PIN_LOS_INV of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT */
        lifd_mux_reg_PIN_LOS_0_19_INVERT_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_OFF + subfield_offset),
                                                    LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 lifd_mux_field_range_PIN_LOS_INV_get( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_range_PIN_LOS_INV_get( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_PIN_LOS_INV_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_PIN_LOS_INV_get", stop_bit, 19 );
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
    /* (0x00150150 bits 19:0) field PIN_LOS_INV of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT */
    reg_value = lifd_mux_reg_PIN_LOS_0_19_INVERT_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_MSK)
                  >> LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_MSK, LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_PIN_LOS_INV_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lifd_mux_field_PIN_LOS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                  lifd_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_PIN_LOS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                  lifd_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_PIN_LOS_EN_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_PIN_LOS_EN_set", value );

    /* (0x00150154 bits 19:0) field PIN_LOS_EN of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN */
    lifd_mux_reg_PIN_LOS_0_19_EN_field_set( b_ptr,
                                            h_ptr,
                                            LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_MSK,
                                            LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_OFF,
                                            value);
}

static INLINE UINT32 lifd_mux_field_PIN_LOS_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_PIN_LOS_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150154 bits 19:0) field PIN_LOS_EN of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN */
    reg_value = lifd_mux_reg_PIN_LOS_0_19_EN_read(  b_ptr, h_ptr);
    value = (reg_value & LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_MSK) >> LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_PIN_LOS_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_range_PIN_LOS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                        lifd_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_range_PIN_LOS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                        lifd_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_PIN_LOS_EN_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_PIN_LOS_EN_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_PIN_LOS_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00150154 bits 19:0) field PIN_LOS_EN of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00150154 bits 19:0) field PIN_LOS_EN of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN */
        lifd_mux_reg_PIN_LOS_0_19_EN_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_OFF + subfield_offset),
                                                LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 lifd_mux_field_range_PIN_LOS_EN_get( lifd_mux_buffer_t *b_ptr,
                                                          lifd_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_range_PIN_LOS_EN_get( lifd_mux_buffer_t *b_ptr,
                                                          lifd_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_PIN_LOS_EN_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_PIN_LOS_EN_get", stop_bit, 19 );
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
    /* (0x00150154 bits 19:0) field PIN_LOS_EN of register PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN */
    reg_value = lifd_mux_reg_PIN_LOS_0_19_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_MSK)
                  >> LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_MSK, LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_PIN_LOS_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lifd_mux_field_DLOS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                lifd_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_DLOS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                lifd_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_DLOS_INV_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_DLOS_INV_set", value );

    /* (0x00150158 bits 19:0) field DLOS_INV of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT */
    lifd_mux_reg_SERDES_0_19_DLOS_INVERT_field_set( b_ptr,
                                                    h_ptr,
                                                    LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_MSK,
                                                    LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_OFF,
                                                    value);
}

static INLINE UINT32 lifd_mux_field_DLOS_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_DLOS_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150158 bits 19:0) field DLOS_INV of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT */
    reg_value = lifd_mux_reg_SERDES_0_19_DLOS_INVERT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_MSK) >> LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_DLOS_INV_get", value );

    return value;
}
static INLINE void lifd_mux_field_range_DLOS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                      lifd_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_range_DLOS_INV_set( lifd_mux_buffer_t *b_ptr,
                                                      lifd_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_DLOS_INV_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_DLOS_INV_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_DLOS_INV_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00150158 bits 19:0) field DLOS_INV of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00150158 bits 19:0) field DLOS_INV of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT */
        lifd_mux_reg_SERDES_0_19_DLOS_INVERT_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_OFF + subfield_offset),
                                                        LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 lifd_mux_field_range_DLOS_INV_get( lifd_mux_buffer_t *b_ptr,
                                                        lifd_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_range_DLOS_INV_get( lifd_mux_buffer_t *b_ptr,
                                                        lifd_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_DLOS_INV_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_DLOS_INV_get", stop_bit, 19 );
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
    /* (0x00150158 bits 19:0) field DLOS_INV of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT */
    reg_value = lifd_mux_reg_SERDES_0_19_DLOS_INVERT_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_MSK)
                  >> LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_MSK, LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_DLOS_INV_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lifd_mux_field_DLOS_EN_set( lifd_mux_buffer_t *b_ptr,
                                               lifd_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_DLOS_EN_set( lifd_mux_buffer_t *b_ptr,
                                               lifd_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_DLOS_EN_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_DLOS_EN_set", value );

    /* (0x0015015c bits 19:0) field DLOS_EN of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN */
    lifd_mux_reg_SERDES_0_19_DLOS_EN_field_set( b_ptr,
                                                h_ptr,
                                                LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_MSK,
                                                LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_OFF,
                                                value);
}

static INLINE UINT32 lifd_mux_field_DLOS_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_DLOS_EN_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0015015c bits 19:0) field DLOS_EN of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN */
    reg_value = lifd_mux_reg_SERDES_0_19_DLOS_EN_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_MSK) >> LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_DLOS_EN_get", value );

    return value;
}
static INLINE void lifd_mux_field_range_DLOS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                     lifd_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_range_DLOS_EN_set( lifd_mux_buffer_t *b_ptr,
                                                     lifd_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_DLOS_EN_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_DLOS_EN_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_DLOS_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0015015c bits 19:0) field DLOS_EN of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0015015c bits 19:0) field DLOS_EN of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN */
        lifd_mux_reg_SERDES_0_19_DLOS_EN_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_OFF + subfield_offset),
                                                    LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 lifd_mux_field_range_DLOS_EN_get( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_range_DLOS_EN_get( lifd_mux_buffer_t *b_ptr,
                                                       lifd_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_DLOS_EN_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_DLOS_EN_get", stop_bit, 19 );
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
    /* (0x0015015c bits 19:0) field DLOS_EN of register PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN */
    reg_value = lifd_mux_reg_SERDES_0_19_DLOS_EN_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_MSK)
                  >> LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_MSK, LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_DLOS_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lifd_mux_field_LOS_POL_INV_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_LOS_POL_INV_set( lifd_mux_buffer_t *b_ptr,
                                                   lifd_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lifd_mux_field_LOS_POL_INV_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "lifd_mux_field_LOS_POL_INV_set", value );

    /* (0x00150160 bits 19:0) field LOS_POL_INV of register PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19 */
    lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_field_set( b_ptr,
                                                                  h_ptr,
                                                                  LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_MSK,
                                                                  LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_OFF,
                                                                  value);
}

static INLINE UINT32 lifd_mux_field_LOS_POL_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_LOS_POL_INV_get( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00150160 bits 19:0) field LOS_POL_INV of register PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19 */
    reg_value = lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_read( b_ptr,
                                                                         h_ptr);
    value = (reg_value & LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_MSK) >> LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_OFF;
    IOLOG( "%s -> 0x%08x", "lifd_mux_field_LOS_POL_INV_get", value );

    return value;
}
static INLINE void lifd_mux_field_range_LOS_POL_INV_set( lifd_mux_buffer_t *b_ptr,
                                                         lifd_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void lifd_mux_field_range_LOS_POL_INV_set( lifd_mux_buffer_t *b_ptr,
                                                         lifd_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_LOS_POL_INV_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_LOS_POL_INV_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_LOS_POL_INV_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00150160 bits 19:0) field LOS_POL_INV of register PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19 */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00150160 bits 19:0) field LOS_POL_INV of register PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19 */
        lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_field_set( b_ptr,
                                                                      h_ptr,
                                                                      subfield_mask << (LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_OFF + subfield_offset),
                                                                      LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_OFF + subfield_offset,
                                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 lifd_mux_field_range_LOS_POL_INV_get( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lifd_mux_field_range_LOS_POL_INV_get( lifd_mux_buffer_t *b_ptr,
                                                           lifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lifd_mux_field_range_LOS_POL_INV_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lifd_mux_field_range_LOS_POL_INV_get", stop_bit, 19 );
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
    /* (0x00150160 bits 19:0) field LOS_POL_INV of register PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19 */
    reg_value = lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_read( b_ptr,
                                                                         h_ptr);
    field_value = (reg_value & LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_MSK)
                  >> LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_MSK, LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lifd_mux_field_range_LOS_POL_INV_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LIFD_MUX_IO_INLINE_H */
