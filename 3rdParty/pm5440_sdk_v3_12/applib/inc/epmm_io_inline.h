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
 *     and register accessor functions for the epmm block
 *****************************************************************************/
#ifndef _EPMM_IO_INLINE_H
#define _EPMM_IO_INLINE_H

#include "epmm_loc.h"
#include "epmm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EPMM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for epmm
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
    epmm_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} epmm_buffer_t;
static INLINE void epmm_buffer_init( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void epmm_buffer_init( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "epmm_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void epmm_buffer_flush( epmm_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void epmm_buffer_flush( epmm_buffer_t *b_ptr )
{
    IOLOG( "epmm_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 epmm_reg_read( epmm_buffer_t *b_ptr,
                                    epmm_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_read( epmm_buffer_t *b_ptr,
                                    epmm_handle_t *h_ptr,
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
static INLINE void epmm_reg_write( epmm_buffer_t *b_ptr,
                                   epmm_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_write( epmm_buffer_t *b_ptr,
                                   epmm_handle_t *h_ptr,
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

static INLINE void epmm_field_set( epmm_buffer_t *b_ptr,
                                   epmm_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_set( epmm_buffer_t *b_ptr,
                                   epmm_handle_t *h_ptr,
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

static INLINE void epmm_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                   epmm_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                   epmm_handle_t *h_ptr,
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

static INLINE void epmm_burst_read( epmm_buffer_t *b_ptr,
                                    epmm_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void epmm_burst_read( epmm_buffer_t *b_ptr,
                                    epmm_handle_t *h_ptr,
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

static INLINE void epmm_burst_write( epmm_buffer_t *b_ptr,
                                     epmm_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void epmm_burst_write( epmm_buffer_t *b_ptr,
                                     epmm_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE epmm_poll( epmm_buffer_t *b_ptr,
                                              epmm_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_poll( epmm_buffer_t *b_ptr,
                                              epmm_handle_t *h_ptr,
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
 *  register access functions for epmm
 * ==================================================================================
 */

static INLINE void epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_write( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_write( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG,
                    value);
}

static INLINE void epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_field_set( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_field_set( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG,
                    mask,
                    PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_write( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_write( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG,
                    value);
}

static INLINE void epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_field_set( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_field_set( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG,
                    mask,
                    PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_ING_100G_LINK_CFG_REG_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_100G_LINK_CFG_REG_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_100G_LINK_CFG_REG_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_ING_100G_LINK_CFG_REG,
                    value);
}

static INLINE void epmm_reg_ING_100G_LINK_CFG_REG_field_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_100G_LINK_CFG_REG_field_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_100G_LINK_CFG_REG_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_ING_100G_LINK_CFG_REG,
                    mask,
                    PMC_EPMM120_REG_ING_100G_LINK_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_ING_100G_LINK_CFG_REG_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_100G_LINK_CFG_REG_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_ING_100G_LINK_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_100G_LINK_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_EGR_100G_LINK_CFG_REG_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_100G_LINK_CFG_REG_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_100G_LINK_CFG_REG_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_EGR_100G_LINK_CFG_REG,
                    value);
}

static INLINE void epmm_reg_EGR_100G_LINK_CFG_REG_field_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_100G_LINK_CFG_REG_field_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_100G_LINK_CFG_REG_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_EGR_100G_LINK_CFG_REG,
                    mask,
                    PMC_EPMM120_REG_EGR_100G_LINK_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_EGR_100G_LINK_CFG_REG_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_100G_LINK_CFG_REG_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_EGR_100G_LINK_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_100G_LINK_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_ING_10G_LINK_CFG_REG_array_write( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_10G_LINK_CFG_REG_array_write( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_10G_LINK_CFG_REG_array_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_ING_10G_LINK_CFG_REG(N),
                    value);
}

static INLINE void epmm_reg_ING_10G_LINK_CFG_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_10G_LINK_CFG_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_10G_LINK_CFG_REG_array_field_set", N, mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_ING_10G_LINK_CFG_REG(N),
                    mask,
                    PMC_EPMM120_REG_ING_10G_LINK_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_ING_10G_LINK_CFG_REG_array_read( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_10G_LINK_CFG_REG_array_read( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_ING_10G_LINK_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "epmm_reg_ING_10G_LINK_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_write( epmm_buffer_t *b_ptr,
                                                                 epmm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_write( epmm_buffer_t *b_ptr,
                                                                 epmm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG(N),
                    value);
}

static INLINE void epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set", N, mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG(N),
                    mask,
                    PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void epmm_reg_EGR_10G_LINK_CFG_REG_array_write( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_10G_LINK_CFG_REG_array_write( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_10G_LINK_CFG_REG_array_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG(N),
                    value);
}

static INLINE void epmm_reg_EGR_10G_LINK_CFG_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_10G_LINK_CFG_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_10G_LINK_CFG_REG_array_field_set", N, mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG(N),
                    mask,
                    PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_EGR_10G_LINK_CFG_REG_array_read( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_10G_LINK_CFG_REG_array_read( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "epmm_reg_EGR_10G_LINK_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void epmm_reg_EGR_LF_RF_OS_EN_REG_array_write( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_LF_RF_OS_EN_REG_array_write( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_LF_RF_OS_EN_REG_array_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG(N),
                    value);
}

static INLINE void epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                 epmm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                 epmm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set", N, mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG(N),
                    mask,
                    PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_EGR_LF_RF_OS_EN_REG_array_read( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_LF_RF_OS_EN_REG_array_read( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "epmm_reg_EGR_LF_RF_OS_EN_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void epmm_reg_ING_40G_LINK_CFG_REG_array_write( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_40G_LINK_CFG_REG_array_write( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_40G_LINK_CFG_REG_array_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_ING_40G_LINK_CFG_REG(N),
                    value);
}

static INLINE void epmm_reg_ING_40G_LINK_CFG_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_40G_LINK_CFG_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_40G_LINK_CFG_REG_array_field_set", N, mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_ING_40G_LINK_CFG_REG(N),
                    mask,
                    PMC_EPMM120_REG_ING_40G_LINK_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_ING_40G_LINK_CFG_REG_array_read( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_40G_LINK_CFG_REG_array_read( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_ING_40G_LINK_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "epmm_reg_ING_40G_LINK_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void epmm_reg_EGR_40G_LINK_CFG_REG_array_write( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_40G_LINK_CFG_REG_array_write( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_40G_LINK_CFG_REG_array_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_EGR_40G_LINK_CFG_REG(N),
                    value);
}

static INLINE void epmm_reg_EGR_40G_LINK_CFG_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_40G_LINK_CFG_REG_array_field_set( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_40G_LINK_CFG_REG_array_field_set", N, mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_EPMM120_REG_EGR_40G_LINK_CFG_REG(N),
                    mask,
                    PMC_EPMM120_REG_EGR_40G_LINK_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_EGR_40G_LINK_CFG_REG_array_read( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_40G_LINK_CFG_REG_array_read( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_EPMM120_REG_EGR_40G_LINK_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "epmm_reg_EGR_40G_LINK_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void epmm_reg_ING_GSUP_OVR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_GSUP_OVR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_GSUP_OVR_INT_EN_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN,
                    value);
}

static INLINE void epmm_reg_ING_GSUP_OVR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_GSUP_OVR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_GSUP_OVR_INT_EN_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN,
                    mask,
                    PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_ING_GSUP_OVR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_GSUP_OVR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_GSUP_OVR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_ING_GSUP_UDR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_GSUP_UDR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_GSUP_UDR_INT_EN_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN,
                    value);
}

static INLINE void epmm_reg_ING_GSUP_UDR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_GSUP_UDR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_GSUP_UDR_INT_EN_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN,
                    mask,
                    PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_ING_GSUP_UDR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_GSUP_UDR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_GSUP_UDR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_ING_CBR_OVR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_CBR_OVR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_CBR_OVR_INT_EN_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_CBR_OVR_INT_EN,
                    value);
}

static INLINE void epmm_reg_ING_CBR_OVR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_CBR_OVR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_CBR_OVR_INT_EN_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_CBR_OVR_INT_EN,
                    mask,
                    PMC_EPMM120_REG_ING_CBR_OVR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_ING_CBR_OVR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_CBR_OVR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_CBR_OVR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_CBR_OVR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_ING_CBR_UDR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_CBR_UDR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_CBR_UDR_INT_EN_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_CBR_UDR_INT_EN,
                    value);
}

static INLINE void epmm_reg_ING_CBR_UDR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_CBR_UDR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_CBR_UDR_INT_EN_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_CBR_UDR_INT_EN,
                    mask,
                    PMC_EPMM120_REG_ING_CBR_UDR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_ING_CBR_UDR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_CBR_UDR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_CBR_UDR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_CBR_UDR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_EGR_GSUP_OVR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_GSUP_OVR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_GSUP_OVR_INT_EN_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN,
                    value);
}

static INLINE void epmm_reg_EGR_GSUP_OVR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_GSUP_OVR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_GSUP_OVR_INT_EN_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN,
                    mask,
                    PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_EGR_GSUP_OVR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_GSUP_OVR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_GSUP_OVR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_EGR_GSUP_UDR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_GSUP_UDR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_GSUP_UDR_INT_EN_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN,
                    value);
}

static INLINE void epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_GSUP_UDR_INT_EN_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN,
                    mask,
                    PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_EGR_GSUP_UDR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_GSUP_UDR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_GSUP_UDR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_EGR_CBR_OVR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_CBR_OVR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_CBR_OVR_INT_EN_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN,
                    value);
}

static INLINE void epmm_reg_EGR_CBR_OVR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_CBR_OVR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_CBR_OVR_INT_EN_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN,
                    mask,
                    PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_EGR_CBR_OVR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_CBR_OVR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_CBR_OVR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_EGR_CBR_UDR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_CBR_UDR_INT_EN_write( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_CBR_UDR_INT_EN_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN,
                    value);
}

static INLINE void epmm_reg_EGR_CBR_UDR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_CBR_UDR_INT_EN_field_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_CBR_UDR_INT_EN_field_set", mask, ofs, value );
    epmm_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN,
                    mask,
                    PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 epmm_reg_EGR_CBR_UDR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_CBR_UDR_INT_EN_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_CBR_UDR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void epmm_reg_ING_GSUP_OVR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_GSUP_OVR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_GSUP_OVR_INT_STAT_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT,
                    value);
}

static INLINE void epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    epmm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 epmm_reg_ING_GSUP_OVR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_GSUP_OVR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_GSUP_OVR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_OVR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_OVR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_ING_GSUP_OVR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void epmm_reg_ING_GSUP_UDR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_GSUP_UDR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_GSUP_UDR_INT_STAT_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT,
                    value);
}

static INLINE void epmm_reg_ING_GSUP_UDR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_GSUP_UDR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_GSUP_UDR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    epmm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 epmm_reg_ING_GSUP_UDR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_GSUP_UDR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_GSUP_UDR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_UDR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_UDR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_ING_GSUP_UDR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void epmm_reg_ING_CBR_OVR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_CBR_OVR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_CBR_OVR_INT_STAT_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT,
                    value);
}

static INLINE void epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    epmm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 epmm_reg_ING_CBR_OVR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_CBR_OVR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_CBR_OVR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_OVR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_OVR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_ING_CBR_OVR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void epmm_reg_ING_CBR_UDR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_CBR_UDR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_ING_CBR_UDR_INT_STAT_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT,
                    value);
}

static INLINE void epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    epmm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 epmm_reg_ING_CBR_UDR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_CBR_UDR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_CBR_UDR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_UDR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_UDR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_ING_CBR_UDR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void epmm_reg_EGR_GSUP_OVR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_GSUP_OVR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_GSUP_OVR_INT_STAT_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT,
                    value);
}

static INLINE void epmm_reg_EGR_GSUP_OVR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_GSUP_OVR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_GSUP_OVR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    epmm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 epmm_reg_EGR_GSUP_OVR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_GSUP_OVR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_GSUP_OVR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_OVR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_OVR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_EGR_GSUP_OVR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void epmm_reg_EGR_GSUP_UDR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_GSUP_UDR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_GSUP_UDR_INT_STAT_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT,
                    value);
}

static INLINE void epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    epmm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 epmm_reg_EGR_GSUP_UDR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_GSUP_UDR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_GSUP_UDR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                        epmm_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_EGR_GSUP_UDR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void epmm_reg_EGR_CBR_OVR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_CBR_OVR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_CBR_OVR_INT_STAT_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT,
                    value);
}

static INLINE void epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    epmm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 epmm_reg_EGR_CBR_OVR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_CBR_OVR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_CBR_OVR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_OVR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_OVR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_EGR_CBR_OVR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void epmm_reg_EGR_CBR_UDR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_CBR_UDR_INT_STAT_write( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "epmm_reg_EGR_CBR_UDR_INT_STAT_write", value );
    epmm_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT,
                    value);
}

static INLINE void epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    epmm_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 epmm_reg_EGR_CBR_UDR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_CBR_UDR_INT_STAT_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_CBR_UDR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_UDR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_UDR_INT_STAT_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_EGR_CBR_UDR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 epmm_reg_ING_GSUP_OVR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_GSUP_OVR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_GSUP_OVR_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_OVR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_OVR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_ING_GSUP_OVR_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 epmm_reg_ING_GSUP_UDR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_GSUP_UDR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_GSUP_UDR_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_GSUP_UDR_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_UDR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_UDR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_ING_GSUP_UDR_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_ING_GSUP_UDR_INT_VAL,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 epmm_reg_ING_CBR_OVR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_CBR_OVR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_CBR_OVR_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_OVR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_OVR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_ING_CBR_OVR_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 epmm_reg_ING_CBR_UDR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_ING_CBR_UDR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_ING_CBR_UDR_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_UDR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_UDR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_ING_CBR_UDR_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 epmm_reg_EGR_GSUP_OVR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_GSUP_OVR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_GSUP_OVR_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_GSUP_OVR_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_OVR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_OVR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_EGR_GSUP_OVR_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_EGR_GSUP_OVR_INT_VAL,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 epmm_reg_EGR_GSUP_UDR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_GSUP_UDR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_GSUP_UDR_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                       epmm_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_EGR_GSUP_UDR_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 epmm_reg_EGR_CBR_OVR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_CBR_OVR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_CBR_OVR_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_OVR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_OVR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_EGR_CBR_OVR_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 epmm_reg_EGR_CBR_UDR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_reg_EGR_CBR_UDR_INT_VAL_read( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = epmm_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "epmm_reg_EGR_CBR_UDR_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_UDR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_UDR_INT_VAL_poll( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "epmm_reg_EGR_CBR_UDR_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return epmm_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL,
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
static INLINE void epmm_field_SW_RESET_100G_set( epmm_buffer_t *b_ptr,
                                                 epmm_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_SW_RESET_100G_set( epmm_buffer_t *b_ptr,
                                                 epmm_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_SW_RESET_100G_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_SW_RESET_100G_set", value );

    /* (0x00000000 bits 2) field SW_RESET_100G of register PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG */
    epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_100G_MSK,
                                                     EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_100G_OFF,
                                                     value);
}

static INLINE UINT32 epmm_field_SW_RESET_100G_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_SW_RESET_100G_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field SW_RESET_100G of register PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG */
    reg_value = epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_100G_MSK) >> EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_100G_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_SW_RESET_100G_get", value );

    return value;
}
static INLINE void epmm_field_SW_RESET_40G_set( epmm_buffer_t *b_ptr,
                                                epmm_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_SW_RESET_40G_set( epmm_buffer_t *b_ptr,
                                                epmm_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_SW_RESET_40G_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_SW_RESET_40G_set", value );

    /* (0x00000000 bits 1) field SW_RESET_40G of register PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG */
    epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_40G_MSK,
                                                     EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_40G_OFF,
                                                     value);
}

static INLINE UINT32 epmm_field_SW_RESET_40G_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_SW_RESET_40G_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field SW_RESET_40G of register PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG */
    reg_value = epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_40G_MSK) >> EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_40G_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_SW_RESET_40G_get", value );

    return value;
}
static INLINE void epmm_field_SW_RESET_10G_set( epmm_buffer_t *b_ptr,
                                                epmm_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_SW_RESET_10G_set( epmm_buffer_t *b_ptr,
                                                epmm_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_SW_RESET_10G_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_SW_RESET_10G_set", value );

    /* (0x00000000 bits 0) field SW_RESET_10G of register PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG */
    epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_10G_MSK,
                                                     EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_10G_OFF,
                                                     value);
}

static INLINE UINT32 epmm_field_SW_RESET_10G_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_SW_RESET_10G_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field SW_RESET_10G of register PMC_EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG */
    reg_value = epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_10G_MSK) >> EPMM120_REG_GLOBAL_SOFT_RESET_CONFIG_REG_BIT_SW_RESET_10G_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_SW_RESET_10G_get", value );

    return value;
}
static INLINE void epmm_field_UPI_OS_TYPE_set( epmm_buffer_t *b_ptr,
                                               epmm_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_UPI_OS_TYPE_set( epmm_buffer_t *b_ptr,
                                               epmm_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_UPI_OS_TYPE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "epmm_field_UPI_OS_TYPE_set", value );

    /* (0x00000014 bits 15:8) field UPI_OS_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
    epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_MSK,
                                                         EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_OFF,
                                                         value);
}

static INLINE UINT32 epmm_field_UPI_OS_TYPE_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_UPI_OS_TYPE_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 15:8) field UPI_OS_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
    reg_value = epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_MSK) >> EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_UPI_OS_TYPE_get", value );

    return value;
}
static INLINE void epmm_field_range_UPI_OS_TYPE_set( epmm_buffer_t *b_ptr,
                                                     epmm_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_UPI_OS_TYPE_set( epmm_buffer_t *b_ptr,
                                                     epmm_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_UPI_OS_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_UPI_OS_TYPE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_UPI_OS_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 15:8) field UPI_OS_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
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
        /* (0x00000014 bits 15:8) field UPI_OS_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
        epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_OFF + subfield_offset),
                                                             EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_UPI_OS_TYPE_get( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_UPI_OS_TYPE_get( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_UPI_OS_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_UPI_OS_TYPE_get", stop_bit, 7 );
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
    /* (0x00000014 bits 15:8) field UPI_OS_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
    reg_value = epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_MSK)
                  >> EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_MSK, EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_OS_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_UPI_OS_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_UPI_PKT_TYPE_set( epmm_buffer_t *b_ptr,
                                                epmm_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_UPI_PKT_TYPE_set( epmm_buffer_t *b_ptr,
                                                epmm_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_UPI_PKT_TYPE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "epmm_field_UPI_PKT_TYPE_set", value );

    /* (0x00000014 bits 7:0) field UPI_PKT_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
    epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_MSK,
                                                         EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_OFF,
                                                         value);
}

static INLINE UINT32 epmm_field_UPI_PKT_TYPE_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_UPI_PKT_TYPE_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 7:0) field UPI_PKT_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
    reg_value = epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_MSK) >> EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_UPI_PKT_TYPE_get", value );

    return value;
}
static INLINE void epmm_field_range_UPI_PKT_TYPE_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_UPI_PKT_TYPE_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_UPI_PKT_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_UPI_PKT_TYPE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_UPI_PKT_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 7:0) field UPI_PKT_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
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
        /* (0x00000014 bits 7:0) field UPI_PKT_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
        epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_OFF + subfield_offset),
                                                             EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_UPI_PKT_TYPE_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_UPI_PKT_TYPE_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_UPI_PKT_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_UPI_PKT_TYPE_get", stop_bit, 7 );
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
    /* (0x00000014 bits 7:0) field UPI_PKT_TYPE of register PMC_EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG */
    reg_value = epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_MSK)
                  >> EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_MSK, EPMM120_REG_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_BIT_UPI_PKT_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_UPI_PKT_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_ING_100G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_100G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_100G_LINK_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_100G_LINK_ENABLE_set", value );

    /* (0x00000060 bits 1) field ING_100G_LINK_ENABLE of register PMC_EPMM120_REG_ING_100G_LINK_CFG_REG */
    epmm_reg_ING_100G_LINK_CFG_REG_field_set( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_100G_LINK_CFG_REG_BIT_ING_100G_LINK_ENABLE_MSK,
                                              EPMM120_REG_ING_100G_LINK_CFG_REG_BIT_ING_100G_LINK_ENABLE_OFF,
                                              value);
}

static INLINE UINT32 epmm_field_ING_100G_LINK_ENABLE_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_100G_LINK_ENABLE_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 1) field ING_100G_LINK_ENABLE of register PMC_EPMM120_REG_ING_100G_LINK_CFG_REG */
    reg_value = epmm_reg_ING_100G_LINK_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_100G_LINK_CFG_REG_BIT_ING_100G_LINK_ENABLE_MSK) >> EPMM120_REG_ING_100G_LINK_CFG_REG_BIT_ING_100G_LINK_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_100G_LINK_ENABLE_get", value );

    return value;
}
static INLINE void epmm_field_ING_100G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_100G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_100G_LINK_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_100G_LINK_RESET_set", value );

    /* (0x00000060 bits 0) field ING_100G_LINK_RESET of register PMC_EPMM120_REG_ING_100G_LINK_CFG_REG */
    epmm_reg_ING_100G_LINK_CFG_REG_field_set( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_100G_LINK_CFG_REG_BIT_ING_100G_LINK_RESET_MSK,
                                              EPMM120_REG_ING_100G_LINK_CFG_REG_BIT_ING_100G_LINK_RESET_OFF,
                                              value);
}

static INLINE UINT32 epmm_field_ING_100G_LINK_RESET_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_100G_LINK_RESET_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 0) field ING_100G_LINK_RESET of register PMC_EPMM120_REG_ING_100G_LINK_CFG_REG */
    reg_value = epmm_reg_ING_100G_LINK_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_100G_LINK_CFG_REG_BIT_ING_100G_LINK_RESET_MSK) >> EPMM120_REG_ING_100G_LINK_CFG_REG_BIT_ING_100G_LINK_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_100G_LINK_RESET_get", value );

    return value;
}
static INLINE void epmm_field_EGR_100G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_100G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_100G_LINK_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_100G_LINK_ENABLE_set", value );

    /* (0x00000130 bits 1) field EGR_100G_LINK_ENABLE of register PMC_EPMM120_REG_EGR_100G_LINK_CFG_REG */
    epmm_reg_EGR_100G_LINK_CFG_REG_field_set( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_100G_LINK_CFG_REG_BIT_EGR_100G_LINK_ENABLE_MSK,
                                              EPMM120_REG_EGR_100G_LINK_CFG_REG_BIT_EGR_100G_LINK_ENABLE_OFF,
                                              value);
}

static INLINE UINT32 epmm_field_EGR_100G_LINK_ENABLE_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_100G_LINK_ENABLE_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000130 bits 1) field EGR_100G_LINK_ENABLE of register PMC_EPMM120_REG_EGR_100G_LINK_CFG_REG */
    reg_value = epmm_reg_EGR_100G_LINK_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_100G_LINK_CFG_REG_BIT_EGR_100G_LINK_ENABLE_MSK) >> EPMM120_REG_EGR_100G_LINK_CFG_REG_BIT_EGR_100G_LINK_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_100G_LINK_ENABLE_get", value );

    return value;
}
static INLINE void epmm_field_EGR_100G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_100G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_100G_LINK_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_100G_LINK_RESET_set", value );

    /* (0x00000130 bits 0) field EGR_100G_LINK_RESET of register PMC_EPMM120_REG_EGR_100G_LINK_CFG_REG */
    epmm_reg_EGR_100G_LINK_CFG_REG_field_set( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_100G_LINK_CFG_REG_BIT_EGR_100G_LINK_RESET_MSK,
                                              EPMM120_REG_EGR_100G_LINK_CFG_REG_BIT_EGR_100G_LINK_RESET_OFF,
                                              value);
}

static INLINE UINT32 epmm_field_EGR_100G_LINK_RESET_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_100G_LINK_RESET_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000130 bits 0) field EGR_100G_LINK_RESET of register PMC_EPMM120_REG_EGR_100G_LINK_CFG_REG */
    reg_value = epmm_reg_EGR_100G_LINK_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_100G_LINK_CFG_REG_BIT_EGR_100G_LINK_RESET_MSK) >> EPMM120_REG_EGR_100G_LINK_CFG_REG_BIT_EGR_100G_LINK_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_100G_LINK_RESET_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void epmm_field_ING_10G_LINK_DP_SEL_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_LINK_DP_SEL_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_10G_LINK_DP_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_LINK_DP_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_10G_LINK_DP_SEL_set", N, value );

    /* ((0x00000024 + (N) * 0x04) bits 3:2) field ING_10G_LINK_DP_SEL of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
    epmm_reg_ING_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_MSK,
                                                   EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_ING_10G_LINK_DP_SEL_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_LINK_DP_SEL_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_10G_LINK_DP_SEL_get", N, 11);
    /* ((0x00000024 + (N) * 0x04) bits 3:2) field ING_10G_LINK_DP_SEL of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_MSK) >> EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_10G_LINK_DP_SEL_get", N, value );

    return value;
}
static INLINE void epmm_field_range_ING_10G_LINK_DP_SEL_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_10G_LINK_DP_SEL_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_range_ING_10G_LINK_DP_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_LINK_DP_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_LINK_DP_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_LINK_DP_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000024 + (N) * 0x04) bits 3:2) field ING_10G_LINK_DP_SEL of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
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
        /* ((0x00000024 + (N) * 0x04) bits 3:2) field ING_10G_LINK_DP_SEL of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
        epmm_reg_ING_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_OFF + subfield_offset),
                                                       EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_10G_LINK_DP_SEL_get( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_10G_LINK_DP_SEL_get( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
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

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_range_ING_10G_LINK_DP_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_LINK_DP_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_LINK_DP_SEL_get", stop_bit, 1 );
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
    /* ((0x00000024 + (N) * 0x04) bits 3:2) field ING_10G_LINK_DP_SEL of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_MSK)
                  >> EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_MSK, EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_DP_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_LINK_DP_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_ING_10G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_10G_LINK_ENABLE_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_LINK_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_10G_LINK_ENABLE_set", N, value );

    /* ((0x00000024 + (N) * 0x04) bits 1) field ING_10G_LINK_ENABLE of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
    epmm_reg_ING_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_ENABLE_MSK,
                                                   EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_ENABLE_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_ING_10G_LINK_ENABLE_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_LINK_ENABLE_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_10G_LINK_ENABLE_get", N, 11);
    /* ((0x00000024 + (N) * 0x04) bits 1) field ING_10G_LINK_ENABLE of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_ENABLE_MSK) >> EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_10G_LINK_ENABLE_get", N, value );

    return value;
}
static INLINE void epmm_field_ING_10G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_10G_LINK_RESET_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_LINK_RESET_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_10G_LINK_RESET_set", N, value );

    /* ((0x00000024 + (N) * 0x04) bits 0) field ING_10G_LINK_RESET of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
    epmm_reg_ING_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_RESET_MSK,
                                                   EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_RESET_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_ING_10G_LINK_RESET_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_LINK_RESET_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_10G_LINK_RESET_get", N, 11);
    /* ((0x00000024 + (N) * 0x04) bits 0) field ING_10G_LINK_RESET of register PMC_EPMM120_REG_ING_10G_LINK_CFG_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_RESET_MSK) >> EPMM120_REG_ING_10G_LINK_CFG_REG_BIT_ING_10G_LINK_RESET_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_10G_LINK_RESET_get", N, value );

    return value;
}
static INLINE void epmm_field_ING_CBR10G_OS_FORCE_EN_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_CBR10G_OS_FORCE_EN_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_CBR10G_OS_FORCE_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_CBR10G_OS_FORCE_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_CBR10G_OS_FORCE_EN_set", N, value );

    /* ((0x00000064 + (N) * 0x4) bits 4) field ING_CBR10G_OS_FORCE_EN of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_OS_FORCE_EN_MSK,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_OS_FORCE_EN_OFF,
                                                      value);
}

static INLINE UINT32 epmm_field_ING_CBR10G_OS_FORCE_EN_get( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_CBR10G_OS_FORCE_EN_get( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_CBR10G_OS_FORCE_EN_get", N, 11);
    /* ((0x00000064 + (N) * 0x4) bits 4) field ING_CBR10G_OS_FORCE_EN of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_OS_FORCE_EN_MSK) >> EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_OS_FORCE_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_CBR10G_OS_FORCE_EN_get", N, value );

    return value;
}
static INLINE void epmm_field_ING_GSUP10G_FORCE_RF_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_GSUP10G_FORCE_RF_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_GSUP10G_FORCE_RF_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_GSUP10G_FORCE_RF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_GSUP10G_FORCE_RF_set", N, value );

    /* ((0x00000064 + (N) * 0x4) bits 3) field ING_GSUP10G_FORCE_RF of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_GSUP10G_FORCE_RF_MSK,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_GSUP10G_FORCE_RF_OFF,
                                                      value);
}

static INLINE UINT32 epmm_field_ING_GSUP10G_FORCE_RF_get( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_GSUP10G_FORCE_RF_get( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_GSUP10G_FORCE_RF_get", N, 11);
    /* ((0x00000064 + (N) * 0x4) bits 3) field ING_GSUP10G_FORCE_RF of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_GSUP10G_FORCE_RF_MSK) >> EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_GSUP10G_FORCE_RF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_GSUP10G_FORCE_RF_get", N, value );

    return value;
}
static INLINE void epmm_field_ING_GSUP10G_FORCE_LF_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_GSUP10G_FORCE_LF_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_GSUP10G_FORCE_LF_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_GSUP10G_FORCE_LF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_GSUP10G_FORCE_LF_set", N, value );

    /* ((0x00000064 + (N) * 0x4) bits 2) field ING_GSUP10G_FORCE_LF of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_GSUP10G_FORCE_LF_MSK,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_GSUP10G_FORCE_LF_OFF,
                                                      value);
}

static INLINE UINT32 epmm_field_ING_GSUP10G_FORCE_LF_get( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_GSUP10G_FORCE_LF_get( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_GSUP10G_FORCE_LF_get", N, 11);
    /* ((0x00000064 + (N) * 0x4) bits 2) field ING_GSUP10G_FORCE_LF of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_GSUP10G_FORCE_LF_MSK) >> EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_GSUP10G_FORCE_LF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_GSUP10G_FORCE_LF_get", N, value );

    return value;
}
static INLINE void epmm_field_ING_CBR10G_FORCE_RF_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_CBR10G_FORCE_RF_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_CBR10G_FORCE_RF_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_CBR10G_FORCE_RF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_CBR10G_FORCE_RF_set", N, value );

    /* ((0x00000064 + (N) * 0x4) bits 1) field ING_CBR10G_FORCE_RF of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_FORCE_RF_MSK,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_FORCE_RF_OFF,
                                                      value);
}

static INLINE UINT32 epmm_field_ING_CBR10G_FORCE_RF_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_CBR10G_FORCE_RF_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_CBR10G_FORCE_RF_get", N, 11);
    /* ((0x00000064 + (N) * 0x4) bits 1) field ING_CBR10G_FORCE_RF of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_FORCE_RF_MSK) >> EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_FORCE_RF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_CBR10G_FORCE_RF_get", N, value );

    return value;
}
static INLINE void epmm_field_ING_CBR10G_FORCE_LF_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_CBR10G_FORCE_LF_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_CBR10G_FORCE_LF_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_CBR10G_FORCE_LF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_CBR10G_FORCE_LF_set", N, value );

    /* ((0x00000064 + (N) * 0x4) bits 0) field ING_CBR10G_FORCE_LF of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_FORCE_LF_MSK,
                                                      EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_FORCE_LF_OFF,
                                                      value);
}

static INLINE UINT32 epmm_field_ING_CBR10G_FORCE_LF_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_CBR10G_FORCE_LF_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_CBR10G_FORCE_LF_get", N, 11);
    /* ((0x00000064 + (N) * 0x4) bits 0) field ING_CBR10G_FORCE_LF of register PMC_EPMM120_REG_ING_10G_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_FORCE_LF_MSK) >> EPMM120_REG_ING_10G_LF_RF_OS_EN_REG_BIT_ING_CBR10G_FORCE_LF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_CBR10G_FORCE_LF_get", N, value );

    return value;
}
static INLINE void epmm_field_EGR_10G_LINK_DP_SEL_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_LINK_DP_SEL_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_10G_LINK_DP_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_LINK_DP_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_10G_LINK_DP_SEL_set", N, value );

    /* ((0x000000f4 + (N) * 0x04) bits 3:2) field EGR_10G_LINK_DP_SEL of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
    epmm_reg_EGR_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_MSK,
                                                   EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_EGR_10G_LINK_DP_SEL_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_LINK_DP_SEL_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_10G_LINK_DP_SEL_get", N, 11);
    /* ((0x000000f4 + (N) * 0x04) bits 3:2) field EGR_10G_LINK_DP_SEL of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
    reg_value = epmm_reg_EGR_10G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_MSK) >> EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_10G_LINK_DP_SEL_get", N, value );

    return value;
}
static INLINE void epmm_field_range_EGR_10G_LINK_DP_SEL_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_10G_LINK_DP_SEL_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_range_EGR_10G_LINK_DP_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_LINK_DP_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_LINK_DP_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_LINK_DP_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000000f4 + (N) * 0x04) bits 3:2) field EGR_10G_LINK_DP_SEL of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
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
        /* ((0x000000f4 + (N) * 0x04) bits 3:2) field EGR_10G_LINK_DP_SEL of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
        epmm_reg_EGR_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_OFF + subfield_offset),
                                                       EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_10G_LINK_DP_SEL_get( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_10G_LINK_DP_SEL_get( epmm_buffer_t *b_ptr,
                                                               epmm_handle_t *h_ptr,
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

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_range_EGR_10G_LINK_DP_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_LINK_DP_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_LINK_DP_SEL_get", stop_bit, 1 );
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
    /* ((0x000000f4 + (N) * 0x04) bits 3:2) field EGR_10G_LINK_DP_SEL of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
    reg_value = epmm_reg_EGR_10G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_MSK)
                  >> EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_MSK, EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_DP_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_LINK_DP_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_EGR_10G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_10G_LINK_ENABLE_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_LINK_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_10G_LINK_ENABLE_set", N, value );

    /* ((0x000000f4 + (N) * 0x04) bits 1) field EGR_10G_LINK_ENABLE of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
    epmm_reg_EGR_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_ENABLE_MSK,
                                                   EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_ENABLE_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_EGR_10G_LINK_ENABLE_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_LINK_ENABLE_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_10G_LINK_ENABLE_get", N, 11);
    /* ((0x000000f4 + (N) * 0x04) bits 1) field EGR_10G_LINK_ENABLE of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
    reg_value = epmm_reg_EGR_10G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_ENABLE_MSK) >> EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_10G_LINK_ENABLE_get", N, value );

    return value;
}
static INLINE void epmm_field_EGR_10G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_10G_LINK_RESET_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_LINK_RESET_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_10G_LINK_RESET_set", N, value );

    /* ((0x000000f4 + (N) * 0x04) bits 0) field EGR_10G_LINK_RESET of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
    epmm_reg_EGR_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_RESET_MSK,
                                                   EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_RESET_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_EGR_10G_LINK_RESET_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_LINK_RESET_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_10G_LINK_RESET_get", N, 11);
    /* ((0x000000f4 + (N) * 0x04) bits 0) field EGR_10G_LINK_RESET of register PMC_EPMM120_REG_EGR_10G_LINK_CFG_REG index N=0..11 */
    reg_value = epmm_reg_EGR_10G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_RESET_MSK) >> EPMM120_REG_EGR_10G_LINK_CFG_REG_BIT_EGR_10G_LINK_RESET_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_10G_LINK_RESET_get", N, value );

    return value;
}
static INLINE void epmm_field_EGR_CBR10G_OS_FORCE_EN_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_CBR10G_OS_FORCE_EN_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_CBR10G_OS_FORCE_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_CBR10G_OS_FORCE_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_CBR10G_OS_FORCE_EN_set", N, value );

    /* ((0x00000134 + (N) * 0x4) bits 5) field EGR_CBR10G_OS_FORCE_EN of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_OS_FORCE_EN_MSK,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_OS_FORCE_EN_OFF,
                                                  value);
}

static INLINE UINT32 epmm_field_EGR_CBR10G_OS_FORCE_EN_get( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_CBR10G_OS_FORCE_EN_get( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_CBR10G_OS_FORCE_EN_get", N, 11);
    /* ((0x00000134 + (N) * 0x4) bits 5) field EGR_CBR10G_OS_FORCE_EN of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_EGR_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_OS_FORCE_EN_MSK) >> EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_OS_FORCE_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_CBR10G_OS_FORCE_EN_get", N, value );

    return value;
}
static INLINE void epmm_field_EGR_GSUP_TX_SW_FORCE_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_GSUP_TX_SW_FORCE_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_GSUP_TX_SW_FORCE_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_GSUP_TX_SW_FORCE_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_GSUP_TX_SW_FORCE_set", N, value );

    /* ((0x00000134 + (N) * 0x4) bits 4:3) field EGR_GSUP_TX_SW_FORCE of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_MSK,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_OFF,
                                                  value);
}

static INLINE UINT32 epmm_field_EGR_GSUP_TX_SW_FORCE_get( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_GSUP_TX_SW_FORCE_get( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_GSUP_TX_SW_FORCE_get", N, 11);
    /* ((0x00000134 + (N) * 0x4) bits 4:3) field EGR_GSUP_TX_SW_FORCE of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_EGR_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_MSK) >> EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_GSUP_TX_SW_FORCE_get", N, value );

    return value;
}
static INLINE void epmm_field_range_EGR_GSUP_TX_SW_FORCE_set( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_GSUP_TX_SW_FORCE_set( epmm_buffer_t *b_ptr,
                                                              epmm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_range_EGR_GSUP_TX_SW_FORCE_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_GSUP_TX_SW_FORCE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_GSUP_TX_SW_FORCE_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_GSUP_TX_SW_FORCE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000134 + (N) * 0x4) bits 4:3) field EGR_GSUP_TX_SW_FORCE of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
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
        /* ((0x00000134 + (N) * 0x4) bits 4:3) field EGR_GSUP_TX_SW_FORCE of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
        epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_OFF + subfield_offset),
                                                      EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_GSUP_TX_SW_FORCE_get( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_GSUP_TX_SW_FORCE_get( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
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

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_range_EGR_GSUP_TX_SW_FORCE_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_GSUP_TX_SW_FORCE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_GSUP_TX_SW_FORCE_get", stop_bit, 1 );
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
    /* ((0x00000134 + (N) * 0x4) bits 4:3) field EGR_GSUP_TX_SW_FORCE of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_EGR_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_MSK)
                  >> EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_MSK, EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_TX_SW_FORCE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_GSUP_TX_SW_FORCE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_EGR_GSUP_CNSQ_ACT_EN_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_GSUP_CNSQ_ACT_EN_set( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_GSUP_CNSQ_ACT_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_GSUP_CNSQ_ACT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_GSUP_CNSQ_ACT_EN_set", N, value );

    /* ((0x00000134 + (N) * 0x4) bits 2) field EGR_GSUP_CNSQ_ACT_EN of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_CNSQ_ACT_EN_MSK,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_CNSQ_ACT_EN_OFF,
                                                  value);
}

static INLINE UINT32 epmm_field_EGR_GSUP_CNSQ_ACT_EN_get( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_GSUP_CNSQ_ACT_EN_get( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_GSUP_CNSQ_ACT_EN_get", N, 11);
    /* ((0x00000134 + (N) * 0x4) bits 2) field EGR_GSUP_CNSQ_ACT_EN of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_EGR_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_CNSQ_ACT_EN_MSK) >> EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_GSUP_CNSQ_ACT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_GSUP_CNSQ_ACT_EN_get", N, value );

    return value;
}
static INLINE void epmm_field_EGR_CBR10G_FORCE_RF_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_CBR10G_FORCE_RF_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_CBR10G_FORCE_RF_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_CBR10G_FORCE_RF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_CBR10G_FORCE_RF_set", N, value );

    /* ((0x00000134 + (N) * 0x4) bits 1) field EGR_CBR10G_FORCE_RF of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_FORCE_RF_MSK,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_FORCE_RF_OFF,
                                                  value);
}

static INLINE UINT32 epmm_field_EGR_CBR10G_FORCE_RF_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_CBR10G_FORCE_RF_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_CBR10G_FORCE_RF_get", N, 11);
    /* ((0x00000134 + (N) * 0x4) bits 1) field EGR_CBR10G_FORCE_RF of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_EGR_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_FORCE_RF_MSK) >> EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_FORCE_RF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_CBR10G_FORCE_RF_get", N, value );

    return value;
}
static INLINE void epmm_field_EGR_CBR10G_FORCE_LF_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_CBR10G_FORCE_LF_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_CBR10G_FORCE_LF_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_CBR10G_FORCE_LF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_CBR10G_FORCE_LF_set", N, value );

    /* ((0x00000134 + (N) * 0x4) bits 0) field EGR_CBR10G_FORCE_LF of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_FORCE_LF_MSK,
                                                  EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_FORCE_LF_OFF,
                                                  value);
}

static INLINE UINT32 epmm_field_EGR_CBR10G_FORCE_LF_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_CBR10G_FORCE_LF_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_CBR10G_FORCE_LF_get", N, 11);
    /* ((0x00000134 + (N) * 0x4) bits 0) field EGR_CBR10G_FORCE_LF of register PMC_EPMM120_REG_EGR_LF_RF_OS_EN_REG index N=0..11 */
    reg_value = epmm_reg_EGR_LF_RF_OS_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_FORCE_LF_MSK) >> EPMM120_REG_EGR_LF_RF_OS_EN_REG_BIT_EGR_CBR10G_FORCE_LF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_CBR10G_FORCE_LF_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size3
 * ==================================================================================
 */
static INLINE void epmm_field_ING_40G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_40G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_40G_LINK_ENABLE_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_40G_LINK_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_40G_LINK_ENABLE_set", N, value );

    /* ((0x00000054 + (N) * 0x04) bits 1) field ING_40G_LINK_ENABLE of register PMC_EPMM120_REG_ING_40G_LINK_CFG_REG index N=0..2 */
    epmm_reg_ING_40G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_ING_40G_LINK_CFG_REG_BIT_ING_40G_LINK_ENABLE_MSK,
                                                   EPMM120_REG_ING_40G_LINK_CFG_REG_BIT_ING_40G_LINK_ENABLE_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_ING_40G_LINK_ENABLE_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_40G_LINK_ENABLE_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_40G_LINK_ENABLE_get", N, 2);
    /* ((0x00000054 + (N) * 0x04) bits 1) field ING_40G_LINK_ENABLE of register PMC_EPMM120_REG_ING_40G_LINK_CFG_REG index N=0..2 */
    reg_value = epmm_reg_ING_40G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_ING_40G_LINK_CFG_REG_BIT_ING_40G_LINK_ENABLE_MSK) >> EPMM120_REG_ING_40G_LINK_CFG_REG_BIT_ING_40G_LINK_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_40G_LINK_ENABLE_get", N, value );

    return value;
}
static INLINE void epmm_field_ING_40G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_40G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_40G_LINK_RESET_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_40G_LINK_RESET_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_ING_40G_LINK_RESET_set", N, value );

    /* ((0x00000054 + (N) * 0x04) bits 0) field ING_40G_LINK_RESET of register PMC_EPMM120_REG_ING_40G_LINK_CFG_REG index N=0..2 */
    epmm_reg_ING_40G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_ING_40G_LINK_CFG_REG_BIT_ING_40G_LINK_RESET_MSK,
                                                   EPMM120_REG_ING_40G_LINK_CFG_REG_BIT_ING_40G_LINK_RESET_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_ING_40G_LINK_RESET_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_40G_LINK_RESET_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_ING_40G_LINK_RESET_get", N, 2);
    /* ((0x00000054 + (N) * 0x04) bits 0) field ING_40G_LINK_RESET of register PMC_EPMM120_REG_ING_40G_LINK_CFG_REG index N=0..2 */
    reg_value = epmm_reg_ING_40G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_ING_40G_LINK_CFG_REG_BIT_ING_40G_LINK_RESET_MSK) >> EPMM120_REG_ING_40G_LINK_CFG_REG_BIT_ING_40G_LINK_RESET_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_ING_40G_LINK_RESET_get", N, value );

    return value;
}
static INLINE void epmm_field_EGR_40G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_40G_LINK_ENABLE_set( epmm_buffer_t *b_ptr,
                                                       epmm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_40G_LINK_ENABLE_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_40G_LINK_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_40G_LINK_ENABLE_set", N, value );

    /* ((0x00000124 + (N) * 0x04) bits 1) field EGR_40G_LINK_ENABLE of register PMC_EPMM120_REG_EGR_40G_LINK_CFG_REG index N=0..2 */
    epmm_reg_EGR_40G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_EGR_40G_LINK_CFG_REG_BIT_EGR_40G_LINK_ENABLE_MSK,
                                                   EPMM120_REG_EGR_40G_LINK_CFG_REG_BIT_EGR_40G_LINK_ENABLE_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_EGR_40G_LINK_ENABLE_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_40G_LINK_ENABLE_get( epmm_buffer_t *b_ptr,
                                                         epmm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_40G_LINK_ENABLE_get", N, 2);
    /* ((0x00000124 + (N) * 0x04) bits 1) field EGR_40G_LINK_ENABLE of register PMC_EPMM120_REG_EGR_40G_LINK_CFG_REG index N=0..2 */
    reg_value = epmm_reg_EGR_40G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_EGR_40G_LINK_CFG_REG_BIT_EGR_40G_LINK_ENABLE_MSK) >> EPMM120_REG_EGR_40G_LINK_CFG_REG_BIT_EGR_40G_LINK_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_40G_LINK_ENABLE_get", N, value );

    return value;
}
static INLINE void epmm_field_EGR_40G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_40G_LINK_RESET_set( epmm_buffer_t *b_ptr,
                                                      epmm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_40G_LINK_RESET_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_40G_LINK_RESET_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "epmm_field_EGR_40G_LINK_RESET_set", N, value );

    /* ((0x00000124 + (N) * 0x04) bits 0) field EGR_40G_LINK_RESET of register PMC_EPMM120_REG_EGR_40G_LINK_CFG_REG index N=0..2 */
    epmm_reg_EGR_40G_LINK_CFG_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   EPMM120_REG_EGR_40G_LINK_CFG_REG_BIT_EGR_40G_LINK_RESET_MSK,
                                                   EPMM120_REG_EGR_40G_LINK_CFG_REG_BIT_EGR_40G_LINK_RESET_OFF,
                                                   value);
}

static INLINE UINT32 epmm_field_EGR_40G_LINK_RESET_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_40G_LINK_RESET_get( epmm_buffer_t *b_ptr,
                                                        epmm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "epmm_field_EGR_40G_LINK_RESET_get", N, 2);
    /* ((0x00000124 + (N) * 0x04) bits 0) field EGR_40G_LINK_RESET of register PMC_EPMM120_REG_EGR_40G_LINK_CFG_REG index N=0..2 */
    reg_value = epmm_reg_EGR_40G_LINK_CFG_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & EPMM120_REG_EGR_40G_LINK_CFG_REG_BIT_EGR_40G_LINK_RESET_MSK) >> EPMM120_REG_EGR_40G_LINK_CFG_REG_BIT_EGR_40G_LINK_RESET_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "epmm_field_EGR_40G_LINK_RESET_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void epmm_field_ING_10G_GSUP11_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP11_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 11) field ING_10G_GSUP11_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP11_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP11_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP11_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP11_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 11) field ING_10G_GSUP11_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP11_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP11_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP10_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP10_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 10) field ING_10G_GSUP10_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP10_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP10_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP10_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP10_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 10) field ING_10G_GSUP10_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP10_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP10_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP9_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP9_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 9) field ING_10G_GSUP9_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP9_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP9_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP9_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP9_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 9) field ING_10G_GSUP9_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP9_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP9_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP8_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP8_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 8) field ING_10G_GSUP8_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP8_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP8_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP8_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP8_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 8) field ING_10G_GSUP8_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP8_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP8_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP7_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP7_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 7) field ING_10G_GSUP7_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP7_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP7_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP7_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP7_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 7) field ING_10G_GSUP7_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP7_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP7_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP6_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP6_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 6) field ING_10G_GSUP6_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP6_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP6_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP6_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP6_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 6) field ING_10G_GSUP6_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP6_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP6_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP5_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP5_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 5) field ING_10G_GSUP5_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP5_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP5_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP5_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP5_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 5) field ING_10G_GSUP5_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP5_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP5_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP4_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP4_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 4) field ING_10G_GSUP4_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP4_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP4_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP4_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP4_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 4) field ING_10G_GSUP4_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP4_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP4_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP3_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP3_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 3) field ING_10G_GSUP3_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP3_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP3_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP3_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP3_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 3) field ING_10G_GSUP3_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP3_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP3_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP2_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP2_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 2) field ING_10G_GSUP2_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP2_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP2_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP2_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP2_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 2) field ING_10G_GSUP2_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP2_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP2_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP1_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP1_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 1) field ING_10G_GSUP1_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP1_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP1_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP1_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP1_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 1) field ING_10G_GSUP1_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP1_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP1_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP0_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP0_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_E_set", value );

    /* (0x00000094 bits 0) field ING_10G_GSUP0_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    epmm_reg_ING_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP0_FIFO_OVR_E_MSK,
                                            EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP0_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP0_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP0_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 0) field ING_10G_GSUP0_FIFO_OVR_E of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP0_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_EN_BIT_ING_10G_GSUP0_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_GSUP_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_E_set", value );

    /* (0x00000098 bits 11:0) field ING_10G_GSUP_FIFO_UDR_E of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN */
    epmm_reg_ING_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_MSK,
                                            EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000098 bits 11:0) field ING_10G_GSUP_FIFO_UDR_E of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_MSK) >> EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                                 epmm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                                 epmm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000098 bits 11:0) field ING_10G_GSUP_FIFO_UDR_E of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN */
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
        /* (0x00000098 bits 11:0) field ING_10G_GSUP_FIFO_UDR_E of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN */
        epmm_reg_ING_GSUP_UDR_INT_EN_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_OFF + subfield_offset),
                                                EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_get", stop_bit, 11 );
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
    /* (0x00000098 bits 11:0) field ING_10G_GSUP_FIFO_UDR_E of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_ING_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_MSK)
                  >> EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_MSK, EPMM120_REG_ING_GSUP_UDR_INT_EN_BIT_ING_10G_GSUP_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_ING_100G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_100G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 15) field ING_100G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_100G_CBR_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_100G_CBR_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 15) field ING_100G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_100G_CBR_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_100G_CBR_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_40G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_40G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 14:12) field ING_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 14:12) field ING_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_range_ING_40G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_40G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_OVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000c4 bits 14:12) field ING_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
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
        /* (0x000000c4 bits 14:12) field ING_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
        epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_OFF + subfield_offset),
                                               EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_E_get", stop_bit, 2 );
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
    /* (0x000000c4 bits 14:12) field ING_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_MSK)
                  >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_MSK, EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_40G_CBR_FIFO_OVR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_OVR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR11_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR11_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 11) field ING_10G_CBR11_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR11_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR11_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR11_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR11_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 11) field ING_10G_CBR11_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR11_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR11_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR10_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR10_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 10) field ING_10G_CBR10_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR10_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR10_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR10_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR10_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 10) field ING_10G_CBR10_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR10_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR10_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR9_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR9_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 9) field ING_10G_CBR9_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR9_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR9_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR9_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR9_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 9) field ING_10G_CBR9_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR9_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR9_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR8_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR8_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 8) field ING_10G_CBR8_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR8_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR8_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR8_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR8_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 8) field ING_10G_CBR8_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR8_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR8_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR7_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR7_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 7) field ING_10G_CBR7_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR7_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR7_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR7_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR7_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 7) field ING_10G_CBR7_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR7_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR7_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR6_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR6_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 6) field ING_10G_CBR6_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR6_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR6_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR6_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR6_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 6) field ING_10G_CBR6_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR6_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR6_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR5_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR5_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 5) field ING_10G_CBR5_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR5_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR5_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR5_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR5_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 5) field ING_10G_CBR5_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR5_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR5_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR4_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR4_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 4) field ING_10G_CBR4_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR4_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR4_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR4_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR4_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 4) field ING_10G_CBR4_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR4_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR4_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR3_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR3_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 3) field ING_10G_CBR3_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR3_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR3_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR3_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR3_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 3) field ING_10G_CBR3_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR3_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR3_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR2_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR2_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 2) field ING_10G_CBR2_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR2_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR2_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR2_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR2_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 2) field ING_10G_CBR2_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR2_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR2_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR1_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR1_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 1) field ING_10G_CBR1_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR1_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR1_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR1_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR1_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 1) field ING_10G_CBR1_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR1_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR1_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR0_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR0_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_E_set", value );

    /* (0x000000c4 bits 0) field ING_10G_CBR0_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    epmm_reg_ING_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR0_FIFO_OVR_E_MSK,
                                           EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR0_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR0_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR0_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c4 bits 0) field ING_10G_CBR0_FIFO_OVR_E of register PMC_EPMM120_REG_ING_CBR_OVR_INT_EN */
    reg_value = epmm_reg_ING_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR0_FIFO_OVR_E_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_EN_BIT_ING_10G_CBR0_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_100G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_100G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_E_set", value );

    /* (0x000000c8 bits 15) field ING_100G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
    epmm_reg_ING_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_100G_CBR_FIFO_UDR_E_MSK,
                                           EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_100G_CBR_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c8 bits 15) field ING_100G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
    reg_value = epmm_reg_ING_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_100G_CBR_FIFO_UDR_E_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_100G_CBR_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_ING_40G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_40G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_E_set", value );

    /* (0x000000c8 bits 14:12) field ING_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
    epmm_reg_ING_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_MSK,
                                           EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c8 bits 14:12) field ING_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
    reg_value = epmm_reg_ING_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_range_ING_40G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_40G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000c8 bits 14:12) field ING_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
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
        /* (0x000000c8 bits 14:12) field ING_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
        epmm_reg_ING_CBR_UDR_INT_EN_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_OFF + subfield_offset),
                                               EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_E_get", stop_bit, 2 );
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
    /* (0x000000c8 bits 14:12) field ING_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
    reg_value = epmm_reg_ING_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_MSK)
                  >> EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_MSK, EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_40G_CBR_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_ING_10G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_E_set", value );

    /* (0x000000c8 bits 11:0) field ING_10G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
    epmm_reg_ING_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_MSK,
                                           EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000c8 bits 11:0) field ING_10G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
    reg_value = epmm_reg_ING_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_range_ING_10G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_10G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_CBR_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000c8 bits 11:0) field ING_10G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
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
        /* (0x000000c8 bits 11:0) field ING_10G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
        epmm_reg_ING_CBR_UDR_INT_EN_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_OFF + subfield_offset),
                                               EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_10G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_10G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_E_get", stop_bit, 11 );
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
    /* (0x000000c8 bits 11:0) field ING_10G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_ING_CBR_UDR_INT_EN */
    reg_value = epmm_reg_ING_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_MSK)
                  >> EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_MSK, EPMM120_REG_ING_CBR_UDR_INT_EN_BIT_ING_10G_CBR_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_CBR_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_E_set", value );

    /* (0x00000164 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN */
    epmm_reg_EGR_GSUP_OVR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000164 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_MSK) >> EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                                 epmm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                                 epmm_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000164 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN */
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
        /* (0x00000164 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN */
        epmm_reg_EGR_GSUP_OVR_INT_EN_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_OFF + subfield_offset),
                                                EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_get", stop_bit, 11 );
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
    /* (0x00000164 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_OVR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_MSK)
                  >> EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_MSK, EPMM120_REG_EGR_GSUP_OVR_INT_EN_BIT_EGR_10G_GSUP_FIFO_OVR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 11) field EGR_10G_GSUP11_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP11_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP11_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 11) field EGR_10G_GSUP11_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP11_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP11_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                             epmm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 10) field EGR_10G_GSUP10_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP10_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP10_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 10) field EGR_10G_GSUP10_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP10_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP10_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 9) field EGR_10G_GSUP9_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP9_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP9_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 9) field EGR_10G_GSUP9_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP9_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP9_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 8) field EGR_10G_GSUP8_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP8_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP8_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 8) field EGR_10G_GSUP8_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP8_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP8_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 7) field EGR_10G_GSUP7_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP7_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP7_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 7) field EGR_10G_GSUP7_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP7_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP7_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 6) field EGR_10G_GSUP6_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP6_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP6_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 6) field EGR_10G_GSUP6_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP6_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP6_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 5) field EGR_10G_GSUP5_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP5_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP5_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 5) field EGR_10G_GSUP5_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP5_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP5_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 4) field EGR_10G_GSUP4_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP4_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP4_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 4) field EGR_10G_GSUP4_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP4_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP4_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 3) field EGR_10G_GSUP3_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP3_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP3_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 3) field EGR_10G_GSUP3_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP3_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP3_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 2) field EGR_10G_GSUP2_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP2_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP2_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 2) field EGR_10G_GSUP2_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP2_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP2_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 1) field EGR_10G_GSUP1_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP1_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP1_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 1) field EGR_10G_GSUP1_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP1_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP1_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_set", value );

    /* (0x00000168 bits 0) field EGR_10G_GSUP0_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    epmm_reg_EGR_GSUP_UDR_INT_EN_field_set( b_ptr,
                                            h_ptr,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP0_FIFO_UDR_E_MSK,
                                            EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP0_FIFO_UDR_E_OFF,
                                            value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000168 bits 0) field EGR_10G_GSUP0_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_EN */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP0_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_EN_BIT_EGR_10G_GSUP0_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_100G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_100G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_E_set", value );

    /* (0x00000194 bits 15) field EGR_100G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
    epmm_reg_EGR_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_100G_CBR_FIFO_OVR_E_MSK,
                                           EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_100G_CBR_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000194 bits 15) field EGR_100G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_100G_CBR_FIFO_OVR_E_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_100G_CBR_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_40G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_40G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_E_set", value );

    /* (0x00000194 bits 14:12) field EGR_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
    epmm_reg_EGR_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_MSK,
                                           EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000194 bits 14:12) field EGR_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000194 bits 14:12) field EGR_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
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
        /* (0x00000194 bits 14:12) field EGR_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
        epmm_reg_EGR_CBR_OVR_INT_EN_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_OFF + subfield_offset),
                                               EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_get", stop_bit, 2 );
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
    /* (0x00000194 bits 14:12) field EGR_40G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_MSK)
                  >> EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_MSK, EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_40G_CBR_FIFO_OVR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_E_set", value );

    /* (0x00000194 bits 11:0) field EGR_10G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
    epmm_reg_EGR_CBR_OVR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_MSK,
                                           EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000194 bits 11:0) field EGR_10G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_E_get", value );

    return value;
}
static INLINE void epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000194 bits 11:0) field EGR_10G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
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
        /* (0x00000194 bits 11:0) field EGR_10G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
        epmm_reg_EGR_CBR_OVR_INT_EN_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_OFF + subfield_offset),
                                               EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_get", stop_bit, 11 );
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
    /* (0x00000194 bits 11:0) field EGR_10G_CBR_FIFO_OVR_E of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_MSK)
                  >> EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_MSK, EPMM120_REG_EGR_CBR_OVR_INT_EN_BIT_EGR_10G_CBR_FIFO_OVR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_EGR_100G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_100G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 15) field EGR_100G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_100G_CBR_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_100G_CBR_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 15) field EGR_100G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_100G_CBR_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_100G_CBR_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_40G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_40G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                          epmm_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 14:12) field EGR_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 14:12) field EGR_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                                epmm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000198 bits 14:12) field EGR_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
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
        /* (0x00000198 bits 14:12) field EGR_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
        epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_OFF + subfield_offset),
                                               EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_get", stop_bit, 2 );
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
    /* (0x00000198 bits 14:12) field EGR_40G_CBR_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_MSK)
                  >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_MSK, EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_40G_CBR_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR11_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR11_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 11) field EGR_10G_CBR11_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR11_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR11_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR11_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR11_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 11) field EGR_10G_CBR11_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR11_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR11_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR10_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR10_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                            epmm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 10) field EGR_10G_CBR10_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR10_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR10_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR10_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR10_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 10) field EGR_10G_CBR10_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR10_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR10_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR9_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR9_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 9) field EGR_10G_CBR9_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR9_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR9_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR9_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR9_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 9) field EGR_10G_CBR9_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR9_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR9_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR8_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR8_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 8) field EGR_10G_CBR8_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR8_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR8_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR8_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR8_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 8) field EGR_10G_CBR8_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR8_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR8_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR7_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR7_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 7) field EGR_10G_CBR7_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR7_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR7_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR7_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR7_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 7) field EGR_10G_CBR7_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR7_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR7_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR6_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR6_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 6) field EGR_10G_CBR6_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR6_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR6_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR6_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR6_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 6) field EGR_10G_CBR6_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR6_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR6_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR5_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR5_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 5) field EGR_10G_CBR5_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR5_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR5_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR5_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR5_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 5) field EGR_10G_CBR5_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR5_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR5_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR4_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR4_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 4) field EGR_10G_CBR4_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR4_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR4_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR4_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR4_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 4) field EGR_10G_CBR4_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR4_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR4_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR3_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR3_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 3) field EGR_10G_CBR3_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR3_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR3_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR3_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR3_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 3) field EGR_10G_CBR3_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR3_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR3_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR2_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR2_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 2) field EGR_10G_CBR2_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR2_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR2_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR2_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR2_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 2) field EGR_10G_CBR2_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR2_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR2_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR1_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR1_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 1) field EGR_10G_CBR1_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR1_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR1_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR1_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR1_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 1) field EGR_10G_CBR1_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR1_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR1_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void epmm_field_EGR_10G_CBR0_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR0_FIFO_UDR_E_set( epmm_buffer_t *b_ptr,
                                                           epmm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_E_set", value );

    /* (0x00000198 bits 0) field EGR_10G_CBR0_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    epmm_reg_EGR_CBR_UDR_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR0_FIFO_UDR_E_MSK,
                                           EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR0_FIFO_UDR_E_OFF,
                                           value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR0_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR0_FIFO_UDR_E_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000198 bits 0) field EGR_10G_CBR0_FIFO_UDR_E of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_EN */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR0_FIFO_UDR_E_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_EN_BIT_EGR_10G_CBR0_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void epmm_field_ING_10G_GSUP11_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP11_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 11) field ING_10G_GSUP11_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP11_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP11_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP11_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP11_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 11) field ING_10G_GSUP11_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP11_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP11_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP11_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP11_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 11) field ING_10G_GSUP11_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP11_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP11_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP10_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP10_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 10) field ING_10G_GSUP10_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP10_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP10_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP10_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP10_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 10) field ING_10G_GSUP10_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP10_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP10_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP10_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP10_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 10) field ING_10G_GSUP10_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP10_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP10_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP9_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP9_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 9) field ING_10G_GSUP9_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP9_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP9_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP9_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP9_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 9) field ING_10G_GSUP9_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP9_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP9_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP9_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP9_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 9) field ING_10G_GSUP9_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP9_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP9_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP8_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP8_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 8) field ING_10G_GSUP8_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP8_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP8_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP8_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP8_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 8) field ING_10G_GSUP8_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP8_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP8_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP8_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP8_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 8) field ING_10G_GSUP8_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP8_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP8_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP7_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP7_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 7) field ING_10G_GSUP7_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP7_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP7_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP7_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP7_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 7) field ING_10G_GSUP7_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP7_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP7_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP7_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP7_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 7) field ING_10G_GSUP7_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP7_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP7_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP6_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP6_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 6) field ING_10G_GSUP6_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP6_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP6_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP6_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP6_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 6) field ING_10G_GSUP6_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP6_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP6_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP6_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP6_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 6) field ING_10G_GSUP6_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP6_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP6_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP5_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP5_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 5) field ING_10G_GSUP5_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP5_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP5_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP5_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP5_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 5) field ING_10G_GSUP5_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP5_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP5_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP5_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP5_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 5) field ING_10G_GSUP5_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP5_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP5_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP4_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP4_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 4) field ING_10G_GSUP4_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP4_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP4_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP4_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP4_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 4) field ING_10G_GSUP4_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP4_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP4_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP4_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP4_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 4) field ING_10G_GSUP4_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP4_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP4_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP3_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP3_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 3) field ING_10G_GSUP3_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP3_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP3_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP3_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP3_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 3) field ING_10G_GSUP3_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP3_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP3_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP3_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP3_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 3) field ING_10G_GSUP3_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP3_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP3_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP2_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP2_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 2) field ING_10G_GSUP2_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP2_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP2_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP2_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP2_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 2) field ING_10G_GSUP2_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP2_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP2_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP2_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP2_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 2) field ING_10G_GSUP2_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP2_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP2_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP1_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP1_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 1) field ING_10G_GSUP1_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP1_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP1_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP1_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP1_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 1) field ING_10G_GSUP1_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP1_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP1_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP1_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP1_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 1) field ING_10G_GSUP1_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP1_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP1_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP0_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP0_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000a4 bits 0) field ING_10G_GSUP0_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP0_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP0_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP0_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP0_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 0) field ING_10G_GSUP0_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP0_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP0_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP0_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP0_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_I_poll", value );

    /* (0x000000a4 bits 0) field ING_10G_GSUP0_FIFO_OVR_I of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_STAT */
    return epmm_reg_ING_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP0_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_OVR_INT_STAT_BIT_ING_10G_GSUP0_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_GSUP_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_GSUP_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_I_set_to_clear", value );

    /* (0x000000a8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_I of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT */
    epmm_reg_ING_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_I of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_MSK) >> EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                          epmm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                          epmm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_I of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT */
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
        /* (0x000000a8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_I of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT */
        epmm_reg_ING_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF + subfield_offset),
                                                                  EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_get", stop_bit, 11 );
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
    /* (0x000000a8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_I of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_ING_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_MSK)
                  >> EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_MSK, EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                                  epmm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                                  epmm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_I of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000a8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_I of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT */
        return epmm_reg_ING_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF + subfield_offset),
                                                    value << (EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_I_poll", value );

    /* (0x000000a8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_I of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_STAT */
    return epmm_reg_ING_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_ING_GSUP_UDR_INT_STAT_BIT_ING_10G_GSUP_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_100G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_100G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 15) field ING_100G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_100G_CBR_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_100G_CBR_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 15) field ING_100G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_100G_CBR_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_100G_CBR_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 15) field ING_100G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_100G_CBR_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_100G_CBR_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_40G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_40G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 14:12) field ING_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 14:12) field ING_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_I_get", value );

    return value;
}
static INLINE void epmm_field_range_ING_40G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_40G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000d4 bits 14:12) field ING_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
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
        /* (0x000000d4 bits 14:12) field ING_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
        epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                                 EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_get", stop_bit, 2 );
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
    /* (0x000000d4 bits 14:12) field ING_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_MSK)
                  >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_MSK, EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_OVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000d4 bits 14:12) field ING_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000d4 bits 14:12) field ING_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
        return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                   value << (EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 14:12) field ING_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_40G_CBR_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR11_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR11_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 11) field ING_10G_CBR11_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR11_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR11_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR11_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR11_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 11) field ING_10G_CBR11_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR11_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR11_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR11_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR11_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 11) field ING_10G_CBR11_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR11_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR11_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR10_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR10_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 10) field ING_10G_CBR10_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR10_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR10_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR10_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR10_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 10) field ING_10G_CBR10_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR10_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR10_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR10_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR10_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 10) field ING_10G_CBR10_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR10_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR10_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR9_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR9_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 9) field ING_10G_CBR9_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR9_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR9_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR9_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR9_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 9) field ING_10G_CBR9_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR9_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR9_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR9_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR9_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 9) field ING_10G_CBR9_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR9_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR9_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR8_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR8_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 8) field ING_10G_CBR8_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR8_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR8_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR8_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR8_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 8) field ING_10G_CBR8_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR8_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR8_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR8_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR8_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 8) field ING_10G_CBR8_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR8_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR8_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR7_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR7_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 7) field ING_10G_CBR7_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR7_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR7_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR7_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR7_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 7) field ING_10G_CBR7_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR7_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR7_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR7_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR7_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 7) field ING_10G_CBR7_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR7_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR7_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR6_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR6_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 6) field ING_10G_CBR6_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR6_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR6_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR6_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR6_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 6) field ING_10G_CBR6_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR6_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR6_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR6_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR6_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 6) field ING_10G_CBR6_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR6_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR6_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR5_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR5_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 5) field ING_10G_CBR5_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR5_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR5_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR5_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR5_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 5) field ING_10G_CBR5_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR5_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR5_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR5_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR5_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 5) field ING_10G_CBR5_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR5_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR5_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR4_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR4_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 4) field ING_10G_CBR4_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR4_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR4_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR4_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR4_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 4) field ING_10G_CBR4_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR4_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR4_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR4_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR4_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 4) field ING_10G_CBR4_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR4_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR4_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR3_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR3_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 3) field ING_10G_CBR3_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR3_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR3_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR3_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR3_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 3) field ING_10G_CBR3_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR3_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR3_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR3_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR3_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 3) field ING_10G_CBR3_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR3_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR3_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR2_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR2_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 2) field ING_10G_CBR2_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR2_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR2_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR2_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR2_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 2) field ING_10G_CBR2_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR2_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR2_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR2_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR2_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 2) field ING_10G_CBR2_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR2_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR2_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR1_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR1_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 1) field ING_10G_CBR1_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR1_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR1_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR1_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR1_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 1) field ING_10G_CBR1_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR1_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR1_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR1_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR1_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 1) field ING_10G_CBR1_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR1_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR1_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR0_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR0_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_I_set_to_clear", value );

    /* (0x000000d4 bits 0) field ING_10G_CBR0_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR0_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR0_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR0_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR0_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d4 bits 0) field ING_10G_CBR0_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR0_FIFO_OVR_I_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR0_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR0_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR0_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_I_poll", value );

    /* (0x000000d4 bits 0) field ING_10G_CBR0_FIFO_OVR_I of register PMC_EPMM120_REG_ING_CBR_OVR_INT_STAT */
    return epmm_reg_ING_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR0_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_OVR_INT_STAT_BIT_ING_10G_CBR0_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_100G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_100G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_I_set_to_clear", value );

    /* (0x000000d8 bits 15) field ING_100G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_100G_CBR_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_100G_CBR_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d8 bits 15) field ING_100G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_100G_CBR_FIFO_UDR_I_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_100G_CBR_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_I_poll", value );

    /* (0x000000d8 bits 15) field ING_100G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    return epmm_reg_ING_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_100G_CBR_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_100G_CBR_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_40G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_40G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_I_set_to_clear", value );

    /* (0x000000d8 bits 14:12) field ING_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d8 bits 14:12) field ING_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void epmm_field_range_ING_40G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_40G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000d8 bits 14:12) field ING_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
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
        /* (0x000000d8 bits 14:12) field ING_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
        epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                                 EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_get", stop_bit, 2 );
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
    /* (0x000000d8 bits 14:12) field ING_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_MSK)
                  >> EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_MSK, EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000d8 bits 14:12) field ING_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000d8 bits 14:12) field ING_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
        return epmm_reg_ING_CBR_UDR_INT_STAT_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                   value << (EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_I_poll", value );

    /* (0x000000d8 bits 14:12) field ING_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    return epmm_reg_ING_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_40G_CBR_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_ING_10G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_ING_10G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_I_set_to_clear", value );

    /* (0x000000d8 bits 11:0) field ING_10G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_ING_10G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000d8 bits 11:0) field ING_10G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void epmm_field_range_ING_10G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_ING_10G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000d8 bits 11:0) field ING_10G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
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
        /* (0x000000d8 bits 11:0) field ING_10G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
        epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                                 EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_ING_10G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_10G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_get", stop_bit, 11 );
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
    /* (0x000000d8 bits 11:0) field ING_10G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_ING_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_MSK)
                  >> EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_MSK, EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_CBR_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000d8 bits 11:0) field ING_10G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000d8 bits 11:0) field ING_10G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
        return epmm_reg_ING_CBR_UDR_INT_STAT_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                   value << (EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_I_poll", value );

    /* (0x000000d8 bits 11:0) field ING_10G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_ING_CBR_UDR_INT_STAT */
    return epmm_reg_ING_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_ING_CBR_UDR_INT_STAT_BIT_ING_10G_CBR_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear", value );

    /* (0x00000174 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT */
    epmm_reg_EGR_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000174 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_MSK) >> EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_I_get", value );

    return value;
}
static INLINE void epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                          epmm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                          epmm_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000174 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT */
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
        /* (0x00000174 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT */
        epmm_reg_EGR_GSUP_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  subfield_mask << (EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF + subfield_offset),
                                                                  EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_get", stop_bit, 11 );
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
    /* (0x00000174 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_OVR_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_MSK)
                  >> EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_MSK, EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                                  epmm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                                  epmm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000174 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000174 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT */
        return epmm_reg_EGR_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF + subfield_offset),
                                                    value << (EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_I_poll", value );

    /* (0x00000174 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_STAT */
    return epmm_reg_EGR_GSUP_OVR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_OVR_INT_STAT_BIT_EGR_10G_GSUP_FIFO_OVR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 11) field EGR_10G_GSUP11_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP11_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP11_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 11) field EGR_10G_GSUP11_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP11_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP11_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 11) field EGR_10G_GSUP11_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP11_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP11_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                      epmm_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 10) field EGR_10G_GSUP10_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP10_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP10_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 10) field EGR_10G_GSUP10_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP10_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP10_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 10) field EGR_10G_GSUP10_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP10_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP10_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 9) field EGR_10G_GSUP9_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP9_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP9_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 9) field EGR_10G_GSUP9_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP9_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP9_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 9) field EGR_10G_GSUP9_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP9_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP9_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 8) field EGR_10G_GSUP8_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP8_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP8_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 8) field EGR_10G_GSUP8_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP8_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP8_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 8) field EGR_10G_GSUP8_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP8_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP8_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 7) field EGR_10G_GSUP7_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP7_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP7_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 7) field EGR_10G_GSUP7_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP7_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP7_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 7) field EGR_10G_GSUP7_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP7_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP7_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 6) field EGR_10G_GSUP6_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP6_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP6_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 6) field EGR_10G_GSUP6_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP6_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP6_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 6) field EGR_10G_GSUP6_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP6_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP6_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 5) field EGR_10G_GSUP5_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP5_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP5_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 5) field EGR_10G_GSUP5_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP5_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP5_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 5) field EGR_10G_GSUP5_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP5_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP5_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 4) field EGR_10G_GSUP4_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP4_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP4_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 4) field EGR_10G_GSUP4_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP4_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP4_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 4) field EGR_10G_GSUP4_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP4_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP4_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 3) field EGR_10G_GSUP3_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP3_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP3_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 3) field EGR_10G_GSUP3_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP3_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP3_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 3) field EGR_10G_GSUP3_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP3_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP3_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 2) field EGR_10G_GSUP2_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP2_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP2_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 2) field EGR_10G_GSUP2_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP2_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP2_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 2) field EGR_10G_GSUP2_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP2_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP2_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 1) field EGR_10G_GSUP1_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP1_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP1_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 1) field EGR_10G_GSUP1_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP1_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP1_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 1) field EGR_10G_GSUP1_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP1_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP1_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000178 bits 0) field EGR_10G_GSUP0_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP0_FIFO_UDR_I_MSK,
                                                              EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP0_FIFO_UDR_I_OFF,
                                                              value);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000178 bits 0) field EGR_10G_GSUP0_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP0_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP0_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_poll", value );

    /* (0x00000178 bits 0) field EGR_10G_GSUP0_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_STAT */
    return epmm_reg_EGR_GSUP_UDR_INT_STAT_poll( b_ptr,
                                                h_ptr,
                                                EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP0_FIFO_UDR_I_MSK,
                                                (value<<EPMM120_REG_EGR_GSUP_UDR_INT_STAT_BIT_EGR_10G_GSUP0_FIFO_UDR_I_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_100G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_100G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_I_set_to_clear", value );

    /* (0x000001a4 bits 15) field EGR_100G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_100G_CBR_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_100G_CBR_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a4 bits 15) field EGR_100G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_100G_CBR_FIFO_OVR_I_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_100G_CBR_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_I_poll", value );

    /* (0x000001a4 bits 15) field EGR_100G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    return epmm_reg_EGR_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_100G_CBR_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_100G_CBR_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_40G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_40G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_I_set_to_clear", value );

    /* (0x000001a4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_I_get", value );

    return value;
}
static INLINE void epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
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
        /* (0x000001a4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
        epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                                 EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_get", stop_bit, 2 );
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
    /* (0x000001a4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_MSK)
                  >> EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_MSK, EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000001a4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
        return epmm_reg_EGR_CBR_OVR_INT_STAT_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                   value << (EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_I_poll", value );

    /* (0x000001a4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    return epmm_reg_EGR_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_40G_CBR_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_I_set_to_clear", value );

    /* (0x000001a4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_I_get", value );

    return value;
}
static INLINE void epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
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
        /* (0x000001a4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
        epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                                 EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_get", stop_bit, 11 );
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
    /* (0x000001a4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_MSK)
                  >> EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_MSK, EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000001a4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
        return epmm_reg_EGR_CBR_OVR_INT_STAT_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                   value << (EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR_FIFO_OVR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_I_poll", value );

    /* (0x000001a4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_I of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_STAT */
    return epmm_reg_EGR_CBR_OVR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_OVR_INT_STAT_BIT_EGR_10G_CBR_FIFO_OVR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_100G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_100G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 15) field EGR_100G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_100G_CBR_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_100G_CBR_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 15) field EGR_100G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_100G_CBR_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_100G_CBR_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 15) field EGR_100G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_100G_CBR_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_100G_CBR_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_40G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_40G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                         epmm_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
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
        /* (0x000001a8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
        epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 subfield_mask << (EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                                 EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF + subfield_offset,
                                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_get", stop_bit, 2 );
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
    /* (0x000001a8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_MSK)
                  >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_MSK, EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001a8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000001a8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
        return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                   value << (EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_40G_CBR_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR11_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR11_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 11) field EGR_10G_CBR11_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR11_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR11_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR11_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR11_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 11) field EGR_10G_CBR11_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR11_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR11_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR11_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR11_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 11) field EGR_10G_CBR11_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR11_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR11_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR10_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR10_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                     epmm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 10) field EGR_10G_CBR10_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR10_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR10_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR10_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR10_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 10) field EGR_10G_CBR10_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR10_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR10_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR10_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR10_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 10) field EGR_10G_CBR10_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR10_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR10_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR9_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR9_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 9) field EGR_10G_CBR9_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR9_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR9_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR9_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR9_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 9) field EGR_10G_CBR9_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR9_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR9_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR9_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR9_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 9) field EGR_10G_CBR9_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR9_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR9_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR8_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR8_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 8) field EGR_10G_CBR8_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR8_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR8_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR8_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR8_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 8) field EGR_10G_CBR8_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR8_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR8_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR8_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR8_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 8) field EGR_10G_CBR8_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR8_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR8_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR7_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR7_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 7) field EGR_10G_CBR7_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR7_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR7_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR7_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR7_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 7) field EGR_10G_CBR7_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR7_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR7_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR7_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR7_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 7) field EGR_10G_CBR7_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR7_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR7_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR6_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR6_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 6) field EGR_10G_CBR6_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR6_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR6_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR6_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR6_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 6) field EGR_10G_CBR6_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR6_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR6_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR6_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR6_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 6) field EGR_10G_CBR6_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR6_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR6_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR5_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR5_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 5) field EGR_10G_CBR5_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR5_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR5_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR5_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR5_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 5) field EGR_10G_CBR5_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR5_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR5_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR5_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR5_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 5) field EGR_10G_CBR5_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR5_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR5_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR4_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR4_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 4) field EGR_10G_CBR4_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR4_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR4_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR4_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR4_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 4) field EGR_10G_CBR4_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR4_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR4_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR4_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR4_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 4) field EGR_10G_CBR4_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR4_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR4_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR3_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR3_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 3) field EGR_10G_CBR3_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR3_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR3_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR3_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR3_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 3) field EGR_10G_CBR3_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR3_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR3_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR3_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR3_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 3) field EGR_10G_CBR3_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR3_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR3_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR2_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR2_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 2) field EGR_10G_CBR2_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR2_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR2_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR2_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR2_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 2) field EGR_10G_CBR2_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR2_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR2_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR2_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR2_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 2) field EGR_10G_CBR2_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR2_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR2_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR1_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR1_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 1) field EGR_10G_CBR1_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR1_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR1_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR1_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR1_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 1) field EGR_10G_CBR1_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR1_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR1_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR1_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR1_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 1) field EGR_10G_CBR1_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR1_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR1_FIFO_UDR_I_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void epmm_field_EGR_10G_CBR0_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void epmm_field_EGR_10G_CBR0_FIFO_UDR_I_set_to_clear( epmm_buffer_t *b_ptr,
                                                                    epmm_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_I_set_to_clear", value );

    /* (0x000001a8 bits 0) field EGR_10G_CBR0_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set( b_ptr,
                                                             h_ptr,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR0_FIFO_UDR_I_MSK,
                                                             EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR0_FIFO_UDR_I_OFF,
                                                             value);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR0_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR0_FIFO_UDR_I_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001a8 bits 0) field EGR_10G_CBR0_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR0_FIFO_UDR_I_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR0_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR0_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR0_FIFO_UDR_I_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_I_poll", value );

    /* (0x000001a8 bits 0) field EGR_10G_CBR0_FIFO_UDR_I of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_STAT */
    return epmm_reg_EGR_CBR_UDR_INT_STAT_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR0_FIFO_UDR_I_MSK,
                                               (value<<EPMM120_REG_EGR_CBR_UDR_INT_STAT_BIT_EGR_10G_CBR0_FIFO_UDR_I_OFF),
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
static INLINE UINT32 epmm_field_ING_10G_GSUP11_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP11_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 11) field ING_10G_GSUP11_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP11_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP11_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP11_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP11_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP11_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 11) field ING_10G_GSUP11_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP11_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP11_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP10_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP10_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 10) field ING_10G_GSUP10_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP10_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP10_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP10_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP10_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP10_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 10) field ING_10G_GSUP10_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP10_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP10_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP9_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP9_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 9) field ING_10G_GSUP9_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP9_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP9_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP9_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP9_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP9_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 9) field ING_10G_GSUP9_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP9_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP9_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP8_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP8_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 8) field ING_10G_GSUP8_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP8_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP8_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP8_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP8_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP8_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 8) field ING_10G_GSUP8_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP8_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP8_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP7_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP7_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 7) field ING_10G_GSUP7_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP7_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP7_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP7_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP7_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP7_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 7) field ING_10G_GSUP7_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP7_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP7_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP6_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP6_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 6) field ING_10G_GSUP6_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP6_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP6_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP6_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP6_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP6_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 6) field ING_10G_GSUP6_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP6_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP6_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP5_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP5_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 5) field ING_10G_GSUP5_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP5_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP5_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP5_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP5_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP5_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 5) field ING_10G_GSUP5_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP5_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP5_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP4_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP4_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 4) field ING_10G_GSUP4_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP4_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP4_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP4_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP4_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP4_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 4) field ING_10G_GSUP4_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP4_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP4_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP3_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP3_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 3) field ING_10G_GSUP3_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP3_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP3_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP3_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP3_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP3_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 3) field ING_10G_GSUP3_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP3_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP3_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP2_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP2_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 2) field ING_10G_GSUP2_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP2_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP2_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP2_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP2_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP2_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 2) field ING_10G_GSUP2_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP2_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP2_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP1_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP1_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 1) field ING_10G_GSUP1_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP1_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP1_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP1_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP1_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP1_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 1) field ING_10G_GSUP1_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP1_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP1_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP0_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP0_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 0) field ING_10G_GSUP0_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP0_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP0_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP0_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP0_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP0_FIFO_OVR_V_poll", value );

    /* (0x000000b4 bits 0) field ING_10G_GSUP0_FIFO_OVR_V of register PMC_EPMM120_REG_ING_GSUP_OVR_INT_VAL */
    return epmm_reg_ING_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP0_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_OVR_INT_VAL_BIT_ING_10G_GSUP0_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_GSUP_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_GSUP_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_V of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_MSK) >> EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_get", stop_bit, 11 );
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
    /* (0x000000b8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_V of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_ING_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_MSK)
                  >> EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_MSK, EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                                  epmm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                                  epmm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000b8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_V of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_VAL */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000b8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_V of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_VAL */
        return epmm_reg_ING_GSUP_UDR_INT_VAL_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_OFF + subfield_offset),
                                                   value << (EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_GSUP_FIFO_UDR_V_poll", value );

    /* (0x000000b8 bits 11:0) field ING_10G_GSUP_FIFO_UDR_V of register PMC_EPMM120_REG_ING_GSUP_UDR_INT_VAL */
    return epmm_reg_ING_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_ING_GSUP_UDR_INT_VAL_BIT_ING_10G_GSUP_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 15) field ING_100G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_100G_CBR_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_100G_CBR_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_100G_CBR_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 15) field ING_100G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_100G_CBR_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_100G_CBR_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 14:12) field ING_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_V_get", value );

    return value;
}
static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_V_get", stop_bit, 2 );
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
    /* (0x000000e4 bits 14:12) field ING_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_MSK)
                  >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_MSK, EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_OVR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_OVR_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_OVR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000e4 bits 14:12) field ING_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000e4 bits 14:12) field ING_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
        return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_OFF + subfield_offset),
                                                  value << (EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_40G_CBR_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 14:12) field ING_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_40G_CBR_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR11_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR11_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 11) field ING_10G_CBR11_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR11_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR11_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR11_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR11_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR11_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 11) field ING_10G_CBR11_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR11_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR11_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR10_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR10_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 10) field ING_10G_CBR10_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR10_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR10_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR10_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR10_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR10_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 10) field ING_10G_CBR10_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR10_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR10_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR9_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR9_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 9) field ING_10G_CBR9_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR9_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR9_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR9_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR9_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR9_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 9) field ING_10G_CBR9_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR9_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR9_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR8_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR8_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 8) field ING_10G_CBR8_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR8_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR8_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR8_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR8_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR8_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 8) field ING_10G_CBR8_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR8_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR8_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR7_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR7_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 7) field ING_10G_CBR7_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR7_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR7_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR7_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR7_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR7_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 7) field ING_10G_CBR7_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR7_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR7_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR6_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR6_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 6) field ING_10G_CBR6_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR6_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR6_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR6_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR6_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR6_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 6) field ING_10G_CBR6_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR6_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR6_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR5_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR5_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 5) field ING_10G_CBR5_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR5_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR5_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR5_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR5_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR5_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 5) field ING_10G_CBR5_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR5_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR5_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR4_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR4_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 4) field ING_10G_CBR4_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR4_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR4_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR4_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR4_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR4_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 4) field ING_10G_CBR4_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR4_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR4_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR3_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR3_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 3) field ING_10G_CBR3_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR3_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR3_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR3_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR3_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR3_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 3) field ING_10G_CBR3_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR3_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR3_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR2_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR2_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 2) field ING_10G_CBR2_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR2_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR2_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR2_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR2_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR2_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 2) field ING_10G_CBR2_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR2_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR2_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR1_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR1_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 1) field ING_10G_CBR1_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR1_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR1_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR1_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR1_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR1_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 1) field ING_10G_CBR1_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR1_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR1_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR0_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR0_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e4 bits 0) field ING_10G_CBR0_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR0_FIFO_OVR_V_MSK) >> EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR0_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR0_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR0_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR0_FIFO_OVR_V_poll", value );

    /* (0x000000e4 bits 0) field ING_10G_CBR0_FIFO_OVR_V of register PMC_EPMM120_REG_ING_CBR_OVR_INT_VAL */
    return epmm_reg_ING_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR0_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_OVR_INT_VAL_BIT_ING_10G_CBR0_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_100G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e8 bits 15) field ING_100G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_100G_CBR_FIFO_UDR_V_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_100G_CBR_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_100G_CBR_FIFO_UDR_V_poll", value );

    /* (0x000000e8 bits 15) field ING_100G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    return epmm_reg_ING_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_100G_CBR_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_100G_CBR_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_40G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e8 bits 14:12) field ING_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_40G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_V_get", stop_bit, 2 );
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
    /* (0x000000e8 bits 14:12) field ING_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_MSK)
                  >> EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_MSK, EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_40G_CBR_FIFO_UDR_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_40G_CBR_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000e8 bits 14:12) field ING_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000e8 bits 14:12) field ING_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
        return epmm_reg_ING_CBR_UDR_INT_VAL_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_OFF + subfield_offset),
                                                  value << (EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_40G_CBR_FIFO_UDR_V_poll", value );

    /* (0x000000e8 bits 14:12) field ING_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    return epmm_reg_ING_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_40G_CBR_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_ING_10G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_ING_10G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000e8 bits 11:0) field ING_10G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_MSK) >> EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 epmm_field_range_ING_10G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_ING_10G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_V_get", stop_bit, 11 );
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
    /* (0x000000e8 bits 11:0) field ING_10G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_ING_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_MSK)
                  >> EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_MSK, EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_CBR_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_ING_10G_CBR_FIFO_UDR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_ING_10G_CBR_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000e8 bits 11:0) field ING_10G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000e8 bits 11:0) field ING_10G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
        return epmm_reg_ING_CBR_UDR_INT_VAL_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_OFF + subfield_offset),
                                                  value << (EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_ING_10G_CBR_FIFO_UDR_V_poll", value );

    /* (0x000000e8 bits 11:0) field ING_10G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_ING_CBR_UDR_INT_VAL */
    return epmm_reg_ING_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_ING_CBR_UDR_INT_VAL_BIT_ING_10G_CBR_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000184 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_MSK) >> EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_V_get", value );

    return value;
}
static INLINE UINT32 epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_get( epmm_buffer_t *b_ptr,
                                                                   epmm_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_get", stop_bit, 11 );
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
    /* (0x00000184 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_OVR_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_MSK)
                  >> EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_MSK, EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                                  epmm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                                  epmm_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000184 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_VAL */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000184 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_VAL */
        return epmm_reg_EGR_GSUP_OVR_INT_VAL_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_OFF + subfield_offset),
                                                   value << (EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP_FIFO_OVR_V_poll", value );

    /* (0x00000184 bits 11:0) field EGR_10G_GSUP_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_GSUP_OVR_INT_VAL */
    return epmm_reg_EGR_GSUP_OVR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_OVR_INT_VAL_BIT_EGR_10G_GSUP_FIFO_OVR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP11_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP11_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 11) field EGR_10G_GSUP11_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP11_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP11_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP11_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP11_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP11_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 11) field EGR_10G_GSUP11_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP11_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP11_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP10_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP10_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 10) field EGR_10G_GSUP10_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP10_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP10_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP10_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP10_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                              epmm_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP10_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 10) field EGR_10G_GSUP10_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP10_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP10_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP9_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP9_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 9) field EGR_10G_GSUP9_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP9_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP9_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP9_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP9_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP9_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 9) field EGR_10G_GSUP9_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP9_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP9_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP8_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP8_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 8) field EGR_10G_GSUP8_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP8_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP8_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP8_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP8_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP8_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 8) field EGR_10G_GSUP8_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP8_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP8_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP7_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP7_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 7) field EGR_10G_GSUP7_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP7_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP7_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP7_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP7_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP7_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 7) field EGR_10G_GSUP7_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP7_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP7_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP6_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP6_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 6) field EGR_10G_GSUP6_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP6_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP6_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP6_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP6_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP6_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 6) field EGR_10G_GSUP6_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP6_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP6_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP5_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP5_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 5) field EGR_10G_GSUP5_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP5_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP5_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP5_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP5_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP5_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 5) field EGR_10G_GSUP5_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP5_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP5_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP4_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP4_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 4) field EGR_10G_GSUP4_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP4_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP4_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP4_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP4_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP4_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 4) field EGR_10G_GSUP4_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP4_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP4_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP3_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP3_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 3) field EGR_10G_GSUP3_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP3_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP3_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP3_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP3_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP3_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 3) field EGR_10G_GSUP3_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP3_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP3_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP2_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP2_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 2) field EGR_10G_GSUP2_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP2_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP2_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP2_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP2_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP2_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 2) field EGR_10G_GSUP2_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP2_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP2_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP1_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP1_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 1) field EGR_10G_GSUP1_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP1_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP1_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP1_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP1_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP1_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 1) field EGR_10G_GSUP1_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP1_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP1_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_GSUP0_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_GSUP0_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 0) field EGR_10G_GSUP0_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_GSUP_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP0_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP0_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP0_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP0_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_GSUP0_FIFO_UDR_V_poll", value );

    /* (0x00000188 bits 0) field EGR_10G_GSUP0_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_GSUP_UDR_INT_VAL */
    return epmm_reg_EGR_GSUP_UDR_INT_VAL_poll( b_ptr,
                                               h_ptr,
                                               EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP0_FIFO_UDR_V_MSK,
                                               (value<<EPMM120_REG_EGR_GSUP_UDR_INT_VAL_BIT_EGR_10G_GSUP0_FIFO_UDR_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b4 bits 15) field EGR_100G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_100G_CBR_FIFO_OVR_V_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_100G_CBR_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_100G_CBR_FIFO_OVR_V_poll", value );

    /* (0x000001b4 bits 15) field EGR_100G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    return epmm_reg_EGR_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_100G_CBR_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_100G_CBR_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_V_get", value );

    return value;
}
static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_get", stop_bit, 2 );
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
    /* (0x000001b4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_MSK)
                  >> EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_MSK, EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001b4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000001b4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
        return epmm_reg_EGR_CBR_OVR_INT_VAL_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_OFF + subfield_offset),
                                                  value << (EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_40G_CBR_FIFO_OVR_V_poll", value );

    /* (0x000001b4 bits 14:12) field EGR_40G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    return epmm_reg_EGR_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_40G_CBR_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_MSK) >> EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_V_get", value );

    return value;
}
static INLINE UINT32 epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_get", stop_bit, 11 );
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
    /* (0x000001b4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_OVR_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_MSK)
                  >> EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_MSK, EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001b4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000001b4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
        return epmm_reg_EGR_CBR_OVR_INT_VAL_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_OFF + subfield_offset),
                                                  value << (EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR_FIFO_OVR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR_FIFO_OVR_V_poll", value );

    /* (0x000001b4 bits 11:0) field EGR_10G_CBR_FIFO_OVR_V of register PMC_EPMM120_REG_EGR_CBR_OVR_INT_VAL */
    return epmm_reg_EGR_CBR_OVR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_OVR_INT_VAL_BIT_EGR_10G_CBR_FIFO_OVR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_100G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 15) field EGR_100G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_100G_CBR_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_100G_CBR_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_100G_CBR_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 15) field EGR_100G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_100G_CBR_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_100G_CBR_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_40G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_get( epmm_buffer_t *b_ptr,
                                                                  epmm_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_get", stop_bit, 2 );
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
    /* (0x000001b8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_MSK)
                  >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_MSK, EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                                 epmm_handle_t *h_ptr,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001b8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 0) {
            subfield_offset = start_bit - 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 0;
        }
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000001b8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
        return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_OFF + subfield_offset),
                                                  value << (EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                           epmm_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_40G_CBR_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 14:12) field EGR_40G_CBR_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_40G_CBR_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR11_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR11_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 11) field EGR_10G_CBR11_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR11_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR11_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR11_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR11_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR11_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 11) field EGR_10G_CBR11_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR11_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR11_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR10_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR10_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 10) field EGR_10G_CBR10_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR10_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR10_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR10_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR10_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                             epmm_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR10_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 10) field EGR_10G_CBR10_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR10_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR10_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR9_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR9_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 9) field EGR_10G_CBR9_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR9_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR9_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR9_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR9_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR9_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 9) field EGR_10G_CBR9_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR9_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR9_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR8_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR8_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 8) field EGR_10G_CBR8_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR8_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR8_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR8_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR8_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR8_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 8) field EGR_10G_CBR8_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR8_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR8_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR7_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR7_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 7) field EGR_10G_CBR7_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR7_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR7_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR7_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR7_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR7_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 7) field EGR_10G_CBR7_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR7_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR7_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR6_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR6_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 6) field EGR_10G_CBR6_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR6_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR6_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR6_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR6_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR6_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 6) field EGR_10G_CBR6_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR6_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR6_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR5_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR5_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 5) field EGR_10G_CBR5_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR5_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR5_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR5_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR5_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR5_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 5) field EGR_10G_CBR5_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR5_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR5_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR4_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR4_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 4) field EGR_10G_CBR4_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR4_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR4_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR4_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR4_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR4_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 4) field EGR_10G_CBR4_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR4_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR4_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR3_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR3_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 3) field EGR_10G_CBR3_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR3_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR3_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR3_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR3_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR3_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 3) field EGR_10G_CBR3_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR3_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR3_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR2_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR2_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 2) field EGR_10G_CBR2_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR2_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR2_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR2_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR2_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR2_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 2) field EGR_10G_CBR2_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR2_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR2_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR1_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR1_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 1) field EGR_10G_CBR1_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR1_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR1_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR1_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR1_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR1_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 1) field EGR_10G_CBR1_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR1_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR1_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 epmm_field_EGR_10G_CBR0_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 epmm_field_EGR_10G_CBR0_FIFO_UDR_V_get( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001b8 bits 0) field EGR_10G_CBR0_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    reg_value = epmm_reg_EGR_CBR_UDR_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR0_FIFO_UDR_V_MSK) >> EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR0_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR0_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR0_FIFO_UDR_V_poll( epmm_buffer_t *b_ptr,
                                                                            epmm_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "epmm_field_EGR_10G_CBR0_FIFO_UDR_V_poll", value );

    /* (0x000001b8 bits 0) field EGR_10G_CBR0_FIFO_UDR_V of register PMC_EPMM120_REG_EGR_CBR_UDR_INT_VAL */
    return epmm_reg_EGR_CBR_UDR_INT_VAL_poll( b_ptr,
                                              h_ptr,
                                              EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR0_FIFO_UDR_V_MSK,
                                              (value<<EPMM120_REG_EGR_CBR_UDR_INT_VAL_BIT_EGR_10G_CBR0_FIFO_UDR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EPMM_IO_INLINE_H */
