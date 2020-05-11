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
 *     and register accessor functions for the tl_ic block
 *****************************************************************************/
#ifndef _TL_IC_IO_INLINE_H
#define _TL_IC_IO_INLINE_H

#include "digi_api.h"
#include "tl_ic_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TL_IC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for tl_ic
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
    digi_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} tl_ic_buffer_t;
static INLINE void tl_ic_buffer_init( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void tl_ic_buffer_init( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "tl_ic_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void tl_ic_buffer_flush( tl_ic_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void tl_ic_buffer_flush( tl_ic_buffer_t *b_ptr )
{
    IOLOG( "tl_ic_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 tl_ic_reg_read( tl_ic_buffer_t *b_ptr,
                                     digi_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_read( tl_ic_buffer_t *b_ptr,
                                     digi_handle_t *h_ptr,
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
static INLINE void tl_ic_reg_write( tl_ic_buffer_t *b_ptr,
                                    digi_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_write( tl_ic_buffer_t *b_ptr,
                                    digi_handle_t *h_ptr,
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

static INLINE void tl_ic_field_set( tl_ic_buffer_t *b_ptr,
                                    digi_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 mask,
                                    UINT32 unused_mask,
                                    UINT32 ofs,
                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_set( tl_ic_buffer_t *b_ptr,
                                    digi_handle_t *h_ptr,
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

static INLINE void tl_ic_action_on_write_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_action_on_write_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
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

static INLINE void tl_ic_burst_read( tl_ic_buffer_t *b_ptr,
                                     digi_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void tl_ic_burst_read( tl_ic_buffer_t *b_ptr,
                                     digi_handle_t *h_ptr,
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

static INLINE void tl_ic_burst_write( tl_ic_buffer_t *b_ptr,
                                      digi_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void tl_ic_burst_write( tl_ic_buffer_t *b_ptr,
                                      digi_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE tl_ic_poll( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 mask,
                                               UINT32 value,
                                               PMC_POLL_COMPARISON_TYPE cmp,
                                               UINT32 max_count,
                                               UINT32 *num_failed_polls,
                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE tl_ic_poll( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
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
 *  register access functions for tl_ic
 * ==================================================================================
 */

static INLINE void tl_ic_reg_LP_CTRL_write( tl_ic_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LP_CTRL_write( tl_ic_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_LP_CTRL_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LP_CTRL,
                     value);
}

static INLINE void tl_ic_reg_LP_CTRL_field_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LP_CTRL_field_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_LP_CTRL_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LP_CTRL,
                     mask,
                     PMC_TL_IC_REG_LP_CTRL_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_LP_CTRL_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_LP_CTRL_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_LP_CTRL);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_LP_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_RAM_LP_CTRL_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RAM_LP_CTRL_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_RAM_LP_CTRL_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RAM_LP_CTRL,
                     value);
}

static INLINE void tl_ic_reg_RAM_LP_CTRL_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RAM_LP_CTRL_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_RAM_LP_CTRL_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RAM_LP_CTRL,
                     mask,
                     PMC_TL_IC_REG_RAM_LP_CTRL_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_RAM_LP_CTRL_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_RAM_LP_CTRL_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_RAM_LP_CTRL);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_RAM_LP_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_LINE_SERDES_IDDQ_write( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_IDDQ_write( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_LINE_SERDES_IDDQ_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_IDDQ,
                     value);
}

static INLINE void tl_ic_reg_LINE_SERDES_IDDQ_field_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_IDDQ_field_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_LINE_SERDES_IDDQ_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_IDDQ,
                     mask,
                     PMC_TL_IC_REG_LINE_SERDES_IDDQ_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_LINE_SERDES_IDDQ_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_LINE_SERDES_IDDQ_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_LINE_SERDES_IDDQ);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_LINE_SERDES_IDDQ_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_IDDQ_1_write( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_IDDQ_1_write( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_IDDQ_1_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_IDDQ_1,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_IDDQ_1_field_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_IDDQ_1_field_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_IDDQ_1_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_IDDQ_1,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_IDDQ_1_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_IDDQ_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_IDDQ_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_IDDQ_1);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_IDDQ_1_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_IDDQ_2_write( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_IDDQ_2_write( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_IDDQ_2_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_IDDQ_2,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_IDDQ_2_field_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_IDDQ_2_field_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_IDDQ_2_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_IDDQ_2,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_IDDQ_2_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_IDDQ_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_IDDQ_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_IDDQ_2);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_IDDQ_2_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_PCIE_OH_SERDES_IDDQ_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_PCIE_OH_SERDES_IDDQ_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_PCIE_OH_SERDES_IDDQ_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ,
                     value);
}

static INLINE void tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ,
                     mask,
                     PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_PCIE_OH_SERDES_IDDQ_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_PCIE_OH_SERDES_IDDQ_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_PCIE_OH_SERDES_IDDQ_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_DCSU_IDDQ_write( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_DCSU_IDDQ_write( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_DCSU_IDDQ_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_DCSU_IDDQ,
                     value);
}

static INLINE void tl_ic_reg_DCSU_IDDQ_field_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_DCSU_IDDQ_field_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_DCSU_IDDQ_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_DCSU_IDDQ,
                     mask,
                     PMC_TL_IC_REG_DCSU_IDDQ_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_DCSU_IDDQ_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_DCSU_IDDQ_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_DCSU_IDDQ);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_DCSU_IDDQ_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_RESET_CTRL0_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RESET_CTRL0_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_RESET_CTRL0_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RESET_CTRL0,
                     value);
}

static INLINE void tl_ic_reg_RESET_CTRL0_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RESET_CTRL0_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_RESET_CTRL0_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RESET_CTRL0,
                     mask,
                     PMC_TL_IC_REG_RESET_CTRL0_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_RESET_CTRL0_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_RESET_CTRL0_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_RESET_CTRL0);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_RESET_CTRL0_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_RESET_CTRL1_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RESET_CTRL1_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_RESET_CTRL1_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RESET_CTRL1,
                     value);
}

static INLINE void tl_ic_reg_RESET_CTRL1_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RESET_CTRL1_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_RESET_CTRL1_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RESET_CTRL1,
                     mask,
                     PMC_TL_IC_REG_RESET_CTRL1_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_RESET_CTRL1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_RESET_CTRL1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_RESET_CTRL1);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_RESET_CTRL1_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_RESET_CTRL2_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RESET_CTRL2_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_RESET_CTRL2_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RESET_CTRL2,
                     value);
}

static INLINE void tl_ic_reg_RESET_CTRL2_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RESET_CTRL2_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_RESET_CTRL2_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RESET_CTRL2,
                     mask,
                     PMC_TL_IC_REG_RESET_CTRL2_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_RESET_CTRL2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_RESET_CTRL2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_RESET_CTRL2);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_RESET_CTRL2_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_RESET_CTRL3_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RESET_CTRL3_write( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_RESET_CTRL3_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RESET_CTRL3,
                     value);
}

static INLINE void tl_ic_reg_RESET_CTRL3_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_RESET_CTRL3_field_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_RESET_CTRL3_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_RESET_CTRL3,
                     mask,
                     PMC_TL_IC_REG_RESET_CTRL3_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_RESET_CTRL3_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_RESET_CTRL3_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_RESET_CTRL3);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_RESET_CTRL3_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_IRIG_CTRL_write( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_IRIG_CTRL_write( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_IRIG_CTRL_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_IRIG_CTRL,
                     value);
}

static INLINE void tl_ic_reg_IRIG_CTRL_field_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_IRIG_CTRL_field_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_IRIG_CTRL_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_IRIG_CTRL,
                     mask,
                     PMC_TL_IC_REG_IRIG_CTRL_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_IRIG_CTRL_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_IRIG_CTRL_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_IRIG_CTRL);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_IRIG_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_1_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_1_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_LINE_SERDES_INTR_EN_1_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1,
                     value);
}

static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_1_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_1_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_LINE_SERDES_INTR_EN_1_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1,
                     mask,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_LINE_SERDES_INTR_EN_1_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_1B_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_1B_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_1B_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_1B,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_1B_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_1B_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_1B_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_1B,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_1B_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_1B_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_1B_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_EN_1B);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_EN_1B_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_1_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_1_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_OH_SERDES_INTR_EN_1_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_1,
                     value);
}

static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_1_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_1_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_OH_SERDES_INTR_EN_1_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_1,
                     mask,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_1_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_OH_SERDES_INTR_EN_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_OH_SERDES_INTR_EN_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_OH_SERDES_INTR_EN_1);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_OH_SERDES_INTR_EN_1_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_DCSU_INTR_EN_1_write( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_DCSU_INTR_EN_1_write( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_DCSU_INTR_EN_1_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_DCSU_INTR_EN_1,
                     value);
}

static INLINE void tl_ic_reg_DCSU_INTR_EN_1_field_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_DCSU_INTR_EN_1_field_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_DCSU_INTR_EN_1_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_DCSU_INTR_EN_1,
                     mask,
                     PMC_TL_IC_REG_DCSU_INTR_EN_1_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_DCSU_INTR_EN_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_DCSU_INTR_EN_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_DCSU_INTR_EN_1);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_DCSU_INTR_EN_1_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_2_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_2_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_LINE_SERDES_INTR_EN_2_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2,
                     value);
}

static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2,
                     mask,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_LINE_SERDES_INTR_EN_2_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_2A_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_2A_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_2A_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_2A_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_2A_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_2A_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_2A_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_2A_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_EN_2A_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_2B_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_2B_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_2B_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_2B_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_2B_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_EN_2B_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_2_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_2_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_OH_SERDES_INTR_EN_2_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_2,
                     value);
}

static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_OH_SERDES_INTR_EN_2_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_2,
                     mask,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_2_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_OH_SERDES_INTR_EN_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_OH_SERDES_INTR_EN_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_OH_SERDES_INTR_EN_2);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_OH_SERDES_INTR_EN_2_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_DCSU_INTR_EN_2_write( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_DCSU_INTR_EN_2_write( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_DCSU_INTR_EN_2_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_DCSU_INTR_EN_2,
                     value);
}

static INLINE void tl_ic_reg_DCSU_INTR_EN_2_field_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_DCSU_INTR_EN_2_field_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_DCSU_INTR_EN_2_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_DCSU_INTR_EN_2,
                     mask,
                     PMC_TL_IC_REG_DCSU_INTR_EN_2_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_DCSU_INTR_EN_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_DCSU_INTR_EN_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_DCSU_INTR_EN_2);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_DCSU_INTR_EN_2_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_3_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_3_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_LINE_SERDES_INTR_EN_3_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3,
                     value);
}

static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3,
                     mask,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_3_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_3_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_LINE_SERDES_INTR_EN_3_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_3A_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_3A_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_3A_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_3A_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_3A_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_3A_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_3A_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_3A_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_EN_3A_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_3B_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_3B_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_3B_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_3B_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_3B_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_EN_3B_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_3_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_3_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_OH_SERDES_INTR_EN_3_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_3,
                     value);
}

static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_OH_SERDES_INTR_EN_3_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_3,
                     mask,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_3_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_OH_SERDES_INTR_EN_3_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_OH_SERDES_INTR_EN_3_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_OH_SERDES_INTR_EN_3);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_OH_SERDES_INTR_EN_3_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_DCSU_INTR_EN_3_write( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_DCSU_INTR_EN_3_write( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_DCSU_INTR_EN_3_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_DCSU_INTR_EN_3,
                     value);
}

static INLINE void tl_ic_reg_DCSU_INTR_EN_3_field_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_DCSU_INTR_EN_3_field_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_DCSU_INTR_EN_3_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_DCSU_INTR_EN_3,
                     mask,
                     PMC_TL_IC_REG_DCSU_INTR_EN_3_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_DCSU_INTR_EN_3_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_DCSU_INTR_EN_3_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_DCSU_INTR_EN_3);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_DCSU_INTR_EN_3_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_4_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_4_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_LINE_SERDES_INTR_EN_4_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4,
                     value);
}

static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_4_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_LINE_SERDES_INTR_EN_4_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_LINE_SERDES_INTR_EN_4_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4,
                     mask,
                     PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_4_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_4_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_LINE_SERDES_INTR_EN_4_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_4A_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_4A_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_4A_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_4A_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_4A_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_4A_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_4A_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_4A_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_EN_4A_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_4B_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_4B_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_4B_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B,
                     value);
}

static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_4B_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SYS_SERDES_INTR_EN_4B_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SYS_SERDES_INTR_EN_4B_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B,
                     mask,
                     PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_4B_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_4B_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_EN_4B_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_4_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_4_write( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_OH_SERDES_INTR_EN_4_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_4,
                     value);
}

static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_4_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_OH_SERDES_INTR_EN_4_field_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_OH_SERDES_INTR_EN_4_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_4,
                     mask,
                     PMC_TL_IC_REG_OH_SERDES_INTR_EN_4_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_OH_SERDES_INTR_EN_4_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_OH_SERDES_INTR_EN_4_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_OH_SERDES_INTR_EN_4);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_OH_SERDES_INTR_EN_4_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_SERDES_CSU_RESET_SYNC_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SERDES_CSU_RESET_SYNC_write( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_SERDES_CSU_RESET_SYNC_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC,
                     value);
}

static INLINE void tl_ic_reg_SERDES_CSU_RESET_SYNC_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_SERDES_CSU_RESET_SYNC_field_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_SERDES_CSU_RESET_SYNC_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC,
                     mask,
                     PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_SERDES_CSU_RESET_SYNC_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SERDES_CSU_RESET_SYNC_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SERDES_CSU_RESET_SYNC_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_APRAM_CFG_write( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_APRAM_CFG_write( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tl_ic_reg_APRAM_CFG_write", value );
    tl_ic_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_APRAM_CFG,
                     value);
}

static INLINE void tl_ic_reg_APRAM_CFG_field_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_APRAM_CFG_field_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tl_ic_reg_APRAM_CFG_field_set", mask, ofs, value );
    tl_ic_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_TL_IC_REG_APRAM_CFG,
                     mask,
                     PMC_TL_IC_REG_APRAM_CFG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 tl_ic_reg_APRAM_CFG_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_APRAM_CFG_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_TL_IC_REG_APRAM_CFG);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_APRAM_CFG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_SUM_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_LINE_SERDES_INTR_SUM_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_TL_IC_REG_LINE_SERDES_INTR_SUM);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_LINE_SERDES_INTR_SUM_read", reg_value);
    return reg_value;
}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_SUM_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_SUM_1_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_1);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_SUM_1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_SUM_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_SYS_SERDES_INTR_SUM_2_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_2);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_SYS_SERDES_INTR_SUM_2_read", reg_value);
    return reg_value;
}

static INLINE UINT32 tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read", reg_value);
    return reg_value;
}

static INLINE UINT32 tl_ic_reg_DCSU_INTR_SUM_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_DCSU_INTR_SUM_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_TL_IC_REG_DCSU_INTR_SUM);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_DCSU_INTR_SUM_read", reg_value);
    return reg_value;
}

static INLINE UINT32 tl_ic_reg_AVS_STATUS_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_AVS_STATUS_read( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_TL_IC_REG_AVS_STATUS);

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_AVS_STATUS_read", reg_value);
    return reg_value;
}

static INLINE void tl_ic_reg_GPCFG_array_burst_read( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 ofs,
                                                     UINT32 len,
                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void tl_ic_reg_GPCFG_array_burst_read( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 ofs,
                                                     UINT32 len,
                                                     UINT32 *value )
{
    if (len + ofs > 12)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "tl_ic_reg_GPCFG_array_burst_read", ofs, len, 12 );
    tl_ic_burst_read( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_TL_IC_REG_GPCFG(ofs),
                      len,
                      value);

    IOLOG( "%s of %d words ofs=%d len=%d", "tl_ic_reg_GPCFG_array_burst_read", 12, ofs, len);
}

static INLINE UINT32 tl_ic_reg_GPCFG_array_read( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_reg_GPCFG_array_read( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = tl_ic_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_TL_IC_REG_GPCFG(N));

    IOLOG( "%s -> 0x%08x;", "tl_ic_reg_GPCFG_array_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void tl_ic_field_PGMRCLK_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PGMRCLK_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PGMRCLK_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PGMRCLK_LOW_PWR_set", value );

    /* (0x00000000 bits 13) field PGMRCLK_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_PGMRCLK_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_PGMRCLK_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_PGMRCLK_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PGMRCLK_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 13) field PGMRCLK_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_PGMRCLK_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_PGMRCLK_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PGMRCLK_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_LINE_DSIS_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINE_DSIS_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINE_DSIS_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINE_DSIS_LOW_PWR_set", value );

    /* (0x00000000 bits 12) field LINE_DSIS_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_LINE_DSIS_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_LINE_DSIS_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_LINE_DSIS_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINE_DSIS_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 12) field LINE_DSIS_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_LINE_DSIS_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_LINE_DSIS_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINE_DSIS_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_COREOTN_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_COREOTN_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_COREOTN_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_COREOTN_LOW_PWR_set", value );

    /* (0x00000000 bits 11) field COREOTN_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_COREOTN_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_COREOTN_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_COREOTN_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_COREOTN_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 11) field COREOTN_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_COREOTN_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_COREOTN_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_COREOTN_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_SYSOTN_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYSOTN_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYSOTN_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYSOTN_LOW_PWR_set", value );

    /* (0x00000000 bits 10) field SYSOTN_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_SYSOTN_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_SYSOTN_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_SYSOTN_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYSOTN_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 10) field SYSOTN_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_SYSOTN_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_SYSOTN_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYSOTN_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_SIFD_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SIFD_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SIFD_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SIFD_LOW_PWR_set", value );

    /* (0x00000000 bits 9) field SIFD_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_SIFD_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_SIFD_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_SIFD_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SIFD_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 9) field SIFD_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_SIFD_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_SIFD_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SIFD_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_MAPOTN_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MAPOTN_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MAPOTN_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MAPOTN_LOW_PWR_set", value );

    /* (0x00000000 bits 8) field MAPOTN_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_MAPOTN_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_MAPOTN_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_MAPOTN_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MAPOTN_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 8) field MAPOTN_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_MAPOTN_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_MAPOTN_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MAPOTN_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET120_LINE_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET120_LINE_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET120_LINE_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET120_LINE_LOW_PWR_set", value );

    /* (0x00000000 bits 7) field ENET120_LINE_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_ENET120_LINE_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_ENET120_LINE_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_ENET120_LINE_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET120_LINE_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 7) field ENET120_LINE_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_ENET120_LINE_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_ENET120_LINE_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET120_LINE_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET120_SYS_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET120_SYS_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET120_SYS_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET120_SYS_LOW_PWR_set", value );

    /* (0x00000000 bits 6) field ENET120_SYS_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_ENET120_SYS_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_ENET120_SYS_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_ENET120_SYS_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET120_SYS_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 6) field ENET120_SYS_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_ENET120_SYS_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_ENET120_SYS_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET120_SYS_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_MCPB_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MCPB_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MCPB_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MCPB_LOW_PWR_set", value );

    /* (0x00000000 bits 5) field MCPB_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_MCPB_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_MCPB_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_MCPB_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MCPB_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 5) field MCPB_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_MCPB_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_MCPB_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MCPB_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_DCPB_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_DCPB_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_DCPB_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_DCPB_LOW_PWR_set", value );

    /* (0x00000000 bits 4) field DCPB_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_DCPB_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_DCPB_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_DCPB_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_DCPB_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field DCPB_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_DCPB_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_DCPB_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_DCPB_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_CBRC_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_CBRC_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_CBRC_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_CBRC_LOW_PWR_set", value );

    /* (0x00000000 bits 3) field CBRC_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_CBRC_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_CBRC_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_CBRC_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_CBRC_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field CBRC_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_CBRC_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_CBRC_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_CBRC_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_LIFD_MUX_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LIFD_MUX_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LIFD_MUX_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LIFD_MUX_LOW_PWR_set", value );

    /* (0x00000000 bits 2) field LIFD_MUX_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_LIFD_MUX_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_LIFD_MUX_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_LIFD_MUX_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LIFD_MUX_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field LIFD_MUX_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_LIFD_MUX_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_LIFD_MUX_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LIFD_MUX_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_LINEOTN_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINEOTN_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINEOTN_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINEOTN_LOW_PWR_set", value );

    /* (0x00000000 bits 1) field LINEOTN_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_LINEOTN_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_LINEOTN_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_LINEOTN_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINEOTN_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field LINEOTN_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_LINEOTN_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_LINEOTN_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINEOTN_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_DIGI_M1_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_DIGI_M1_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_DIGI_M1_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_DIGI_M1_LOW_PWR_set", value );

    /* (0x00000000 bits 0) field DIGI_M1_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    tl_ic_reg_LP_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 TL_IC_REG_LP_CTRL_BIT_DIGI_M1_LOW_PWR_MSK,
                                 TL_IC_REG_LP_CTRL_BIT_DIGI_M1_LOW_PWR_OFF,
                                 value);
}

static INLINE UINT32 tl_ic_field_DIGI_M1_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_DIGI_M1_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field DIGI_M1_LOW_PWR of register PMC_TL_IC_REG_LP_CTRL */
    reg_value = tl_ic_reg_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LP_CTRL_BIT_DIGI_M1_LOW_PWR_MSK) >> TL_IC_REG_LP_CTRL_BIT_DIGI_M1_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_DIGI_M1_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_COREOTN_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_COREOTN_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_COREOTN_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_COREOTN_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 11) field COREOTN_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_COREOTN_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_COREOTN_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_COREOTN_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_COREOTN_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 11) field COREOTN_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_COREOTN_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_COREOTN_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_COREOTN_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_SYSOTN_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYSOTN_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYSOTN_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYSOTN_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 10) field SYSOTN_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_SYSOTN_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_SYSOTN_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_SYSOTN_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYSOTN_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 10) field SYSOTN_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_SYSOTN_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_SYSOTN_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYSOTN_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_SIFD_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SIFD_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SIFD_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SIFD_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 9) field SIFD_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_SIFD_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_SIFD_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_SIFD_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SIFD_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 9) field SIFD_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_SIFD_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_SIFD_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SIFD_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_MAPOTN_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MAPOTN_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MAPOTN_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MAPOTN_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 8) field MAPOTN_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_MAPOTN_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_MAPOTN_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_MAPOTN_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MAPOTN_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 8) field MAPOTN_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_MAPOTN_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_MAPOTN_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MAPOTN_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET120_LINE_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET120_LINE_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET120_LINE_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET120_LINE_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 7) field ENET120_LINE_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_LINE_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_LINE_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_ENET120_LINE_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET120_LINE_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 7) field ENET120_LINE_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_LINE_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_LINE_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET120_LINE_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET120_SYS_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET120_SYS_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET120_SYS_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET120_SYS_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 6) field ENET120_SYS_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_SYS_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_SYS_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_ENET120_SYS_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET120_SYS_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 6) field ENET120_SYS_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_SYS_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_SYS_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET120_SYS_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_MCPB_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MCPB_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MCPB_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MCPB_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 5) field MCPB_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_MCPB_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_MCPB_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_MCPB_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MCPB_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 5) field MCPB_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_MCPB_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_MCPB_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MCPB_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_DCPB_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_DCPB_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_DCPB_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_DCPB_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 4) field DCPB_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_DCPB_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_DCPB_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_DCPB_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_DCPB_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 4) field DCPB_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_DCPB_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_DCPB_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_DCPB_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_CBRC_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_CBRC_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_CBRC_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_CBRC_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 3) field CBRC_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_CBRC_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_CBRC_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_CBRC_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_CBRC_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 3) field CBRC_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_CBRC_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_CBRC_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_CBRC_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_LINEOTN_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINEOTN_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINEOTN_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINEOTN_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 1) field LINEOTN_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_LINEOTN_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_LINEOTN_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_LINEOTN_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINEOTN_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 1) field LINEOTN_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_LINEOTN_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_LINEOTN_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINEOTN_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_DIGI_M1_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_DIGI_M1_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_DIGI_M1_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_DIGI_M1_RAM_LOW_PWR_set", value );

    /* (0x00000004 bits 0) field DIGI_M1_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    tl_ic_reg_RAM_LP_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_DIGI_M1_RAM_LOW_PWR_MSK,
                                     TL_IC_REG_RAM_LP_CTRL_BIT_DIGI_M1_RAM_LOW_PWR_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_DIGI_M1_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_DIGI_M1_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field DIGI_M1_RAM_LOW_PWR of register PMC_TL_IC_REG_RAM_LP_CTRL */
    reg_value = tl_ic_reg_RAM_LP_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RAM_LP_CTRL_BIT_DIGI_M1_RAM_LOW_PWR_MSK) >> TL_IC_REG_RAM_LP_CTRL_BIT_DIGI_M1_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_DIGI_M1_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI2_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI2_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI2_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI2_IDDQ_set", value );

    /* (0x00000010 bits 13) field SFI2_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
    tl_ic_reg_LINE_SERDES_IDDQ_field_set( b_ptr,
                                          h_ptr,
                                          TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI2_IDDQ_MSK,
                                          TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI2_IDDQ_OFF,
                                          value);
}

static INLINE UINT32 tl_ic_field_SFI2_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 13) field SFI2_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
    reg_value = tl_ic_reg_LINE_SERDES_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI2_IDDQ_MSK) >> TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI2_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI1_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI1_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI1_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI1_IDDQ_set", value );

    /* (0x00000010 bits 12) field SFI1_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
    tl_ic_reg_LINE_SERDES_IDDQ_field_set( b_ptr,
                                          h_ptr,
                                          TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI1_IDDQ_MSK,
                                          TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI1_IDDQ_OFF,
                                          value);
}

static INLINE UINT32 tl_ic_field_SFI1_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 12) field SFI1_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
    reg_value = tl_ic_reg_LINE_SERDES_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI1_IDDQ_MSK) >> TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI1_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_LINE_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINE_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINE_SERDES_IDDQ_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINE_SERDES_IDDQ_set", value );

    /* (0x00000010 bits 11:0) field LINE_SERDES_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
    tl_ic_reg_LINE_SERDES_IDDQ_field_set( b_ptr,
                                          h_ptr,
                                          TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_MSK,
                                          TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_OFF,
                                          value);
}

static INLINE UINT32 tl_ic_field_LINE_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINE_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 11:0) field LINE_SERDES_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
    reg_value = tl_ic_reg_LINE_SERDES_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_MSK) >> TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINE_SERDES_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LINE_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LINE_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_IDDQ_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_IDDQ_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_IDDQ_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 11:0) field LINE_SERDES_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
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
        /* (0x00000010 bits 11:0) field LINE_SERDES_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
        tl_ic_reg_LINE_SERDES_IDDQ_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_OFF + subfield_offset),
                                              TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LINE_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LINE_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_IDDQ_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_IDDQ_get", stop_bit, 11 );
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
    /* (0x00000010 bits 11:0) field LINE_SERDES_IDDQ of register PMC_TL_IC_REG_LINE_SERDES_IDDQ */
    reg_value = tl_ic_reg_LINE_SERDES_IDDQ_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_MSK)
                  >> TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_MSK, TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_IDDQ_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LR_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LR_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LR_CSU_IDDQ_set", value, 3);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LR_CSU_IDDQ_set", value );

    /* (0x00000014 bits 21:20) field LR_CSU_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
    tl_ic_reg_SYS_SERDES_IDDQ_1_field_set( b_ptr,
                                           h_ptr,
                                           TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_MSK,
                                           TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_OFF,
                                           value);
}

static INLINE UINT32 tl_ic_field_LR_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 21:20) field LR_CSU_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_MSK) >> TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_CSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LR_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LR_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_IDDQ_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_IDDQ_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_IDDQ_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 21:20) field LR_CSU_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
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
        /* (0x00000014 bits 21:20) field LR_CSU_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
        tl_ic_reg_SYS_SERDES_IDDQ_1_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_OFF + subfield_offset),
                                               TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LR_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_IDDQ_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_IDDQ_get", stop_bit, 1 );
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
    /* (0x00000014 bits 21:20) field LR_CSU_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_MSK)
                  >> TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_MSK, TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_IDDQ_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LR_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LR_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LR_SERDES_IDDQ_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LR_SERDES_IDDQ_set", value );

    /* (0x00000014 bits 19:0) field LR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
    tl_ic_reg_SYS_SERDES_IDDQ_1_field_set( b_ptr,
                                           h_ptr,
                                           TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_MSK,
                                           TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_OFF,
                                           value);
}

static INLINE UINT32 tl_ic_field_LR_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 19:0) field LR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_MSK) >> TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_SERDES_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LR_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LR_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_IDDQ_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_IDDQ_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_IDDQ_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 19:0) field LR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
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
        /* (0x00000014 bits 19:0) field LR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
        tl_ic_reg_SYS_SERDES_IDDQ_1_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_OFF + subfield_offset),
                                               TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LR_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_IDDQ_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_IDDQ_get", stop_bit, 19 );
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
    /* (0x00000014 bits 19:0) field LR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_1 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_MSK)
                  >> TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_MSK, TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_IDDQ_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SR_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SR_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SR_CSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SR_CSU_IDDQ_set", value );

    /* (0x00000018 bits 24) field SR_CSU_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
    tl_ic_reg_SYS_SERDES_IDDQ_2_field_set( b_ptr,
                                           h_ptr,
                                           TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_CSU_IDDQ_MSK,
                                           TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_CSU_IDDQ_OFF,
                                           value);
}

static INLINE UINT32 tl_ic_field_SR_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 24) field SR_CSU_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_CSU_IDDQ_MSK) >> TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_CSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_CSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_SR_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SR_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SR_SERDES_IDDQ_set", value, 255);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SR_SERDES_IDDQ_set", value );

    /* (0x00000018 bits 23:16) field SR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
    tl_ic_reg_SYS_SERDES_IDDQ_2_field_set( b_ptr,
                                           h_ptr,
                                           TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_MSK,
                                           TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_OFF,
                                           value);
}

static INLINE UINT32 tl_ic_field_SR_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 23:16) field SR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_MSK) >> TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_SERDES_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SR_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SR_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_IDDQ_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_IDDQ_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_IDDQ_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 23:16) field SR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
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
        /* (0x00000018 bits 23:16) field SR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
        tl_ic_reg_SYS_SERDES_IDDQ_2_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_OFF + subfield_offset),
                                               TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SR_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SR_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_IDDQ_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_IDDQ_get", stop_bit, 7 );
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
    /* (0x00000018 bits 23:16) field SR_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_MSK)
                  >> TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_MSK, TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_IDDQ_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SYS_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS_SERDES_IDDQ_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS_SERDES_IDDQ_set", value );

    /* (0x00000018 bits 15:0) field SYS_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
    tl_ic_reg_SYS_SERDES_IDDQ_2_field_set( b_ptr,
                                           h_ptr,
                                           TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_MSK,
                                           TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_OFF,
                                           value);
}

static INLINE UINT32 tl_ic_field_SYS_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 15:0) field SYS_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_MSK) >> TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_SERDES_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SYS_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SYS_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_IDDQ_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_IDDQ_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_IDDQ_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 15:0) field SYS_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
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
        /* (0x00000018 bits 15:0) field SYS_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
        tl_ic_reg_SYS_SERDES_IDDQ_2_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_OFF + subfield_offset),
                                               TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SYS_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SYS_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_IDDQ_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_IDDQ_get", stop_bit, 15 );
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
    /* (0x00000018 bits 15:0) field SYS_SERDES_IDDQ of register PMC_TL_IC_REG_SYS_SERDES_IDDQ_2 */
    reg_value = tl_ic_reg_SYS_SERDES_IDDQ_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_MSK)
                  >> TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_MSK, TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_IDDQ_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_OH_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_CSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_CSU_IDDQ_set", value );

    /* (0x0000001c bits 15) field OH_CSU_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_CSU_IDDQ_MSK,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_CSU_IDDQ_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 15) field OH_CSU_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_CSU_IDDQ_MSK) >> TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_CSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_CSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_OH_TX_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_TX_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_TX_SERDES_IDDQ_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_TX_SERDES_IDDQ_set", value );

    /* (0x0000001c bits 14:12) field OH_TX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_MSK,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_TX_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_TX_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 14:12) field OH_TX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_MSK) >> TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_TX_SERDES_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_TX_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_TX_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_IDDQ_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_IDDQ_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_IDDQ_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000001c bits 14:12) field OH_TX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
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
        /* (0x0000001c bits 14:12) field OH_TX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
        tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_OFF + subfield_offset),
                                                 TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_IDDQ_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_IDDQ_get", stop_bit, 2 );
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
    /* (0x0000001c bits 14:12) field OH_TX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_IDDQ_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_MSK)
                  >> TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_MSK, TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_IDDQ_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_OH_RX_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_RX_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_RX_SERDES_IDDQ_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_RX_SERDES_IDDQ_set", value );

    /* (0x0000001c bits 10:8) field OH_RX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_MSK,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_RX_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_RX_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 10:8) field OH_RX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_MSK) >> TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_RX_SERDES_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_RX_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_RX_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_IDDQ_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_IDDQ_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_IDDQ_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000001c bits 10:8) field OH_RX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
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
        /* (0x0000001c bits 10:8) field OH_RX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
        tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_OFF + subfield_offset),
                                                 TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_IDDQ_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_IDDQ_get", stop_bit, 2 );
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
    /* (0x0000001c bits 10:8) field OH_RX_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_IDDQ_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_MSK)
                  >> TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_MSK, TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_IDDQ_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_PCIE_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PCIE_CSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PCIE_CSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PCIE_CSU_IDDQ_set", value );

    /* (0x0000001c bits 4) field PCIE_CSU_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_CSU_IDDQ_MSK,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_CSU_IDDQ_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_PCIE_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_CSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 4) field PCIE_CSU_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_CSU_IDDQ_MSK) >> TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_CSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_CSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_PCIE_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PCIE_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PCIE_SERDES_IDDQ_set", value, 15);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PCIE_SERDES_IDDQ_set", value );

    /* (0x0000001c bits 3:0) field PCIE_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_MSK,
                                             TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_PCIE_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 3:0) field PCIE_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_MSK) >> TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_SERDES_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_range_PCIE_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_PCIE_SERDES_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_IDDQ_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_IDDQ_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_IDDQ_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000001c bits 3:0) field PCIE_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
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
        /* (0x0000001c bits 3:0) field PCIE_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
        tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_OFF + subfield_offset),
                                                 TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_IDDQ_get( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_IDDQ_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_IDDQ_get", stop_bit, 3 );
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
    /* (0x0000001c bits 3:0) field PCIE_SERDES_IDDQ of register PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_IDDQ_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_MSK)
                  >> TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_MSK, TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_IDDQ_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_FIC2_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_FIC2_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_FIC2_DCSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_FIC2_DCSU_IDDQ_set", value );

    /* (0x00000020 bits 8) field FIC2_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    tl_ic_reg_DCSU_IDDQ_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_DCSU_IDDQ_BIT_FIC2_DCSU_IDDQ_MSK,
                                   TL_IC_REG_DCSU_IDDQ_BIT_FIC2_DCSU_IDDQ_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_FIC2_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC2_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 8) field FIC2_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    reg_value = tl_ic_reg_DCSU_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_IDDQ_BIT_FIC2_DCSU_IDDQ_MSK) >> TL_IC_REG_DCSU_IDDQ_BIT_FIC2_DCSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC2_DCSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET3_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET3_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET3_DCSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET3_DCSU_IDDQ_set", value );

    /* (0x00000020 bits 7) field ENET3_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    tl_ic_reg_DCSU_IDDQ_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_DCSU_IDDQ_BIT_ENET3_DCSU_IDDQ_MSK,
                                   TL_IC_REG_DCSU_IDDQ_BIT_ENET3_DCSU_IDDQ_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_ENET3_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET3_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 7) field ENET3_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    reg_value = tl_ic_reg_DCSU_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_IDDQ_BIT_ENET3_DCSU_IDDQ_MSK) >> TL_IC_REG_DCSU_IDDQ_BIT_ENET3_DCSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET3_DCSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_PTP_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PTP_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PTP_DCSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PTP_DCSU_IDDQ_set", value );

    /* (0x00000020 bits 6) field PTP_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    tl_ic_reg_DCSU_IDDQ_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_DCSU_IDDQ_BIT_PTP_DCSU_IDDQ_MSK,
                                   TL_IC_REG_DCSU_IDDQ_BIT_PTP_DCSU_IDDQ_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_PTP_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PTP_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 6) field PTP_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    reg_value = tl_ic_reg_DCSU_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_IDDQ_BIT_PTP_DCSU_IDDQ_MSK) >> TL_IC_REG_DCSU_IDDQ_BIT_PTP_DCSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PTP_DCSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET2_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET2_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET2_DCSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET2_DCSU_IDDQ_set", value );

    /* (0x00000020 bits 5) field ENET2_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    tl_ic_reg_DCSU_IDDQ_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_DCSU_IDDQ_BIT_ENET2_DCSU_IDDQ_MSK,
                                   TL_IC_REG_DCSU_IDDQ_BIT_ENET2_DCSU_IDDQ_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_ENET2_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET2_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 5) field ENET2_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    reg_value = tl_ic_reg_DCSU_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_IDDQ_BIT_ENET2_DCSU_IDDQ_MSK) >> TL_IC_REG_DCSU_IDDQ_BIT_ENET2_DCSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET2_DCSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET1_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET1_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET1_DCSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET1_DCSU_IDDQ_set", value );

    /* (0x00000020 bits 4) field ENET1_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    tl_ic_reg_DCSU_IDDQ_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_DCSU_IDDQ_BIT_ENET1_DCSU_IDDQ_MSK,
                                   TL_IC_REG_DCSU_IDDQ_BIT_ENET1_DCSU_IDDQ_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_ENET1_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET1_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 4) field ENET1_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    reg_value = tl_ic_reg_DCSU_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_IDDQ_BIT_ENET1_DCSU_IDDQ_MSK) >> TL_IC_REG_DCSU_IDDQ_BIT_ENET1_DCSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET1_DCSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN2_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN2_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN2_DCSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN2_DCSU_IDDQ_set", value );

    /* (0x00000020 bits 1) field OTN2_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    tl_ic_reg_DCSU_IDDQ_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_DCSU_IDDQ_BIT_OTN2_DCSU_IDDQ_MSK,
                                   TL_IC_REG_DCSU_IDDQ_BIT_OTN2_DCSU_IDDQ_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_OTN2_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN2_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 1) field OTN2_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    reg_value = tl_ic_reg_DCSU_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_IDDQ_BIT_OTN2_DCSU_IDDQ_MSK) >> TL_IC_REG_DCSU_IDDQ_BIT_OTN2_DCSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN2_DCSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN1_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN1_DCSU_IDDQ_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN1_DCSU_IDDQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN1_DCSU_IDDQ_set", value );

    /* (0x00000020 bits 0) field OTN1_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    tl_ic_reg_DCSU_IDDQ_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_DCSU_IDDQ_BIT_OTN1_DCSU_IDDQ_MSK,
                                   TL_IC_REG_DCSU_IDDQ_BIT_OTN1_DCSU_IDDQ_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_OTN1_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN1_DCSU_IDDQ_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 0) field OTN1_DCSU_IDDQ of register PMC_TL_IC_REG_DCSU_IDDQ */
    reg_value = tl_ic_reg_DCSU_IDDQ_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_IDDQ_BIT_OTN1_DCSU_IDDQ_MSK) >> TL_IC_REG_DCSU_IDDQ_BIT_OTN1_DCSU_IDDQ_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN1_DCSU_IDDQ_get", value );

    return value;
}
static INLINE void tl_ic_field_LINE_DSIS_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINE_DSIS_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINE_DSIS_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINE_DSIS_RESET_set", value );

    /* (0x00000030 bits 26) field LINE_DSIS_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_LINE_DSIS_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_LINE_DSIS_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_LINE_DSIS_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINE_DSIS_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 26) field LINE_DSIS_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_LINE_DSIS_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_LINE_DSIS_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINE_DSIS_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_PGMRCLK_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PGMRCLK_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PGMRCLK_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PGMRCLK_RESET_set", value );

    /* (0x00000030 bits 25) field PGMRCLK_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_PGMRCLK_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_PGMRCLK_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_PGMRCLK_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PGMRCLK_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 25) field PGMRCLK_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_PGMRCLK_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_PGMRCLK_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PGMRCLK_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_PTP_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PTP_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PTP_DCSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PTP_DCSU_RESET_set", value );

    /* (0x00000030 bits 24) field PTP_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_PTP_DCSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_PTP_DCSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_PTP_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PTP_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 24) field PTP_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_PTP_DCSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_PTP_DCSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PTP_DCSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET2_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET2_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET2_DCSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET2_DCSU_RESET_set", value );

    /* (0x00000030 bits 23) field ENET2_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET2_DCSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET2_DCSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_ENET2_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET2_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 23) field ENET2_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_ENET2_DCSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_ENET2_DCSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET2_DCSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET1_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET1_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET1_DCSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET1_DCSU_RESET_set", value );

    /* (0x00000030 bits 22) field ENET1_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET1_DCSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET1_DCSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_ENET1_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET1_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 22) field ENET1_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_ENET1_DCSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_ENET1_DCSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET1_DCSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_FIC2_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_FIC2_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_FIC2_DCSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_FIC2_DCSU_RESET_set", value );

    /* (0x00000030 bits 19) field FIC2_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_FIC2_DCSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_FIC2_DCSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_FIC2_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC2_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 19) field FIC2_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_FIC2_DCSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_FIC2_DCSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC2_DCSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET3_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET3_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET3_DCSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET3_DCSU_RESET_set", value );

    /* (0x00000030 bits 18) field ENET3_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET3_DCSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET3_DCSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_ENET3_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET3_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 18) field ENET3_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_ENET3_DCSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_ENET3_DCSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET3_DCSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN2_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN2_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN2_DCSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN2_DCSU_RESET_set", value );

    /* (0x00000030 bits 17) field OTN2_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_OTN2_DCSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_OTN2_DCSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_OTN2_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN2_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 17) field OTN2_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_OTN2_DCSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_OTN2_DCSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN2_DCSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN1_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN1_DCSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN1_DCSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN1_DCSU_RESET_set", value );

    /* (0x00000030 bits 16) field OTN1_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_OTN1_DCSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_OTN1_DCSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_OTN1_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN1_DCSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 16) field OTN1_DCSU_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_OTN1_DCSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_OTN1_DCSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN1_DCSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_COREOTN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_COREOTN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_COREOTN_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_COREOTN_RESET_set", value );

    /* (0x00000030 bits 11) field COREOTN_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_COREOTN_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_COREOTN_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_COREOTN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_COREOTN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 11) field COREOTN_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_COREOTN_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_COREOTN_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_COREOTN_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_SYSOTN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYSOTN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYSOTN_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYSOTN_RESET_set", value );

    /* (0x00000030 bits 10) field SYSOTN_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_SYSOTN_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_SYSOTN_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_SYSOTN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYSOTN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 10) field SYSOTN_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_SYSOTN_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_SYSOTN_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYSOTN_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_SIFD_RESET_set( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SIFD_RESET_set( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SIFD_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SIFD_RESET_set", value );

    /* (0x00000030 bits 9) field SIFD_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_SIFD_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_SIFD_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_SIFD_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SIFD_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 9) field SIFD_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_SIFD_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_SIFD_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SIFD_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_MAPOTN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MAPOTN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MAPOTN_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MAPOTN_RESET_set", value );

    /* (0x00000030 bits 8) field MAPOTN_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_MAPOTN_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_MAPOTN_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_MAPOTN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MAPOTN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 8) field MAPOTN_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_MAPOTN_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_MAPOTN_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MAPOTN_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET120_LINE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET120_LINE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET120_LINE_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET120_LINE_RESET_set", value );

    /* (0x00000030 bits 7) field ENET120_LINE_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET120_LINE_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET120_LINE_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_ENET120_LINE_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET120_LINE_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 7) field ENET120_LINE_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_ENET120_LINE_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_ENET120_LINE_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET120_LINE_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET120_SYS_RESET_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET120_SYS_RESET_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET120_SYS_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET120_SYS_RESET_set", value );

    /* (0x00000030 bits 6) field ENET120_SYS_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET120_SYS_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_ENET120_SYS_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_ENET120_SYS_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET120_SYS_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 6) field ENET120_SYS_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_ENET120_SYS_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_ENET120_SYS_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET120_SYS_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_MCPB_RESET_set( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MCPB_RESET_set( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MCPB_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MCPB_RESET_set", value );

    /* (0x00000030 bits 5) field MCPB_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_MCPB_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_MCPB_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_MCPB_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MCPB_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 5) field MCPB_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_MCPB_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_MCPB_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MCPB_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_DCPB_RESET_set( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_DCPB_RESET_set( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_DCPB_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_DCPB_RESET_set", value );

    /* (0x00000030 bits 4) field DCPB_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_DCPB_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_DCPB_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_DCPB_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_DCPB_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 4) field DCPB_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_DCPB_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_DCPB_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_DCPB_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_CBRC_RESET_set( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_CBRC_RESET_set( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_CBRC_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_CBRC_RESET_set", value );

    /* (0x00000030 bits 3) field CBRC_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_CBRC_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_CBRC_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_CBRC_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_CBRC_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 3) field CBRC_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_CBRC_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_CBRC_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_CBRC_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_LIFD_MUX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LIFD_MUX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LIFD_MUX_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LIFD_MUX_RESET_set", value );

    /* (0x00000030 bits 2) field LIFD_MUX_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_LIFD_MUX_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_LIFD_MUX_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_LIFD_MUX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LIFD_MUX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 2) field LIFD_MUX_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_LIFD_MUX_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_LIFD_MUX_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LIFD_MUX_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_LINEOTN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINEOTN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINEOTN_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINEOTN_RESET_set", value );

    /* (0x00000030 bits 1) field LINEOTN_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_LINEOTN_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_LINEOTN_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_LINEOTN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINEOTN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 1) field LINEOTN_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_LINEOTN_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_LINEOTN_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINEOTN_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_DIGI_M1_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_DIGI_M1_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_DIGI_M1_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_DIGI_M1_RESET_set", value );

    /* (0x00000030 bits 0) field DIGI_M1_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    tl_ic_reg_RESET_CTRL0_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL0_BIT_DIGI_M1_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL0_BIT_DIGI_M1_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_DIGI_M1_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_DIGI_M1_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 0) field DIGI_M1_RESET of register PMC_TL_IC_REG_RESET_CTRL0 */
    reg_value = tl_ic_reg_RESET_CTRL0_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL0_BIT_DIGI_M1_RESET_MSK) >> TL_IC_REG_RESET_CTRL0_BIT_DIGI_M1_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_DIGI_M1_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_C8_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_C8_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_C8_CSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_C8_CSU_RESET_set", value );

    /* (0x00000034 bits 24) field C8_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
    tl_ic_reg_RESET_CTRL1_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL1_BIT_C8_CSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL1_BIT_C8_CSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_C8_CSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_C8_CSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 24) field C8_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
    reg_value = tl_ic_reg_RESET_CTRL1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL1_BIT_C8_CSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL1_BIT_C8_CSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_C8_CSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_C8_RESET_set( tl_ic_buffer_t *b_ptr,
                                             digi_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_C8_RESET_set( tl_ic_buffer_t *b_ptr,
                                             digi_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_C8_RESET_set", value, 255);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_C8_RESET_set", value );

    /* (0x00000034 bits 23:16) field C8_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
    tl_ic_reg_RESET_CTRL1_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_C8_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_C8_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 23:16) field C8_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
    reg_value = tl_ic_reg_RESET_CTRL1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_MSK) >> TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_C8_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_C8_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_C8_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_C8_RESET_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_C8_RESET_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_C8_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000034 bits 23:16) field C8_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
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
        /* (0x00000034 bits 23:16) field C8_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
        tl_ic_reg_RESET_CTRL1_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_C8_RESET_get( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_C8_RESET_get( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_C8_RESET_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_C8_RESET_get", stop_bit, 7 );
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
    /* (0x00000034 bits 23:16) field C8_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
    reg_value = tl_ic_reg_RESET_CTRL1_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_MSK, TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_C8_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_S16_SYS_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_S16_SYS_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_S16_SYS_RESET_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_S16_SYS_RESET_set", value );

    /* (0x00000034 bits 15:0) field S16_SYS_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
    tl_ic_reg_RESET_CTRL1_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_S16_SYS_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_S16_SYS_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 15:0) field S16_SYS_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
    reg_value = tl_ic_reg_RESET_CTRL1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_MSK) >> TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_S16_SYS_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_S16_SYS_RESET_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_S16_SYS_RESET_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_S16_SYS_RESET_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_S16_SYS_RESET_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_S16_SYS_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000034 bits 15:0) field S16_SYS_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
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
        /* (0x00000034 bits 15:0) field S16_SYS_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
        tl_ic_reg_RESET_CTRL1_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_S16_SYS_RESET_get( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_S16_SYS_RESET_get( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_S16_SYS_RESET_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_S16_SYS_RESET_get", stop_bit, 15 );
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
    /* (0x00000034 bits 15:0) field S16_SYS_RESET of register PMC_TL_IC_REG_RESET_CTRL1 */
    reg_value = tl_ic_reg_RESET_CTRL1_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_MSK, TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_S16_SYS_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_TITAN_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_TITAN_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_TITAN_CSU_RESET_set", value, 3);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_TITAN_CSU_RESET_set", value );

    /* (0x00000038 bits 21:20) field TITAN_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
    tl_ic_reg_RESET_CTRL2_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_TITAN_CSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_TITAN_CSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 21:20) field TITAN_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
    reg_value = tl_ic_reg_RESET_CTRL2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_TITAN_CSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_TITAN_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_TITAN_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_TITAN_CSU_RESET_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_TITAN_CSU_RESET_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_TITAN_CSU_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000038 bits 21:20) field TITAN_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
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
        /* (0x00000038 bits 21:20) field TITAN_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
        tl_ic_reg_RESET_CTRL2_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_TITAN_CSU_RESET_get( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_TITAN_CSU_RESET_get( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_TITAN_CSU_RESET_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_TITAN_CSU_RESET_get", stop_bit, 1 );
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
    /* (0x00000038 bits 21:20) field TITAN_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
    reg_value = tl_ic_reg_RESET_CTRL2_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_MSK, TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_TITAN_CSU_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_TITAN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_TITAN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_TITAN_RESET_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_TITAN_RESET_set", value );

    /* (0x00000038 bits 19:0) field TITAN_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
    tl_ic_reg_RESET_CTRL2_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_TITAN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_TITAN_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 19:0) field TITAN_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
    reg_value = tl_ic_reg_RESET_CTRL2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_MSK) >> TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_TITAN_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_TITAN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_TITAN_RESET_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_TITAN_RESET_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_TITAN_RESET_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_TITAN_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000038 bits 19:0) field TITAN_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
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
        /* (0x00000038 bits 19:0) field TITAN_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
        tl_ic_reg_RESET_CTRL2_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_TITAN_RESET_get( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_TITAN_RESET_get( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_TITAN_RESET_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_TITAN_RESET_get", stop_bit, 19 );
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
    /* (0x00000038 bits 19:0) field TITAN_RESET of register PMC_TL_IC_REG_RESET_CTRL2 */
    reg_value = tl_ic_reg_RESET_CTRL2_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_MSK, TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_TITAN_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_D8_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_D8_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_D8_CSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_D8_CSU_RESET_set", value );

    /* (0x0000003c bits 31) field D8_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL3_BIT_D8_CSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL3_BIT_D8_CSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_D8_CSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_D8_CSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 31) field D8_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_D8_CSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL3_BIT_D8_CSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_D8_CSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_OH_SERDES_TX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_SERDES_TX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_SERDES_TX_RESET_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_SERDES_TX_RESET_set", value );

    /* (0x0000003c bits 30:28) field OH_SERDES_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_OH_SERDES_TX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_SERDES_TX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 30:28) field OH_SERDES_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_MSK) >> TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_SERDES_TX_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_SERDES_TX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_SERDES_TX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_SERDES_TX_RESET_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_SERDES_TX_RESET_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_SERDES_TX_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 30:28) field OH_SERDES_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
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
        /* (0x0000003c bits 30:28) field OH_SERDES_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
        tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_SERDES_TX_RESET_get( tl_ic_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_SERDES_TX_RESET_get( tl_ic_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_SERDES_TX_RESET_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_SERDES_TX_RESET_get", stop_bit, 2 );
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
    /* (0x0000003c bits 30:28) field OH_SERDES_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_MSK, TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_SERDES_TX_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_OH_SERDES_RX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_SERDES_RX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_SERDES_RX_RESET_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_SERDES_RX_RESET_set", value );

    /* (0x0000003c bits 27:25) field OH_SERDES_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_OH_SERDES_RX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_SERDES_RX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 27:25) field OH_SERDES_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_MSK) >> TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_SERDES_RX_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_SERDES_RX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_SERDES_RX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_SERDES_RX_RESET_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_SERDES_RX_RESET_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_SERDES_RX_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 27:25) field OH_SERDES_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
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
        /* (0x0000003c bits 27:25) field OH_SERDES_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
        tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_SERDES_RX_RESET_get( tl_ic_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_SERDES_RX_RESET_get( tl_ic_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_SERDES_RX_RESET_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_SERDES_RX_RESET_get", stop_bit, 2 );
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
    /* (0x0000003c bits 27:25) field OH_SERDES_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_MSK, TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_SERDES_RX_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_C8_PCIE_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_C8_PCIE_CSU_RESET_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_C8_PCIE_CSU_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_C8_PCIE_CSU_RESET_set", value );

    /* (0x0000003c bits 24) field C8_PCIE_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_CSU_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_CSU_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_C8_PCIE_CSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_C8_PCIE_CSU_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 24) field C8_PCIE_CSU_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_CSU_RESET_MSK) >> TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_CSU_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_C8_PCIE_CSU_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_C8_PCIE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_C8_PCIE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_C8_PCIE_RESET_set", value, 15);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_C8_PCIE_RESET_set", value );

    /* (0x0000003c bits 23:20) field C8_PCIE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_C8_PCIE_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_C8_PCIE_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 23:20) field C8_PCIE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_MSK) >> TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_C8_PCIE_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_C8_PCIE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_C8_PCIE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_C8_PCIE_RESET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_C8_PCIE_RESET_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_C8_PCIE_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 23:20) field C8_PCIE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
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
        /* (0x0000003c bits 23:20) field C8_PCIE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
        tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_C8_PCIE_RESET_get( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_C8_PCIE_RESET_get( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_C8_PCIE_RESET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_C8_PCIE_RESET_get", stop_bit, 3 );
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
    /* (0x0000003c bits 23:20) field C8_PCIE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_MSK, TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_C8_PCIE_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SFI51_TX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI51_TX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI51_TX_RESET_set", value, 3);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI51_TX_RESET_set", value );

    /* (0x0000003c bits 19:18) field SFI51_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_SFI51_TX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI51_TX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 19:18) field SFI51_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_MSK) >> TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI51_TX_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SFI51_TX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SFI51_TX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SFI51_TX_RESET_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SFI51_TX_RESET_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SFI51_TX_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 19:18) field SFI51_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
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
        /* (0x0000003c bits 19:18) field SFI51_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
        tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SFI51_TX_RESET_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SFI51_TX_RESET_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SFI51_TX_RESET_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SFI51_TX_RESET_get", stop_bit, 1 );
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
    /* (0x0000003c bits 19:18) field SFI51_TX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_MSK, TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SFI51_TX_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SFI51_RX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI51_RX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI51_RX_RESET_set", value, 3);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI51_RX_RESET_set", value );

    /* (0x0000003c bits 17:16) field SFI51_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_SFI51_RX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI51_RX_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 17:16) field SFI51_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_MSK) >> TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI51_RX_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SFI51_RX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SFI51_RX_RESET_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SFI51_RX_RESET_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SFI51_RX_RESET_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SFI51_RX_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 17:16) field SFI51_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
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
        /* (0x0000003c bits 17:16) field SFI51_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
        tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SFI51_RX_RESET_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SFI51_RX_RESET_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SFI51_RX_RESET_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SFI51_RX_RESET_get", stop_bit, 1 );
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
    /* (0x0000003c bits 17:16) field SFI51_RX_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_MSK, TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SFI51_RX_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_S16_LINE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_S16_LINE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_S16_LINE_RESET_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_S16_LINE_RESET_set", value );

    /* (0x0000003c bits 11:0) field S16_LINE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                     h_ptr,
                                     TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_MSK,
                                     TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_OFF,
                                     value);
}

static INLINE UINT32 tl_ic_field_S16_LINE_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_S16_LINE_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 11:0) field S16_LINE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_MSK) >> TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_S16_LINE_RESET_get", value );

    return value;
}
static INLINE void tl_ic_field_range_S16_LINE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_S16_LINE_RESET_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_S16_LINE_RESET_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_S16_LINE_RESET_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_S16_LINE_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 11:0) field S16_LINE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
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
        /* (0x0000003c bits 11:0) field S16_LINE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
        tl_ic_reg_RESET_CTRL3_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_OFF + subfield_offset),
                                         TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_S16_LINE_RESET_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_S16_LINE_RESET_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_S16_LINE_RESET_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_S16_LINE_RESET_get", stop_bit, 11 );
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
    /* (0x0000003c bits 11:0) field S16_LINE_RESET of register PMC_TL_IC_REG_RESET_CTRL3 */
    reg_value = tl_ic_reg_RESET_CTRL3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_MSK)
                  >> TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_MSK, TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_S16_LINE_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_IRIG_ONE_PPS_O_EN_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_IRIG_ONE_PPS_O_EN_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_IRIG_ONE_PPS_O_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_IRIG_ONE_PPS_O_EN_set", value );

    /* (0x00000070 bits 3) field IRIG_ONE_PPS_O_EN of register PMC_TL_IC_REG_IRIG_CTRL */
    tl_ic_reg_IRIG_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_IRIG_CTRL_BIT_IRIG_ONE_PPS_O_EN_MSK,
                                   TL_IC_REG_IRIG_CTRL_BIT_IRIG_ONE_PPS_O_EN_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_IRIG_ONE_PPS_O_EN_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_IRIG_ONE_PPS_O_EN_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000070 bits 3) field IRIG_ONE_PPS_O_EN of register PMC_TL_IC_REG_IRIG_CTRL */
    reg_value = tl_ic_reg_IRIG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_IRIG_CTRL_BIT_IRIG_ONE_PPS_O_EN_MSK) >> TL_IC_REG_IRIG_CTRL_BIT_IRIG_ONE_PPS_O_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_IRIG_ONE_PPS_O_EN_get", value );

    return value;
}
static INLINE void tl_ic_field_IRIG_DATO_EN_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_IRIG_DATO_EN_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_IRIG_DATO_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_IRIG_DATO_EN_set", value );

    /* (0x00000070 bits 2) field IRIG_DATO_EN of register PMC_TL_IC_REG_IRIG_CTRL */
    tl_ic_reg_IRIG_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_IRIG_CTRL_BIT_IRIG_DATO_EN_MSK,
                                   TL_IC_REG_IRIG_CTRL_BIT_IRIG_DATO_EN_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_IRIG_DATO_EN_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_IRIG_DATO_EN_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000070 bits 2) field IRIG_DATO_EN of register PMC_TL_IC_REG_IRIG_CTRL */
    reg_value = tl_ic_reg_IRIG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_IRIG_CTRL_BIT_IRIG_DATO_EN_MSK) >> TL_IC_REG_IRIG_CTRL_BIT_IRIG_DATO_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_IRIG_DATO_EN_get", value );

    return value;
}
static INLINE void tl_ic_field_IRIG_CLK0_EN_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_IRIG_CLK0_EN_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_IRIG_CLK0_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_IRIG_CLK0_EN_set", value );

    /* (0x00000070 bits 1) field IRIG_CLK0_EN of register PMC_TL_IC_REG_IRIG_CTRL */
    tl_ic_reg_IRIG_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_IRIG_CTRL_BIT_IRIG_CLK0_EN_MSK,
                                   TL_IC_REG_IRIG_CTRL_BIT_IRIG_CLK0_EN_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_IRIG_CLK0_EN_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_IRIG_CLK0_EN_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000070 bits 1) field IRIG_CLK0_EN of register PMC_TL_IC_REG_IRIG_CTRL */
    reg_value = tl_ic_reg_IRIG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_IRIG_CTRL_BIT_IRIG_CLK0_EN_MSK) >> TL_IC_REG_IRIG_CTRL_BIT_IRIG_CLK0_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_IRIG_CLK0_EN_get", value );

    return value;
}
static INLINE void tl_ic_field_IRIG_ENET_SOURCE_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_IRIG_ENET_SOURCE_set( tl_ic_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_IRIG_ENET_SOURCE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_IRIG_ENET_SOURCE_set", value );

    /* (0x00000070 bits 0) field IRIG_ENET_SOURCE of register PMC_TL_IC_REG_IRIG_CTRL */
    tl_ic_reg_IRIG_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_IRIG_CTRL_BIT_IRIG_ENET_SOURCE_MSK,
                                   TL_IC_REG_IRIG_CTRL_BIT_IRIG_ENET_SOURCE_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_IRIG_ENET_SOURCE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_IRIG_ENET_SOURCE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000070 bits 0) field IRIG_ENET_SOURCE of register PMC_TL_IC_REG_IRIG_CTRL */
    reg_value = tl_ic_reg_IRIG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_IRIG_CTRL_BIT_IRIG_ENET_SOURCE_MSK) >> TL_IC_REG_IRIG_CTRL_BIT_IRIG_ENET_SOURCE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_IRIG_ENET_SOURCE_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI2_TX_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI2_TX_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_MIPS_set", value );

    /* (0x00000100 bits 15) field SFI2_TX_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1 */
    tl_ic_reg_LINE_SERDES_INTR_EN_1_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_TX_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_TX_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI2_TX_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_TX_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 15) field SFI2_TX_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_1_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_TX_INTR_EN_MIPS_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_TX_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI2_RX_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI2_RX_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_MIPS_set", value );

    /* (0x00000100 bits 14) field SFI2_RX_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1 */
    tl_ic_reg_LINE_SERDES_INTR_EN_1_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_RX_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_RX_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI2_RX_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_RX_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 14) field SFI2_RX_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_1_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_RX_INTR_EN_MIPS_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_RX_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI1_TX_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI1_TX_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_MIPS_set", value );

    /* (0x00000100 bits 13) field SFI1_TX_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1 */
    tl_ic_reg_LINE_SERDES_INTR_EN_1_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_TX_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_TX_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI1_TX_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_TX_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 13) field SFI1_TX_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_1_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_TX_INTR_EN_MIPS_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_TX_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI1_RX_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI1_RX_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_MIPS_set", value );

    /* (0x00000100 bits 12) field SFI1_RX_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1 */
    tl_ic_reg_LINE_SERDES_INTR_EN_1_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_RX_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_RX_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI1_RX_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_RX_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 12) field SFI1_RX_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_1_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_RX_INTR_EN_MIPS_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_RX_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_SR_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SR_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_MIPS_set", value );

    /* (0x00000108 bits 24) field SR_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_1B */
    tl_ic_reg_SYS_SERDES_INTR_EN_1B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_1B_BIT_SR_CSU_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_1B_BIT_SR_CSU_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SR_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000108 bits 24) field SR_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_1B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_1B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_1B_BIT_SR_CSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_1B_BIT_SR_CSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_set", value );

    /* (0x0000010c bits 16) field MGMT_FEGE_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_1 */
    tl_ic_reg_OH_SERDES_INTR_EN_1_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_MGMT_FEGE_INTR_EN_MIPS_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_MGMT_FEGE_INTR_EN_MIPS_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000010c bits 16) field MGMT_FEGE_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_1 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_MGMT_FEGE_INTR_EN_MIPS_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_MGMT_FEGE_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_OH_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_MIPS_set", value );

    /* (0x0000010c bits 15) field OH_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_1 */
    tl_ic_reg_OH_SERDES_INTR_EN_1_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_OH_CSU_INTR_EN_MIPS_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_OH_CSU_INTR_EN_MIPS_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000010c bits 15) field OH_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_1 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_OH_CSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_OH_CSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_PCIE_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PCIE_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_MIPS_set", value );

    /* (0x0000010c bits 4) field PCIE_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_1 */
    tl_ic_reg_OH_SERDES_INTR_EN_1_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_PCIE_CSU_INTR_EN_MIPS_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_PCIE_CSU_INTR_EN_MIPS_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_PCIE_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000010c bits 4) field PCIE_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_1 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_PCIE_CSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_PCIE_CSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 8) field FIC2_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC2_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC2_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 8) field FIC2_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC2_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC2_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 7) field FIC1_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC1_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC1_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 7) field FIC1_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC1_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC1_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_PTP_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PTP_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 6) field PTP_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_PTP_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_PTP_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_PTP_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PTP_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 6) field PTP_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_PTP_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_PTP_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 5) field ENET2_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET2_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET2_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 5) field ENET2_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET2_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET2_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 4) field ENET1_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET1_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET1_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 4) field ENET1_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET1_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET1_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 3) field SYS2_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS2_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS2_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 3) field SYS2_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS2_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS2_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_SYS_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 2) field SYS_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_SYS_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 2) field SYS_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 1) field OTN2_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN2_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN2_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 1) field OTN2_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN2_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN2_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_set", value );

    /* (0x00000110 bits 0) field OTN1_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    tl_ic_reg_DCSU_INTR_EN_1_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN1_DCSU_INTR_EN_MIPS_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN1_DCSU_INTR_EN_MIPS_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000110 bits 0) field OTN1_DCSU_INTR_EN_MIPS of register PMC_TL_IC_REG_DCSU_INTR_EN_1 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN1_DCSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN1_DCSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI2_TX_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI2_TX_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_PCIE_set", value );

    /* (0x00000140 bits 15) field SFI2_TX_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_TX_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_TX_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI2_TX_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_TX_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 15) field SFI2_TX_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_TX_INTR_EN_PCIE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_TX_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI2_RX_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI2_RX_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_PCIE_set", value );

    /* (0x00000140 bits 14) field SFI2_RX_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_RX_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_RX_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI2_RX_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_RX_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 14) field SFI2_RX_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_RX_INTR_EN_PCIE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_RX_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI1_TX_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI1_TX_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_PCIE_set", value );

    /* (0x00000140 bits 13) field SFI1_TX_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_TX_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_TX_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI1_TX_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_TX_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 13) field SFI1_TX_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_TX_INTR_EN_PCIE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_TX_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI1_RX_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI1_RX_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_PCIE_set", value );

    /* (0x00000140 bits 12) field SFI1_RX_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_RX_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_RX_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI1_RX_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_RX_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 12) field SFI1_RX_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_RX_INTR_EN_PCIE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_RX_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_set", value );

    /* (0x00000140 bits 11:0) field LINE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 11:0) field LINE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 11:0) field LINE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
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
        /* (0x00000140 bits 11:0) field LINE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
        tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_OFF + subfield_offset),
                                                   TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                       digi_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                       digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_get", stop_bit, 11 );
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
    /* (0x00000140 bits 11:0) field LINE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_MSK)
                  >> TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_MSK, TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LR_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LR_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_DEVICE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_DEVICE_set", value );

    /* (0x00000144 bits 21:20) field LR_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
    tl_ic_reg_SYS_SERDES_INTR_EN_2A_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LR_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 21:20) field LR_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2A_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000144 bits 21:20) field LR_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
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
        /* (0x00000144 bits 21:20) field LR_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
        tl_ic_reg_SYS_SERDES_INTR_EN_2A_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                  digi_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                  digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_get", stop_bit, 1 );
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
    /* (0x00000144 bits 21:20) field LR_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2A_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LR_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LR_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_DEVICE_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_DEVICE_set", value );

    /* (0x00000144 bits 19:0) field LR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
    tl_ic_reg_SYS_SERDES_INTR_EN_2A_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LR_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 19:0) field LR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2A_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000144 bits 19:0) field LR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
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
        /* (0x00000144 bits 19:0) field LR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
        tl_ic_reg_SYS_SERDES_INTR_EN_2A_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_get", stop_bit, 19 );
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
    /* (0x00000144 bits 19:0) field LR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2A_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SR_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SR_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_PCIE_set", value );

    /* (0x00000148 bits 24) field SR_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
    tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_CSU_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_CSU_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SR_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000148 bits 24) field SR_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_CSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_CSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_SR_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SR_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_DEVICE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_DEVICE_set", value );

    /* (0x00000148 bits 23:16) field SR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
    tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SR_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000148 bits 23:16) field SR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000148 bits 23:16) field SR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
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
        /* (0x00000148 bits 23:16) field SR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
        tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_get", stop_bit, 7 );
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
    /* (0x00000148 bits 23:16) field SR_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2B_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_set", value );

    /* (0x00000148 bits 15:0) field SYS_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
    tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000148 bits 15:0) field SYS_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000148 bits 15:0) field SYS_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
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
        /* (0x00000148 bits 15:0) field SYS_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
        tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_get", stop_bit, 15 );
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
    /* (0x00000148 bits 15:0) field SYS_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_2B_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_set", value );

    /* (0x0000014c bits 16) field MGMT_FEGE_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_MGMT_FEGE_INTR_EN_PCIE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_MGMT_FEGE_INTR_EN_PCIE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000014c bits 16) field MGMT_FEGE_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_MGMT_FEGE_INTR_EN_PCIE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_MGMT_FEGE_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_OH_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_PCIE_set", value );

    /* (0x0000014c bits 15) field OH_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_CSU_INTR_EN_PCIE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_CSU_INTR_EN_PCIE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000014c bits 15) field OH_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_CSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_CSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_set", value );

    /* (0x0000014c bits 14:12) field OH_TX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000014c bits 14:12) field OH_TX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000014c bits 14:12) field OH_TX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
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
        /* (0x0000014c bits 14:12) field OH_TX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
        tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                        digi_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                        digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_get", stop_bit, 2 );
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
    /* (0x0000014c bits 14:12) field OH_TX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_MSK, TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_set", value );

    /* (0x0000014c bits 10:8) field OH_RX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000014c bits 10:8) field OH_RX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000014c bits 10:8) field OH_RX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
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
        /* (0x0000014c bits 10:8) field OH_RX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
        tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                        digi_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                        digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_get", stop_bit, 2 );
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
    /* (0x0000014c bits 10:8) field OH_RX_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_MSK, TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_PCIE_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PCIE_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_PCIE_set", value );

    /* (0x0000014c bits 4) field PCIE_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_CSU_INTR_EN_PCIE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_CSU_INTR_EN_PCIE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_PCIE_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000014c bits 4) field PCIE_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_CSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_CSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_set", value, 15);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_set", value );

    /* (0x0000014c bits 3:0) field PCIE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000014c bits 3:0) field PCIE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000014c bits 3:0) field PCIE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
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
        /* (0x0000014c bits 3:0) field PCIE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
        tl_ic_reg_OH_SERDES_INTR_EN_2_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                       digi_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr,
                                                                       digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_get", stop_bit, 3 );
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
    /* (0x0000014c bits 3:0) field PCIE_SERDES_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_2 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_MSK, TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 8) field FIC2_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC2_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC2_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 8) field FIC2_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC2_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC2_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 7) field FIC1_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC1_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC1_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 7) field FIC1_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC1_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC1_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_PTP_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PTP_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 6) field PTP_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_PTP_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_PTP_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_PTP_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PTP_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 6) field PTP_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_PTP_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_PTP_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 5) field ENET2_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET2_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET2_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 5) field ENET2_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET2_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET2_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 4) field ENET1_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET1_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET1_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 4) field ENET1_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET1_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET1_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 3) field SYS2_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS2_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS2_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 3) field SYS2_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS2_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS2_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_SYS_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 2) field SYS_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_SYS_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 2) field SYS_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 1) field OTN2_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN2_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN2_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 1) field OTN2_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN2_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN2_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_set", value );

    /* (0x00000150 bits 0) field OTN1_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    tl_ic_reg_DCSU_INTR_EN_2_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN1_DCSU_INTR_EN_PCIE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN1_DCSU_INTR_EN_PCIE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000150 bits 0) field OTN1_DCSU_INTR_EN_PCIE of register PMC_TL_IC_REG_DCSU_INTR_EN_2 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN1_DCSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN1_DCSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI2_TX_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI2_TX_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_DEVICE_set", value );

    /* (0x00000180 bits 15) field SFI2_TX_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_TX_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_TX_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI2_TX_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_TX_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000180 bits 15) field SFI2_TX_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_3_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_TX_INTR_EN_DEVICE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_TX_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_TX_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI2_RX_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI2_RX_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_DEVICE_set", value );

    /* (0x00000180 bits 14) field SFI2_RX_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_RX_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_RX_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI2_RX_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_RX_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000180 bits 14) field SFI2_RX_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_3_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_RX_INTR_EN_DEVICE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_RX_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_RX_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI1_TX_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI1_TX_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_DEVICE_set", value );

    /* (0x00000180 bits 13) field SFI1_TX_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_TX_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_TX_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI1_TX_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_TX_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000180 bits 13) field SFI1_TX_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_3_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_TX_INTR_EN_DEVICE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_TX_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_TX_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_SFI1_RX_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SFI1_RX_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_DEVICE_set", value );

    /* (0x00000180 bits 12) field SFI1_RX_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_RX_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_RX_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SFI1_RX_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_RX_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000180 bits 12) field SFI1_RX_INTR_EN_DEVICE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_3_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_RX_INTR_EN_DEVICE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_RX_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_RX_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_LINE_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINE_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_PCIE_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_PCIE_set", value );

    /* (0x00000180 bits 11:0) field LINE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LINE_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINE_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000180 bits 11:0) field LINE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_3_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000180 bits 11:0) field LINE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
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
        /* (0x00000180 bits 11:0) field LINE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
        tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_OFF + subfield_offset),
                                                   TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_get", stop_bit, 11 );
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
    /* (0x00000180 bits 11:0) field LINE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_3_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_MSK)
                  >> TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_MSK, TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LR_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LR_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_PCIE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_PCIE_set", value );

    /* (0x00000184 bits 21:20) field LR_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
    tl_ic_reg_SYS_SERDES_INTR_EN_3A_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LR_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000184 bits 21:20) field LR_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3A_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LR_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LR_CSU_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_INTR_EN_PCIE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_INTR_EN_PCIE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_INTR_EN_PCIE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000184 bits 21:20) field LR_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
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
        /* (0x00000184 bits 21:20) field LR_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
        tl_ic_reg_SYS_SERDES_INTR_EN_3A_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LR_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_CSU_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_INTR_EN_PCIE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_INTR_EN_PCIE_get", stop_bit, 1 );
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
    /* (0x00000184 bits 21:20) field LR_CSU_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3A_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_INTR_EN_PCIE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LR_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LR_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_PCIE_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_PCIE_set", value );

    /* (0x00000184 bits 19:0) field LR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
    tl_ic_reg_SYS_SERDES_INTR_EN_3A_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LR_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000184 bits 19:0) field LR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3A_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000184 bits 19:0) field LR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
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
        /* (0x00000184 bits 19:0) field LR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
        tl_ic_reg_SYS_SERDES_INTR_EN_3A_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_get", stop_bit, 19 );
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
    /* (0x00000184 bits 19:0) field LR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3A_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SR_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SR_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_DEVICE_set", value );

    /* (0x00000188 bits 24) field SR_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
    tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_CSU_INTR_EN_DEVICE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_CSU_INTR_EN_DEVICE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SR_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 24) field SR_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_CSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_CSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_CSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_SR_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SR_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_PCIE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_PCIE_set", value );

    /* (0x00000188 bits 23:16) field SR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
    tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SR_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 23:16) field SR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000188 bits 23:16) field SR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
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
        /* (0x00000188 bits 23:16) field SR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
        tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_get", stop_bit, 7 );
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
    /* (0x00000188 bits 23:16) field SR_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3B_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SYS_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_PCIE_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_PCIE_set", value );

    /* (0x00000188 bits 15:0) field SYS_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
    tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SYS_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000188 bits 15:0) field SYS_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                  digi_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                  digi_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000188 bits 15:0) field SYS_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
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
        /* (0x00000188 bits 15:0) field SYS_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
        tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_get", stop_bit, 15 );
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
    /* (0x00000188 bits 15:0) field SYS_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_3B_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_set", value );

    /* (0x0000018c bits 16) field MGMT_FEGE_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_MGMT_FEGE_INTR_EN_DEVICE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_MGMT_FEGE_INTR_EN_DEVICE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000018c bits 16) field MGMT_FEGE_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_MGMT_FEGE_INTR_EN_DEVICE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_MGMT_FEGE_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_OH_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_DEVICE_set", value );

    /* (0x0000018c bits 15) field OH_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_CSU_INTR_EN_DEVICE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_CSU_INTR_EN_DEVICE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000018c bits 15) field OH_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_CSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_CSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_CSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_set", value );

    /* (0x0000018c bits 14:12) field OH_TX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000018c bits 14:12) field OH_TX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000018c bits 14:12) field OH_TX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
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
        /* (0x0000018c bits 14:12) field OH_TX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
        tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_get", stop_bit, 2 );
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
    /* (0x0000018c bits 14:12) field OH_TX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_MSK, TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_set", value );

    /* (0x0000018c bits 10:8) field OH_RX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000018c bits 10:8) field OH_RX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000018c bits 10:8) field OH_RX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
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
        /* (0x0000018c bits 10:8) field OH_RX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
        tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_get", stop_bit, 2 );
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
    /* (0x0000018c bits 10:8) field OH_RX_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_MSK, TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_set", value );

    /* (0x0000018c bits 4) field PCIE_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_CSU_INTR_EN_DEVICE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_CSU_INTR_EN_DEVICE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000018c bits 4) field PCIE_CSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_CSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_CSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_set", value, 15);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_set", value );

    /* (0x0000018c bits 3:0) field PCIE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000018c bits 3:0) field PCIE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_get", value );

    return value;
}
static INLINE void tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000018c bits 3:0) field PCIE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
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
        /* (0x0000018c bits 3:0) field PCIE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
        tl_ic_reg_OH_SERDES_INTR_EN_3_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_get", stop_bit, 3 );
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
    /* (0x0000018c bits 3:0) field PCIE_SERDES_INTR_EN_PCIE of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_3 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_3_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_MSK, TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 8) field FIC2_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC2_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC2_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 8) field FIC2_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC2_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC2_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 7) field FIC1_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC1_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC1_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 7) field FIC1_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC1_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC1_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 6) field PTP_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_PTP_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_PTP_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 6) field PTP_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_PTP_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_PTP_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 5) field ENET2_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET2_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET2_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 5) field ENET2_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET2_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET2_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 4) field ENET1_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET1_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET1_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 4) field ENET1_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET1_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET1_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 3) field SYS2_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS2_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS2_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 3) field SYS2_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS2_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS2_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 2) field SYS_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 2) field SYS_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 1) field OTN2_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN2_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN2_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 1) field OTN2_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN2_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN2_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_set", value );

    /* (0x00000190 bits 0) field OTN1_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    tl_ic_reg_DCSU_INTR_EN_3_field_set( b_ptr,
                                        h_ptr,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN1_DCSU_INTR_EN_DEVICE_MSK,
                                        TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN1_DCSU_INTR_EN_DEVICE_OFF,
                                        value);
}

static INLINE UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000190 bits 0) field OTN1_DCSU_INTR_EN_DEVICE of register PMC_TL_IC_REG_DCSU_INTR_EN_3 */
    reg_value = tl_ic_reg_DCSU_INTR_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN1_DCSU_INTR_EN_DEVICE_MSK) >> TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN1_DCSU_INTR_EN_DEVICE_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_get", value );

    return value;
}
static INLINE void tl_ic_field_LINE_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINE_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_MIPS_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_MIPS_set", value );

    /* (0x000001c0 bits 11:0) field LINE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4 */
    tl_ic_reg_LINE_SERDES_INTR_EN_4_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LINE_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINE_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001c0 bits 11:0) field LINE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_4_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_MSK) >> TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINE_SERDES_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001c0 bits 11:0) field LINE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4 */
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
        /* (0x000001c0 bits 11:0) field LINE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4 */
        tl_ic_reg_LINE_SERDES_INTR_EN_4_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_OFF + subfield_offset),
                                                   TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_get", stop_bit, 11 );
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
    /* (0x000001c0 bits 11:0) field LINE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4 */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_EN_4_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_MSK)
                  >> TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_MSK, TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LR_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LR_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_MIPS_set", value, 3);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_MIPS_set", value );

    /* (0x000001c4 bits 21:20) field LR_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
    tl_ic_reg_SYS_SERDES_INTR_EN_4A_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LR_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001c4 bits 21:20) field LR_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_4A_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_CSU_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LR_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LR_CSU_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_INTR_EN_MIPS_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_INTR_EN_MIPS_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_INTR_EN_MIPS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001c4 bits 21:20) field LR_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
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
        /* (0x000001c4 bits 21:20) field LR_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
        tl_ic_reg_SYS_SERDES_INTR_EN_4A_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LR_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_CSU_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_INTR_EN_MIPS_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_INTR_EN_MIPS_get", stop_bit, 1 );
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
    /* (0x000001c4 bits 21:20) field LR_CSU_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_4A_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_INTR_EN_MIPS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LR_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LR_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_MIPS_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_MIPS_set", value );

    /* (0x000001c4 bits 19:0) field LR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
    tl_ic_reg_SYS_SERDES_INTR_EN_4A_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LR_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001c4 bits 19:0) field LR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_4A_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_SERDES_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001c4 bits 19:0) field LR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
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
        /* (0x000001c4 bits 19:0) field LR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
        tl_ic_reg_SYS_SERDES_INTR_EN_4A_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_get", stop_bit, 19 );
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
    /* (0x000001c4 bits 19:0) field LR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_4A_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SR_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SR_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_MIPS_set", value, 255);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_MIPS_set", value );

    /* (0x000001c8 bits 23:16) field SR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
    tl_ic_reg_SYS_SERDES_INTR_EN_4B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SR_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001c8 bits 23:16) field SR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_4B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_SERDES_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001c8 bits 23:16) field SR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
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
        /* (0x000001c8 bits 23:16) field SR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
        tl_ic_reg_SYS_SERDES_INTR_EN_4B_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_get", stop_bit, 7 );
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
    /* (0x000001c8 bits 23:16) field SR_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_4B_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SYS_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_MIPS_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_MIPS_set", value );

    /* (0x000001c8 bits 15:0) field SYS_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
    tl_ic_reg_SYS_SERDES_INTR_EN_4B_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_MSK,
                                               TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SYS_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001c8 bits 15:0) field SYS_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_4B_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_MSK) >> TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_SERDES_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                  digi_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                  digi_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001c8 bits 15:0) field SYS_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
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
        /* (0x000001c8 bits 15:0) field SYS_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
        tl_ic_reg_SYS_SERDES_INTR_EN_4B_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_OFF + subfield_offset),
                                                   TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_get", stop_bit, 15 );
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
    /* (0x000001c8 bits 15:0) field SYS_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_EN_4B_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_MSK, TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_set", value );

    /* (0x000001cc bits 14:12) field OH_TX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    tl_ic_reg_OH_SERDES_INTR_EN_4_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001cc bits 14:12) field OH_TX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001cc bits 14:12) field OH_TX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
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
        /* (0x000001cc bits 14:12) field OH_TX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
        tl_ic_reg_OH_SERDES_INTR_EN_4_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_get", stop_bit, 2 );
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
    /* (0x000001cc bits 14:12) field OH_TX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_4_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_MSK, TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_set", value, 7);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_set", value );

    /* (0x000001cc bits 10:8) field OH_RX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    tl_ic_reg_OH_SERDES_INTR_EN_4_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001cc bits 10:8) field OH_RX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                    digi_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001cc bits 10:8) field OH_RX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
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
        /* (0x000001cc bits 10:8) field OH_RX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
        tl_ic_reg_OH_SERDES_INTR_EN_4_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                      digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_get", stop_bit, 2 );
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
    /* (0x000001cc bits 10:8) field OH_RX_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_4_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_MSK, TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_set", value, 15);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_set", value );

    /* (0x000001cc bits 3:0) field PCIE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    tl_ic_reg_OH_SERDES_INTR_EN_4_field_set( b_ptr,
                                             h_ptr,
                                             TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_MSK,
                                             TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_OFF,
                                             value);
}

static INLINE UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000001cc bits 3:0) field PCIE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_MSK) >> TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_get", value );

    return value;
}
static INLINE void tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_set( tl_ic_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000001cc bits 3:0) field PCIE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
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
        /* (0x000001cc bits 3:0) field PCIE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
        tl_ic_reg_OH_SERDES_INTR_EN_4_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_OFF + subfield_offset),
                                                 TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_get( tl_ic_buffer_t *b_ptr,
                                                                     digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_get", stop_bit, 3 );
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
    /* (0x000001cc bits 3:0) field PCIE_SERDES_INTR_EN_MIPS of register PMC_TL_IC_REG_OH_SERDES_INTR_EN_4 */
    reg_value = tl_ic_reg_OH_SERDES_INTR_EN_4_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_MSK)
                  >> TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_MSK, TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_SYS_RST_SYNC_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_SYS_RST_SYNC_set( tl_ic_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_SYS_RST_SYNC_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_SYS_RST_SYNC_set", value );

    /* (0x00000240 bits 31:16) field SYS_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
    tl_ic_reg_SERDES_CSU_RESET_SYNC_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_MSK,
                                               TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_SYS_RST_SYNC_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_RST_SYNC_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000240 bits 31:16) field SYS_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
    reg_value = tl_ic_reg_SERDES_CSU_RESET_SYNC_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_MSK) >> TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_RST_SYNC_get", value );

    return value;
}
static INLINE void tl_ic_field_range_SYS_RST_SYNC_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_SYS_RST_SYNC_set( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_RST_SYNC_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_RST_SYNC_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_RST_SYNC_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000240 bits 31:16) field SYS_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
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
        /* (0x00000240 bits 31:16) field SYS_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
        tl_ic_reg_SERDES_CSU_RESET_SYNC_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_OFF + subfield_offset),
                                                   TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_SYS_RST_SYNC_get( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SYS_RST_SYNC_get( tl_ic_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_RST_SYNC_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_RST_SYNC_get", stop_bit, 15 );
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
    /* (0x00000240 bits 31:16) field SYS_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
    reg_value = tl_ic_reg_SERDES_CSU_RESET_SYNC_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_MSK)
                  >> TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_MSK, TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_RST_SYNC_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_LINE_RST_SYNC_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_LINE_RST_SYNC_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_LINE_RST_SYNC_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_LINE_RST_SYNC_set", value );

    /* (0x00000240 bits 11:0) field LINE_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
    tl_ic_reg_SERDES_CSU_RESET_SYNC_field_set( b_ptr,
                                               h_ptr,
                                               TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_MSK,
                                               TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_OFF,
                                               value);
}

static INLINE UINT32 tl_ic_field_LINE_RST_SYNC_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINE_RST_SYNC_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000240 bits 11:0) field LINE_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
    reg_value = tl_ic_reg_SERDES_CSU_RESET_SYNC_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_MSK) >> TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINE_RST_SYNC_get", value );

    return value;
}
static INLINE void tl_ic_field_range_LINE_RST_SYNC_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_range_LINE_RST_SYNC_set( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_RST_SYNC_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_RST_SYNC_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_RST_SYNC_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000240 bits 11:0) field LINE_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
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
        /* (0x00000240 bits 11:0) field LINE_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
        tl_ic_reg_SERDES_CSU_RESET_SYNC_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_OFF + subfield_offset),
                                                   TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tl_ic_field_range_LINE_RST_SYNC_get( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LINE_RST_SYNC_get( tl_ic_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_RST_SYNC_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_RST_SYNC_get", stop_bit, 11 );
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
    /* (0x00000240 bits 11:0) field LINE_RST_SYNC of register PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC */
    reg_value = tl_ic_reg_SERDES_CSU_RESET_SYNC_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_MSK)
                  >> TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_MSK, TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_RST_SYNC_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tl_ic_field_APRAM_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_APRAM_RAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_APRAM_RAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_APRAM_RAM_LOW_PWR_set", value );

    /* (0x0000024c bits 2) field APRAM_RAM_LOW_PWR of register PMC_TL_IC_REG_APRAM_CFG */
    tl_ic_reg_APRAM_CFG_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_APRAM_CFG_BIT_APRAM_RAM_LOW_PWR_MSK,
                                   TL_IC_REG_APRAM_CFG_BIT_APRAM_RAM_LOW_PWR_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_APRAM_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_APRAM_RAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000024c bits 2) field APRAM_RAM_LOW_PWR of register PMC_TL_IC_REG_APRAM_CFG */
    reg_value = tl_ic_reg_APRAM_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_APRAM_CFG_BIT_APRAM_RAM_LOW_PWR_MSK) >> TL_IC_REG_APRAM_CFG_BIT_APRAM_RAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_APRAM_RAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_APRAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_APRAM_LOW_PWR_set( tl_ic_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_APRAM_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_APRAM_LOW_PWR_set", value );

    /* (0x0000024c bits 1) field APRAM_LOW_PWR of register PMC_TL_IC_REG_APRAM_CFG */
    tl_ic_reg_APRAM_CFG_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_APRAM_CFG_BIT_APRAM_LOW_PWR_MSK,
                                   TL_IC_REG_APRAM_CFG_BIT_APRAM_LOW_PWR_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_APRAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_APRAM_LOW_PWR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000024c bits 1) field APRAM_LOW_PWR of register PMC_TL_IC_REG_APRAM_CFG */
    reg_value = tl_ic_reg_APRAM_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_APRAM_CFG_BIT_APRAM_LOW_PWR_MSK) >> TL_IC_REG_APRAM_CFG_BIT_APRAM_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_APRAM_LOW_PWR_get", value );

    return value;
}
static INLINE void tl_ic_field_APRAM_RESET_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tl_ic_field_APRAM_RESET_set( tl_ic_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tl_ic_field_APRAM_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tl_ic_field_APRAM_RESET_set", value );

    /* (0x0000024c bits 0) field APRAM_RESET of register PMC_TL_IC_REG_APRAM_CFG */
    tl_ic_reg_APRAM_CFG_field_set( b_ptr,
                                   h_ptr,
                                   TL_IC_REG_APRAM_CFG_BIT_APRAM_RESET_MSK,
                                   TL_IC_REG_APRAM_CFG_BIT_APRAM_RESET_OFF,
                                   value);
}

static INLINE UINT32 tl_ic_field_APRAM_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_APRAM_RESET_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000024c bits 0) field APRAM_RESET of register PMC_TL_IC_REG_APRAM_CFG */
    reg_value = tl_ic_reg_APRAM_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_APRAM_CFG_BIT_APRAM_RESET_MSK) >> TL_IC_REG_APRAM_CFG_BIT_APRAM_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_APRAM_RESET_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 tl_ic_field_SFI2_TX_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_TX_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 15) field SFI2_TX_INTR of register PMC_TL_IC_REG_LINE_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI2_TX_INTR_MSK) >> TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI2_TX_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_TX_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_SFI2_RX_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI2_RX_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 14) field SFI2_RX_INTR of register PMC_TL_IC_REG_LINE_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI2_RX_INTR_MSK) >> TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI2_RX_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI2_RX_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_SFI1_TX_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_TX_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 13) field SFI1_TX_INTR of register PMC_TL_IC_REG_LINE_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI1_TX_INTR_MSK) >> TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI1_TX_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_TX_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_SFI1_RX_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SFI1_RX_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 12) field SFI1_RX_INTR of register PMC_TL_IC_REG_LINE_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI1_RX_INTR_MSK) >> TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI1_RX_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SFI1_RX_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_LINE_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LINE_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000080 bits 11:0) field LINE_SERDES_INTR of register PMC_TL_IC_REG_LINE_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_LINE_SERDES_INTR_MSK) >> TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_LINE_SERDES_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LINE_SERDES_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_range_LINE_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LINE_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LINE_SERDES_INTR_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LINE_SERDES_INTR_get", stop_bit, 11 );
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
    /* (0x00000080 bits 11:0) field LINE_SERDES_INTR of register PMC_TL_IC_REG_LINE_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_LINE_SERDES_INTR_SUM_read(  b_ptr, h_ptr);
    field_value = (reg_value & TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_LINE_SERDES_INTR_MSK)
                  >> TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_LINE_SERDES_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_LINE_SERDES_INTR_MSK, TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_LINE_SERDES_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LINE_SERDES_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 tl_ic_field_LR_CSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_CSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 21:20) field LR_CSU_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_1 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_1_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_CSU_INTR_MSK) >> TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_CSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_CSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_range_LR_CSU_INTR_get( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_CSU_INTR_get( tl_ic_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_CSU_INTR_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_CSU_INTR_get", stop_bit, 1 );
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
    /* (0x00000084 bits 21:20) field LR_CSU_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_1 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_1_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_CSU_INTR_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_CSU_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_CSU_INTR_MSK, TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_CSU_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_CSU_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 tl_ic_field_LR_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_LR_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000084 bits 19:0) field LR_SERDES_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_1 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_1_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_SERDES_INTR_MSK) >> TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_SERDES_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_LR_SERDES_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_range_LR_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_LR_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_LR_SERDES_INTR_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_LR_SERDES_INTR_get", stop_bit, 19 );
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
    /* (0x00000084 bits 19:0) field LR_SERDES_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_1 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_1_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_SERDES_INTR_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_SERDES_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_SERDES_INTR_MSK, TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_SERDES_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_LR_SERDES_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 tl_ic_field_SR_CSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_CSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 24) field SR_CSU_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_2 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_CSU_INTR_MSK) >> TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_CSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_CSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_SR_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SR_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 23:16) field SR_SERDES_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_2 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_SERDES_INTR_MSK) >> TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_SERDES_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SR_SERDES_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_range_SR_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SR_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SR_SERDES_INTR_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SR_SERDES_INTR_get", stop_bit, 7 );
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
    /* (0x00000088 bits 23:16) field SR_SERDES_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_2 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_SERDES_INTR_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_SERDES_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_SERDES_INTR_MSK, TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_SERDES_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SR_SERDES_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 tl_ic_field_SYS_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000088 bits 15:0) field SYS_SERDES_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_2 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SYS_SERDES_INTR_MSK) >> TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SYS_SERDES_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_SERDES_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_range_SYS_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_SYS_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_SYS_SERDES_INTR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_SYS_SERDES_INTR_get", stop_bit, 15 );
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
    /* (0x00000088 bits 15:0) field SYS_SERDES_INTR of register PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_2 */
    reg_value = tl_ic_reg_SYS_SERDES_INTR_SUM_2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SYS_SERDES_INTR_MSK)
                  >> TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SYS_SERDES_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SYS_SERDES_INTR_MSK, TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SYS_SERDES_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_SYS_SERDES_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 tl_ic_field_MGMT_FEGE_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_MGMT_FEGE_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 16) field MGMT_FEGE_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_MGMT_FEGE_INTR_MSK) >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_MGMT_FEGE_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_MGMT_FEGE_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_OH_CSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_CSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 15) field OH_CSU_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_CSU_INTR_MSK) >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_CSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_CSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_OH_TX_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_TX_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 14:12) field OH_TX_SERDES_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_TX_SERDES_INTR_MSK) >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_TX_SERDES_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_TX_SERDES_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_TX_SERDES_INTR_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_TX_SERDES_INTR_get", stop_bit, 2 );
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
    /* (0x0000008c bits 14:12) field OH_TX_SERDES_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_TX_SERDES_INTR_MSK)
                  >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_TX_SERDES_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_TX_SERDES_INTR_MSK, TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_TX_SERDES_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_TX_SERDES_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 tl_ic_field_OH_RX_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OH_RX_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 10:8) field OH_RX_SERDES_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_RX_SERDES_INTR_MSK) >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_RX_SERDES_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OH_RX_SERDES_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                              digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_OH_RX_SERDES_INTR_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_OH_RX_SERDES_INTR_get", stop_bit, 2 );
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
    /* (0x0000008c bits 10:8) field OH_RX_SERDES_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_RX_SERDES_INTR_MSK)
                  >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_RX_SERDES_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_RX_SERDES_INTR_MSK, TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_RX_SERDES_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_OH_RX_SERDES_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 tl_ic_field_PCIE_CSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_CSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 4) field PCIE_CSU_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_CSU_INTR_MSK) >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_CSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_CSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_PCIE_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PCIE_SERDES_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000008c bits 3:0) field PCIE_SERDES_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_SERDES_INTR_MSK) >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_SERDES_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PCIE_SERDES_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_range_PCIE_SERDES_INTR_get( tl_ic_buffer_t *b_ptr,
                                                             digi_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tl_ic_field_range_PCIE_SERDES_INTR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tl_ic_field_range_PCIE_SERDES_INTR_get", stop_bit, 3 );
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
    /* (0x0000008c bits 3:0) field PCIE_SERDES_INTR of register PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM */
    reg_value = tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_SERDES_INTR_MSK)
                  >> TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_SERDES_INTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_SERDES_INTR_MSK, TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_SERDES_INTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tl_ic_field_range_PCIE_SERDES_INTR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 tl_ic_field_FIC2_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC2_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 8) field FIC2_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_FIC2_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_FIC2_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC2_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_FIC1_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_FIC1_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 7) field FIC1_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_FIC1_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_FIC1_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_FIC1_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_PTP_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_PTP_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 6) field PTP_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_PTP_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_PTP_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_PTP_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_ENET2_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET2_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 5) field ENET2_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_ENET2_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_ENET2_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET2_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_ENET1_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_ENET1_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 4) field ENET1_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_ENET1_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_ENET1_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_ENET1_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_SYS2_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS2_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 3) field SYS2_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_SYS2_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_SYS2_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS2_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_SYS_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_SYS_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 2) field SYS_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_SYS_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_SYS_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_SYS_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_OTN2_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN2_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 1) field OTN2_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_OTN2_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_OTN2_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN2_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_OTN1_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_OTN1_DCSU_INTR_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 0) field OTN1_DCSU_INTR of register PMC_TL_IC_REG_DCSU_INTR_SUM */
    reg_value = tl_ic_reg_DCSU_INTR_SUM_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_DCSU_INTR_SUM_BIT_OTN1_DCSU_INTR_MSK) >> TL_IC_REG_DCSU_INTR_SUM_BIT_OTN1_DCSU_INTR_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_OTN1_DCSU_INTR_get", value );

    return value;
}
static INLINE UINT32 tl_ic_field_AVS_ENB_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tl_ic_field_AVS_ENB_get( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000284 bits 0) field AVS_ENB of register PMC_TL_IC_REG_AVS_STATUS */
    reg_value = tl_ic_reg_AVS_STATUS_read(  b_ptr, h_ptr);
    value = (reg_value & TL_IC_REG_AVS_STATUS_BIT_AVS_ENB_MSK) >> TL_IC_REG_AVS_STATUS_BIT_AVS_ENB_OFF;
    IOLOG( "%s -> 0x%08x", "tl_ic_field_AVS_ENB_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_burstsize12
 * ==================================================================================
 */
static INLINE void tl_ic_lfield_GP_CONFIG_get( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value[12] ) ALWAYS_INLINE;
static INLINE void tl_ic_lfield_GP_CONFIG_get( tl_ic_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 value[12] )
{
    IOLOG( "%s ", "tl_ic_lfield_GP_CONFIG_get");
    tl_ic_reg_GPCFG_array_burst_read( b_ptr,
                                      h_ptr,
                                      0,
                                      12,
                                      (UINT32 *)value);
}

static INLINE UINT32 tl_ic_lfield_range_GP_CONFIG_get( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 tl_ic_lfield_range_GP_CONFIG_get( tl_ic_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "tl_ic_lfield_range_GP_CONFIG_get", start_bit, stop_bit );
    }
    reg_value = tl_ic_reg_GPCFG_array_read( b_ptr,
                                            h_ptr,
                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "tl_ic_lfield_range_GP_CONFIG_get", start_bit, stop_bit, value );
    return value;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TL_IC_IO_INLINE_H */
